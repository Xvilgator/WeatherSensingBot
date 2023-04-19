#ifndef KX022_H_
#define KX022_H_

#include"stdint.h"
#include "Stdbool.h"
#define KX022_ADDR      0x3C

#define KX022_CNTL1     0x18
#define KX022_CNTL2     0x19
#define KX022_CNTL3     0x1A
#define KX022_ODCNTL    0x1B
#define KX022_XoutH     0x07
#define KX022_XoutL     0x06
#define KX022_YoutH     0x09
#define KX022_YoutL     0x08
#define KX022_ZoutL     0x09
#define KX022_ZoutH     0x0A
#define KX022_TILT_TIMER    0x22
/******************************************************/
/*       Accelerometer KX022 Driver API               */
/******************************************************/
//sensor software model
/*
typedef struct {
    uint16_t ui16ManufacturerID;
    uint16_t ui16DeviceID;
    uint8_t ui8DeviceSerialNum[5];

} kx022_t;
*/
//initializes by writng in the control word 1,2,3.
bool KX022_dev_info();
void KX022_init();

//Configure output data rate by writing in ODCNTL(0x1B).*set PC1 in CNTL1 to 0 first
void KX022_Config_ODR (uint8_t);

/*
 *  value(in hex)   |    0       1   2   3   4   5   6   7       8       9       1A      1B
 *  ODR             |   12.5    25  50  100 200 400 800 1600    0.781   1.563   3.125   6.25
 */

uint16_t KX022_Xout ();     //get  X axis acceleration

uint16_t KX022_Xout ();     //get  Y axis acceleration

uint16_t KX022_Xout ();     //get  Z axis acceleration

//resets current tilt position (in TSCP) register by writing 0x20h in 0x10h
void KX022_Reset_CurrentTiltPosition ();


void KX022_Reset_PreviousTiltPosition ();   //resets previous tilt position register by writing 0x20h in 0x11h
/*      Bit         Description
 *      LE(5)       Left State (X-)
 *      RI(4)       Right State (X+)
 *      DO(3)       Down State (Y-)
 *      UP(2)       Up State (Y+)
 *      FD(1)       Face-Down State (Z-)
 *      FU(0)       Face-Up State (Z+)
 */
//reads current tilt position
uint8_t KX022_Read_CurrentTiltPosition ();

//reads previous tilt position
uint8_t KX022_Read_CurrentTiltPosition ();

//Checks for interrrupt based on value in INS1(0x12), INS2, INS3, and Control register
int KX022_CheckInterrupt ();

#endif /* KX022_H_ */
