/*
 * File:   my_adc.c
 * Author: Juan David Velasquez Rosero 
 *
 * Created on 29 de septiembre de 2018, 10:22 AM
 */
#include "my_osc.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

/*Registro FOSTSEL (Address 0xF80006) IESO(bit 7) Reservado(Bits 6-3) FNOSC(bits 2-0)
 * FNOSC_PRI = XXXA= 1010 = 010 = Primary (XT, HS, EC) oscillator
 * FNOSC_PRIPLL Primary XT with PLL
 */
_FOSCSEL( FNOSC_PRIPLL );

/*Registro FOSC (Address 0xF80008) FCKSM(bit 7 6) Reservado(Bits 5-3) OSCIOFNC (bit 2) POSCMD(bits 1-0)
 * POSCMD_XT = XXXD = 1101 = 01 =  XT Crystal Oscillator mode 
 * FCKSM_CSECMD = XX7X = 01 = Clock switching is enabled, Fail-Safe Clock Monitor is disabled
 */
_FOSC( POSCMD_XT & FCKSM_CSECMD );

/*Registro FOSC (Address 0xF8000A) FWDTEN(bit 7) WINDIS(Bit 6) PLLKEN (bit 5) WDTPRE (bit 4) WDTPOST(bits 3-0)
 *WDTPOST_PS32768 = XXXF = 1111 = Watchdog Timer Postscaler bits 1:32,768
 *FWDTEN_OFF = FF7F = 0111 = 0 = Watchdog Timer Enable bit = Watchdog Timer enabled/disabled by user software (LPRC can be disabled
 *by clearing the SWDTEN bit in the RCON register)
 */
_FWDT( WDTPOST_PS32768 & FWDTEN_OFF );

/*Registro FPOR (Address 0xf8000c) Reservado(bit 7-3) FPWRT(Bit 2-0)
 * FPWRT_PWR64 = FFFE = 1110 = 110 = Power-on Reset Timer Value Select bits  PWRT = 64 ms
 */
_FPOR( FPWRT_PWR64 );

void OsciladorConfig(void){
    
    if ((OSCCONbits.COSC & 1) && !(OSCCONbits.COSC & 4))/* check not already in PLL mode (0?1) */
    {
      __builtin_write_OSCCONH(0);        /* Clock Switch to non PLL mode */
      __builtin_write_OSCCONL(0x01);     /* Start clock switching */
      asm("nop");
      asm("nop");
      while (OSCCONbits.COSC != 0) ;
    }
    
    PLLFBD = 0x22;                       /* configure Oscillator PLL : PLLDIV (M)*/
    CLKDIV = 0x00;                       /* configure Oscillator PLL : FRCDIV ; PLLPOST (N2); PLLPRE (N1)*/
    __builtin_write_OSCCONH(3);          /* Clock Switch to desired configuration */
    __builtin_write_OSCCONL(0x01);       /* Start clock switching */
    AD1PCFGL = 0;
    AD2PCFGL = 0x00;
    while (OSCCONbits.COSC != 3) ;       /* Wait for Oscillator Stabilisation */
    while (OSCCONbits.LOCK != 1) ;
    PR1 = 0xFFFF;                        /* Period */
}
