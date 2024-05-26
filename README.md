# DipRadSW

## Description

DipRadSW is a software project developed for custom PCB with STM32F407VET for Masters degree on FER.

## Peripherals

- USB type B
- RS232 serial (uart4)
- Micro USB B type (usart1)
- Encoder (Tim1)
- 6 buttons with 7th button as reset
- Serial Wire for flashing and debugging with JTAG capabilities
- 16 GPIOs for upgrading to new features
- 1 SPI for LCD (spi1)
- 1 SPI for Touchpad (spi3)
- 1 SPI for SD card (spi2)

## Installation

Provide step by step series of examples and explanations about how to get your development environment running.

```bash
git clone https://github.com/Zugislav/DipRadSW.git
cd DipRadSW
```

using STM32 extension for VSCode and OpenOCD for flashing and debugging with Makefile to build the project.

## Code organisation
We have 4 task using FreeRTOS
1. LCD Task
    Main task for printing text on LCD about our frequency and handling all touchpad inputs
    We can comunicate with F2-F4 buttons that has the same functions as buttons on screen
    Updating frequency from encoder rotations to the pointer to the value
    Having second screen for handling input of frequency by choosing
2. Button Task
    Handling 2 buttons interrupt to potentialy add new features
    Handling Encoder SW IRQ to enable smaller pulses so we can have a precise frequency
3. Encoder Task
    Updating the LCD Task with new values from Timer hadnling encoder values

Printing on serial is done by mutex holding the task not printing garbage.
Encoder values are transmitted by queue in FreeRTOS

## Upgrades
1. New HW printed with fewer mistakes
2. USB as Custom HID that encoder can function as mouse
3. FAT32 enable with microSD card on SPI2

