/*
 * File:   main.c
 * Author: Nonato Dias
 *
 * Created on 2 de Maio de 2018, 23:54
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p18f4550.h>
#include "ledRGB.h"
#include "ZXgesture-library.h"

#define _XTAL_FREQ 20000000         // Definição do clock do sistema
#pragma config PLLDIV = 1       
#pragma config CPUDIV = OSC1_PLL2   // Oscilador do sistema / 1 e  96MHz do circuito PLL / 2
#pragma config FOSC = HS            // Oscilador HS 
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOR = OFF
#pragma config MCLRE = ON 
#pragma config PWRT = OFF
#pragma config PBADEN = OFF


#define COUNT_MAX 250
#define _ON 1
#define _OFF 0

#define PAGINA_COR 0
#define AJUSTE_FINO 1

void AFSmartBoard_Init (void);
void busy_delay();

void main(void){
    //controle de led
    char isLightON = 0;
    
    //posicao
    int x = NO_DATA;
    int z = NO_DATA;
    
    //GESTO MAO PARADA
    int valid_z = 0;
    int rate_z = 0;
    int count_z = 0;
   
    //GESTO TCHAU
    int count_tchau = 0;
    
    //DADOS DO SENSOR DE GESTO
    uint8_t gesture = 0;
    int data_1 = 0;
    int data_2 = 0;
    
    int substate_toggle = AJUSTE_FINO;
    
    AFSmartBoard_Init();// função que inicializa a placa AFSmartBoard 
    init_ledRGB(); // Inicializa a trilha de led
    init_XZ_Gesture(); // Inicializa a porta serial com 115200 baudrates
    
    //PWM_REGISTER = 5;
    /*while(1){
        ledBlink();
    }*/
    
    
    while(1){
        //LUZ DESLIGADA
        if(isLightON == _OFF){
            if(anyGestureAvailable(&gesture, &data_1, &data_2)){
                
                /********* TCHAU ************/
                if(gesture == ZX_EVENTO_GESTO){
                    switch(data_1){
                        case SENTIDO_DIREITO:
                            count_tchau = count_tchau == -1 ? count_tchau - 1 : count_tchau == 1 ? 0 : 1;
                            break;
                        case SENTIDO_ESQUERDO:
                            count_tchau = count_tchau == 1 ? count_tchau + 1 : count_tchau == -1 ? 0 : -1;
                            break;
                    }
                    if(abs(count_tchau) == 2){
                        isLightON = _ON;
                        ledON();
                        count_tchau = 0;
                        //__delay_ms(100); //espera para usuario nao mudar a cor ao ligar
                    }
                }
                
            }
            
        //LUZ LIGADA
        }else{
            if(count_z > COUNT_MAX){//MAO PARADA
                count_z = 0;
                isLightON = _OFF;
                ledOFF();
                continue;// reinicia o while
            }
            else if(anyGestureAvailable(&gesture, &data_1, &data_2)){  
                
            /********* MAO PARADA ************/
                z = getLastZ();
                if(z != NO_DATA){
                    //UART_Write(z); //DEBUGGER Z 
                    rate_z = abs(valid_z - z);
                    if(rate_z < 15){
                        count_z += 1;
                    }else{
                        valid_z = z;//ultimo valor valido de z
                        count_z = 0;
                    }
                }//FIM MAO PARADA
                
                x = getLastX();
                
            /**** ESTADOS INTERNOS  ***/
                UART_Write(x);
                if(gesture == ZX_EVENTO_GESTO){
                    
                    if(data_1 == SENTIDO_PARA_BAIXO){
                        substate_toggle = substate_toggle == PAGINA_COR ? AJUSTE_FINO : PAGINA_COR;
                        ledBlink();
                        buzy_delay();
                        continue;
                    }
                    /********* AJUSTE FINO ************/
                    if(substate_toggle == AJUSTE_FINO){
                        if(data_1 == SENTIDO_DIREITO){
                            increasePWM();
                        }
                        if(data_1 == SENTIDO_ESQUERDO){
                            decreasePWM();
                        }
                    }
                
                    /********* PÁGINA DE COR ************/  
                    if(substate_toggle == PAGINA_COR){
                        if(data_1 == SENTIDO_DIREITO){
                            nextPage();
                        }
                        if(data_1 == SENTIDO_ESQUERDO){
                            prevPage();
                        }
                    }
                }
            }
        }
    }
    return;
}


void AFSmartBoard_Init () {         
    RCON = 0X80;            // Limpa o Registro de Reset         
    ADCON1 = 0x0F;          // Configura todas a portas como Portas Analogicas exeto  A0 
    CMCON = 0x0F;           // Desabilita o Comparador         
    LATA = 0;         
    TRISA = 0;         
    T0CON = 0b11000101;     // Habilita Timer , 8 bits,clock interno, preescale 1:64         
    LATB = 0;               // Limpa Latch PortB         
    TRISB = 0;              // Coloca todos como  tudo Saida         
    LATD = 0;         
    TRISD = 0x00;           // Colocar PORTD como saída         
    LATE = 0; 
} 

void buzy_delay(){
    int c, d;
    for (c = 1; c <= 200; c++)
       for (d = 1; d <= 400; d++)
       {}
}