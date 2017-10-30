/*
============================================================================
 Name : Suiveur Test
 Author : Carlos Torres
 Version : A
 Description : test des entrées analogiques pour le suiveur
============================================================================
*/
// Include Files
#include <libarmus.h>

// Global Functions
int iSuiveur();

#define iAnalog_input_suiveur 8

int main()
{
	int iResSuiveur;
	
	//THREAD thread1;
	//thread1 = THREAD_CreateSimple(iSuiveur);
	
	LCD_ClearAndPrint("\n\n\n\n\n\n\n");

	/*while (1){
		LCD_Printf("%d\n", iResSuiveur = iSuiveur());
		THREAD_MSleep(250);
	
	}*/
	
	while (1){
		iResSuiveur = iSuiveur();
		if (iResSuiveur == 1) {
			LCD_Printf("Tout droit\n");
		}
		else if (iResSuiveur == 2){
			v
		}
		else if (iResSuiveur == 3 ){
			LCD_Printf("Tourner droite\n");
		}
		else if (iResSuiveur == 4){
			LCD_Printf("Autre lecture\n");
		}
		else {
			LCD_Printf("Erreur\n");
		}
		THREAD_MSleep(250);
	}
	
	//THREAD_Destroy(&thread1);
	
	return 0;
}

//*****************************************************************************
// Nom de la fct:   iSuiveur               Fait par: Carlos Torres
// Date:            2017-10-29
// Revision :       A
// Modification :
// Prototype:       int iSuiveur();
// Description:
//
// Parametres   sortie int pour donner l'état de lecture du suiveur.
//
//*****************************************************************************

int iSuiveur(){
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
	iValSuiveur = ANALOG_Read(iAnalog_input_suiveur);
	
	// sortie
	int iResSuiveur = 0;
	
	if (iValSuiveur <= (dVoltCentre + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltCentre - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 1;
	}
	else if (iValSuiveur <= (dVoltGauche + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltGauche - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 2;
	}
	else if (iValSuiveur <= (dVoltDroite + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltDroite - dMargeVolt) * dConvert_V_int)){
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
