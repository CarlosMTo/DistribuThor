/*
============================================================================
 Name : Tobias.cpp
 Author :
 Version :
 Description : Hello world - Exe source file
============================================================================
*/

/*
============================================================================
 Name : ProjetTobias.cpp
 Author :
 Version :
 Description : Hello world - Exe source file
============================================================================
*/

// Include Files

#include <libarmus.h>
#define ENCODEURD 20
#define ENCODEURG 20

#define VITG 51
#define VITD 52

#define dCm_Coche 0.373
#define dCirconference 91.12
#define dPI 3.1416

#define UC unsigned char


void vAvancer(int iDistance,int ucVitesse);
void  vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN);
int iPID(void);
void vCalibration(void);

int _iVitesse = 51;
int iCompteurDistance = 0;
int iNbCycles = 0;
bool init = true;

// Global Functions

int main()
{
	int EncodeurD,EncodeurG;
	LCD_ClearAndPrint("\n\n\n\n\n\n\n");

	//MOTOR_SetSpeed(MOTOR_LEFT,46);
	//MOTOR_SetSpeed(MOTOR_RIGHT,50);


	SYSTEM_ResetTimer();
	ENCODER_Read(ENCODER_RIGHT);

	while(true)
	{

		if((DIGITALIO_Read(BMP_FRONT) == 1) || (DIGITALIO_Read(BMP_LEFT) == 1) || (DIGITALIO_Read(BMP_RIGHT) == 1) ||(DIGITALIO_Read(BMP_REAR) == 1))
		{
			MOTOR_SetSpeed(MOTOR_LEFT,0);
			MOTOR_SetSpeed(MOTOR_RIGHT,0);
		}
		else /*if(init)
		{
			vAvancer(0,51);
			//init = false;
		}
		else*/
		{
			vAvancer(50,_iVitesse);
			// Virage gauche 90
			vTourner(90, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
			vAvancer(10,_iVitesse);
			// Virage droite 90
			vTourner(90, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
			vAvancer(10,_iVitesse);
			// Virage droite 90
			vTourner(90, MOTOR_RIGHT, MOTOR_LEFT, ENCODER_LEFT);
			vAvancer(10,_iVitesse);
			// Virage gauche 90
			vTourner(90, MOTOR_LEFT, MOTOR_RIGHT, ENCODER_RIGHT);
			//vAvancer(5,_iVitesse);
			// Virage droite 45
			//vAvancer(25,_iVitesse);
			// Virage gauche 90
			//vAvancer(45,_iVitesse);
			// Virage droit 45
			//vAvancer(30,_iVitesse);
			// Virage droite 13,5
			//vAvancer(70,_iVitesse);

			MOTOR_SetSpeed(MOTOR_LEFT,0);
			MOTOR_SetSpeed(MOTOR_RIGHT,0);
			break;
			//vCalibration();
		}
	}

	return 0;
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

void vAvancer(int iDistance,int ucVitesse)
{
	int iNbEncodeur = 0;

	iNbEncodeur = iDistance/dCm_Coche;
	//init = false;
	while(iCompteurDistance <= iNbEncodeur)
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
		//_iVitesse = ucVitesse;

		LCD_Printf("\n Vitesse Droite: %d\n" ,ucVitesse);
		MOTOR_SetSpeed(MOTOR_LEFT,51);
		MOTOR_SetSpeed(MOTOR_RIGHT,ucVitesse);
	}
	iCompteurDistance = 0;
	iNbCycles = 0;
}

//*****************************************************************************
// Nom de la fct:   vTournerDroite               Fait par: Carlos Torres
// Date:            2017-10-05
// Revision :       A
// Modification :
// Prototype:       void vTournerDroite(int iAngle);
// Description:		tourne à droite pour une valeur de iAngle en degrés
//
// Parametres   Entre : iAngle
//					  : iMoteur_RUN (moteur qui effectuera la rotation)
//					  : iMoteur_STOP (moteur qui servira de pivot)
//              Sortie: Aucun
//
//*****************************************************************************

void  vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN)
{
	int iValEnc = 0, iCompteurTour = 0, iNbCochesTotal = 0;

	iNbCochesTotal = (dCirconference*iAngle/360)/dCm_Coche;
	MOTOR_SetSpeed(iMoteur_RUN,0);
	MOTOR_SetSpeed(iMoteur_STOP,0);

	ENCODER_Read(iEncodeur_RUN);
	//ENCODER_Read(iMoteur_STOP);
	SYSTEM_ResetTimer();

	while (iCompteurTour <= iNbCochesTotal)
	{
		MOTOR_SetSpeed(iMoteur_RUN, 50);

		if(SYSTEM_ReadTimerMSeconds() >= 100.0)
		{
			SYSTEM_ResetTimer();
			iValEnc = ENCODER_Read(iEncodeur_RUN);
			iCompteurTour += iValEnc;
		}
	}
	MOTOR_SetSpeed(iMoteur_RUN,0);
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

int iPID(void)
{
	int iValEncD = 0, iCalTrans = 0, iCalCumul = 0;

	if(SYSTEM_ReadTimerMSeconds() >= 500.0)
	{
		SYSTEM_ResetTimer();
		iValEncD = ENCODER_Read(ENCODER_RIGHT);
		iCompteurDistance += iValEncD;
		//iNbCycles++;

		iCalCumul = iNbCycles*ENCODEURD - iCompteurDistance;
		iCalTrans = ENCODEURD - iValEncD;
		//LCD_Printf("iCalCumul : %d   iCalTrans : %d\n", iCalCumul, iCalTrans);
		iCalTrans = (0.4 * iCalCumul) + (1.4 * iCalTrans);
		//LCD_Printf("iCalCumul : %d   iCalTrans : %d\n", iCalCumul, iCalTrans);
		iNbCycles++;
	}

	return iCalTrans;
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

void vCalibration(void)
{
	int iValEncD,iValEncG;

	MOTOR_SetSpeed(MOTOR_LEFT,VITG);
	MOTOR_SetSpeed(MOTOR_RIGHT,VITD);

	if(SYSTEM_ReadTimerMSeconds() >= 500.0)
	{
		SYSTEM_ResetTimer();
		iValEncD = ENCODER_Read(ENCODER_RIGHT);
		iValEncG = ENCODER_Read(ENCODER_LEFT);
		LCD_Printf("Val enc D : %d Val enc G : %d \n",iValEncD,iValEncG);
	}
}
