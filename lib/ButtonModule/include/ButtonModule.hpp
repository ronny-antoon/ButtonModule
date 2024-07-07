#pragma once

/**
 * @file ButtonModule.hpp
 * @brief Defines the ButtonModule class.
 * @details Header file declaring the implementation of the ButtonModule class.
 * @author Ronny Antoon
 * @copyright MetaHouse LTD.
 */
#include <Arduino.h>

#ifndef CONFIG_R_B_M_ACTIVE_HIGH
#define CONFIG_R_B_M_ACTIVE_HIGH true
#endif
#ifndef CONFIG_R_B_M_LONG_PRESS_TIME
#define CONFIG_R_B_M_LONG_PRESS_TIME 2000
#endif
#ifndef CONFIG_R_B_M_DEBOUNCE_TIME
#define CONFIG_R_B_M_DEBOUNCE_TIME 90
#endif
#ifndef CONFIG_R_B_M_TIME_BETWEEN_DOUBLE_PRESS
#define CONFIG_R_B_M_TIME_BETWEEN_DOUBLE_PRESS 500
#endif
#ifndef CONFIG_R_B_M_STACK_DEPTH
#define CONFIG_R_B_M_STACK_DEPTH 3000
#endif

#include "ButtonModuleInterface.hpp"

#include <MultiPrinterLoggerInterface.hpp> // MultiPrinterLoggerInterface
#include <TaskTracker.hpp>

/**
 * @brief Implementation of the ButtonModule class.
 *
 * @details The ButtonModule class is responsible for managing the button module,
 * detecting button status changes, and invoking callback functions for single press,
 * double press, and long press events.
 */
class ButtonModule : public ButtonModuleInterface
{
public:
    /**
     * @brief Constructor for ButtonModule.
     *
     * @param pin The pin of the button module.
     * @param logger Pointer to the logger instance.
     * @param isActiveHigh Flag indicating whether the button module triggers on raising or falling edge.
     * @param longPressDurationMs Duration for a long press in milliseconds.
     * @param debounceTimeMs Debounce time in milliseconds.
     * @param usStackDepth Stack depth for the task.
     * @param timeBetweenDoublePress Time between double press events in milliseconds.
     */
    ButtonModule(
        uint8_t const pin, MultiPrinterLoggerInterface *const logger = nullptr,
        bool const isActiveHigh = CONFIG_R_B_M_ACTIVE_HIGH,
        uint16_t const longPressDurationMs = CONFIG_R_B_M_LONG_PRESS_TIME,
        uint16_t const debounceTimeMs = CONFIG_R_B_M_DEBOUNCE_TIME,
        uint16_t const usStackDepth = CONFIG_R_B_M_STACK_DEPTH,
        uint16_t const timeBetweenDoublePress = CONFIG_R_B_M_TIME_BETWEEN_DOUBLE_PRESS);

    /**
     * @brief Destructor for ButtonModule.
     */
    ~ButtonModule() override;

    /**
     * @brief Sets the callback function for a single press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param callbackParameter The void pointer parameter for the callback function.
     */
    void setSinglePressCallback(CallbackButtonFunction callback, AnyType *callbackParameter = nullptr) override;

    /**
     * @brief Sets the callback function for a double press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param callbackParameter The void pointer parameter for the callback function.
     */
    void setDoublePressCallback(CallbackButtonFunction callback, AnyType *callbackParameter = nullptr) override;

    /**
     * @brief Sets the callback function for a long press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param callbackParameter The void pointer parameter for the callback function.
     */
    void setLongPressCallback(CallbackButtonFunction callback, AnyType *callbackParameter = nullptr) override;

private:
    MultiPrinterLoggerInterface *const m_logger; ///< Logger for logging

    uint8_t m_pin;    ///< Pin of the button module
    bool m_onRaising; ///< Flag indicating whether the button module triggers on raising or falling edge

    void (*m_singlePressCallback)(void *) = nullptr; ///< Callback function for single press
    void *m_singlePressCallbackParameter = nullptr;  ///< Parameter for the callback function for single press

    void (*m_doublePressCallback)(void *) = nullptr; ///< Callback function for double press
    void *m_doublePressCallbackParameter = nullptr;  ///< Parameter for the callback function for double press

    void (*m_longPressCallback)(void *) = nullptr; ///< Callback function for long press
    void *m_longPressCallbackParameter = nullptr;  ///< Parameter for the callback function for long press

    uint8_t m_checkInterval;                          ///< Check interval for button trigger
    uint8_t m_debounceTime;                           ///< Debounce time for button trigger
    uint16_t m_longPressTime;                         ///< Long press time for button trigger
    uint16_t m_timeBetweenDoublePress;                ///< Time between double press for button trigger
    TaskHandle_t m_buttonTriggerTaskHandle = nullptr; ///< Task handle for the button trigger task

    bool m_wasPressed;
    unsigned long m_lastPressTime;
    unsigned long m_lastReleaseTime;
    bool m_triggerFired;
    uint8_t m_countPress;

    /**
     * @brief Button trigger task.
     *
     * @details This task detects button triggers and invokes the appropriate callback functions.
     */
    void buttonTriggerTask();
    void resetButtonState();
    void handleButtonPress();
    void handleButtonRelease();
    void handleSingleOrDoublePress();

    /**
     * @brief Starts listening for button triggers.
     *
     * @param usStackDepth Stack depth for the task.
     * @param taskName The name of the task.
     * @param checkInterval The check interval for button triggers.
     * @param debounceTime The debounce time for button triggers.
     * @param longPressTime The long press time for button triggers.
     * @param timeBetweenDoublePress The time between double presses for button triggers.
     */
    void startListening(
        uint16_t usStackDepth = 3000, char const *taskName = nullptr, uint8_t checkInterval = 30,
        uint8_t debounceTime = 90, uint16_t longPressTime = 1000,
        uint16_t timeBetweenDoublePress = 500);

    /**
     * @brief Stops listening for button triggers.
     */
    void stopListening();

    /**
     * @brief Checks if the button is currently pressed.
     *
     * @return true if the button is pressed, false otherwise.
     */
    bool const isPressed() const;

    // Delete copy constructor and assignment operator
    ButtonModule(ButtonModule const &) = delete;
    void operator=(ButtonModule const &) = delete;
};
