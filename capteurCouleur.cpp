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

char CapteurCouleur::DetecZone(void)
{
	char cZone;
	int red,blue,green,clear;

	color_Read(red, blue, green, clear);

	//Rouge
	if((red < 652) && (red > 367) && (blue < 160) && (blue > 40) && (green < 230) && (green > 108) && (clear < 680) && (clear > 450))
	{
		cZone = 1;
	}
	//Bleu
	if((red < 150) && (red > 40) && (blue < 353) && (blue > 208) && (green < 205) && (green > 109) && (clear < 481) && (clear > 343))
	{
		cZone = 2;
	}
	//Vert
	if((red < 131) && (red > 35) && (blue < 151) && (blue > 53) && (green < 231) && (green > 125) && (clear < 367) && (clear > 215))
	{
		cZone = 3;
	}
	//Jaune
	if((red < 926) && (red > 770) && (blue < 320) && (blue > 193) && (green < 926) && (green > 753) && (clear < 926) && (clear > 866))
	{
		cZone = 4;
	}
	//Blanc
	if((red < 926) && (red > 866) && (blue < 957) && (blue > 726) && (green < 926) && (green > 866) && (clear < 926) && (clear > 866))
	{
		cZone = 5;
	}
	//Gris
	if((red < 561) && (red > 370) && (blue < 507) && (blue > 359) && (green < 608) && (green > 400) && (clear < 955) && (clear > 866))
	{
		cZone = 6;
	}
	//Rose
	if((red < 885) && (red > 693) && (blue < 390) && (blue > 283) && (green < 500) && (green > 360) && (clear < 916) && (clear > 876))
	{
		cZone = 7;
	}

	return cZone;
}
