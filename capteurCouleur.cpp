#include <libarmus.h>
#include "capteurCouleur.h"

int adjd_dev;

// fonctions globales
CapteurCouleur::CapteurCouleur()
{
	int red, blue, green, clear;

		//initialisation du capteur
		ERROR_CHECK(color_Init(adjd_dev));

		cap_SetValue(CAP_RED, 15);
		cap_SetValue(CAP_GREEN, 15);
		cap_SetValue(CAP_BLUE, 15);
		cap_SetValue(CAP_CLEAR, 15);

		integrationTime_SetValue(INTEGRATION_RED, 4095);
		integrationTime_SetValue(INTEGRATION_GREEN, 4095);
		integrationTime_SetValue(INTEGRATION_BLUE, 4095);
		integrationTime_SetValue(INTEGRATION_CLEAR, 4095);

}

//permet de changer la valeur des registres
void CapteurCouleur::adjd_SetRegister(unsigned char reg, unsigned char val)
{
	unsigned char data[2];
	data[0] = reg;
	data[1] = val;
	armus::i2c_Write(adjd_dev, 2, data);
}

//permet de changer la valeur des registres de 16 bits
void CapteurCouleur::adjd_SetRegister16(unsigned char reg, int val)
{
	unsigned char data[2];
	data[0] = reg;
	data[1] = val & 0xFF;
	armus::i2c_Write(adjd_dev, 2, data);
	data[0] = reg+1;
	data[1] = (val >> 8) & 0xFF;
	armus::i2c_Write(adjd_dev, 2, data);
}

//permet de lire la valeur des registres
unsigned char CapteurCouleur::adjd_ReadRegister(unsigned char reg)
{
	unsigned char val;

	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);

	return val;
}

//permet de lire la valeur des registres de 16 bits
int CapteurCouleur::adjd_ReadRegister16(unsigned char reg)
{
	int val16;
	unsigned char val;
	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);
	val16 = val;
	reg = reg+1;
	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);
	val16 = val16 + ((val << 8) & 0xFF00);
	return val16;
}


// Permet de connaitre la valeur du CAP dans le registre
// prend comme argument une constante CAP_RED, CAP_BLUE, CAP_CLEAR ou CAP_GREEN
// retourne un unsigned char de la valeur
unsigned char CapteurCouleur::cap_GetValue(unsigned char cap_address)
{
	unsigned char cap_value;

	cap_value = adjd_ReadRegister(cap_address);

	return cap_value;
}


// Permet de changer la valeur du CAP dans le registre
// prend comme premier argument une constante CAP_RED, CAP_BLUE, CAP_CLEAR ou CAP_GREEN
// le second argument est compris entre 0 et 15, et determine la valeur du cap a ecrire dans le registre
void CapteurCouleur::cap_SetValue(unsigned char cap_address, unsigned char cap_value)
{
	adjd_SetRegister(cap_address, cap_value);
}



// Permet de connaitre la valeur du CAP dans le registre
// address est une constante comme INTEGRATION_RED, ...
// retourne un int de la valeur
int CapteurCouleur::integrationTime_GetValue(unsigned char address)
{
	int time_value;

	time_value = adjd_ReadRegister16(address);

	return time_value;
}


// Permet de changer la valeur du CAP dans le registre
// address est une constante comme INTEGRATION_RED, ...
// time_value est compris entre 0 et 4095
void CapteurCouleur::integrationTime_SetValue(unsigned char address, int time_value)
{
	adjd_SetRegister16(address, time_value);
}


// Vous devez vous-meme modifier cette fonction tout dependamment de la sortie numerique utilisee
void CapteurCouleur::led_TurnOff()
{
	// TODO : code a changer
	DIGITALIO_Write(9, 0);
}

// Vous devez vous-meme modifier cette fonction tout dependamment de la sortie numerique utilisee
void CapteurCouleur::led_TurnOn()
{
	// TODO : code a changer
	DIGITALIO_Write(9, 1);
}

// permet de faire une lecture differentielle avec et sans eclairage de la led
void CapteurCouleur::color_Read(int& data_red, int& data_blue, int& data_green, int& data_clear)
{
	//premiere lecture sans eclairage
	led_TurnOff();

	adjd_SetRegister(ADJD_REG_CONFIG, 1 << CONFIG_TOFS);
	adjd_SetRegister(ADJD_REG_CTRL, 1 << CTRL_GOFS);
	while(adjd_ReadRegister(ADJD_REG_CTRL))
	{
		THREAD_MSleep(50);
	}

	//lecture avec eclairage
	led_TurnOn();
	adjd_SetRegister(ADJD_REG_CTRL, 1 << CTRL_GSSR);
	while(adjd_ReadRegister(ADJD_REG_CTRL))
	{
		THREAD_MSleep(50);
	}

	//eteindre la led
	led_TurnOff();

	data_red = adjd_ReadRegister16(DATA_RED_LO);
	data_green = adjd_ReadRegister16(DATA_GREEN_LO);
	data_blue = adjd_ReadRegister16(DATA_BLUE_LO);
	data_clear = adjd_ReadRegister16(DATA_CLEAR_LO);
}

void CapteurCouleur::color_ReadToCalibrate(int& data_red, int& data_blue, int& data_green, int& data_clear)
{
	led_TurnOn();
	adjd_SetRegister(ADJD_REG_CONFIG, 0 << CONFIG_TOFS);
	adjd_SetRegister(ADJD_REG_CTRL, 1 << CTRL_GSSR);
	while(adjd_ReadRegister(ADJD_REG_CTRL))
	{
		THREAD_MSleep(50);
	}
	led_TurnOff();

	data_red = adjd_ReadRegister16(DATA_RED_LO);
	data_green = adjd_ReadRegister16(DATA_GREEN_LO);
	data_blue = adjd_ReadRegister16(DATA_BLUE_LO);
	data_clear = adjd_ReadRegister16(DATA_CLEAR_LO);

}

// l argument est un integer qui ne doit pas etre modifie
int CapteurCouleur::color_Init(int& dev_handle)
{
	int error;
	error = armus::i2c_RegisterDevice(ADJD_S371_QR999_SADR, 100000, 1000, dev_handle);

	return error;
}

// Fonction dection zone fonction couleur.

int CapteurCouleur::DetecCarte(int cPointage)
{
	int cCarte = 0;
	int red,blue,green,clear;
	float Y,U,V;

	color_Read(red, blue, green, clear);

	Y = (0.299 * red) + (0.587 * green) + (0.114 * blue);
	U = (0.492 *(blue - Y));
	V = (0.877 * (red - Y));

	//LCD_ClearAndPrint ("Y: %.3f  U: %.3f  V: %.3f  ",Y,U,V);

	//10
	if((Y < 140) && (Y > 70) && (U < -10) && (U > -22) && (V < 40) && (V > 5) )
	{

		LCD_ClearAndPrint("Valeur de la carte: 10");
		cCarte = 10;
	}

	//6
	if((Y < 555) && (Y > 435) && (U < (-150)) && (U > (-190)) && (V < 405) && (V > 349) )
	{
		LCD_ClearAndPrint("Valeur de la carte: 6");
		cCarte = 6;
	}


	//9
	if((Y < 218) && (Y > 175) && (U < (-41)) && (U > (-52)) && (V < 83) && (V > 67) )
	{
		LCD_ClearAndPrint("Valeur de la carte: 9");
		cCarte = 9;
	}
	//Ace
	if((Y < 340) && (Y > 259) && (U < 150) && (U > (-73)) && (V < (-50)) && (V >(-85) ) )
		{
			LCD_ClearAndPrint("Valeur de la carte: Ace");
			if (cPointage >= 11)
				cCarte = 1;
			else
				cCarte = 11;
		}
	//7
	if((Y < 270) && (Y > 185) && (U < (-15)) && (U > (-25)) && (V < (-55)) && (V > (-95) ))
		{
			//LCD_ClearAndPrint("Valeur de la carte: 7");
			cCarte = 7;
			LCD_ClearAndPrint("Valeur de la carte: %d", cCarte);
		}
	//4
	if((Y < 935) && (Y > 800) && (U < (-285)) && (U > (-305)) && (V < 70 ) && (V > 60) )
		{
			LCD_ClearAndPrint("Valeur de la carte: 4");
			cCarte = 4;
		}
	//5
	if((Y < 775) && (Y > 650) && (U < 22) && (U > 5) && (V < 26) && (V > 10) )
		{
			LCD_ClearAndPrint("Valeur de la carte: 5");
			cCarte = 5;
		}

	return cCarte;
}
