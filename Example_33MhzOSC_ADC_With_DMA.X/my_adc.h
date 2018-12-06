/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   my_adc library
 * Author: Juan David Velasquez Rosero 
 * Comments: 
 * Revision history: V1.0 29/09/2018 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MY_ADC_H
#define	MY_ADC_H


#include <stdint.h>
#include <stdbool.h>

/*** ADC Channel Definitions *****************************************/
#define MY_ADC_CHANNEL_POTENTIOMETER ADC_CHANNEL_5
#define MY_ADC_CHANNEL_TEMPERATURE_SENSOR ADC_CHANNEL_4
#define MY_ADC_CHANNEL_UNO ADC_CHANNEL_1

typedef enum
{
    MY_ADC_CHANNEL_5 = 5,
    MY_ADC_CHANNEL_4 = 4,
    MY_ADC_CHANNEL_1 = 1        
} MY_ADC_CHANNEL;

typedef enum
{
    AN0 = 1,
    AN1 = 2,
    AN2 = 4,
    AN3 = 8,
    AN4 = 16,
    AN5 = 32,
    AN6 = 64,
    AN7 = 128,
    AN8 = 256,
    AN9 = 512,
    AN10 = 1024,
    AN11 = 2048,
    AN12 = 4096,
    AN13 = 8192,
    AN14 = 16384,
    AN15 = 32768
} MY_ADC_CHANNELS;

typedef enum
{
    ADC_MODE_AUTO,
    ADC_MODE_MANUAL
} MY_ADC_MODE;

typedef struct adc_struct
{
    MY_ADC_MODE adc_mode;
    
} ADC_MODULE;

bool ADC_SetConfiguration(MY_ADC_MODE mode);
bool ADC1_SetConfiguration_Auto(void);
bool ADC2_SetConfiguration_Auto(void);

bool ADC1_SetConfiguration_With_DMA0(uint16_t *channels, uint16_t size);
bool DMA0_Init(uint16_t size);

void ADC1_selectChannel(MY_ADC_CHANNELS channel);
void ADC2_selectChannel(MY_ADC_CHANNELS channel);

uint16_t readADC1_Manual();
uint16_t readADC2_Manual();

#endif	/* MY_ADC_H */

