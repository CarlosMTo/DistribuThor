#include <libarmus.h>
#include "deplacement.h"
#define ENCODEURD 12
#define ENCODEURG 13
#define dCm_Coche 0.390
#define dCirconference 91.12
#define VITG 59
#define VITD 62

int _iVitesse = VITD;
int _erreurTot = 0;

bool init = true;
int compteurG = 0;
int compteurD = 0;
int cycles = 0;
int iCompteurDistance = 0;
int err = 0;

//Fonction qui permet au robot d'avancer

void Deplacement::vAvancer(int dist,int ucVitesse)
{
	//ucVitesse += iPID();
	//_iVitesse = ucVitesse;
	MOTOR_SetSpeed(MOTOR_LEFT,ucVitesse);
	MOTOR_SetSpeed(MOTOR_RIGHT,VITD);
	float iNbEncodeur = dist/dCm_Coche;
	while(compteurD <= iNbEncodeur)
	{
		ucVitesse += iPID();

		if((ucVitesse < 30) && (ucVitesse > 0))
		{
			ucVitesse = 30;
		}
		if(ucVitesse > 100)
		{
			ucVitesse = 100;
		}
		if((ucVitesse > -30) && (ucVitesse < 0))
		{
			ucVitesse = -30;
		}
		if(ucVitesse < -100)
		{
			ucVitesse = -100;
		}

		if((DIGITALIO_Read(BMP_FRONT) == 1) || (DIGITALIO_Read(BMP_LEFT) == 1) || (DIGITALIO_Read(BMP_RIGHT) == 1) ||(DIGITALIO_Read(BMP_REAR) == 1))
		{
			MOTOR_SetSpeed(MOTOR_LEFT,0);
			MOTOR_SetSpeed(MOTOR_RIGHT,0);
		}
		else
		{
			/*if(init)
			{
				MOTOR_SetSpeed(MOTOR_LEFT,55);
				if(SYSTEM_ReadTimerMSeconds() >= 250.0)
				{
					init = false;
					SYSTEM_ResetTimer();
				}
			}
			else
			{
				MOTOR_SetSpeed(MOTOR_LEFT,62);
			}
			*/
			MOTOR_SetSpeed(MOTOR_LEFT,VITG);
			MOTOR_SetSpeed(MOTOR_RIGHT,ucVitesse);
			//LCD_Printf("compteur D: %i compteur G: %i\n" , compteurD, compteurG);
			//_iVitesse = ucVitesse;
		}
	}
	//init = false;

	//LCD_Printf("Vitesse G: %i \n" ,ucVitesse);

	//LCD_Printf("Gauche: %d" ,ucVitesse);
	//LCD_Printf("droite: %d \n" , 46);
	compteurG = 0;
	compteurD = 0;
	err = 0;
	ENCODER_Read(ENCODER_RIGHT);
	ENCODER_Read(ENCODER_LEFT);
}

// Fonction permet robot de tourner a gauche ou droite

void Deplacement::vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN)
{
	int iValEnc = 0, iCompteurTour = 0, iNbCochesTotal = 0;

	iNbCochesTotal = (dCirconference*iAngle/360)/dCm_Coche;
	if (iNbCochesTotal < 0) // Si l'angle est négatif (Pour reculer)
		iNbCochesTotal *= -1;
	MOTOR_SetSpeed(iMoteur_RUN,0);
	MOTOR_SetSpeed(iMoteur_STOP,0);

	ENCODER_Read(ENCODER_RIGHT);
	ENCODER_Read(ENCODER_LEFT);
	SYSTEM_ResetTimer();

	while (iCompteurTour <= iNbCochesTotal)
	{
		if (iAngle < 0)
			MOTOR_SetSpeed(iMoteur_RUN, -50); // Pour reculer
		else
			MOTOR_SetSpeed(iMoteur_RUN, 50);

		if(SYSTEM_ReadTimerMSeconds() >= 100.0)
		{
			SYSTEM_ResetTimer();
			iValEnc = ENCODER_Read(iEncodeur_RUN);
			iCompteurTour += iValEnc;
		}
	}
	MOTOR_SetSpeed(iMoteur_STOP,0);
	MOTOR_SetSpeed(iMoteur_RUN,0);
	ENCODER_Read(ENCODER_RIGHT);
	ENCODER_Read(ENCODER_LEFT);
}

//*****************************************************************************
// Nom de la fct:   vAvancer               Fait par: Remi Lamoureux Levesque
// Date:            2017-09-28
// Revision :       A
// Modification :
// Prototype:       void vAvancer(char cVitesse);
// Description:
//
// Parametres   Entre : cVitesse
//              Sortie: Aucun
//
//*****************************************************************************

int Deplacement::iPID(void)
{
	int encD = 0;
	int encG = 0;
	int multi = 0;
	float incrVitesse = 0;

	if(SYSTEM_ReadTimerMSeconds() >= 150.0)
	{
		SYSTEM_ResetTimer();
		//cycles++;
		encD = ENCODER_Read(ENCODER_RIGHT);
		encG = ENCODER_Read(ENCODER_LEFT);
		compteurG += encG;
		compteurD += encD;

		float diff = encG - encD;
		err += compteurG - compteurD;
		incrVitesse = ((diff * 0.6) + (err * 0.008));
		//LCD_Printf("err %i \n", (int)diff);
		/*if (compteurD > 150){
			incrVitesse = ((diff * 0.6) + (err * 0.008));
		}
		else{
			incrVitesse = 0;
		}*/
		//_erreurTot += incrVitesse;
		//LCD_Printf("Valeur erreur Totale : %f \n", incrVitesse);
		//LCD_Printf("diff Total : %d \n", (int)diff);
		//multi = (int)incrVitesse + 1;

		if((incrVitesse) > 1.0)
		{
			//LCD_Printf("PLUS %i \n", (int)incrVitesse);

			//LCD_Printf("PLUS ");
			//if((incrVitesse - (int)incrVitesse) > 0.5)
			//	multi = (int)incrVitesse + 1;
			//else
				multi = (int)incrVitesse;

			//_erreurTot = 0;
		}
		else if((incrVitesse) < -1.0)
		{
			//LCD_Printf("MOIN %i \n", (int)incrVitesse);

			//if((incrVitesse - (int)incrVitesse) < -0.5)
				multi = (int)incrVitesse - 1;
			//else
				//multi = (int)incrVitesse;

			//multi = multi *2;
			//_erreurTot = 0;
		}
		//LCD_Printf("Valeur enc G : %d", encG);
		//LCD_Printf("Valeur de diff : %d \n", diff);
		//LCD_Printf("Valeur de err %d \n", err);
	}
	return multi;
}
