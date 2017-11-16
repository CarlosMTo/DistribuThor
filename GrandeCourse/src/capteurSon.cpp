#include "capteurSon.h"
#include <libarmus.h>

#define PIN_SON 3


/*bool CapteurSon::ecouter(bool Etat)
{
		if(ANALOG_Read(PIN_SON) > 850)
		{
			LCD_Printf("%i \n", ANALOG_Read(PIN_SON));
			Etat = !Etat;
		}
		//if(DIGITALIO_Read(PIN_SON) == 1)
			//m_son = true;
		/*if(m_son)
		{
			MOTOR_SetSpeed(MOTOR_LEFT,50);
			MOTOR_SetSpeed(MOTOR_RIGHT,50);
		}

		return Etat;
		//THREAD_MSleep(40);
}*/
void CapteurSon::ecouter()
{
		if(ANALOG_Read(PIN_SON) > 700 && m_son == false)
			m_son = true;
			//LCD_Printf("%i \n", ANALOG_Read(PIN_SON));


		//if(DIGITALIO_Read(PIN_SON) == 1)
			//m_son = true;
		/*if(m_son)
		{
			MOTOR_SetSpeed(MOTOR_LEFT,50);
			MOTOR_SetSpeed(MOTOR_RIGHT,50);
		}*/

		//return Etat;
		//THREAD_MSleep(40);
}
