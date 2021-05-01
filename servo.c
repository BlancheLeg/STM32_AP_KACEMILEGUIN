#include "mbed.h"

int servoOnOff(int verif);
PwmOut servo(D8);

const int positionMin = 1000;
const int positionMax = 2000;

int servoOnOff(int verif)
{
    if(verif==0)//cas porte fermé
    {
        servo.period_ms(20);
        for(int i=positionMin; i<positionMax; i += 10) 
        {
            servo.pulsewidth_us(i);
        }
        wait(3);
        return verif=1;//indication porte ouverte
    }
    else  //cas porte ouverte
    {

        for(int i=positionMax; i>positionMin; i -= 10) 
        {
            servo.pulsewidth_us(i);
        } 
        return verif=0;//indication porte fermé
    }
}
int servoinit(int verif)
    {
        if(verif==1)//porte ouverte
        {

        for(int i=positionMax; i>positionMin; i -= 10) 
        {
            servo.pulsewidth_us(i);
        }
        return verif=0;//indication porte fermé
        }
    }
