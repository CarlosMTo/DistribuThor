#include "capteurSon.h"
#include <libarmus.h>

#define PIN_SON 3


void CapteurSon::ecouter()
{
	if(ANALOG_Read(PIN_SON) > 900)
		LCD_Printf("AAAAAAAAAAAAAAAAAAA HIGH \n");

	while(1)
	{
		if(ANALOG_Read(PIN_SON) > 900)
			LCD_Printf("AAAAAAAAAAAAAAAAAAA HIGH \n");
		//if(DIGITALIO_Read(PIN_SON) == 1)
			//m_son = true;
		/*if(m_son)
		{
			MOTOR_SetSpeed(MOTOR_LEFT,50);
			MOTOR_SetSpeed(MOTOR_RIGHT,50);
		}*/

		THREAD_MSleep(40);
	}
}
