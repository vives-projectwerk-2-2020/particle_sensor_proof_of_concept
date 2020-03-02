/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
/* Includes */
#include "mbed.h"

/* Pinout */
Serial device(D1,D0);
Serial pc(USBTX, USBRX);

/* Const declaration */ 
const unsigned int PACKET_SIZE =10;

/* Program */

int checkSum(int beginData, int endData,uint8_t Package[]){
    int checksum=0;
    for(int t=beginData; t<=endData; t++ ){
        checksum += Package[t];
    }
    return checksum%256;    // onboard checksum uses overflow ignore, so to compensate you need to get the rest of the devision
}

int main()
{ 
    device.baud(9600);      // make sure this is NOT in a loop but on the first line of your main !!                          
    uint8_t buffer[PACKET_SIZE];    
    int headData;
    while(1) {
        headData = device.getc();
        if(headData == 0xAA){            
            buffer[0] = headData;
            /* Is not a foolprove methode for the measured value can be 0xAB */
            // int index = 1;
            // while(headData != 0xAB){        
            //     headData = device.getc();
            //     buffer[index] = headData;
            //     index++;
            // }            
            for( int t = 1; t<PACKET_SIZE; t++){
                buffer[t] = device.getc();
            }

        } else {
            pc.printf(" \r\n flushing \r\n");
            // dont read for the data is wrong
        }

    /*   Prints the buffer in an ordered fashion*/
    // for(int i=0; i<PACKET_SIZE; i++){
    //     pc.printf(" %x",buffer[i]);
    // }
    // pc.printf(" -- \r\n--");   
        double PM2_5Value = buffer[3] * 256 + buffer[2]/10.0;
        double PM10Value = buffer[5] *256 + buffer[4]/10.0;        

        pc.printf("\nThe sensorID is %X %X \r\n", buffer[6],buffer[7]);
        pc.printf("The air contains %.1lf µg/m³ of PM2.5 \r\n", PM2_5Value);
        pc.printf("The air contains %.1lf µg/m³ of PM10 \r\n", PM10Value);
        
        if(buffer[8] = checkSum(2,7,buffer)){
            pc.printf("The current Checksum is correct \r\n"); 
        } else {
            pc.printf("The current Checksum is wrong, the data is corrupt \r\n");
        }
       
            
    } 
}