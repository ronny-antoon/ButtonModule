#include <Arduino.h>
#include "ButtonModule.hpp"

// Constructor
ButtonModule::ButtonModule(
    uint8_t const pin, MultiPrinterLoggerInterface *const logger,
    bool const isActiveHigh,
    uint16_t const longPressDurationMs,
    uint16_t const debounceTimeMs,
    uint16_t const usStackDepth,
    uint16_t const timeBetweenDoublePress)
    : m_pin(pin),
      m_logger(logger),
      m_onRaising(isActiveHigh),
      m_longPressTime(longPressDurationMs),
      m_debounceTime(debounceTimeMs),
      m_checkInterval(30),
      m_timeBetweenDoublePress(timeBetweenDoublePress),
      m_buttonTriggerTaskHandle(nullptr),
      m_wasPressed(false),
      m_lastPressTime(0),
      m_lastReleaseTime(0),
      m_triggerFired(false),
      m_countPress(0)
{
    Log_Debug(m_logger, "Created with parameters: pin = %d, onRaising = %s", pin, isActiveHigh ? "HIGH" : "LOW");
    pinMode(m_pin, INPUT);

    startListening(usStackDepth, "buttonTriggerTask", 30, debounceTimeMs, longPressDurationMs, timeBetweenDoublePress);
}

// Destructor
ButtonModule::~ButtonModule()
{
    Log_Debug(m_logger, "Destroyed");
    stopListening();
}

// Start listening for button triggers
void ButtonModule::startListening(
    uint16_t usStackDepth, char const *taskName, uint8_t checkInterval,
    uint8_t debounceTime, uint16_t longPressTime,
    uint16_t timeBetweenDoublePress)
{
    Log_Verbose(m_logger, "Button listening started with parameters: usStackDepth=%d, checkInterval=%d, debounceTime=%d, longPressTime=%d, timeBetweenDoublePress=%d",
                usStackDepth, checkInterval, debounceTime, longPressTime, timeBetweenDoublePress);

    m_checkInterval = checkInterval;
    m_debounceTime = debounceTime;
    m_longPressTime = longPressTime;
    m_timeBetweenDoublePress = timeBetweenDoublePress;

    bool nameing = true;
    if (taskName == nullptr || strlen(taskName) < 2 || strcmp(taskName, "") == 0 || strlen(taskName) > 50)
    {
        nameing = false;
    }

    stopListening();

    xTASK_CREATE_TRACKED(
        [](void *thisPointer)
        { static_cast<ButtonModule *>(thisPointer)->buttonTriggerTask(); },
        nameing ? taskName : "buttonTriggerTask",
        usStackDepth,
        this,
        1,
        &m_buttonTriggerTaskHandle);
}

// Stop listening for button triggers
void ButtonModule::stopListening()
{
    if (m_buttonTriggerTaskHandle != nullptr)
    {
        Log_Verbose(m_logger, "Button listening stopped");
        xTASK_DELETE_TRACKED(&m_buttonTriggerTaskHandle);
    }
    m_buttonTriggerTaskHandle = nullptr;
}

// Set single press callback
void ButtonModule::setSinglePressCallback(CallbackButtonFunction callback, AnyType *callbackParameter)
{
    Log_Verbose(m_logger, "On single press callback set");
    m_singlePressCallback = callback;
    m_singlePressCallbackParameter = callbackParameter;
}

// Set double press callback
void ButtonModule::setDoublePressCallback(CallbackButtonFunction callback, AnyType *callbackParameter)
{
    Log_Verbose(m_logger, "On double press callback set");
    m_doublePressCallback = callback;
    m_doublePressCallbackParameter = callbackParameter;
}

// Set long press callback
void ButtonModule::setLongPressCallback(CallbackButtonFunction callback, AnyType *callbackParameter)
{
    Log_Verbose(m_logger, "On long press callback set");
    m_longPressCallback = callback;
    m_longPressCallbackParameter = callbackParameter;
}

// Check if the button is pressed
bool const ButtonModule::isPressed() const
{
    return (digitalRead(m_pin) == m_onRaising);
}

// Reset button state
void ButtonModule::resetButtonState()
{
    m_wasPressed = false;
    m_lastPressTime = 0;
    m_lastReleaseTime = 0;
    m_triggerFired = false;
    m_countPress = 0;
}

// Handle button press
void ButtonModule::handleButtonPress()
{
    if (!m_wasPressed)
    {
        m_lastPressTime = millis();
        m_lastReleaseTime = 0;
        m_wasPressed = true;
    }
    else
    {
        if (m_longPressCallback && m_countPress == 0 && millis() - m_lastPressTime >= m_longPressTime)
        {
            Log_Verbose(m_logger, "Long press detected");
            m_longPressCallback(m_longPressCallbackParameter);
            m_triggerFired = true;
        }
    }
}

// Handle button release
void ButtonModule::handleButtonRelease()
{
    if (m_wasPressed)
    {
        m_lastReleaseTime = millis();
        m_countPress++;
        m_wasPressed = false;
    }

    if (m_countPress > 0)
    {
        if (m_lastPressTime - m_lastReleaseTime <= m_debounceTime)
        {
            resetButtonState();
        }
        else
        {
            handleSingleOrDoublePress();
        }
    }
}

// Handle single or double press
void ButtonModule::handleSingleOrDoublePress()
{
    if (m_singlePressCallback && (!m_doublePressCallback || millis() - m_lastReleaseTime > m_timeBetweenDoublePress))
    {
        Log_Verbose(m_logger, "Single press detected");
        m_singlePressCallback(m_singlePressCallbackParameter);
        m_triggerFired = true;
    }
    else if (m_doublePressCallback && m_countPress >= 2)
    {
        Log_Verbose(m_logger, "Double press detected");
        m_doublePressCallback(m_doublePressCallbackParameter);
        m_triggerFired = true;
    }
}

// Button trigger task
void ButtonModule::buttonTriggerTask()
{
    resetButtonState();

    while (true)
    {
        if (m_triggerFired)
        {
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
        vTaskDelay(m_checkInterval / portTICK_PERIOD_MS);
    }
}
