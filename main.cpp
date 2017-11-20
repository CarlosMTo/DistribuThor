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
	char cEmplacement = 0;
	int PointJoueur[3] = {0,0,0};
	CardDetector detect;

while(true)
{
	detect.getPoint();

	THREAD_MSleep(500);

	switch(cPhase)
	{
	case 0:
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
		if(cEmplacement == 2)
		{
			cPhase++;
		}
	break;

	case 1:
	break;

	case 2:
	break;

	default:
	break;
	}



}



	return 0;
}
