// Include Files

#include <libarmus.h>
#include "capteurSon.h"
#include "deplacement.h"
#include "capteurCouleur.h"
#include "capteurInfra.h"
#include "capteurLigne.h"
#include "cardDetector.h"

void DonnerCarte();

#define iAnalog_input_suiveur 8

bool JoueurDetecte();
int iSuiveur();

int main()
{
	char cPhase = 0;
	char cJoueur = 0;
	int cEmplacement = 0;
	int compteur=0;
	int PointJoueur[3] = {0,0,0};
	char oldstateR, oldstateV, actualStateR, actualStateV;
	CardDetector detect;
	CapteurCouleur couleur;

	oldstateV = DIGITALIO_Read(9);
	oldstateR = DIGITALIO_Read(10);


	//AUDIO_PlayFile("OutHere1.wav");

	//AUDIO_SetVolume(60) ;

while(true)
{
	//detect.getPoint();

			/*
			LCD_ClearAndPrint("Voulez-vous une autre carte? \n");
			LCD_ClearAndPrint("Si OUI, appuyez sur le button vert \n"); //digitalread 9
			LCD_ClearAndPrint("Si NON, appuyez sur le button rouge \n");//digitalread 10

			actualStateV = DIGITALIO_Read(9);
			THREAD_MSleep(10);

			if(oldstateV != actualStateV)
			{

				couleur.DetecCarte(PointJoueur[]);

				oldstateV = actualStateV;
				PointJoueur[cEmplacement] += detect.getPoint();
				//tourner le moteur de la carte
			}



			actualStateR = DIGITALIO_Read(10);
			THREAD_MSleep(10);

			if(oldstateR != actualStateR)
			{


				oldstateR = actualStateR;
				cJoueur++;											//no card, move to next player
			}


	THREAD_MSleep(500);
	*/

	switch(cPhase)
	{
	case 0:		//tour 1

		//appel de la fonction emplacement (phil)

		if(JoueurDetecte())
		{
			cEmplacement++;
			//LCD_Printf("%d\n", cEmplacement);


		//if(cJoueur == cEmplacement - 1)//Detection de l'emplacement (Retourner dans cEmplacement)
		//{
			//Points des joueurs
			PointJoueur[cEmplacement - 1] += couleur.DetecCarte(PointJoueur[cEmplacement - 1]);
			//LCD_Printf("%s \n", PointJoueur[cEmplacement-1]);
			THREAD_MSleep(500);
			//donne carte
			DonnerCarte();
			THREAD_MSleep(3000);
			//Points des joueurs
			PointJoueur[cEmplacement - 1] += couleur.DetecCarte(PointJoueur[cEmplacement - 1]);
			THREAD_MSleep(500);
			//donne carte
			DonnerCarte();
			LCD_Printf("\n Pointage du joueur: %d \n", PointJoueur[cEmplacement-1]);
			THREAD_MSleep(5000);
			MOTOR_SetSpeed(MOTOR_LEFT,40);
			MOTOR_SetSpeed(MOTOR_RIGHT,43);
			THREAD_MSleep(1000);
		}
			//cJoueur++; // Prochain joueur;
		//}
		if(cEmplacement == 3)
		{
			cEmplacement = 0;
			cPhase++;
		}

	break;

	case 1:		//tour 2
		/*
		if(cJoueur == cEmplacement)
		{
			LCD_ClearAndPrint("Voulez-vous une autre carte? \n");
			LCD_ClearAndPrint("Si OUI, appuyez sur le button vert \n"); //digitalread 9
			LCD_ClearAndPrint("Si NON, appuyez sur le button rouge \n");//digitalread 10

			oldstate = DIGITALIO_Read(9);
			THREAD_MSleep(10);

			if(oldstate != DIGITALIO_Read(9))
			{
				LCD_ClearAndPrint("Vert");
				PointJoueur[cEmplacement] += detect.getPoint();
				//tourner le moteur de la carte
			}



			oldstate = DIGITALIO_Read(10);
			THREAD_MSleep(10);

			if(oldstate != DIGITALIO_Read(10))
			{
				LCD_ClearAndPrint("Rouge");
				cJoueur++;											//no card, move to next player
			}
		}
		if(cEmplacement == 3)
		{
			if(PointJoueur[cEmplacement] < 17)
			{
				PointJoueur[cEmplacement] += detect.getPoint();

				//tourner moteur de la carte
			}

			if(PointJoueur[cEmplacement] > 17)
				cPhase++;
		}
		*/

	break;

	case 2:		//tour final

		if(PointJoueur[0] > PointJoueur[2])
			LCD_Printf("Joueur 1 a gagne");
		if(PointJoueur[0] < PointJoueur[2])
			LCD_Printf("Joueur 1 a perdu");

		if(PointJoueur[1] > PointJoueur[2])
			LCD_Printf("Joueur 2 a gagne");
		if(PointJoueur[1] < PointJoueur[2])
			LCD_Printf("Joueur 2 a perdu");



	break;
	default:
		break;
	}



}
	return 0;
}

bool JoueurDetecte()
{
	int iResSuiveur;
	iResSuiveur = iSuiveur();
	switch(iResSuiveur)
	{
	case 1:
		//LCD_Printf("Tout droit\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	case 2:
		//LCD_Printf("Tourner Gauche\n");
		MOTOR_SetSpeed(MOTOR_LEFT,0);
		MOTOR_SetSpeed(MOTOR_RIGHT,40);
		break;
	case 3:
		//LCD_Printf("Tourner droite\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,0);
		break;
	case 4:
		//LCD_Printf("Autre lecture\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	case 5:
		LCD_Printf("Joueur\n");
		MOTOR_SetSpeed(MOTOR_LEFT,0);
		MOTOR_SetSpeed(MOTOR_RIGHT,0);
		THREAD_MSleep(250);
		return true;
		break;
	case 6:
		//LCD_Printf("Erreur\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	default:
		// LCD_Printf("Par défaut\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	}
	THREAD_MSleep(100);

	return false;
}

int iSuiveur()
{
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
	else if (iValSuiveur <=  10/*Valeur obtenue en pratique*/){
		iResSuiveur = 5;
	}
	else{
		iResSuiveur = 6;
	}

	return iResSuiveur;
}

void DonnerCarte()
{
	MOTOR_SetSpeed(6, 100);
	THREAD_MSleep(1000);
	MOTOR_SetSpeed(6,0);
	THREAD_MSleep(100);
}
