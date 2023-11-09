#ifndef IS_PRESSED_TEST_HPP
#define IS_PRESSED_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>

#include "ButtonModule.hpp"

TEST(ButtonModuleTest, IsPressed)
{
    // Create a ButtonModule instance for testing
    ButtonModule buttonModule(5);

    // Assuming the button is not pressed initially
    EXPECT_FALSE(buttonModule.isPressed());

    // Simulate a pressed state (modify this based on your actual logic)
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);

    // Assuming the button is pressed now
    EXPECT_TRUE(buttonModule.isPressed());

    // Reset the pin state (modify this based on your actual logic)
    digitalWrite(5, LOW);
}

#endif // IS_PRESSED_TEST_HPP
