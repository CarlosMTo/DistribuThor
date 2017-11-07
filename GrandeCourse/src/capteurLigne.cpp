#include <capteurLigne.h>
#include <libarmus.h>

#define PIN_CAPTEUR_LIGNE 8

int CapteurLigne::suiveur()
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
	iValSuiveur = ANALOG_Read(PIN_CAPTEUR_LIGNE);

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
	else if (iValSuiveur == 0){
		iResSuiveur = 4;
	}
	else{
		iResSuiveur = 5;
	}

	return iResSuiveur;
}
