#include "KX022.h"
#include "stdint.h"

/******************************************************/
/*       Accelerometer KX022 Driver API               */
/******************************************************/


/*
int modifyBit(int n, int p, int b)
{
    int mask = 1 << p;
    return ((n & ~mask) | (b << p));
}
void write_d(int a, int address){
    int *p = (int*)(address);
    *p = a;

}*/
//initializes by writng in the control word 1,2,3.
/*****************************************************************************/
//write 0x40 in 0x18(CNTL1), set default ODR(50Hz),make PC1 bit high
void KX022_init(){

    i2cSend(KX022_ADDR, 1,0x1840);
    i2cSend(KX022_ADDR, 1,0x1B02);  //write 0x02 in 0x1B to set ODR = 50Hz
    i2cSend(KX022_ADDR, 1,0x18C0);

}

void KX022_init_tilt(){
    i2cSend(KX022_ADDR, 1,0x1841);  //write 0x41 to CNTL1
    i2cSend(KX022_ADDR, 1,0x2201);  //write 0x01 in 0x22 (80ms in tilt timer)
    i2cSend(KX022_ADDR, 1,0x1840);  //make PC1 in CNTL1 high (start operation)

}

//Configure output data rate by writing in ODCNTL(0x1B).*set PC1 in CNTL1 to 0 first
void KX022_Config_ODR (uint8_t a)
{
    uint16_t b,c;
    b = LOWBYTE(i2cReceive(KX022_ADDR, 1, 0x18));
    b = b & (0x7F);         //make PC1 bit low to stop operation
    c = (a | 0x1B00);
    i2cSend(KX022_ADDR, 1, b);  //send CNTL1
    i2cSend(KX022_ADDR, 1, c);  //send ODR
    b = b|(0x80);               //Make PC1 high again
    i2cSend(KX022_ADDR, 1, b);  //send CNTL1
}

/*
 *  value(in hex)   |    0       1   2   3   4   5   6   7       8       9       1A      1B
 *  ODR             |   12.5    25  50  100 200 400 800 1600    0.781   1.563   3.125   6.25
 */
//if read value from 0x0F is 0x14 return 1;
bool KX022_dev_info(){
    return((i2cReceive(KX022_ADDR, 1, 0x0F)) == (0x14));
}
//get  X axis acceleration
uint16_t KX022_Xout (){
    uint16_t a=0;
    a = ((LOWBYTE(i2Receive(KX022_ADDR, 1, KX022_XoutH ))) << 8);
    a =a + LOWBYTE(i2Receive(KX022_ADDR, 1, KX022_XoutL ));
    return (a);
}
//get  Y axis acceleration
uint16_t KX022_Yout () {
    uint16_t a=0;
        a = ((LOWBYTE(i2Receive(KX022_ADDR, 1, KX022_YoutH ))) << 8);
        a =a + LOWBYTE(i2Receive(KX022_ADDR, 1, KX022_YoutL ));
        return (a);
}

//get  Z axis acceleration
uint16_t KX022_Zout ()    {
    uint16_t a=0;
        a = ((LOWBYTE(i2Receive(KX022_ADDR, 1, KX022_ZoutH ))) << 8);
        a =a + LOWBYTE(i2Receive(KX022_ADDR, 1, KX022_ZoutL ));
        return (a);
}
//resets current tilt position (in TSCP) register by writing 0x20h in 0x10h
void KX022_Reset_CurrentTiltPosition (){
    i2cSend(KX022_ADDR, 1, 0x1020);
}

//resets previous tilt position register by writing 0x20h in 0x11h
void KX022_Reset_PreviousTiltPosition (){
    i2cSend(KX022_ADDR, 1, 0x1120);
}
/*  No.(in hex)    Bit         Description
 *   32            LE(5)       Left State (X-)
 *   16            RI(4)       Right State (X+)
 *   8             DO(3)       Down State (Y-)
 *   4             UP(2)       Up State (Y+)
 *   2             FD(1)       Face-Down State (Z-)
 *   1             FU(0)       Face-Up State (Z+)
 */
//reads current tilt position
uint8_t KX022_Read_CurrentTiltPosition (){
    return(LOWBYTE(i2cReceive(KX022_ADDR, 1, 0x10)));
}

//reads previous tilt position
float KX022_Read_PreviousTiltPosition (){
    return(LOWBYTE(i2cReceive(KX022_ADDR, 1, 0x11)));
}

//Checks for interrrupt based on value in INS1(0x12), INS2, INS3, and Control register
int KX022_CheckInterrupt ();
