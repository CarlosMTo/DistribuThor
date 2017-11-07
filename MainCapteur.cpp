/*
============================================================================
 Name : THOBIAS
 Author :
 Version :
 Description : Hello world - Exe source file
============================================================================
*/

// Include Files

#include <libarmus.h>
#include "capteur_infrarouge.h"
#define ENCODEURD 12
#define ENCODEURG 13
#define dCm_Coche 0.390
#define dCirconference 91.12
#define VITG 59
#define VITD 62

#define UC unsigned char


void vAvancer(int distance,int ucVitesse);
void vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN);
int Capter(int ir1, int ir2, int ir0);
int _iVitesse = VITD;
int _erreurTot = 0;
int iPID(void);
bool init = true;
int compteurG = 0;
int compteurD = 0;
int cycles = 0;
int iCompteurDistance = 0;
int err = 0;


// Global Functions

int main()
{
	float ir1 = 80.0, ir2 = 80.0, ir0 = 80.0;
	int ir;
	float irCompare1, irCompare2, irCompare0;

	ir = Capter(ir1, ir2, ir0);

	while (true)
	{
		switch(ir)
		{
		case 1:
			ir1 = DistanceCapteurChoisi(ir);
			THREAD_MSleep(1000);
			irCompare1 = DistanceCapteurChoisi(ir);

			if (irCompare1 <= ir1 + 3 && irCompare1 >= ir1 - 3)
			{
				LCD_Printf("Objet 1 \n");
			}
			break;
		case 2:
			ir2 = DistanceCapteurChoisi(ir);
			THREAD_MSleep(1000);
			irCompare2 = DistanceCapteurChoisi(ir);

			if (irCompare2 <= ir2 + 3 && irCompare2 >= ir2 - 3)
			{
				LCD_Printf("Objet 2 \n");
				vTourner(45, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
				vTourner(-45, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
			}
			break;
		case 0:
			ir0 = DistanceCapteurChoisi(ir);
			THREAD_MSleep(1000);
			irCompare0 = DistanceCapteurChoisi(ir);

			if (irCompare0 <= ir0 + 3 && irCompare0 >= ir0 - 3)
			{
				LCD_Printf("Objet 3 \n");
				vTourner(45, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
				vTourner(-45, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
			}
			break;
		default:
			break;
		}

			// LCD_Printf("Pas d'objet!!!!! \n");
		ir = Capter(ir1, ir2, ir0);
	}
		//LCD_Printf("IR1= %f\n", ir1);
		// THREAD_MSleep(500);
	/*
	int EncodeurD,EncodeurG;
	LCD_ClearAndPrint("\n\n\n\n\n\n\n");

	//MOTOR_SetSpeed(MOTOR_LEFT,70);
	//MOTOR_SetSpeed(MOTOR_RIGHT,70);

	SYSTEM_ResetTimer();
	ENCODER_Read(ENCODER_RIGHT);
	ENCODER_Read(ENCODER_LEFT);

	//AUDIO_SetVolume(80);
	//AUDIO_PlayFile("ThisCallingRobot.wav");

	while(true)
	{


		/*MOTOR_SetSpeed(MOTOR_LEFT,58);
		MOTOR_SetSpeed(MOTOR_RIGHT,61);
		if(SYSTEM_ReadTimerMSeconds() >= 200.0)
		{
			SYSTEM_ResetTimer();
			int iValEncD = ENCODER_Read(ENCODER_RIGHT);
			int encG = ENCODER_Read(ENCODER_LEFT);
			LCD_Printf("Gauche: %d \n" , encG);
			LCD_Printf("droite: %d \n" ,iValEncD );
		}



			//vAvancer(0,_iVitesse);

	}

	vAvancer(50, _iVitesse);
    vAvancer(50, _iVitesse);
	vAvancer(50, _iVitesse);
	vAvancer(50, _iVitesse);
	// Virage gauche 90
	vTourner(90, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
	vAvancer(32,_iVitesse);
	// Virage droite 90
	vTourner(90, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
	vAvancer(30,_iVitesse);
	// Virage droite 90
	vTourner(95, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
	vAvancer(40,_iVitesse);
	// Virage gauche 90
	vTourner(90, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
	//vAvancer(5,_iVitesse);
	vAvancer(25,_iVitesse);
	// Virage droite 45
	vTourner(45, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
	//vAvancer(25,_iVitesse);
	vAvancer(25,_iVitesse);
	// Virage gauche 90
	vTourner(90, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
	//vAvancer(45,_iVitesse);
	vAvancer(50,_iVitesse);
	// Virage droit 45
	vTourner(40, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
	//vAvancer(30,_iVitesse);
	vAvancer(25,_iVitesse);
	// Virage droite 13,5
	vTourner(25, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
	//vAvancer(70,_iVitesse);
	vAvancer(100,_iVitesse);
	MOTOR_SetSpeed(MOTOR_LEFT, 0);
	MOTOR_SetSpeed(MOTOR_RIGHT, 0);

	*/
	return 0;
}


int Capter(int ir1, int ir2, int ir0)
{
	int nbCapteur;
	//ir1 = 80.0;
	//ir2 = 80.0;
	//ir0 = 80.0;
	while(ir1 >= 20.0 && ir2 >= 20.0 && ir0 >= 20.0 )
	{
		ir1 = DistanceCapteurChoisi(1);
		ir2 = DistanceCapteurChoisi(2);
		ir0 = DistanceCapteurChoisi(0);
		vAvancer(5, _iVitesse);
	}
	if (ir1 < 20.0)
		nbCapteur = 1;
	if (ir2 < 20.0)
		nbCapteur = 2;
	if (ir0 < 20.0)
		nbCapteur = 0;

	MOTOR_SetSpeed(MOTOR_LEFT,0);
	MOTOR_SetSpeed(MOTOR_RIGHT,0);
	THREAD_MSleep(50);

	return nbCapteur;
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

void vAvancer(int dist,int ucVitesse)
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

void vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN)
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

int iPID()
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
