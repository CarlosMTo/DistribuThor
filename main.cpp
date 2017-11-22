// Include Files

#include <libarmus.h>
#include "capteurSon.h"
#include "deplacement.h"
#include "capteurCouleur.h"
#include "capteurInfra.h"
#include "capteurLigne.h"
#include "cardDetector.h"


int main()
{
	char cPhase = 0;
	char cJoueur = 0;
	char cEmplacement = 1;
	int PointJoueur[3] = {0,0,0};
	char oldstate;
	CardDetector detect;

while(true)
{
	detect.getPoint();

	THREAD_MSleep(500);

	switch(cPhase)
	{
	case 0:		//tour 1

		//appel de la fonction emplacement (phil)

		//cEmplacement =

		if(cJoueur == cEmplacement)//Detection de l'emplacement (Retourner dans cEmplacement)
		{
			//Points des joueurs
			PointJoueur[cEmplacement] += detect.getPoint();

			//donne carte
			//Points des joueurs
			PointJoueur[cEmplacement] += detect.getPoint();
			//donne carte
			cJoueur++; // Prochain joueur;
		}
		if(cEmplacement == 3)
		{
			cPhase++;
		}

	break;

	case 1:		//tour 2

		if(cJoueur == cEmplacement)
		{
			LCD_ClearAndPrint("Voulez-vous une autre carte? \n");
			LCD_ClearAndPrint("Si oui, appuyez sur le button vert \n"); //digitalread 9
			LCD_ClearAndPrint("Si non, appuyez sur le button rouge \n");//digitalread 10

			oldstate = DIGITALIO_Read(9);
			THREAD_MSleep(10);

			if(oldstate != DIGITALIO_Read(9))
			{
				PointJoueur[cEmplacement] += detect.getPoint();
				//tourner le moteur de la carte
			}



			oldstate = DIGITALIO_Read(10);
			THREAD_MSleep(10);

			if(oldstate != DIGITALIO_Read(10))
			{
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
