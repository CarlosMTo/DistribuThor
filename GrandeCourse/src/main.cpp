

// Include Files

#include <libarmus.h>
#include "capteurSon.h"
#include "deplacement.h"
#include "capteurCouleur.h"
#include "capteurInfra.h"
#include "capteurLigne.h"
extern int _iVitesse;

// Global Functions
extern float _compteurms;
int sec = 0;
bool init = false;
int main()
{

	//bool Actif = false;
	LCD_Printf("nigger \n");
	//LCD_Printf("%i \n", ANALOG_Read(3));

	CapteurSon cptson;
	cptson.m_son = false;
	Deplacement dep(59);
	CapteurCouleur coul;
	SYSTEM_ResetTimer();
	while(true)
	{
		cptson.ecouter();
		//LCD_Printf("%i \n", ANALOG_Read(3));
		//LCD_Printf("%f \n", _compteurms );
		if(cptson.m_son == true)
		{
			init = true;
			//LCD_Printf("%i", init );
			dep.vAvancer();
		}
		if(sec >= 5)
		{
			MOTOR_SetSpeed(MOTOR_LEFT,0);
			MOTOR_SetSpeed(MOTOR_RIGHT,0);
			break;
		}

		if(init)
		{
			if(_compteurms > 999)
			{
				LCD_Printf("%i", sec);
				_compteurms = 0;
				SYSTEM_ResetTimer();
				sec++;
			}
		}
		else
		{
			//LCD_Printf("nope");
			SYSTEM_ResetTimer();
		}

		THREAD_MSleep(100);
	}

	LCD_ClearAndPrint("eat shit and die");
	/*while(1)
		{
			//Actif = cptson.ecouter(Actif);
			cptson.ecouter();
			LCD_Printf("%i \n", ANALOG_Read(3));

			if(cptson.m_son == true)
			{
				LCD_ClearAndPrint("Demarage");
				//dep.vAvancer(10, _iVitesse);
				dep.vAvancer(10, 59);
			}
			else
			{
				LCD_ClearAndPrint("Arret");
				dep.vAvancer(0,0);
			}
			THREAD_MSleep(70);
		}*/
	return 0;
}
