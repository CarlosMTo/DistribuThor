

// Include Files

#include <libarmus.h>
#include "capteurSon.h"
#include "deplacement.h"
#include "capteurCouleur.h"
#include "capteurInfra.h"
#include "capteurLigne.h"
#include "capteurs.h"

extern int _iVitesse;

// Global Functions

int main()
{
	bool Actif;
	//LCD_Printf("nigger \n");

	CapteurSon cptson;
	Deplacement dep;
	while(1)
		{
			Actif = cptson.ecouter(Actif);

			if(Actif == true)
			{
				LCD_ClearAndPrint("Demarage");
				dep.vAvancer(10, _iVitesse);
			}
			else
			{
				LCD_ClearAndPrint("Arret");
				dep.vAvancer(0,0);
			}
		}
	return 0;
}
