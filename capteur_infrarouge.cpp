/*
============================================================================
 Nom : capteur_infrarouge.cpp
 Auteurs : Philippe Dupras et Louis Côté
 Version : 1.1
 Description : Fonctions et programme testant les capteurs infrarouges
============================================================================
*/

// Include Files

#include <libarmus.h>
#include <math.h>
#include "capteur_infrarouge.h"
// Global Functions

/*int main()
{
	float ir1;
	float ir2;
	float ir0;

	while(1)
	{
		ir1 = DistanceCapteurChoisi(1);
		ir2 = DistanceCapteurChoisi(2);
		ir0 = DistanceCapteurChoisi(0);
		LCD_Printf("IR1= %f IR2= %f IR0= %f \n", ir1, ir2, ir0);
		THREAD_MSleep(500);
	}
	return 0;
}*/

float DistanceCapteurChoisi(int numCapteur)
{
	DIGITALIO_Write(A0, 0); // A0
	DIGITALIO_Write(A1, 0); // A1
	DIGITALIO_Write(A2, 0); // A2

	switch(numCapteur)
	{
	case 1:
		DIGITALIO_Write(A0, 1); // A0
		DIGITALIO_Write(A1, 0); // A1
		break;
	case 2:
		DIGITALIO_Write(A0, 0); // A0
		DIGITALIO_Write(A1, 1); // A1
		break;
	case 0:
		DIGITALIO_Write(A0, 0); // A0
		DIGITALIO_Write(A1, 0); // A1
		break;
	default:
		break;
	}

	return CalculDeDistance(voltage(ANALOG_Read(2)));
}


float CalculDeDistance(float fTensionDistance)
{
	float fDistance;

	fDistance = 28.652 * pow(fTensionDistance, -1.167);

	return fDistance;
}

float voltage(int EntreeAnalogique)
{
	// Les valeurs lues varient entre 0 et 1023. 1023 valant 5V.
	// On convertit cette valeur en une valeur de tension pour calculer la distance
	float voltage = ((float)EntreeAnalogique) * 5.0 / 1023.0;
	return voltage;
}


/*
void Avancer(int vitesse, int vitesseD, int vitesseG, int *err, int *compteurG, int *compteurD,)
{

	MOTOR_SetSpeed(MOTOR_LEFT,vitesse);
	MOTOR_SetSpeed(MOTOR_RIGHT,vitesseD);
	//float iNbEncodeur = dist/dCm_Coche;
	//while(compteurD <= iNbEncodeur)
	//{
	vitesse += PID();

	if((vitesse < 30) && (vitesse > 0))
	{
		vitesse = 30;
	}
	if(vitesse > 100)
	{
		vitesse = 100;
	}
	if((vitesse > -30) && (vitesse < 0))
	{
		vitesse = -30;
	}
	if(vitesse < -100)
	{
		vitesse = -100;
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

		MOTOR_SetSpeed(MOTOR_LEFT,vitesseG);
		MOTOR_SetSpeed(MOTOR_RIGHT,vitesse);
				//LCD_Printf("compteur D: %i compteur G: %i\n" , compteurD, compteurG);
				//_iVitesse = vitesse;
	}
	THREAD_MSleep(50);
	//compteurG = 0;
	//compteurD = 0;
	*err = 0;
	ENCODER_Read(ENCODER_RIGHT);
	ENCODER_Read(ENCODER_LEFT);

}
*/

/*
int PID(int *compteurG, int *compteurD, int *err)
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
		*compteurG += encG;
		*compteurD += encD;

		float diff = encG - encD;
		*err += *compteurG - *compteurD;
		incrVitesse = ((diff * 0.6) + (err * 0.008));
		//LCD_Printf("err %i \n", (int)diff);
		if (compteurD > 150){
			incrVitesse = ((diff * 0.6) + (err * 0.008));
		}
		else{
			incrVitesse = 0;
		}
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



		//if(encG != ENCODEURD)
		//{
			//iCalTrans = multi;
		//}

		//multi = (int)incrVitesse;
	}

	/*float val = _iVitesse + multi;
	if(val - (int)val > 0.5)
		return (int)val + 1;
	else
		return (int)val;

	return multi;

	//LCD_Printf("Valeur PID : %d \n", multi);
}

*/

/* Essai non concluant avec la fonction IR_Detect()
 * Elle ne retourne pas ce qu'on veut (La distance)
void afficher_detection()
{
	int ir_f;

	 while(1)
	 {
	 	ir_f = IR_Detect(IR_FRONT);
	 	switch(ir_f)
		{
	 		case 0: LCD_Printf("Rien\n");
	 		case 1: LCD_Printf("Droite\n");
	 		case 2: LCD_Printf("Gauche\n");
	 		case 3: LCD_Printf("Gauche+Droite\n");
	 		default:
	 			break;
		}
	 	THREAD_MSleep(500);
	 }
}*/

