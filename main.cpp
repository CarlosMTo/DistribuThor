// Include Files

#include <libarmus.h>
#include "capteurSon.h"
#include "deplacement.h"
#include "capteurCouleur.h"
#include "capteurInfra.h"
#include "capteurLigne.h"
#include "cardDetector.h"

void DonnerCarte(int compteur);

#define iAnalog_input_suiveur 8

bool JoueurDetecte();
int iSuiveur();

int main()
{
	unsigned int stream;
	char cPhase = 0;
	char cJoueur = 0;
	int cEmplacement = 0;
	int compteur=1;
	int PointJoueur[3] = {0,0,0};
	char oldstateR, oldstateV, actualStateR, actualStateV;
	CardDetector detect;
	CapteurCouleur couleur;

	oldstateV = DIGITALIO_Read(11);
	oldstateR = DIGITALIO_Read(10);
	SYSTEM_ResetTimer();

	AUDIO_SetVolume(100) ;
	stream = AUDIO_PlayFile("Commence_Robot.wav");

	while(true)
	{

		if(20 < SYSTEM_ReadTimerMSeconds())
		{
			SYSTEM_ResetTimer();
			DIGITALIO_Write(13,1);
			DIGITALIO_Write(14,0);
		}
		else
		{
			DIGITALIO_Write(13,0);
			DIGITALIO_Write(14,1);
		}




		switch(cPhase)
		{
		case 0:		//tour 1


			//THREAD_MSleep(3000);
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
				THREAD_MSleep(250);
				//donne carte
				DonnerCarte(compteur);
				compteur++;
				THREAD_MSleep(3000);
				//Points des joueurs
				PointJoueur[cEmplacement - 1] += couleur.DetecCarte(PointJoueur[cEmplacement - 1]);
				THREAD_MSleep(250);
				//donne carte
				DonnerCarte(compteur);
				compteur++;
				LCD_Printf("\n Pointage du joueur %d: %d \n", cEmplacement, PointJoueur[cEmplacement-1]);
				THREAD_MSleep(5000);
				MOTOR_SetSpeed(MOTOR_LEFT,45);
				MOTOR_SetSpeed(MOTOR_RIGHT,48);
				THREAD_MSleep(500);
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

			if(JoueurDetecte())
			{
				cEmplacement++;
				if(cEmplacement == 3)
				{
					while(PointJoueur[cEmplacement - 1] < 17)
					{
						PointJoueur[cEmplacement - 1] += couleur.DetecCarte(PointJoueur[cEmplacement - 1]);
						THREAD_MSleep(250);
						//tourner moteur de la carte
						DonnerCarte(compteur);
						THREAD_MSleep(5000);
					}

					if(PointJoueur[cEmplacement - 1] >= 17)
					{
						cPhase++;
					}

					cEmplacement = 0;
					MOTOR_SetSpeed(MOTOR_LEFT,45);
					MOTOR_SetSpeed(MOTOR_RIGHT,48);
					THREAD_MSleep(500);
					break;
				}
				else
				{
					//AUDIO_StopPlayback(stream);
					//stream = AUDIO_PlayFile("Choix_Robot.wav");
					LCD_ClearAndPrint("\n Pointage du joueur %d: %d \n", cEmplacement, PointJoueur[cEmplacement-1]);
					LCD_Printf("Voulez-vous une autre carte? \n");
					LCD_Printf("Si OUI, appuyez sur le bouton vert \n"); //digitalread 9
					LCD_Printf("Si NON, appuyez sur le bouton rouge \n");//digitalread 10

					actualStateR = DIGITALIO_Read(10); // bouton rouge
					THREAD_MSleep(10);

					while(oldstateR == actualStateR && PointJoueur[cEmplacement - 1] < 21)
					{

						actualStateV = DIGITALIO_Read(11);		//bouton vert
						THREAD_MSleep(10);

						if(oldstateV != actualStateV)
						{
							oldstateV = actualStateV;
							PointJoueur[cEmplacement - 1] += couleur.DetecCarte(PointJoueur[cEmplacement - 1]);
							THREAD_MSleep(250);
							//tourner le moteur de la carte
							DonnerCarte(compteur);
							LCD_Printf("\n Pointage du joueur %d: %d \n", cEmplacement, PointJoueur[cEmplacement-1]);
							THREAD_MSleep(1000);
						}

						actualStateR = DIGITALIO_Read(10);		//no card, move to next player
						//THREAD_MSleep(500);
					}

					oldstateR = actualStateR;
					MOTOR_SetSpeed(MOTOR_LEFT,45);
					MOTOR_SetSpeed(MOTOR_RIGHT,48);
					THREAD_MSleep(500);
				}
			}
			/*
			if(cEmplacement == 3)
			{
				if(PointJoueur[cEmplacement - 1] < 17)
				{
					PointJoueur[cEmplacement - 1] += couleur.DetecCarte(PointJoueur[cEmplacement - 1]);

					//tourner moteur de la carte
					DonnerCarte(compteur);
					THREAD_MSleep(5000);
				}

				if(PointJoueur[cEmplacement - 1] >= 17)
					cPhase++;

				cEmplacement = 0;
			}
*/

		break;

		case 2:		//tour final
			LCD_ClearAndPrint("Pointage du croupier: %d \n", PointJoueur[2]);
			if((PointJoueur[0] > PointJoueur[2] && PointJoueur[0] <= 21) || (PointJoueur[0] <= 21 && PointJoueur[2] > 21))
				LCD_Printf("Joueur 1 a gagne avec %d\n", PointJoueur[0]);
			else if(PointJoueur[0] < PointJoueur[2] || PointJoueur[0] > 21)
				LCD_Printf("Joueur 1 a perdu avec %d\n", PointJoueur[0]);
			else
				LCD_Printf("Joueur 1 et le croupier sont a egalite avec %d\n", PointJoueur[0]);

			if((PointJoueur[1] > PointJoueur[2] && PointJoueur[1] <= 21)  || (PointJoueur[1] <= 21 && PointJoueur[2] > 21))
				LCD_Printf("Joueur 2 a gagne avec %d\n", PointJoueur[1]);
			else if(PointJoueur[1] < PointJoueur[2] || PointJoueur[1] > 21)
				LCD_Printf("Joueur 2 a perdu avec %d\n", PointJoueur[1]);
			else
				LCD_Printf("Joueur 2 et le croupier sont a egalite avec %d\n", PointJoueur[1]);
			cPhase++;

			MOTOR_SetSpeed(MOTOR_LEFT,0);
			MOTOR_SetSpeed(MOTOR_RIGHT,0);

		break;
		default:
			AUDIO_StopPlayback(stream);
			stream = AUDIO_PlayFile("Fin_Robot.wav");
			LCD_Printf("Fin de la partie! Merci d'avoir participe! \n");
			THREAD_MSleep(5000);
			return 0;
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
		MOTOR_SetSpeed(MOTOR_LEFT,45);
		MOTOR_SetSpeed(MOTOR_RIGHT,48);
		break;
	case 2:
		//LCD_Printf("Tourner Gauche\n");
		MOTOR_SetSpeed(MOTOR_LEFT,0);
		MOTOR_SetSpeed(MOTOR_RIGHT,48);
		break;
	case 3:
		//LCD_Printf("Tourner droite\n");
		MOTOR_SetSpeed(MOTOR_LEFT,45);
		MOTOR_SetSpeed(MOTOR_RIGHT,0);
		break;
	case 4:
		//LCD_Printf("Autre lecture\n");
		MOTOR_SetSpeed(MOTOR_LEFT,45);
		MOTOR_SetSpeed(MOTOR_RIGHT,48);
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
		MOTOR_SetSpeed(MOTOR_LEFT,45);
		MOTOR_SetSpeed(MOTOR_RIGHT,48);
		break;
	default:
		// LCD_Printf("Par défaut\n");
		MOTOR_SetSpeed(MOTOR_LEFT,45);
		MOTOR_SetSpeed(MOTOR_RIGHT,48);
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

void DonnerCarte(int compteur)
{
	MOTOR_SetSpeed(6, 100);
	if (compteur % 2 != 0)
		THREAD_MSleep(750);
	else
		THREAD_MSleep(750);
	MOTOR_SetSpeed(6,0);
	THREAD_MSleep(100);
}
