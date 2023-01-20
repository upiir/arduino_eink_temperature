#ifndef _EPD_1in9_H_
#define _EPD_1in9_H_

#include <Arduino.h>
#include <Wire.h>
#include <stdlib.h>
// address
#define adds_com  	0x3C
#define adds_data	0x3D

#define EPD_BUSY_PIN 7
#define EPD_RST_PIN 8



extern unsigned char DSPNUM_1in9_on[];
extern unsigned char DSPNUM_1in9_off[];
extern unsigned char DSPNUM_1in9_WB[];
extern unsigned char DSPNUM_1in9_W0[];
extern unsigned char DSPNUM_1in9_W1[];
extern unsigned char DSPNUM_1in9_W2[];
extern unsigned char DSPNUM_1in9_W3[];
extern unsigned char DSPNUM_1in9_W4[];
extern unsigned char DSPNUM_1in9_W5[];
extern unsigned char DSPNUM_1in9_W6[];
extern unsigned char DSPNUM_1in9_W7[];
extern unsigned char DSPNUM_1in9_W8[];
extern unsigned char DSPNUM_1in9_W9[];

void GPIOInit(void);
void EPD_1in9_Reset(void);
void EPD_1in9_SendCommand(unsigned char Reg);
void EPD_1in9_SendData(unsigned char Data);
unsigned char EPD_1in9_readCommand(unsigned char Reg);
unsigned char EPD_1in9_readData(unsigned char Data);
void EPD_1in9_ReadBusy(void);
void EPD_1in9_lut_DU_WB(void);
void EPD_1in9_lut_GC(void);
void EPD_1in9_lut_5S(void);
void EPD_1in9_Temperature(void);
void EPD_1in9_init(void);
void EPD_1in9_Write_Screen(unsigned char *image);
void EPD_1in9_Write_Screen1(unsigned char *image);
void EPD_1in9_sleep(void);

#endif
