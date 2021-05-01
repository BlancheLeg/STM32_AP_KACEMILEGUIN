#include "mbed.h"
// définition des pins:  sorties numériques et sorties pwm

DigitalOut moteur1a(D3); // Marche avant du premier moteur
DigitalOut moteur1b(D4); // Marche arrière du premier moteur
DigitalOut moteur2a(D5); // Marche Led
DigitalOut moteur2b(D6);

PwmOut pwm1(D9);   // vitesse du premier moteur
PwmOut pwm2(D10);  // intensité led

       
    // on commence par le premier moteur seulement, en marche avant, vitesse lente (10%)
    int ventiloOnOff(int verifventil)
    {
      if (verifventil==0)//ventilateur éteint
          {
              pwm1.write(1.00f);  //pleine puissance
              moteur1a = 0;
              moteur1b = 1;
              moteur2a = 0;
              moteur2b = 0;  
              return(!verifventil);
          }
           else//ventilateur allumé
          {
                 moteur1b = 0;
                  return(!verifventil);
          }
    }
    int ledOnOff(int verifventil)
    {
      if (verifled==0)//led éteintes
          {
          pwm2.write(1.00f);  //luminosité max
          moteur2a = 0;
          moteur2b = 1;  
        return(!verifled);
          }
           else//ventilateur allumé
          {
                 moteur1b = 0;
             return(!verifled);
          }
    }
int ventilinit(int verifventil)
    {
      if (verifventil==1)//ventilateur allumé
      {
             moteur1b = 0;
             return(!verifventil);
          }
    }
    int ledinit(int verifled)
    {
      if (verifled==1)//led éteintes
          {
             moteur1b = 0;
             return(!verifled);
          }
    }
    
