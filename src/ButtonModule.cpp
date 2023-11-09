#include "ButtonModule.hpp"

#include <esp32-hal-gpio.h>    // pinMode, digitalRead
#include <freertos/FreeRTOS.h> // TaskHandle_t
#include <freertos/task.h>     // TaskHandle_t

#include <Arduino.h>                          // TODO: remove
#define INCLUDE_uxTaskGetStackHighWaterMark 1 // TODO: remove

void ButtonModule::buttonTriggerTask()
{
    // Variables to track button press/release events and trigger firing
    bool wasPressed = false;                                              // Flag indicating if the button was previously pressed
    unsigned long lastPressTime = 0;                                      // Timestamp of the last button press
    unsigned long lastReleaseTime = 0;                                    // Timestamp of the last button release
    bool triggerFired = false;                                            // Flag indicating if a trigger event was fired
    uint8_t countPress = 0;                                               // Count of consecutive button presses
    bool doublePressOption = _doublePressCallback == NULL ? false : true; // Flag indicating if double press is configured

    while (true)
    {
        // Check if the trigger event fired; if so, wait until the button is released
        if (triggerFired)
        {
            if (isPressed())
                continue;
            else
            {
                // Reset variables after button release
                Serial.println(uxTaskGetStackHighWaterMark(NULL)); // TODO: remove
                triggerFired = false;
                wasPressed = false;
                lastPressTime = 0;
                lastReleaseTime = 0;
                countPress = 0;
                continue;
            }
        }

        if (isPressed())
        {
            // Check if the button is pressed for the first time
            if (!wasPressed)
            {
                lastPressTime = millis();
                lastReleaseTime = 0;
                wasPressed = true;
                continue;
            }
            else
            {
                // Check if the button is pressed for a long press and it's not a double press
                if (_longPressCallback && countPress == 0 && millis() - lastPressTime >= _longPressTime)
                {
                    _longPressCallback(_longPressCallbackParameter);
                    triggerFired = true;
                    continue;
                }
            }
        }
        else
        {
            // Check if the button is released
            if (wasPressed)
            {
                lastReleaseTime = millis();
                countPress++;
                wasPressed = false;
            }

            if (countPress > 0)
            {
                // Check if the button is released within the debounce time
                if (lastPressTime - lastReleaseTime <= _debounceTime)
                {
                    // Ignore short button release (debounce)
                    wasPressed = false;
                    lastPressTime = 0;
                    lastReleaseTime = 0;
                    continue;
                }

                // Check if the button is released after the time between double presses
                if (_singlePressCallback && (!_doublePressCallback || millis() - lastReleaseTime > _timeBetweenDoublePress))
                {
                    // Invoke single press callback
                    _singlePressCallback(_singlePressCallbackParameter);
                    triggerFired = true;
                    continue;
                }
                else
                {
                    if (_doublePressCallback && countPress >= 2)
                    {
                        // Invoke double press callback
                        _doublePressCallback(_doublePressCallbackParameter);
                        triggerFired = true;
                        continue;
                    }
                }
            }
        }

        // Wait for the next iteration
        delay(_checkInterval);
    }
}

ButtonModule::ButtonModule(uint8_t pin, bool onRaising)
{
    // Initialize member variables with default values
    _pin = pin;                           // Pin connected to the button module
    _onRaising = onRaising;               // Indicates whether the button triggers on the rising or falling edge
    _singlePressCallback = NULL;          // Callback function for a single press event
    _singlePressCallbackParameter = NULL; // Parameter for the single press callback function
    _doublePressCallback = NULL;          // Callback function for a double press event
    _doublePressCallbackParameter = NULL; // Parameter for the double press callback function
    _longPressCallback = NULL;            // Callback function for a long press event
    _longPressCallbackParameter = NULL;   // Parameter for the long press callback function
    _checkInterval = 30;                  // Interval for checking the button trigger
    _debounceTime = 90;                   // Debounce time to filter out noise
    _longPressTime = 1000;                // Duration to consider a button press as a long press
    _timeBetweenDoublePress = 500;        // Time between consecutive button presses for a double press
    buttonTriggerTask_handle = NULL;      // Task handle for the button trigger detection task

    // Set pin mode to input
    pinMode(_pin, INPUT);
}

ButtonModule::~ButtonModule()
{
    // Clean up and stop listening when the instance is destroyed
    stopListening();
}

bool ButtonModule::isPressed()
{
    // Check if the button is pressed based on the configured edge
    return digitalRead(_pin) == _onRaising;
}

void ButtonModule::onSinglePress(void (*callback)(void *), void *_pParameter)
{
    // Set the callback function and its parameter for a single press event
    _singlePressCallback = callback;
    _singlePressCallbackParameter = _pParameter;
}

void ButtonModule::onDoublePress(void (*callback)(void *), void *_pParameter)
{
    // Set the callback function and its parameter for a double press event
    _doublePressCallback = callback;
    _doublePressCallbackParameter = _pParameter;
}

void ButtonModule::onLongPress(void (*callback)(void *), void *_pParameter)
{
    // Set the callback function and its parameter for a long press event
    _longPressCallback = callback;
    _longPressCallbackParameter = _pParameter;
}

void ButtonModule::startListening(uint32_t usStackDepth, uint8_t checkInterval, uint8_t debounceTime, uint16_t longPressTime, uint16_t timeBetweenDoublePress)
{
    // Set configuration parameters for button trigger detection
    _checkInterval = checkInterval;
    _debounceTime = debounceTime;
    _longPressTime = longPressTime;
    _timeBetweenDoublePress = timeBetweenDoublePress;

    // Stop any existing listening task
    stopListening();

    // Create a new task for button trigger detection
    buttonTriggerTask_handle = new TaskHandle_t();
    xTaskCreate(
        [](void *thisPointer)
        { ((ButtonModule *)thisPointer)->buttonTriggerTask(); },
        "buttonTriggerTask",
        usStackDepth,
        this,
        1,
        buttonTriggerTask_handle);
}

void ButtonModule::stopListening()
{
    // Stop the listening task and clean up resources
    if (buttonTriggerTask_handle != NULL)
        vTaskDelete(*buttonTriggerTask_handle);

    delete buttonTriggerTask_handle;
    buttonTriggerTask_handle = NULL;
}
