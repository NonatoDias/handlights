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


#include <stdint.h>
#include <xc.h>
#include "ZXgesture-library.h"

uint8_t msg; // Variável para guardar a opção
uint8_t _last_x = NO_DATA;
uint8_t _last_z = NO_DATA;

void UART_Init()
{
    BRGH    = 0;    // High Baud Rate Select bit
    BRG16   = 1;    // 16-Bit Baud Rate Register Enable bit
    SYNC    = 0;    // EUSART mode select bit
    SPBRG   = 10;   // boundrate 115200
    SPEN    = 1;    // Serial Port Enable bit
    CREN    = 1;    // Continuous Receive Enable bit
    TXEN    = 1;    // Transmit Enable bit
    TRISC7  = 1;    // Input RX
    TRISC6  = 0;    // Output TX

}

uint8_t UART_Data_Ready()
{
   return RCIF;
}
uint8_t UART_Read()
{
  while(!RCIF);
  return RCREG;
}

void UART_Write(uint8_t data)
{
  while(!TRMT);
  TXREG = data;
}

int init_XZ_Gesture(void){
    UART_Init(); // Inicializa a porta serial com 115200 baudrate
    return 1; //true
}

/**
 * Verifica se há alguma mensagem gesto enviada pelo sensor
 * @param gesture
 * @param data1
 * @param data2
 * @return 
 */
int anyGestureAvailable(uint8_t *gesture, int *data1, int *data2){
    if(UART_Data_Ready()) // Se a serial estiver disponível
    {
        msg = UART_Read(); // Opção que será interpretada
        switch(msg){
            case ZX_EVENTO_GESTO:
                *gesture = msg;
                *data1 = UART_Read(); //Tipo de gesto;
                *data2 = UART_Read(); //Velocidade
                return 1;
        
            case ZX_UART_RANGES:
                *gesture = ZX_UART_RANGES;
                *data1 = UART_Read();
                *data2 = UART_Read();
                return 1;
        
            case ZX_FIM:
                *gesture = ZX_FIM;
                *data1 = NO_DATA;
                *data2 = NO_DATA;
                return 1; 
        
            case ZX_GESTO_X:
                *gesture = ZX_GESTO_X;
                *data1 = UART_Read(); //Posicao 120 bias
                *data2 = NO_DATA;
                
                //ultima posicao de x lida
                _last_x = *data1 > MAX_X ? NO_DATA : *data1;
                
                return 1; 
            
            case  ZX_GESTO_Z:
                *gesture = ZX_GESTO_Z;
                *data1 = UART_Read(); //profundidade
                *data2 = NO_DATA;
                
                //ultima posicao de z lida
                _last_z = *data1 > MAX_Z ? NO_DATA : *data1;
                
                return 1; 
        }
    }
    return 0;
}

int getLastX(){
    return _last_x;
}

int getLastZ(){
    return _last_z;
}

/**
 * Método para teste de mesagem e código
 * @param gesture_code
 * @param gesture
 * @param data1
 * @param data2
 * @return 
 */
int test_msg_data(uint8_t gesture_code, uint8_t gesture, int data1, int data2){
    
    //Todas as mensagem devem ser maiores que 0xF0
    if(gesture <= 0xF0){
        return -1;
    }
    if(gesture_code == gesture){
        
        if(data1 != NO_DATA && data2 != NO_DATA){
            if(data1 > 0xF0){//Toda dado enviado deve ser menor que 0xF0
                return -1;
            }
            if(data2 > 0xF0){//Toda dado enviado deve ser menor que 0xF0
                return -1;
            }
        }
    }
    return 1;
}

/**
 * Loop para testar mensagens
 * @return 
 */
void test_all_msg(){
        uint8_t gesture;
        uint8_t gesture_code;
        int data_1;
        int data_2;
        int erro = 0;
        while(erro != 1){
            if(anyGestureAvailable(&gesture, &data_1, &data_2)) // Se a serial estiver disponível
            {
                if(test_msg_data(ZX_EVENTO_GESTO, gesture, data_1, data_2) == -1){
                    erro = 1;
                    break;
                }
                if(test_msg_data(ZX_UART_RANGES, gesture, data_1, data_2) == -1){
                    erro = 1;
                    break;
                }
                if(test_msg_data(ZX_FIM, gesture, data_1, data_2) == -1){
                    erro = 1;
                    break;
                }
                if(test_msg_data(ZX_GESTO_X, gesture, data_1, data_2) == -1){
                    erro = 1;
                    break;
                }
                if(test_msg_data(ZX_GESTO_Z, gesture, data_1, data_2) == -1){
                    erro = 1;
                    break;
                }
            }
        }
        gesture_code = &gesture;
}