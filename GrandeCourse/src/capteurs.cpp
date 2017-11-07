#include <capteurs.h>
#include <math.h>
float Capteur::GetDistance(int capteur)
{
	DIGITALIO_Write(A0, 0); // A0
	DIGITALIO_Write(A1, 0); // A1
	DIGITALIO_Write(A2, 0); // A2

	switch(capteur)
	{
	case 1:
		DIGITALIO_Write(A0, 1); // A0
		DIGITALIO_Write(A1, 0); // A1
		break;
	case 2:
		DIGITALIO_Write(A0, 0); // A0
		DIGITALIO_Write(A1, 1); // A1
		break;
	case 0:
		DIGITALIO_Write(A0, 0); // A0
		DIGITALIO_Write(A1, 0); // A1
		break;
	default:
		break;
	}

	float distance = 28.652 * pow((((float)ANALOG_Read(2)) * 5.0 / 1023.0), -1.167);
	return distance;
}
