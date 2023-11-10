#ifndef IS_PRESSED_TEST_HPP
#define IS_PRESSED_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>

#include "ButtonModule.hpp"

TEST(ButtonModuleTest, IsPressed)
{
    ButtonModule buttonModule(5);

    EXPECT_FALSE(buttonModule.isPressed());

#ifdef WOKWI_ENVIRONMENT
    Serial.println("wokwi press IsPressed");
#else
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
#endif // WOKWI_ENVIRONMENT
    delay(300);

    EXPECT_TRUE(buttonModule.isPressed());

#ifdef WOKWI_ENVIRONMENT
    Serial.println("wokwi release IsPressed");
#else
    digitalWrite(5, LOW);
#endif // WOKWI_ENVIRONMENT
}

#endif // IS_PRESSED_TEST_HPP
