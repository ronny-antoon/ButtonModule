#ifndef IS_PRESSED_TEST_HPP
#define IS_PRESSED_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>

#include "ButtonModule.hpp"

class ButtonModuleTest : public ::testing::Test
{
protected:
    int buttonPin1 = 5;
    bool onRaising1 = true;
    int buttonPin2 = 14;
    bool onRaising2 = false;
    uint32_t _startFreeHeap = 0;

    ButtonModule *buttonModule1;
    ButtonModule *buttonModule2;

    // Set up function to run before each test case
    void SetUp() override
    {
        _startFreeHeap = ESP.getFreeHeap(); // Record initial free heap size
        buttonModule1 = new ButtonModule(buttonPin1, onRaising1);
        buttonModule2 = new ButtonModule(buttonPin2, onRaising2);

        pinMode(buttonPin2, OUTPUT);
        digitalWrite(buttonPin2, !onRaising2);
    }

    // Tear down function to run after each test case
    void TearDown() override
    {
        delete buttonModule1;
        delete buttonModule2;
        if (ESP.getFreeHeap() != _startFreeHeap)
            FAIL() << "Memory leak of " << _startFreeHeap - ESP.getFreeHeap() << " bytes"; // Fail the test if there is a memory leak
    }
};

TEST_F(ButtonModuleTest, isPressed)
{
    EXPECT_FALSE(buttonModule1->isPressed());
    EXPECT_FALSE(buttonModule2->isPressed());

    pinMode(buttonPin1, OUTPUT);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    digitalWrite(buttonPin2, onRaising2);

    EXPECT_TRUE(buttonModule1->isPressed());
    EXPECT_TRUE(buttonModule2->isPressed());

    digitalWrite(buttonPin1, !onRaising1);
    digitalWrite(buttonPin2, !onRaising2);

    EXPECT_FALSE(buttonModule1->isPressed());
    EXPECT_FALSE(buttonModule2->isPressed());
}

#endif // IS_PRESSED_TEST_HPP
