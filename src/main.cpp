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

void setup()
{
    Serial.begin(115200);

    Serial.printf("Minimum heap that has ever been available: %u\n", esp_get_minimum_free_heap_size());
    Serial.printf("Before Initialize, free heap: %u\n", esp_get_free_heap_size());

    MultiPrinterLogger *logger = new MultiPrinterLogger();
    logger->addPrinter(&Serial);
    logger->setLogLevel(MultiPrinterLoggerInterface::LogLevel::VERBOSE);
    logger->setColorEnabled(true);

    ButtonModule *buttonModule = new ButtonModule(5, true, logger);

    int *parameter = new int(10);
    buttonModule->onSinglePress(singlePressCallback, parameter);

    buttonModule->startListening(2000, nullptr, 30, 90, 1000, 500);

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