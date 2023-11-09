#ifndef BUTTON_MODULE_INTERFACE_HPP
#define BUTTON_MODULE_INTERFACE_HPP

/**
 * @file ButtonModuleInterface.hpp
 * @brief Defines the ButtonModuleInterface class
 * @details Header file declaring the abstract interface for button modules
 * @version 1.0.0
 * @date 2023/11/08
 * @author Ronny Antoon
 * @copyright MetaHouse LTD.
 */

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
    virtual bool isPressed() = 0;

    /**
     * @brief Sets the callback function for a single press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    virtual void onSinglePress(void (*callback)(void *), void *_pParameter) = 0;

    /**
     * @brief Sets the callback function for a double press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    virtual void onDoublePress(void (*callback)(void *), void *_pParameter) = 0;

    /**
     * @brief Sets the callback function for a long press event.
     *
     * @param callback The callback function, taking a void pointer as a parameter.
     * @param _pParameter The void pointer parameter for the callback function.
     */
    virtual void onLongPress(void (*callback)(void *), void *_pParameter) = 0;

    /**
     * @brief Starts listening for button triggers.
     *
     * @param usStackDepth Stack depth for the task.
     * @param checkInterval The check interval for button triggers.
     * @param debounceTime The debounce time for button triggers.
     * @param longPressTime The long press time for button triggers.
     * @param timeBetweenDoublePress The time between double presses for button triggers.
     */
    virtual void startListening(uint32_t usStackDepth, uint8_t checkInterval, uint8_t debounceTime, uint16_t longPressTime, uint16_t timeBetweenDoublePress) = 0;

    /**
     * @brief Stops listening for button triggers.
     */
    virtual void stopListening() = 0;
};

#endif // BUTTON_MODULE_INTERFACE_HPP