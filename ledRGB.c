/*
 * File:   ledRGB.c
 * Author: Nonato Dias
 *
 * Created on 2 de Maio de 2018, 23:53
 */


#include <xc.h>
#include <math.h>
#include <stdint.h>
#include "ledRGB.h"

#define _XTAL_FREQ 20000000         // Definição do clock do sistema
color color_pages_[8] = {WHITE, BLUE, RED, GREEN, LIGHTBLUE, HOTPINK, YELLOW};
int index_color_ = 0;
int _pwm_count = 0;

/**
 * Seleciona a cor do led
 * @param ledCor (enum color)
 */


void led_control(color ledCor){
  switch(ledCor){
    case RED:
        LED_R = 1;
        LED_G = 0;
        LED_B = 0;
      break;
    case GREEN:
        LED_R = 0;
        LED_G = 1;
        LED_B = 0;
        break;
    case BLUE:
        LED_R = 0;
        LED_G = 0;
        LED_B = 1;
        break;
    case LIGHTBLUE:
        LED_R = 0;
        LED_G = 1;
        LED_B = 1;
        break;
    case HOTPINK:
        LED_R = 1;
        LED_G = 0;
        LED_B = 1;
        break;
    case YELLOW:
        LED_R = 1;
        LED_G = 1;
        LED_B = 0;
        break;
    case WHITE:
        LED_R = 1;
        LED_G = 1;
        LED_B = 1;
        break;
    case NONE:
        LED_R = 0;
        LED_G = 0;
        LED_B = 0;
        break;
    default: 
      break;
  }
}
/*
void led_control(color ledCor){
  switch(ledCor){
    case RED:
        LED_R = 0;
        LED_G = 1;
        LED_B = 1;
      break;
    case GREEN:
        LED_R = 1;
        LED_G = 0;
        LED_B = 1;
        break;
    case BLUE:
        LED_R = 1;
        LED_G = 1;
        LED_B = 0;
        break;
    case LIGHTBLUE:
        LED_R = 1;
        LED_G = 0;
        LED_B = 0;
        break;
    case HOTPINK:
        LED_R = 0;
        LED_G = 1;
        LED_B = 0;
        break;
    case YELLOW:
        LED_R = 0;
        LED_G = 0;
        LED_B = 1;
        break;
    case WHITE:
        LED_R = 0;
        LED_G = 0;
        LED_B = 0;
        break;
    case NONE:
        LED_R = 1;
        LED_G = 1;
        LED_B = 1;
        break;
    default: 
      break;
  }
}*/


/**
 * configura pwm
 */
void pwm_config_(){
    TRISC2 = 0;		/* Set CCP1 pin as output for PWM out */
    PR2 = 199;		/* Load period value */
    //OSCCON=0x72;         /* set internal clock to 8MHz */
    TRISCbits.TRISC2=0;  /* Set CCP1 pin as output for PWM out */
    PR2=199;             /* load period value in PR2 register */ 
    CCPR1L=1;            /* load duty cycle */
    T2CON=0;             /* no pre-scalar,timer2 is off */
    CCP1CON=0x0C;        /* set PWM mode and no decimal value for PWM */
    TMR2=0;
    T2CONbits.TMR2ON=1;  /* Turn ON Timer2 */
};

/**
 * Inicializa leds
 */
void init_ledRGB(){
    pwm_config_();
    PWM_REGISTER = PWM_MAX;
    led_control(NONE);
};


void nextPage(){
    index_color_ = index_color_ > 5 ? 0 : index_color_ +1;
    led_control(color_pages_[index_color_]);
}


void prevPage(){
    index_color_ = index_color_ < 1 ? 6 : index_color_ -1;
    led_control(color_pages_[index_color_]);
}

void ledON(){
    led_control(color_pages_[index_color_]);
}

void ledOFF(){
    led_control(NONE);
}

void ledBlink(){
    int c, d;
    ledOFF();
    //__delay_ms(300);
    for (c = 1; c <= 500; c++)
       for (d = 1; d <= 400; d++)
       {}
    ledON();
}

void increasePWM(){
    _pwm_count = _pwm_count < 1 ? _pwm_count : _pwm_count-1;//intervalo[0,3]
    int val = PWM_MAX - ceil((PWM_MAX - 5)*_pwm_count/3);
    PWM_REGISTER = val > PWM_MAX ? PWM_MAX : val;
}

void decreasePWM(){
    _pwm_count = _pwm_count > 2 ? _pwm_count : _pwm_count+1;//intervalo[0,3]
    int val = PWM_MAX - ceil((PWM_MAX - 5)*_pwm_count/3);
    PWM_REGISTER = val < 0 ? 0 : val;
}