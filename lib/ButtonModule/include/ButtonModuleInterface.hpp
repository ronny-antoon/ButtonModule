#pragma once

/**
 * @file ButtonModuleInterface.hpp
 * @brief Defines the ButtonModuleInterface class
 * @details Header file declaring the abstract interface for button modules
 * @author Ronny Antoon
 * @copyright MetaHouse LTD.
 */

#include <Arduino.h>

/**
 * @brief Interface for button modules.
 *
 * @details This abstract class serves as an interface for button modules,
 * providing methods for checking button status and setting callback functions
 * for single press, double press, and long press events.
 */
class ButtonModuleInterface
{
public:
    /**
     * @brief Virtual destructor for ButtonModuleInterface.
     */
    virtual ~ButtonModuleInterface() = default;

    /**
     * @brief Checks if the button is currently pressed.
     *
     * @return true if the button is pressed, false otherwise.
     */
    virtual bool const isPressed() const = 0;

    /**
     * @brief Sets the callback function for a single press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    virtual void onSinglePress(void (*callback)(void *), void *_pParameter = nullptr) = 0;

    /**
     * @brief Sets the callback function for a double press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    virtual void onDoublePress(void (*callback)(void *), void *_pParameter = nullptr) = 0;

    /**
     * @brief Sets the callback function for a long press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    virtual void onLongPress(void (*callback)(void *), void *_pParameter = nullptr) = 0;

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
    virtual void startListening(
        uint16_t usStackDepth = 3000, char const *taskName = "buttonTriggerTask", uint8_t checkInterval = 30,
        uint8_t debounceTime = 90, uint16_t longPressTime = 1000,
        uint16_t timeBetweenDoublePress = 500) = 0;

    /**
     * @brief Stops listening for button triggers.
     */
    virtual void stopListening() = 0;
};