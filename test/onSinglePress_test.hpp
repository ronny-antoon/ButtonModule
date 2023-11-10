#ifndef ON_SINGLE_PRESS_TEST_HPP
#define ON_SINGLE_PRESS_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ButtonModule.hpp"

class MockCallback : public ::testing::Mock
{
public:
    MOCK_METHOD0(myCallback, void());
};
// Create a mock callback

TEST(ButtonModuleTest, OnSinglePress)
{
    MockCallback mockCallback;
    // Create a ButtonModule instance for testing
    ButtonModule buttonModule(5);

    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    // Set the single press callback to the mock callback
    buttonModule.onSinglePress([](void *parameter)
                               { ((MockCallback *)parameter)->myCallback(); },
                               &mockCallback);

#ifdef WOKWI_ENVIRONMENT
    Serial.println("wokwi press");
    delay(300);
    Serial.println("wokwi release");
    delay(300);
#else
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(200);
    digitalWrite(5, LOW);
    delay(100);
#endif // WOKWI_ENVIRONMENT
}

#endif // ON_SINGLE_PRESS_TEST_HPP
