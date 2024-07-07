#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Arduino.h>
#include "ButtonModule.hpp"

void myCallback(void *parameter)
{
    int *myparam = (int *)(parameter);
    (*myparam)++;
}

class MyClass
{
public:
    virtual void myCallback(void *parameter)
    {
        int *myparam = (int *)(parameter);
        (*myparam)++;
    }
};

class MockMyClass : public MyClass
{
public:
    MOCK_METHOD(void, myCallback, (void *parameter), (override));
};

class CallbackTest : public ::testing::Test
{
protected:
    int buttonPin1 = 5;
    bool onRaising1 = true;
    int buttonPin2 = 14;
    bool onRaising2 = false;
    int *counterCheck;

    ButtonModule *buttonModule1;
    ButtonModule *buttonModule2;
    MockMyClass *mockMyClass;

    void SetUp() override
    {
        buttonModule1 = new ButtonModule(buttonPin1, nullptr, onRaising1);
        buttonModule2 = new ButtonModule(buttonPin2, nullptr, onRaising2);
        counterCheck = new int(0);
        mockMyClass = new MockMyClass();

        pinMode(buttonPin2, OUTPUT);
        digitalWrite(buttonPin1, !onRaising1);
    }

    void TearDown() override
    {
        delete buttonModule1;
        delete buttonModule2;
        delete counterCheck;
        delete mockMyClass;
    }
};

TEST_F(CallbackTest, OnSinglePressByFunction)
{
    buttonModule1->setSinglePressCallback(myCallback, counterCheck);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->setSinglePressCallback(myCallback, counterCheck);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnSinglePressByLambda)
{
    buttonModule1->setSinglePressCallback([](void *parameter)
                                          { ((int *)parameter)[0]++; }, counterCheck);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->setSinglePressCallback([](void *parameter)
                                          { ((int *)parameter)[0]++; }, counterCheck);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnSinglePressByLambda_mock_callback)
{
    buttonModule1->setSinglePressCallback([](void *parameter)
                                          { ((MockMyClass *)parameter)->myCallback(parameter); }, mockMyClass);
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass)).Times(1);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    buttonModule2->setSinglePressCallback([](void *parameter)
                                          { ((MockMyClass *)parameter)->myCallback(parameter); }, mockMyClass);
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass)).Times(1);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
}

TEST_F(CallbackTest, OnDoublePressByFunction)
{
    buttonModule1->setDoublePressCallback(myCallback, counterCheck);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->setDoublePressCallback(myCallback, counterCheck);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnDoublePressByLambda)
{
    buttonModule1->setDoublePressCallback([](void *parameter)
                                          { ((int *)parameter)[0]++; }, counterCheck);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->setDoublePressCallback([](void *parameter)
                                          { ((int *)parameter)[0]++; }, counterCheck);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnDoublePressByLambda_mock_callback)
{
    buttonModule1->setDoublePressCallback([](void *parameter)
                                          { ((MockMyClass *)parameter)->myCallback(parameter); }, mockMyClass);
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass)).Times(1);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    buttonModule2->setDoublePressCallback([](void *parameter)
                                          { ((MockMyClass *)parameter)->myCallback(parameter); }, mockMyClass);
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass)).Times(1);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
}

TEST_F(CallbackTest, OnLongPressByFunction)
{
    buttonModule1->setLongPressCallback(myCallback, counterCheck);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(2500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->setLongPressCallback(myCallback, counterCheck);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(2500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnLongPressByLambda)
{
    buttonModule1->setLongPressCallback([](void *parameter)
                                        { ((int *)parameter)[0]++; }, counterCheck);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(2500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->setLongPressCallback([](void *parameter)
                                        { ((int *)parameter)[0]++; }, counterCheck);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(2500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnLongPressByLambda_mock_callback)
{
    buttonModule1->setLongPressCallback([](void *parameter)
                                        { ((MockMyClass *)parameter)->myCallback(parameter); }, mockMyClass);
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass)).Times(1);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(2500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    buttonModule2->setLongPressCallback([](void *parameter)
                                        { ((MockMyClass *)parameter)->myCallback(parameter); }, mockMyClass);
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass)).Times(1);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(2500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
}