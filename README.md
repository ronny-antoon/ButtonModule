# ButtonModule Library

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate/?hosted_button_id=BACPRJTAU4G4E)
[![Latest](https://img.shields.io/github/v/tag/ronny-antoon/ButtonModule?color=red&label=last+release)](https://github.com/ronny-antoon/ButtonModule/releases)
[![Latest](https://badges.registry.platformio.org/packages/ronny-antoon/library/ButtonModule.svg)](https://registry.platformio.org/libraries/ronny-antoon/ButtonModule)
[![UnitTests](https://github.com/ronny-antoon/ButtonModule/actions/workflows/build-and-test-embeded.yaml/badge.svg)](https://github.com/ronny-antoon/ButtonModule/actions/workflows/build-and-test-embeded.yaml)

The ButtonModule Library is a C++ library providing an abstract interface and implementation for button modules. It facilitates easy integration of button functionalities into ESP32/Arduino projects, offering callback functions for single press, double press, and long press events.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [API](#API)
- [Example](#example)
- [License](#license)
- [Contributions](#contributions)
- [Platformio Registry](#platformio-registry)

## Introduction

The ButtonModule Library simplifies the management of button modules in ESP32/Arduino projects. It provides an abstract interface for handling button events and an implementation for detecting single press, double press, and long press actions.

## Features

- `Button Event Handling`: Abstracts button interactions with callback functions for single press, double press, and long press events.
- `Configurability`: Allows users to configure parameters such as check interval, debounce time, long press time, and time between double presses.
- `Edge Detection`: Supports both rising and falling edge detection for button presses.

## Installation

**Method 1**:
To use the **ButtonModule** library in your PlatformIO project, follow these steps:

1. Open "platformio.ini", a project configuration file located in the root of PlatformIO project.

2. Add the following line to the `lib_deps` option of `[env:]` section:
`ronny-antoon/ButtonModule@^1.0.1`

3. Build a project, PlatformIO will automatically install dependencies.

**Method 2**:
To use the **ButtonModule** library in your Arduino project, follow these steps:

1. Download the latest release from the [GitHub repository](https://github.com/ronny-antoon/ButtonModule).

2. In the Arduino IDE, click "Sketch" -> "Include Library" -> "Add .ZIP Library" and select the downloaded `.zip` file.

3. Make sure to link your project with the necessary ESP-IDF libraries for NVS support.

## Usage

1. Create an instance of the ButtonModule class, specifying the button's pin and edge detection preference.
```cpp
#include <ButtonModule.hpp>

// Create a ButtonModule instance with pin 5 and rising edge detection
ButtonModule buttonModule(5, true);
```

2. Set callback functions for button events.
```cpp
// Set callback functions for button events
buttonModule.onSinglePress(singlePressCallback, NULL);
buttonModule.onDoublePress(doublePressCallback, NULL);
buttonModule.onLongPress(longPressCallback, NULL);
```

3. Start listening for button triggers with specified configuration parameters.
```cpp
// Start listening with custom configuration
buttonModule.startListening();
```

4. Handle button events in your callback functions.
```cpp
void singlePressCallback(void *)
{
    // Handle single press event
}

void doublePressCallback(void *)
{
    // Handle double press event
}

void longPressCallback(void *)
{
    // Handle long press event
}
```

## API

The ButtonModule Library provides the following classes and interfaces:
- `ButtonModuleInterface`: Abstract interface for button modules.
- `ButtonModule`: Implementation of the ButtonModuleInterface for button modules.

Detailed documentation and usage examples can be found in the library source code.

## Example

Here's a simple example of how to use the ButtonModule library to configure and handle button events:

```cpp
#include <ButtonModule.hpp>

// Create a ButtonModule instance with pin 5 and rising edge detection
ButtonModule buttonModule(5, true);

// Set callback functions for button events
buttonModule.onSinglePress(singlePressCallback, NULL);
buttonModule.onDoublePress(doublePressCallback, NULL);
buttonModule.onLongPress(longPressCallback, NULL);

void setup()
{
    // Start listening with custom configuration
    buttonModule.startListening(10000, 30, 90, 1000, 500);
}

void loop()
{
    // Your main loop code goes here
}
```

For a complete list of methods and parameters, refer to the class documentation in the source code.

## License

This library is provided under the MIT License. You are free to use, modify, and distribute it as needed.

## Contributions

If you'd like to contribute to the Database Library, please follow these guidelines:
1. Fork the repository.
2. Make your changes and document them.
3. Submit a pull request.

## Author

This library was created by Ronny Antoon. You can contact me at [ronny.antoon@gmail.com] for any questions or feedback.

## Platformio Registry

https://registry.platformio.org/libraries/ronny-antoon/ButtonModule