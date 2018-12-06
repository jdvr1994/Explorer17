/*
 * File:   my_adc.c
 * Author: Juan David Velasquez Rosero 
 *
 * Created on 29 de septiembre de 2018, 10:22 AM
 */
#include "my_adc.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

bool ADC_SetConfiguration(MY_ADC_MODE mode){
    if(mode == ADC_MODE_AUTO){
        ADC1_SetConfiguration_Auto();
        ADC2_SetConfiguration_Auto();
    }
    return true;
}

bool ADC1_SetConfiguration_Auto(void)
{
    AD1CON1bits.ADSIDL=0; //Continuar el funcionamiento del modulo en el modo inactivo
    AD1CON1bits.FORM=0; // Data Output Format bits --- 00 = Integer (DOUT = 0000 dddd dddd dddd)
    AD1CON1bits.SSRC=7; // Fuente del disparo de conversion --- 111 = Internal counter finaliza el muestreo e inicia la conversion (auto convert)
    AD1CON1bits.ADON = 1; // ADON: A/D Operating Mode bit --- 1 = modulo ADC operando
    AD1CON1bits.ASAM=1; // A/D Sample Auto-Start bit --- 1 = El muestreo comienza inmediatamente después de que finaliza la última conversión. El bit SAMP se establece automáticamente.
    AD1CON1bits.SAMP=1; // SAMP: A/D Sample Enable bit ---- 1 = At least one A/D sample/hold amplifier is sampling
    AD1CON2bits.VCFG=7; // Voltage Reference Configuration bits --- 1xx = A/D VREFH( AVDD ) y A/D VREFL( AVSS )
    AD1CON2bits.CSCNA=1; // Scan Input Selections for CH0+ S/H Input for MUX A Input Multiplexer Setting bit --- 1 = Scan inputs
    // AD1CON2bits.SMPI::: El numero de secuencias muestra/conversion por interrupcion es SMPI + 1,
    /* Ademas de determinar cuando la interrupcion ocurre, estos bits tambien determinan 
     * cuantas conversiones deben ocurrir antes de que el ADC comience a almacenar informacion 
     * en el primer Buffer (o la alternativa al primer buffer cuando AD1CON2bits.BUFM = 1)
     * 
     * Por ejemplo si SMPI = 1 entonces habra dos conversiones por interrupcion. 
     * La primer conversion sera almacenada en AD1BUF0 y la segunda en AD1BUF1.
     * Despues de la segunda conversion la bandera de interrupcion del ADC sera configurada.
     * La siguiente conversion sera almacenada en AD1BUF0 si AD1CON2bits.BUFM = 0 
     * o AD1BUF8 si AD1CON2bits.BUFM = 1
     */
    AD1CON1bits.SIMSAM = 0; // Enable Sequential Sampling
    AD1CON2bits.SMPI=1; // Sample/Convert Sequences Per Interrupt Selection bits -- 0001 = Interrupts at the completion of conversion for each 2nd sample/convert sequence
    AD1CON2bits.BUFM=0; // 0 = Buffer configured as one 16-word buffer ADCBUF(15...0)
    AD1CON2bits.ALTS=0; // Alternate Input Sample Mode Select bit --- 0 = Always use MUX A input multiplexer settings*/
    AD1CON3bits.SAMC=1; // Auto Sample Time = 1 * TAD
    AD1CON3bits.ADRC=1;
    AD1CON3bits.ADCS=0;//00000000 = TCY · (ADCS<7:0> + 1) = 1 · TCY = TAD
    AD1CON1bits.AD12B = 1; // Select 12-bit mode
    
    return true;
}

bool ADC2_SetConfiguration_Auto(void)
{
    AD2CON1bits.ADSIDL=0; //Continuar el funcionamiento del modulo en el modo inactivo
    AD2CON1bits.FORM=0; // Data Output Format bits --- 00 = Integer (DOUT = 0000 dddd dddd dddd)
    AD2CON1bits.SSRC=7; // Fuente del disparo de conversion --- 111 = Internal counter finaliza el muestreo e inicia la conversion (auto convert)
    AD2CON1bits.ADON = 1; // ADON: A/D Operating Mode bit --- 1 = modulo ADC operando
    AD2CON1bits.ASAM=1; // A/D Sample Auto-Start bit --- 1 = El muestreo comienza inmediatamente después de que finaliza la última conversión. El bit SAMP se establece automáticamente.
    AD2CON1bits.SAMP=1; // SAMP: A/D Sample Enable bit ---- 1 = At least one A/D sample/hold amplifier is sampling
    AD2CON2bits.VCFG=7; // Voltage Reference Configuration bits --- 1xx = A/D VREFH( AVDD ) y A/D VREFL( AVSS )
    AD2CON2bits.CSCNA=1; // Scan Input Selections for CH0+ S/H Input for MUX A Input Multiplexer Setting bit --- 1 = Scan inputs
    // AD1CON2bits.SMPI::: El numero de secuencias muestra/conversion por interrupcion es SMPI + 1,
    /* Ademas de determinar cuando la interrupcion ocurre, estos bits tambien determinan 
     * cuantas conversiones deben ocurrir antes de que el ADC comience a almacenar informacion 
     * en el primer Buffer (o la alternativa al primer buffer cuando AD1CON2bits.BUFM = 1)
     * 
     * Por ejemplo si SMPI = 1 entonces habra dos conversiones por interrupcion. 
     * La primer conversion sera almacenada en AD1BUF0 y la segunda en AD1BUF1.
     * Despues de la segunda conversion la bandera de interrupcion del ADC sera configurada.
     * La siguiente conversion sera almacenada en AD1BUF0 si AD1CON2bits.BUFM = 0 
     * o AD1BUF8 si AD1CON2bits.BUFM = 1
     */
    AD2CON1bits.SIMSAM = 0; // Enable Sequential Sampling
    AD2CON2bits.SMPI=1; // Sample/Convert Sequences Per Interrupt Selection bits -- 0001 = Interrupts at the completion of conversion for each 2nd sample/convert sequence
    AD2CON2bits.BUFM=0; // 0 = Buffer configured as one 16-word buffer ADCBUF(15...0)
    AD2CON2bits.ALTS=0; // Alternate Input Sample Mode Select bit --- 0 = Always use MUX A input multiplexer settings*/
    AD2CON3bits.SAMC=1; // Auto Sample Time = 1 * TAD
    AD2CON3bits.ADRC=1;
    AD2CON3bits.ADCS=0;// 00000000 = TCY · (ADCS<7:0> + 1) = 1 · TCY = TAD
    AD2CON1bits.AD12B = 1; // Select 12-bit mode
    
    return true;
}

bool ADC1_SetConfiguration_With_DMA0(uint16_t *channels, uint16_t size)
{
    
    AD1CON1bits.ADON = 0; // ADON: A/D Operating Mode bit --- 0 = modulo ADC apagado para configurar
    AD1CON1bits.AD12B = 1; // Select 12-bit mode
    AD1CON2bits.CHPS = 0; // Select 0-channel mode (Convert ONLY channel CH0))
    AD1CON1bits.ADSIDL=0; //Continuar el funcionamiento del modulo en el modo inactivo
    AD1CON1bits.FORM=0; // Data Output Format bits --- 00 = Integer (DOUT = 0000 dddd dddd dddd)
    AD1CON1bits.SSRC=7; // Fuente del disparo de conversion --- 111 = Internal counter finaliza el muestreo e inicia la conversion (auto convert)
    AD1CON1bits.ASAM=1; // A/D Sample Auto-Start bit --- 1 = El muestreo comienza inmediatamente después de que finaliza la última conversión. El bit SAMP se establece automáticamente.
    AD1CON1bits.SAMP=1; // SAMP: A/D Sample Enable bit ---- 1 = At least one A/D sample/hold amplifier is sampling
    AD1CON2bits.VCFG=7; // Voltage Reference Configuration bits --- 1xx = A/D VREFH( AVDD ) y A/D VREFL( AVSS )
    AD1CON2bits.CSCNA=1; // (Enable Channel Scanning) Scan Input Selections for CH0+ S/H Input for MUX A Input Multiplexer Setting bit --- 1 = Scan inputs
    // AD1CON2bits.SMPI::: El numero de secuencias muestra/conversion por interrupcion es SMPI + 1,
    /* Ademas de determinar cuando la interrupcion ocurre, estos bits tambien determinan 
     * cuantas conversiones deben ocurrir antes de que el ADC comience a almacenar informacion 
     * en el primer Buffer (o la alternativa al primer buffer cuando AD1CON2bits.BUFM = 1)
     * 
     * Por ejemplo si SMPI = 1 entonces habra dos conversiones por interrupcion. 
     * La primer conversion sera almacenada en AD1BUF0 y la segunda en AD1BUF1.
     * Despues de la segunda conversion la bandera de interrupcion del ADC sera configurada.
     * La siguiente conversion sera almacenada en AD1BUF0 si AD1CON2bits.BUFM = 0 
     * o AD1BUF8 si AD1CON2bits.BUFM = 1
     */
    AD1CON1bits.SIMSAM = 0; // 0 = Enable Sequential Sampling (Only AD!CSSXbits == 1) For this example CCS0 and CSS3
    AD1CON2bits.BUFM=0; // Always start at beginning of buffer after
    AD1CON2bits.ALTS=0; // Alternate Input Sample Mode Select bit --- 0 = Always use MUX A input multiplexer settings*/
    AD1CON3bits.SAMC=0; // Auto Sample Time bits ---- 11111 = 31 TAD 1 = 1TAD
    AD1CON3bits.ADRC = 0; // ADC Clock is derived from Systems Clock
    AD1CON3bits.ADCS=7; // with Tcy/N n=7 (LIMITE PROVADO)
    // ADC Conversion Clock TAD = TCY * (ADCS + 1) = (1/40M) * 8 =// 200 ns (5 MHz)
    // ADC Conversion Time for 12-bit Tconv = 14 * TAD = 2800 ns (357.142 KHz)
    
    //------------- SMPI, CSSX and DMA1CNT se configuran en conjunto -------
    //----------esto permite muestrear canales simultaneamente -------------
    AD1CON2bits.SMPI = size-1; // Select 9 conversions between interrupt (N-1)
    
    uint8_t i=0;
    uint16_t channel=0;
    for(i=0;i<size;i++){
        channel += channels[i];
    }
    AD1CSSL = channel;
    AD1CSSH = 0;
    //AD1CSSL = 0b0000000000110000;
    
    AD1CON1bits.ADDMABM = 0; // (NO ENTIENDO) DMA buffers are written in Scatter/Gather mode. The module provides a Scatter/Gather address to the DMA channel, based on the index of the analog input and the size of the DMA buffer
    AD1CON4bits.DMABL = 2;//Each analog input buffer contains 4 words
    IFS0bits.AD1IF = 0; // Clear the Analog-to-Digital interrupt flag bit
    IEC0bits.AD1IE = 0; // Do Not Enable Analog-to-Digital interrupt
    AD1CON1bits.ADON = 1; // Turn on the ADC
    
    return true;
}

bool DMA0_Init(uint16_t size){
    DMA0CONbits.AMODE = 0; // Configure DMA for Register indirect with post increment
    DMA0CONbits.MODE = 2; // Configure DMA for Continuous Ping-Pong mode
    DMA0PAD = (int)&ADC1BUF0;// Configure DMA Buffer with ADC1BUF0
    DMA0CNT = (size-1); // Configure number of cycle before interrupt activation
    DMA0REQ = 13; //  DMA Peripheral IRQ  (ADC1 ? ADC1 convert done) (El activador del contador de ciclos))
    IFS0bits.DMA0IF = 0; //Clear the DMA interrupt flag bit
    IEC0bits.DMA0IE = 1; //Set the DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;// Turn on the DMA0
}

void ADC1_selectChannel(MY_ADC_CHANNELS channel){
    if(channel>=AN0 && channel<=AN15){
        AD1CSSH = 0x00;
        AD1CSSL = channel;
    }
}

void ADC2_selectChannel(MY_ADC_CHANNELS channel){
    if(channel>=AN0 && channel<=AN15){
        AD2CSSL = channel;
    }
}

uint16_t readADC1_Manual(){
    AD1CON1bits.SAMP = 1;    // Comienza a tomar muestras  
    Nop();Nop();Nop();Nop();Nop(); // Retardos.

    AD1CON1bits.SAMP = 0;     // Termina de tomar muestras e inicia conversion 
    Nop();Nop();Nop();Nop();Nop(); // Retardos.
    
    return ADC1BUF0;
}

uint16_t readADC2_Manual(){
    AD2CON1bits.SAMP = 1;    // Comienza a tomar muestras  
    Nop();Nop();Nop();Nop();Nop(); // Retardos.

    AD2CON1bits.SAMP = 0;     // Termina de tomar muestras e inicia conversion 
    Nop();Nop();Nop();Nop();Nop(); // Retardos.
    
    return ADC2BUF0;
}
