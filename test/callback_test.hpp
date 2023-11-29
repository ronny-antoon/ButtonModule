#ifndef CALLBACK_TEST_HPP
#define CALLBACK_TEST_HPP

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
    uint32_t _startFreeHeap = 0;
    int *counterCheck;

    ButtonModule *buttonModule1;
    ButtonModule *buttonModule2;
    MockMyClass *mockMyClass;

    void SetUp() override
    {
        _startFreeHeap = ESP.getFreeHeap(); // Record initial free heap size
        buttonModule1 = new ButtonModule(buttonPin1, onRaising1);
        buttonModule2 = new ButtonModule(buttonPin2, onRaising2);
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
        if (ESP.getFreeHeap() != _startFreeHeap)
            FAIL() << "Memory leak of " << _startFreeHeap - ESP.getFreeHeap() << " bytes"; // Fail the test if there is a memory leak
    }
};

TEST_F(CallbackTest, OnSinglePressByFunction)
{
    buttonModule1->onSinglePress(myCallback,
                                 counterCheck);
    buttonModule1->startListening();
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->onSinglePress(myCallback,
                                 counterCheck);
    buttonModule2->startListening();
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnSinglePressByLamda)
{
    buttonModule1->onSinglePress([](void *parameter)
                                 { ((int *)parameter)[0]++; },
                                 counterCheck);
    buttonModule1->startListening();
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->onSinglePress([](void *parameter)
                                 { ((int *)parameter)[0]++; },
                                 counterCheck);
    buttonModule2->startListening();
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnSinglePressByLamda_mock_callback)
{
    buttonModule1->onSinglePress([](void *parameter)
                                 { ((MockMyClass *)parameter)->myCallback(parameter); },
                                 mockMyClass);
    buttonModule1->startListening();
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass))
        .Times(1);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    buttonModule2->onSinglePress([](void *parameter)
                                 { ((MockMyClass *)parameter)->myCallback(parameter); },
                                 mockMyClass);
    buttonModule2->startListening();
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass))
        .Times(1);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
}

TEST_F(CallbackTest, OnDoublePressByFunction)
{
    buttonModule1->onDoublePress(myCallback,
                                 counterCheck);
    buttonModule1->startListening();
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

    buttonModule2->onDoublePress(myCallback,
                                 counterCheck);
    buttonModule2->startListening();
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

TEST_F(CallbackTest, OnDoublePressByLamda)
{
    buttonModule1->onDoublePress([](void *parameter)
                                 { ((int *)parameter)[0]++; },
                                 counterCheck);
    buttonModule1->startListening();
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

    buttonModule2->onDoublePress([](void *parameter)
                                 { ((int *)parameter)[0]++; },
                                 counterCheck);
    buttonModule2->startListening();
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

TEST_F(CallbackTest, OnDoublePressByLamda_mock_callback)
{
    buttonModule1->onDoublePress([](void *parameter)
                                 { ((MockMyClass *)parameter)->myCallback(parameter); },
                                 mockMyClass);
    buttonModule1->startListening();
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass))
        .Times(1);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    buttonModule2->onDoublePress([](void *parameter)
                                 { ((MockMyClass *)parameter)->myCallback(parameter); },
                                 mockMyClass);
    buttonModule2->startListening();
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass))
        .Times(1);
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
    buttonModule1->onLongPress(myCallback,
                               counterCheck);
    buttonModule1->startListening();
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(1500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->onLongPress(myCallback,
                               counterCheck);
    buttonModule2->startListening();
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(1500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnLongPressByLamda)
{
    buttonModule1->onLongPress([](void *parameter)
                               { ((int *)parameter)[0]++; },
                               counterCheck);
    buttonModule1->startListening();
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(1500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    EXPECT_EQ(*counterCheck, 1);

    buttonModule2->onLongPress([](void *parameter)
                               { ((int *)parameter)[0]++; },
                               counterCheck);
    buttonModule2->startListening();
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(1500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    EXPECT_EQ(*counterCheck, 2);
}

TEST_F(CallbackTest, OnLongPressByLamda_mock_callback)
{
    buttonModule1->onLongPress([](void *parameter)
                               { ((MockMyClass *)parameter)->myCallback(parameter); },
                               mockMyClass);
    buttonModule1->startListening();
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass))
        .Times(1);
    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(1500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    buttonModule2->onLongPress([](void *parameter)
                               { ((MockMyClass *)parameter)->myCallback(parameter); },
                               mockMyClass);
    buttonModule2->startListening();
    EXPECT_CALL(*mockMyClass, myCallback(mockMyClass))
        .Times(1);
    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(1500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
}

// test the max allocatable heap after some press events
TEST_F(CallbackTest, HeapUsage)
{
    buttonModule1->onSinglePress(myCallback,
                                 counterCheck);
    buttonModule1->onDoublePress(myCallback,
                                 counterCheck);
    buttonModule1->onLongPress(myCallback,
                               counterCheck);
    buttonModule1->startListening();

    buttonModule2->onSinglePress(myCallback,
                                 counterCheck);
    buttonModule2->onDoublePress(myCallback,
                                 counterCheck);
    buttonModule2->onLongPress(myCallback,
                               counterCheck);
    buttonModule2->startListening();

    int maxAllocatableHeap = ESP.getMaxAllocHeap();

    pinMode(buttonPin1, OUTPUT);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    digitalWrite(buttonPin1, onRaising1);
    delay(150);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);
    digitalWrite(buttonPin1, onRaising1);
    delay(1500);
    digitalWrite(buttonPin1, !onRaising1);
    delay(150);

    pinMode(buttonPin2, OUTPUT);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    digitalWrite(buttonPin2, onRaising2);
    delay(150);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);
    digitalWrite(buttonPin2, onRaising2);
    delay(1500);
    digitalWrite(buttonPin2, !onRaising2);
    delay(150);

    EXPECT_EQ(ESP.getMaxAllocHeap(), maxAllocatableHeap);
}
#endif // CALLBACK_TEST_HPP