#include "ButtonModule.hpp"

void ButtonModule::buttonTriggerTask()
{
    resetButtonState();

    while (true)
    {
        if (triggerFired)
        {
            // Wait until the button is released before resetting variables
            if (!isPressed())
                resetButtonState();
        }
        else
        {
            if (isPressed())
                handleButtonPress();
            else
                handleButtonRelease();
        }
        // Wait for the next iteration
        vTaskDelay(_checkInterval / portTICK_PERIOD_MS);
    }
}

void ButtonModule::resetButtonState()
{
    wasPressed = false;
    lastPressTime = 0;
    lastReleaseTime = 0;
    triggerFired = false;
    countPress = 0;
}

void ButtonModule::handleButtonPress()
{
    if (!wasPressed)
    {
        // First time the button is pressed
        lastPressTime = millis();
        lastReleaseTime = 0;
        wasPressed = true;
    }
    else
    {
        // Button pressed again, check for long press and not a double press
        if (_longPressCallback && countPress == 0 && millis() - lastPressTime >= _longPressTime)
        {
            Log_Verbose(_logger, "Long press detected");
            _longPressCallback(_longPressCallbackParameter);
            triggerFired = true;
        }
    }
}

void ButtonModule::handleButtonRelease()
{
    if (wasPressed)
    {
        // Button was released
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
            resetButtonState();
        }
        else
        {
            // Check for single or double press
            handleSingleOrDoublePress();
        }
    }
}

void ButtonModule::handleSingleOrDoublePress()
{
    if (_singlePressCallback && (!_doublePressCallback || millis() - lastReleaseTime > _timeBetweenDoublePress))
    {
        Log_Verbose(_logger, "Single press detected");
        _singlePressCallback(_singlePressCallbackParameter);
        triggerFired = true;
    }
    else if (_doublePressCallback && countPress >= 2)
    {
        Log_Verbose(_logger, "Double press detected");
        _doublePressCallback(_doublePressCallbackParameter);
        triggerFired = true;
    }
}

ButtonModule::ButtonModule(
    uint8_t const pin, bool const onRaising,
    MultiPrinterLoggerInterface *const logger)
    : _pin(pin),
      _onRaising(onRaising),
      _logger(logger)
{
    Log_Debug(_logger, "Created with parameters: pin = %d, onRaising = %s", pin, onRaising ? "HIGH" : "LOW");
    // Set pin mode to input
    pinMode(_pin, INPUT);
}

ButtonModule::~ButtonModule()
{
    Log_Debug(_logger, "Destroyed");
    // Clean up and stop listening when the instance is destroyed
    stopListening();
}

bool const ButtonModule::isPressed() const
{
    // Check if the button is pressed based on the configured edge
    return (digitalRead(_pin) == _onRaising);
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

void ButtonModule::startListening(
    uint16_t usStackDepth, uint8_t checkInterval,
    uint8_t debounceTime, uint16_t longPressTime,
    uint16_t timeBetweenDoublePress)
{
    Log_Verbose(_logger, "Button listening started with parameters: usStackDepth=%d,  checkInterval=%d, debounceTime=%d, longPressTime=%d, timeBetweenDoublePress=%d",
                usStackDepth, checkInterval, debounceTime, longPressTime, timeBetweenDoublePress);
    // Set configuration parameters for button trigger detection
    _checkInterval = checkInterval;
    _debounceTime = debounceTime;
    _longPressTime = longPressTime;
    _timeBetweenDoublePress = timeBetweenDoublePress;

    // Stop any existing listening task
    stopListening();

    // Start a new listening task
    xTASK_CREATE_TRACKED(
        [](void *thisPointer)
        { static_cast<ButtonModule *>(thisPointer)->buttonTriggerTask(); },
        "buttonTriggerTask",
        usStackDepth,
        this,
        1,
        &_buttonTriggerTaskHandle);
    // xTaskCreate(
    //     [](void *thisPointer)
    //     { static_cast<ButtonModule *>(thisPointer)->buttonTriggerTask(); },
    //     "buttonTriggerTask",
    //     usStackDepth,
    //     this,
    //     1,
    //     &_buttonTriggerTaskHandle);
}

void ButtonModule::stopListening()
{
    // Stop the listening task and clean up resources
    if (_buttonTriggerTaskHandle != nullptr)
    {
        Log_Verbose(_logger, "Button listening stopped");
        xTASK_DELETE_TRACKED(&_buttonTriggerTaskHandle);
        // vTaskDelete(_buttonTriggerTaskHandle);
    }
    _buttonTriggerTaskHandle = nullptr;
}
