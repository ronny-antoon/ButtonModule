#ifndef CALLBACK_TEST_HPP
#define CALLBACK_TEST_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Arduino.h>

#include "ButtonModule.hpp"

class MockCallback : public ::testing::Mock
{
public:
    MOCK_METHOD0(myCallback, void());
};

class callback_test : public ::testing::Test
{
protected:
    MockCallback mockCallback;
    ButtonModule *buttonModule;

    void SetUp() override
    {
        buttonModule = new ButtonModule(5);
    }

    void TearDown() override
    {
        delete buttonModule;
    }
};

TEST_F(callback_test, OnSinglePress)
{
    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    buttonModule->onSinglePress([](void *parameter)
                                { ((MockCallback *)parameter)->myCallback(); },
                                &mockCallback);
    buttonModule->startListening();

#ifdef WOKWI_ENVIRONMENT
    Serial.println("wokwi press OnSinglePress");
    delay(150);
    Serial.println("wokwi release OnSinglePress");
    delay(150);
#else
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(150);
    digitalWrite(5, LOW);
    delay(150);
#endif // WOKWI_ENVIRONMENT
}

TEST_F(callback_test, OnDoublePress)
{
    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    buttonModule->onDoublePress([](void *parameter)
                                { ((MockCallback *)parameter)->myCallback(); },
                                &mockCallback);
    buttonModule->startListening();

#ifdef WOKWI_ENVIRONMENT
    Serial.println("wokwi press OnDoublePress");
    delay(150);
    Serial.println("wokwi release OnDoublePress");
    delay(150);
    Serial.println("wokwi press OnDoublePress");
    delay(150);
    Serial.println("wokwi release OnDoublePress");
    delay(150);
#else
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(150);
    digitalWrite(5, LOW);
    delay(150);
    digitalWrite(5, HIGH);
    delay(150);
    digitalWrite(5, LOW);
    delay(150);
#endif // WOKWI_ENVIRONMENT
}

TEST_F(callback_test, OnLongPress)
{
    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    buttonModule->onLongPress([](void *parameter)
                              { ((MockCallback *)parameter)->myCallback(); },
                              &mockCallback);
    buttonModule->startListening();

#ifdef WOKWI_ENVIRONMENT
    Serial.println("wokwi press OnLongPress");
    delay(1500);
    Serial.println("wokwi release OnLongPress");
    delay(1500);
#else
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1500);
#endif // WOKWI_ENVIRONMENT
}

#endif // CALLBACK_TEST_HPP