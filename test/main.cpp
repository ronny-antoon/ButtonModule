#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "isPressed_test.hpp"
#include "callback_test.hpp"
// #include "startListening_test.hpp"
// #include "stopListening_test.hpp"

void setup()
{
    Serial.begin(115200);
    TaskTracker::getInstance();
    ::testing::InitGoogleMock();
}

void loop()
{
    if (RUN_ALL_TESTS())
        ;

    delay(1000);

    Serial.println("-----------------------------------Finished all tests!-----------------------------------");

    delay(100000);
}