#!/bin/bash

#JFlash.exe -openprj"D:\tanzhongqiang\stm32_workspace\LiteOS_STM32F7\STM32F767.jflash" -open"D:\tanzhongqiang\stm32_workspace\LiteOS_STM32F7\LiteOS\out\STM32F769IDISCOVERY\Huawei_LiteOS.bin,0x8000000" -auto -startapp -exit
#JFlash.exe -openprj"D:\tanzhongqiang\stm32_workspace\LiteOS_STM32F7\STM32F767.jflash" -open"D:\tanzhongqiang\stm32_workspace\LiteOS_STM32F7\LiteOS\out\STM32F767_Nucleo\Huawei_LiteOS.bin,0x8000000" -auto -startapp -exit
JFlash.exe -openprj"./STM32F767.jflash" -open"./build/stm32f767.bin,0x8000000" -auto -startapp -exit