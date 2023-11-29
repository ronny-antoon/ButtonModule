#ifndef BUTTON_MODULE_HPP
#define BUTTON_MODULE_HPP

/**
 * @file ButtonModule.hpp
 * @brief Defines the ButtonModule class
 * @details Header file declaring the implementation of the ButtonModule class
 * @author Ronny Antoon
 * @copyright MetaHouse LTD.
 */
#include <MultiPrinterLoggerInterface.hpp>    // MultiPrinterLoggerInterface
#include <esp32-hal-gpio.h>                   // pinMode, digitalRead
#include <freertos/FreeRTOS.h>                // TaskHandle_t
#include <freertos/task.h>                    // TaskHandle_t
#define INCLUDE_uxTaskGetStackHighWaterMark 1 // TaskHandle_t
#define INCLUDE_vTaskDelete 1                 // vTaskDelete

#include "ButtonModuleInterface.hpp"

/**
 * @brief Implementation of the ButtonModule class.
 *
 * @details The ButtonModule class is responsible for managing the button module,
 * detecting button status changes, and invoking callback functions for single press,
 * double press, and long press events.
 */
class ButtonModule : public ButtonModuleInterface
{
private:
    MultiPrinterLoggerInterface *const _logger; // Logger for logging

    uint8_t _pin = 0;       // Pin of the button module
    bool _onRaising = true; // Flag indicating whether the button module triggers on raising or falling edge

    void (*_singlePressCallback)(void *) = nullptr; // Callback function for single press
    void *_singlePressCallbackParameter = nullptr;  // Parameter for the callback function for single press

    void (*_doublePressCallback)(void *) = nullptr; // Callback function for double press
    void *_doublePressCallbackParameter = nullptr;  // Parameter for the callback function for double press

    void (*_longPressCallback)(void *) = nullptr; // Callback function for long press
    void *_longPressCallbackParameter = nullptr;  // Parameter for the callback function for long press

    uint8_t _checkInterval;                          // Check interval for button trigger
    uint8_t _debounceTime;                           // Debounce time for button trigger
    uint16_t _longPressTime;                         // Long press time for button trigger
    uint16_t _timeBetweenDoublePress;                // Time between double press for button trigger
    TaskHandle_t _buttonTriggerTaskHandle = nullptr; // Task handle for the button trigger task

    /**
     * @brief Button trigger task.
     *
     * @details This task detects button triggers and invokes the appropriate callback functions.
     */
    void buttonTriggerTask();

public:
    /**
     * @brief Constructor for ButtonModule.
     *
     * @param pin The pin of the button module.
     * @param onRaising Flag indicating whether the button module triggers on raising or falling edge.
     */
    ButtonModule(
        uint8_t const pin, bool const onRaising = true,
        MultiPrinterLoggerInterface *const logger = nullptr);

    /**
     * @brief Destructor for ButtonModule.
     */
    ~ButtonModule() override;

    /**
     * @brief Checks if the button is currently pressed.
     *
     * @return true if the button is pressed, false otherwise.
     */
    bool const isPressed() const override;

    /**
     * @brief Sets the callback function for a single press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    void onSinglePress(void (*callback)(void *), void *_pParameter = nullptr) override;

    /**
     * @brief Sets the callback function for a double press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    void onDoublePress(void (*callback)(void *), void *_pParameter = nullptr) override;

    /**
     * @brief Sets the callback function for a long press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    void onLongPress(void (*callback)(void *), void *_pParameter = nullptr) override;

    /**
     * @brief Starts listening for button triggers.
     *
     * @param usStackDepth Stack depth for the task.
     * @param checkInterval The check interval for button triggers.
     * @param debounceTime The debounce time for button triggers.
     * @param longPressTime The long press time for button triggers.
     * @param timeBetweenDoublePress The time between double presses for button triggers.
     */
    void startListening(
        uint16_t usStackDepth = 3000, uint8_t checkInterval = 30,
        uint8_t debounceTime = 90, uint16_t longPressTime = 1000,
        uint16_t timeBetweenDoublePress = 500) override;

    /**
     * @brief Stops listening for button triggers.
     */
    void stopListening() override;
};

#endif // BUTTON_MODULE_HPP
