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
// Global Functions

float CalculDeDistance(float fTensionDistance);
float voltage(int EntreeAnalogique);
//void afficher_detection();
float DistanceCapteurChoisi(int numCapteur);

int main()
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
}

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

/* Essai non concluant avec la fonction IR_Detect()
 * Elle ne retourne pas ce qu'on veut (La distance)
 * void afficher_detection()
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







// Include Files

//#include <libarmus.h>
/*
// Global Functions
void Avancer();
// Variables

void imprimer()
{
	LCD_Printf("test1\n");
}

bool i = true;

int main()
{
	LCD_Printf("Hello World!\n My name is Philippe\n");
	SYSTEM_ResetTimer();
	while(i)
	{
		Avancer();
		if(DIGITALIO_Read(BMP_FRONT) == 1)
		{
			MOTOR_SetSpeed(MOTOR_LEFT, 0);
			MOTOR_SetSpeed(MOTOR_RIGHT, 0);
			i = false;
		}
	}
	return 0;
}

void Avancer()
{
	int vitesse;
	int valEncD; // Valeur de l'encodeur droit
	int valEncG; // Valeur de l'encodeur gauche
	int ENCODEURD; // Valeur trouvée encodeur droit
	int ENCODEURG; // Valeur trouvée encodeur gauche
	int calTrans; // Valeur de transition

	valEncG = ENCODER_Read(ENCODER_LEFT);
	valEncG = ENCODER_Read(ENCODER_RIGHT);

	if(SYSTEM_ReadTimerMSeconds() >= 500.0)
	{
		LCD_Printf("%d\n", SYSTEM_ReadTimerMSeconds());
		SYSTEM_ResetTimer();
		valEncG = ENCODER_Read(ENCODER_LEFT);
		valEncG = ENCODER_Read(ENCODER_RIGHT);
	}

	MOTOR_SetSpeed(MOTOR_LEFT, 75);
	MOTOR_SetSpeed(MOTOR_RIGHT, 75);
}
*/
