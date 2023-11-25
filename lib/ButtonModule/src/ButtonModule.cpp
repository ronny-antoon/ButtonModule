#include "ButtonModule.hpp"

void ButtonModule::buttonTriggerTask()
{
    // Variables to track button press/release events and trigger firing
    bool wasPressed = false;                                                 // Flag indicating if the button was previously pressed
    unsigned long lastPressTime = 0;                                         // Timestamp of the last button press
    unsigned long lastReleaseTime = 0;                                       // Timestamp of the last button release
    bool triggerFired = false;                                               // Flag indicating if a trigger event was fired
    uint8_t countPress = 0;                                                  // Count of consecutive button presses
    bool doublePressOption = _doublePressCallback == nullptr ? false : true; // Flag indicating if double press is configured

    while (true)
    { // @note This task is not meant to be stopped, so it runs in an infinite loop, and should delay at the end of each iteration to allow other tasks to run, so never use continue.
        // Check if the trigger event fired; if so, wait until the button is released
        if (triggerFired)
        {
            if (!isPressed())
            {
                Log_Verbose(_logger, "buttonTriggerTask with free stack availabe: %d", uxTaskGetStackHighWaterMark(nullptr));

                // Reset variables after button release
                triggerFired = false;
                wasPressed = false;
                lastPressTime = 0;
                lastReleaseTime = 0;
                countPress = 0;
            }
        }
        else
        {
            if (isPressed())
            {
                // Check if the button is pressed for the first time
                if (!wasPressed)
                {
                    lastPressTime = millis();
                    lastReleaseTime = 0;
                    wasPressed = true;
                }
                else
                {
                    // Check if the button is pressed for a long press and it's not a double press
                    if (_longPressCallback && countPress == 0 && millis() - lastPressTime >= _longPressTime)
                    {
                        Log_Debug(_logger, "Long press detected");
                        _longPressCallback(_longPressCallbackParameter);
                        triggerFired = true;
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
                        countPress--;
                    }
                    else
                    {
                        // Check if the button is released after the time between double presses
                        if (_singlePressCallback && (!_doublePressCallback || millis() - lastReleaseTime > _timeBetweenDoublePress))
                        {
                            Log_Debug(_logger, "Single press detected");
                            // Invoke single press callback
                            _singlePressCallback(_singlePressCallbackParameter);
                            triggerFired = true;
                        }
                        else
                        {
                            if (_doublePressCallback && countPress >= 2)
                            {
                                Log_Debug(_logger, "Double press detected");
                                // Invoke double press callback
                                _doublePressCallback(_doublePressCallbackParameter);
                                triggerFired = true;
                            }
                        }
                    }
                }
            }
        }
        // Wait for the next iteration
        delay(_checkInterval);
    }
}

ButtonModule::ButtonModule(uint8_t pin, bool onRaising, MultiPrinterLoggerInterface *logger) : _pin(pin),
                                                                                               _onRaising(onRaising),
                                                                                               _logger(logger),
                                                                                               _singlePressCallback(nullptr),
                                                                                               _singlePressCallbackParameter(nullptr),
                                                                                               _doublePressCallback(nullptr),
                                                                                               _doublePressCallbackParameter(nullptr),
                                                                                               _longPressCallback(nullptr),
                                                                                               _longPressCallbackParameter(nullptr),
                                                                                               _checkInterval(30),
                                                                                               _debounceTime(90),
                                                                                               _longPressTime(1000),
                                                                                               _timeBetweenDoublePress(500),
                                                                                               _buttonTriggerTaskHandle(nullptr)
{
    Log_Debug(_logger, "Button module created with parameters: pin=%d, onRaising=%d", pin, onRaising ? "HIGH" : "LOW");
    // Set pin mode to input
    pinMode(_pin, INPUT);
}

ButtonModule::~ButtonModule()
{
    Log_Debug(_logger, "Button module destroyed");
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
    Log_Verbose(_logger, "On single press callback set");
    // Set the callback function and its parameter for a single press event
    _singlePressCallback = callback;
    _singlePressCallbackParameter = _pParameter;
}

void ButtonModule::onDoublePress(void (*callback)(void *), void *_pParameter)
{
    Log_Verbose(_logger, "On double press callback set");
    // Set the callback function and its parameter for a double press event
    _doublePressCallback = callback;
    _doublePressCallbackParameter = _pParameter;
}

void ButtonModule::onLongPress(void (*callback)(void *), void *_pParameter)
{
    Log_Verbose(_logger, "On long press callback set");
    // Set the callback function and its parameter for a long press event
    _longPressCallback = callback;
    _longPressCallbackParameter = _pParameter;
}

void ButtonModule::startListening(uint16_t usStackDepth, uint8_t checkInterval, uint8_t debounceTime, uint16_t longPressTime, uint16_t timeBetweenDoublePress)
{
    Log_Debug(_logger, "Button listening started with parameters: checkInterval=%d, debounceTime=%d, longPressTime=%d, timeBetweenDoublePress=%d",
              checkInterval, debounceTime, longPressTime, timeBetweenDoublePress);
    // Set configuration parameters for button trigger detection
    _checkInterval = checkInterval;
    _debounceTime = debounceTime;
    _longPressTime = longPressTime;
    _timeBetweenDoublePress = timeBetweenDoublePress;

    // Stop any existing listening task
    stopListening();

    // Start a new listening task
    xTaskCreate(
        [](void *thisPointer)
        { ((ButtonModule *)thisPointer)->buttonTriggerTask(); },
        "buttonTriggerTask",
        usStackDepth,
        this,
        1,
        &_buttonTriggerTaskHandle);
}

void ButtonModule::stopListening()
{
    Log_Debug(_logger, "Button listening stopped");
    // Stop the listening task and clean up resources
    if (_buttonTriggerTaskHandle != nullptr)
        vTaskDelete(_buttonTriggerTaskHandle);
    _buttonTriggerTaskHandle = nullptr;
}
