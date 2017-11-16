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
	MOTOR_SetSpeed(MOTOR_LEFT,35);
	MOTOR_SetSpeed(MOTOR_RIGHT,35);
	
	while (1){
		iResSuiveur = iSuiveur();
		if (iResSuiveur == 1) {
			LCD_Printf("Tout droit\n");
			MOTOR_SetSpeed(MOTOR_LEFT,35);
			MOTOR_SetSpeed(MOTOR_RIGHT,35);
		}
		else if (iResSuiveur == 2){
			LCD_Printf("Tourner Gauche\n");
			MOTOR_SetSpeed(MOTOR_LEFT,0);
			MOTOR_SetSpeed(MOTOR_RIGHT,35);
		}
		else if (iResSuiveur == 3 ){
			LCD_Printf("Tourner droite\n");
			MOTOR_SetSpeed(MOTOR_LEFT,35);
			MOTOR_SetSpeed(MOTOR_RIGHT,0);
		}
		else if (iResSuiveur == 4){
			LCD_Printf("Autre lecture\n");
			MOTOR_SetSpeed(MOTOR_LEFT,35);
			MOTOR_SetSpeed(MOTOR_RIGHT,35);
		}
		else {
			LCD_Printf("Erreur\n");
			MOTOR_SetSpeed(MOTOR_LEFT,35);
			MOTOR_SetSpeed(MOTOR_RIGHT,35);
		}
		THREAD_MSleep(100);
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
	double dVoltDroite = 3.57;
	double dVoltCentre = 2.14;
	double dVoltGauche = 4.33;
	
	// marge d'erreur voltage
	double dMargeVolt = 0.10;
	
	// conversion double analogiques
	double dConvert_V_int = 1023.0/5.0;
	
	// lecture port analogique
	int iValSuiveur;
	iValSuiveur = ANALOG_Read(iAnalog_input_suiveur);
	
	// sortie
	int iResSuiveur = 0;
	
	if ((iValSuiveur <= (dVoltCentre + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltCentre - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 1;
	}
	else if ((iValSuiveur <= (dVoltGauche + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltGauche - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 2;
	}
	else if ((iValSuiveur <= (dVoltDroite + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltDroite - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 3;
	}
	else if (iValSuiveur >=  150){
		iResSuiveur = 4;
	}
	else{
		iResSuiveur = 5;
	}	
	
	return iResSuiveur;
}
