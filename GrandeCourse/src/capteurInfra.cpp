#include <capteurInfra.h>
#include <math.h>
#include <libarmus.h>

float CapteurInfra::DistanceCapteurChoisi(int numCapteur)
{
	DIGITALIO_Write(A0, 0); // A0
	DIGITALIO_Write(A1, 0); // A1
	DIGITALIO_Write(A2, 0); // A2

	switch(numCapteur)
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

	return CalculDeDistance(voltage(ANALOG_Read(2)));
}


float CapteurInfra::CalculDeDistance(float fTensionDistance)
{
	float fDistance;

	fDistance = 28.652 * pow(fTensionDistance, -1.167);

	return fDistance;
}

float CapteurInfra::voltage(int EntreeAnalogique)
{
	// Les valeurs lues varient entre 0 et 1023. 1023 valant 5V.
	// On convertit cette valeur en une valeur de tension pour calculer la distance
	float voltage = ((float)EntreeAnalogique) * 5.0 / 1023.0;
	return voltage;
}
