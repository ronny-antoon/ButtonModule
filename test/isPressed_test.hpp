#ifndef IS_PRESSED_TEST_HPP
#define IS_PRESSED_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>

#include "ButtonModule.hpp"

TEST(ButtonModuleTest, IsPressed)
{
    ButtonModule buttonModule(5);

    EXPECT_FALSE(buttonModule.isPressed());

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(300);

    EXPECT_TRUE(buttonModule.isPressed());

    digitalWrite(5, LOW);
}

#endif // IS_PRESSED_TEST_HPP
