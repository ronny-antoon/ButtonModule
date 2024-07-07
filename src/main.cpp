#include <Arduino.h>
#include <esp_system.h>

#include "ButtonModule.hpp"
#include <MultiPrinterLogger.hpp>
#include <MultiPrinterLoggerInterface.hpp>

#define INCLUDE_uxTaskGetStackHighWaterMark 1 // TaskHandle_t

void singlePressCallback(void *parameter)
{
    int *value = (int *)parameter;
    Serial.printf("Single Press Callback, parameter: %d\n", *value);

    Serial.printf("High watermark: %u\n", uxTaskGetStackHighWaterMark(NULL));
}

void doublePressCallback(void *parameter)
{
    int *value = (int *)parameter;
    Serial.printf("Double Press Callback, parameter: %d\n", *value);
}

void longPressCallback(void *parameter)
{
    int *value = (int *)parameter;
    Serial.printf("Long Press Callback, parameter: %d\n", *value);
}

void setup()
{
    Serial.begin(115200);

    Serial.printf("Minimum heap that has ever been available: %u\n", esp_get_minimum_free_heap_size());
    Serial.printf("Before Initialize, free heap: %u\n", esp_get_free_heap_size());

    MultiPrinterLogger *logger = new MultiPrinterLogger();
    logger->addPrinter(&Serial);
    logger->setLogLevel(MultiPrinterLoggerInterface::LogLevel::VERBOSE);
    logger->setColorEnabled(true);

    // ButtonModule *buttonModule = new ButtonModule(5, logger, true);

    int *parameter = new int(10);
    // buttonModule->setSinglePressCallback(singlePressCallback, parameter);
    // buttonModule->setDoublePressCallback(doublePressCallback, parameter);
    // buttonModule->setLongPressCallback(longPressCallback, parameter);

    ButtonModule *buttonModule1 = new ButtonModule(23, logger, false);
    buttonModule1->setSinglePressCallback(singlePressCallback, parameter);
    buttonModule1->setDoublePressCallback(doublePressCallback, parameter);
    buttonModule1->setLongPressCallback(longPressCallback, parameter);

    Serial.printf("Minimum heap that has ever been available: %u\n", esp_get_minimum_free_heap_size());
    Serial.printf("After Initialize, free heap: %u\n", esp_get_free_heap_size());
}

int counter = 0;
void loop()
{
    delay(1000);
    Serial.printf("Loop %d\n", counter++);

    if (counter == 10)
    {
        counter = 0;
        xTASK_LIST_PRINT();
    }
}