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

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(150);
    digitalWrite(5, LOW);
    delay(150);
}

TEST_F(callback_test, OnDoublePress)
{
    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    buttonModule->onDoublePress([](void *parameter)
                                { ((MockCallback *)parameter)->myCallback(); },
                                &mockCallback);
    buttonModule->startListening();

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(150);
    digitalWrite(5, LOW);
    delay(150);
    digitalWrite(5, HIGH);
    delay(150);
    digitalWrite(5, LOW);
    delay(150);
}

TEST_F(callback_test, OnLongPress)
{
    EXPECT_CALL(mockCallback, myCallback()).Times(1);

    buttonModule->onLongPress([](void *parameter)
                              { ((MockCallback *)parameter)->myCallback(); },
                              &mockCallback);
    buttonModule->startListening();

    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    delay(1500);
    digitalWrite(5, LOW);
    delay(1500);
}

#endif // CALLBACK_TEST_HPP