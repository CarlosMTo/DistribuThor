#ifndef CAPTEUR_INFRAROUGE
#define CAPTEUR_INFRAROUGE


#include <libarmus.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



/*#define ENCODEURD 12
#define ENCODEURG 13
#define dCm_Coche 0.390
#define dCirconference 91.12
#define VITG 59
#define VITD 62

#define UC unsigned char

int _iVitesse = VITD;
int _erreurTot = 0;
bool init = true;
int compteurG = 0;
int compteurD = 0;
//int cycles = 0;
//int iCompteurDistance = 0;
int err = 0;
*/

float CalculDeDistance(float fTensionDistance);
float voltage(int EntreeAnalogique);
float DistanceCapteurChoisi(int numCapteur);
//void Avancer(int vitesse, int vitesseD, int vitesseG, int *err)
//int PID(int *compteurG, int *compteurD, int *err);

#endif
