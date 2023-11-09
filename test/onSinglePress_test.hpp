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
MockCallback mockCallback;

TEST(ButtonModuleTest, OnSinglePress)
{
    // Create a ButtonModule instance for testing
    ButtonModule buttonModule(5);
    pinMode(5, OUTPUT);

    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    // Set the single press callback to the mock callback
    buttonModule.onSinglePress([](void *parameter)
                               { mockCallback.myCallback(); },
                               nullptr);

    // Simulate a single press event (modify this based on your actual logic)
    digitalWrite(5, HIGH);
    delay(200);
    digitalWrite(5, LOW);
    delay(100);
}

#endif // ON_SINGLE_PRESS_TEST_HPP
