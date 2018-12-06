#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#include "adc.h"
#include "lcd.h"
#include "timer_10us.h"
#include "buttons.h"
#include "leds.h"
#include "my_adc.h"
#include "my_osc.h"
 
#define FCY 33165000
#include <libpic30.h>  // Libreria Para usar __delay_ms(); 

static void TimerEventHandler( void );

/*
 * Numero de canales a leer por el ADC1
 * Para este ejemplo 2 canales (AN0 y AN3) 
 * Pueden ser mas si se desea 
 */

#define NUM_ADC1_CHANNEL	2
uint16_t BufferADC[NUM_ADC1_CHANNEL] __attribute__((space(dma)));

void __attribute__((__interrupt__)) _DMA0Interrupt(void){
    IFS0bits.DMA0IF = 0; //Clear the DMA0 Interrupt Flag
}

int main(void) {
    uint16_t adcAN0, adcAN3, adcAN5;
    //------ CONFIGURACION TIMER 
    TIMER_SetConfiguration ( TIMER_CONFIGURATION_10US );//Se configura la duracion de 1 TICK = 10us (Maximo valor estable conseguido)
    TIMER_RequestTick( &TimerEventHandler, 5 );// La fucion del timer se activa cada 5 TICKS es decir 5*10us = 50us
    
    //------- CONGIGURACION OSCILADOR Y LCD ---------
    OsciladorConfig();// Se configura el oscilador a 33.16 Mhz (Maximo valor estable conseguido)
    LCD_Initialize();
    
    //------- CONGIGURACION ADC ---------
    ADC_SetConfiguration(ADC_MODE_AUTO);//Configuro el ADC2 en modo continuo (Solo debo hacer la lectura de ADC2BUF0)
    ADC2_selectChannel(AN5);//COnfiguro la entrada analogica a leer por el ADC2 ( AN0 - AN15 )

    /* 
     * Esta configuracion es necesaria para el funcionamiento de ADC1 con DMA
     * En el arreglo se introduce las entradas analogicas que desea leer EN ORDEN ASCENDENTE 
     * Por ejemplo channels[NUM_ADC1_CHANNEL] = {AN2 AN3 AN8 AN10} 
     * Tenga en cuenta que el NUM_ADC1_CHANNEL debe ser configurado con el numero de entradas a leer
     * Las lecturas se recuperan desde el BufferADC[x] = la lectura del channels[i]
     * Es decir
     * BufferADC[0] = Lectura de AN2
     * BufferADC[1] = Lectura de AN3
     * BufferADC[2] = Lectura de AN8
     * BufferADC[3] = Lectura de AN10
     */
    
    uint16_t channels[NUM_ADC1_CHANNEL] = {AN0, AN3}; // BufferADC[0] = Lectura de AN0 // BufferADC[1] = Lectura de AN3
    
    uint16_t size = sizeof(channels)/ sizeof(channels[0]);
    DMA0STA = __builtin_dmaoffset(BufferADC); //
    ADC1_SetConfiguration_With_DMA0(channels, size);
    DMA0_Init(size);
    
    
    while(1){
        adcAN0 = BufferADC[0];
        adcAN3 = BufferADC[1];
        adcAN5 = ADC2BUF0;
        printf( "\f\nA0:%d A1:%d A2:%d",adcAN0,adcAN3,adcAN5);
    }
    
}

static void TimerEventHandler(void)
{
    //Function Timer
}

