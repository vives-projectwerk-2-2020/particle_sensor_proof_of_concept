/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


Serial device(D1,D0);
Serial pc(USBTX, USBRX);
const unsigned int PACKET_SIZE =10;
int main()
{ 
    device.baud(9600);  
    //int regelPauze =0;
    while(1) {
        
        if(pc.readable()) {
            device.putc(pc.getc());
        }
        /*     // my code
        if(device.readable()) {
            int test = device.getc();
            pc.printf("%d, ",test);
            regelPauze++;
            if(regelPauze==4){
                pc.printf("-- \r\n--");
                regelPauze =0;
            }
        }
        */
        /*     // example from teacher
        if(device.readable()){
            uint8_t buffer[PACKET_SIZE];
            if( device.read(buffer,PACKET_SIZE,) == PACKET_SIZE){
                for(int i=0; i<PACKET_SIZE;i++){
                    pc.printf("data %d",buffer[i]);
                }
                pc.printf(" ----- \r\n");
            }
        }
        */
        if(device.readable()){
            uint8_t buffer[PACKET_SIZE];
            device.gets((char*) buffer,PACKET_SIZE);
            for(int i=0; i<PACKET_SIZE; i++){
                pc.printf("%d ",buffer[i]);
            }
            pc.printf("---- \r\n");
        }   
        

    }
       
}