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
const unsigned int PACKET_SIZE =11;

/* Program */


int checkSum(int beginData, int endData,uint8_t Package[PACKET_SIZE]){
    int checksum=0;
    for(int t=beginData; t<=endData; t++ ){
        checksum += Package[t];
    }
    return checksum%256;    // onboard checksum uses overflow ignore, so to compensate you need to get the rest of the devision
}


int main()
{ 
    device.baud(9600);      // make sure this is NOT in a loop but on the first line of your main !!
    while(1) {
        
        if(pc.readable()) {
            device.putc(pc.getc());
        }

        if(device.readable()){
            bool correctContentOrder = false;
            uint8_t buffer[PACKET_SIZE];
            device.gets((char*) buffer,PACKET_SIZE);
            
            /*Prints the buffer in an ordered fashion
            *for(int i=0; i<PACKET_SIZE-1; i++){
            *    pc.printf(" %x",buffer[i]);
            *}
            *pc.printf(" -- \r\n--");            //add \r before new line to empty your buffer (makes sure it is always on a new line)
            */
           
            if(buffer[0] == 170 && buffer[1] == 192 && buffer[9] == 171){
                pc.printf("\nDe Message header/tail and the Commander numbers are correct \r\n");
                correctContentOrder = true;
            }

            if(correctContentOrder == true){
                int PM2_5Value = buffer[3] * 256 + buffer[2]/10;
                int PM10Value = buffer[5] *256 + buffer[4]/10;
                

                pc.printf("the sensorID is %X %X \r\n", buffer[6],buffer[7]);
                pc.printf("The air contains %d µg/m³ of PM2.5 \r\n", PM2_5Value);
                pc.printf("The air contains %d µg/m³ of PM10 \r\n", PM10Value);
                pc.printf("The current Checksum is %d en should be %d \r\n\n",checkSum(2,7,buffer),buffer[8]);
            }
        }   

    }
}
