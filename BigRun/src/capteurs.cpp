#include "capteurs.h"
#include <math.h>

#define PIN_CAPTEUR_LIGNE 8

/*float Capteur::GetDistance(int capteur)
{
	DIGITALIO_Write(A0, 0); // A0
	DIGITALIO_Write(A1, 0); // A1
	DIGITALIO_Write(A2, 0); // A2

	switch(capteur)
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

	float distance = 28.652 * pow((((float)ANALOG_Read(2)) * 5.0 / 1023.0), -1.167);
	return distance;
}*/
int Capteur::GetLineVoltage()
{
	// Voltages Analogiques
	double dVoltCentre = 3.72;
	double dVoltGauche = 2.22;
	double dVoltDroite = 4.52;

	// marge d'erreur voltage
	double dMargeVolt = 0.10;

	// conversion double analogiques
	double dConvert_V_int = 1023.0/5.0;

	// lecture port analogique
	int iValSuiveur;
	//iValSuiveur = ANALOG_Read(PIN_CAPTEUR_LIGNE);

	// sortie
	int iResSuiveur = 0;

	if (iValSuiveur <= ((dVoltCentre + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltCentre - dMargeVolt) * dConvert_V_int))
	{
		iResSuiveur = 1;
	}
	else if (iValSuiveur <= ((dVoltGauche + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltGauche - dMargeVolt) * dConvert_V_int))
	{
		iResSuiveur = 2;
	}
	else if (iValSuiveur <= ((dVoltDroite + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltDroite - dMargeVolt) * dConvert_V_int))
	{
		iResSuiveur = 3;
	}
	else if (iValSuiveur == 0)
	{
		iResSuiveur = 4;
	}
	else
	{
		iResSuiveur = 5;
	}

	return iResSuiveur;
}
