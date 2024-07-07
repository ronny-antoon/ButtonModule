#pragma once

/**
 * @file ButtonModuleInterface.hpp
 * @brief Defines the ButtonModuleInterface class.
 * @details Header file declaring the abstract interface for button modules.
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
    using CallbackButtonFunction = void (*)(void *); ///< Type definition for a button callback function.
    using AnyType = void;                            ///< Type definition for any type parameter.

    /**
     * @brief Virtual destructor for ButtonModuleInterface.
     */
    virtual ~ButtonModuleInterface() = default;

    /**
     * @brief Sets the callback function for a single press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param callbackParameter The void pointer parameter for the callback function.
     */
    virtual void setSinglePressCallback(CallbackButtonFunction callback, AnyType *callbackParameter = nullptr) = 0;

    /**
     * @brief Sets the callback function for a double press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param callbackParameter The void pointer parameter for the callback function.
     */
    virtual void setDoublePressCallback(CallbackButtonFunction callback, AnyType *callbackParameter = nullptr) = 0;

    /**
     * @brief Sets the callback function for a long press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param callbackParameter The void pointer parameter for the callback function.
     */
    virtual void setLongPressCallback(CallbackButtonFunction callback, AnyType *callbackParameter = nullptr) = 0;
};
