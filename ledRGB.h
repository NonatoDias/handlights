
/********************************************
* Biblioteca para CONTROLE de fita de led   *
********************************************/

/**
 * MCU: PIC18F4550
 * Frequency: 20MHz  
 */


/* 
 * File:   
 * Author: Nonato 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LED_RGB_H
#define	LED_RGB_H

#include <xc.h> // include processor files - each processor file is guarded.  




/************* led **************/
#define LED_R   LATAbits.LATA1
#define LED_G   LATAbits.LATA0
#define LED_B   LATAbits.LATA2
#define PWM_REGISTER CCPR1L
#define PWM_MAX 199
#define PIN_PWM LATBbits.LATC2


typedef enum {
  BLUE,
  RED,
  GREEN,
  LIGHTBLUE,
  HOTPINK,
  YELLOW,
  WHITE,
  NONE
} color;

/**
 * Seleciona a cor do led
 * @param ledCor (enum color)
 */
void led_control(color ledCor);

/**
 * configura pwm
 */
void pwm_config_();

/**
 * Inicializa leds
 */
void init_ledRGB();


void nextPage();
void prevPage();
void ledON();
void ledOFF();
void ledBlink();

void increasePWM();
void decreasePWM();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LED_RGB_H */

