
/****************************************
* Biblioteca para comunicação entre o   *
*   pic18F4550 e o módulo ZX_gesture    *
****************************************/

/**
 * MCU: PIC18F4550
 * Frequency: 20MHz  
 * UART - Boundrate 115200
 */


/* 
 * File:   
 * Author: Nonato 
 * Comments:
 * Revision history: 
 */

/**********************************************
Pic18F4550 Pin / ZX Sensor Board / Function
 ---------------------------------------
 5V                 VCC              Power
 GND                GND              Ground
 RC7                 TX               Data

***********************************************/


#ifndef ZX_GESTURE_H
#define	ZX_GESTURE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>





#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


/*****************************************
*          ZX Sensor - mensagens         *
******************************************/
#define ZX_FIM              0xFF
#define ZX_UART_RANGES      0xFE
#define ZX_GESTO_X          0xFA
#define ZX_GESTO_Z          0xFB
#define ZX_EVENTO_GESTO     0xFC
#define ZX_ID               0xF1

/* Constantes */
#define ZX_ERROR            0xFF
#define NO_DATA             0xFF
#define MAX_X               240
#define MAX_Z               240

#define SENTIDO_DIREITO     0x01
#define SENTIDO_ESQUERDO    0x02
#define SENTIDO_PARA_BAIXO  0x03
#define SENTIDO_PARA_CIMA   0x04
#define SENTIDO_FLUTUANDO   0x11
#define SEM_GESTO           0xFF
    
void    UART_Init(void);
uint8_t UART_Data_Ready(void);
uint8_t UART_Read(void);
void UART_Write(uint8_t data);

int init_ZX_Gesture(void);
int anyGestureAvailable(char *gesture, int *data1, int *data2);
int getLastX();
int getLastZ();


//testes
int test_msg_data(uint8_t gesture_code, uint8_t gesture, int data1, int data2);
void test_all_msg();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ZX_GESTURE_H */

