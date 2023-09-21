# Description
LM75A Library for STM32 HAL Driver
* Author: Umut Avcı
* IDE: STM32CubeIDE
* STM32: STM32F407G-DISC1
* Language: C

# Features
* LM75A structure holds I2C bus pointer, adress and decimal mode values
* Multiple LM75A's can be used and their structure values are independent with each other
* Three decimal modes: 0.5, 0.25, 0.125 (LM75A_DEC_5, LM75A_DEC_25, LM75A_DEC_125)
* Works with both 9-bit and 11-bit temperature versions
* Library is portable and should work with different STM32 boards
