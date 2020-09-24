

/*
 * Logger.cpp
 *
 *  Created on: March 27, 2020
 *      Author: Quincy Jones
 *
 * Copyright (c) <2020> <Quincy Jones - quincy@implementedrobotics.com/>
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

// Primary Include
#include <nomad_app.h>

// C System Files
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>

// C++ System Files

// Project Includes
#include "main.h"
#include <Peripherals/uart.h>
#include <Peripherals/spi.h>
#include <Peripherals/gpio.h>
#include <Peripherals/flash.h>

#include <DRV8323.h>
#include <LEDService.h>
#include <Logger.h>

osThreadId_t sig_thread;
extern "C" void signal_set(void *arg)
{
    Logger::Instance().Print("Start: \r\n");
    for (;;)
    {
        uint32_t flag = osThreadFlagsWait(0x3, osFlagsWaitAll, 3000);
        Logger::Instance().Print("Flag: %d\r\n", flag);
    }

    osThreadExit();
}

void StartCommunicationThreads()
{
    // Start UART
    osThreadAttr_t task_attributes;
    memset(&task_attributes, 0, sizeof(osThreadAttr_t));
    task_attributes.name = "UART_TASK";
    task_attributes.priority = (osPriority_t) osPriorityNormal;
    task_attributes.stack_size = 512;
    osThreadNew(init_uart_threads, (void *)USART2, &task_attributes);

    // Start CAN
}

void StartLEDService()
{
    // Start LED Service
    osThreadAttr_t task_attributes;
    memset(&task_attributes, 0, sizeof(osThreadAttr_t));
    task_attributes.name = "LED_TASK";
    task_attributes.priority = (osPriority_t) osPriorityNormal;
    task_attributes.stack_size = 2048;

    osThreadNew(status_led_thread, NULL, &task_attributes);
}

void DebugTask()
{
    // for(;;)
    // {
    //     //LEDService::Instance().Blink(100,900);
    //     LEDService::Instance().On();
    //     osDelay(500);
    //     LEDService::Instance().Off();
    //     osDelay(500);
    //     LEDService::Instance().Blink(3000, 2000);
    //     osDelay(6000);
    // }

// Spi TEST

// // SPI1 = Encoder
// GPIO_t mosi = {ENC_MOSI_GPIO_Port, ENC_MOSI_Pin};
// GPIO_t miso = {ENC_MISO_GPIO_Port, ENC_MISO_Pin};
// GPIO_t nss = {ENC_CS_GPIO_Port, ENC_CS_Pin};

// SPIDevice encoder_dev(SPI1, mosi, miso, nss);
// encoder_dev.Enable();
// uint16_t position = 0;

// for (;;)
// {
//     encoder_dev.Select();
//     position = encoder_dev.Receive16();
//     position &= 0x3FFF; // Data in last 14 bits.
//     encoder_dev.Deselect();

//     Logger::Instance().Print("Pos: %d\r\n", position);
//     osDelay(100);
// }

    // // Signal Test
    // osThreadAttr_t task_attributes;
    // memset(&task_attributes, 0, sizeof(osThreadAttr_t));
    // task_attributes.name = "SIGNAL_TEST";
    // task_attributes.priority = (osPriority_t) osPriorityNormal;
    // task_attributes.stack_size = 2048;

    // sig_thread = osThreadNew(signal_set, NULL, &task_attributes);

}

void FlashTest()
{
    // test_save save;
    // save.signature=100;
    // save.version=10;
    
    // bool status_open = FlashDevice::Instance().Open(ADDR_FLASH_PAGE_252, sizeof(save), FlashDevice::WRITE);
    // bool status = FlashDevice::Instance().Write(0, (uint8_t *)&save, sizeof(save));
    // FlashDevice::Instance().Close();

    // Logger::Instance().Print("Status: %d\r\n", status);


    // test_save load;
    // load.signature = 0;
    // load.version = 0;
    // load.b = 0;
    // FlashDevice::Instance().Open(ADDR_FLASH_PAGE_252, sizeof(load), FlashDevice::READ);
    // FlashDevice::Instance().Read(0, (uint8_t *)&load, sizeof(load));
    // FlashDevice::Instance().Close();

    // Logger::Instance().Print("Load: %d\r\n", load.signature);
}

void DRV_Test()
{
    // Spi TEST

// SPI1 = Encoder
GPIO_t mosi = {DRV_MOSI_GPIO_Port, DRV_MOSI_Pin};
GPIO_t miso = {DRV_MISO_GPIO_Port, DRV_MISO_Pin};
GPIO_t nss = {DRV_CS_GPIO_Port, DRV_CS_Pin};

GPIO_t enable = {DRV_ENABLE_GPIO_Port, DRV_ENABLE_Pin};
GPIO_t n_fault = {DRV_nFAULT_GPIO_Port, DRV_nFAULT_Pin};

SPIDevice drv_spi(SPI2, mosi, miso, nss);
drv_spi.Enable(); // Enable SPI 

DRV8323 drv_dev(&drv_spi, enable, n_fault);

// Enable DRV
drv_dev.EnableDriver();

 // Give it time to power up
osDelay(10);

// Init Driver Settings
drv_dev.Init();

// //for (;;)
// //{

//     uint16_t test = drv_dev.test();   
 //    Logger::Instance().Print("Status: %X\r\n", 10);
//     osDelay(100);
// //}
// drv_dev.DisableDriver();
// osDelay(10000);
}



extern "C" int app_main()
{

    Logger::Instance().Enable(true);
    // Start Communications Threads (UART/CAN)
    StartCommunicationThreads();

    // Delay
    osDelay(500);
    
    // Init LED Service Task
    StartLEDService();

    // Delay
    osDelay(500);

    //DRV_Test(); 
    // FlashTest();
    // Init Misc Polling Task

    // Init Motor Control Task

    // Init a temp debug Task
    DebugTask();    
    // Infinite Loop.
    for (;;)
    {
        osThreadFlagsSet(sig_thread, 0x3);
        osDelay(6000);
    }

    // Should not get here
    return 0;
}