#include <libarmus.h>

#define ADJD_S371_QR999_SADR 	0x74
#define CAP_RED					0x6
#define CAP_GREEN				0x7
#define CAP_BLUE				0x8
#define CAP_CLEAR				0x9
#define INTEGRATION_RED			10
#define INTEGRATION_GREEN		12
#define INTEGRATION_BLUE		14
#define INTEGRATION_CLEAR		16
#define ADJD_REG_CTRL			0
#define ADJD_REG_CONFIG			1
#define DATA_RED_LO				64
#define DATA_GREEN_LO			66
#define DATA_BLUE_LO			68
#define DATA_CLEAR_LO			70

#define CTRL_GSSR				0
#define CTRL_GOFS				1

#define CONFIG_TOFS				0

int adjd_dev;


// fonctions globales


//permet de changer la valeur des registres
void adjd_SetRegister(unsigned char reg, unsigned char val)
{
	unsigned char data[2];
	data[0] = reg;
	data[1] = val;
	armus::i2c_Write(adjd_dev, 2, data);
}

//permet de changer la valeur des registres de 16 bits
void adjd_SetRegister16(unsigned char reg, int val)
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
unsigned char adjd_ReadRegister(unsigned char reg)
{
	unsigned char val;

	armus::i2c_ReadAfterWrite(adjd_dev, 1, &reg, 1, &val);

	return val;
}

//permet de lire la valeur des registres de 16 bits
int adjd_ReadRegister16(unsigned char reg)
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
unsigned char cap_GetValue(unsigned char cap_address)
{
	unsigned char cap_value;

	cap_value = adjd_ReadRegister(cap_address);

	return cap_value;
}


// Permet de changer la valeur du CAP dans le registre
// prend comme premier argument une constante CAP_RED, CAP_BLUE, CAP_CLEAR ou CAP_GREEN
// le second argument est compris entre 0 et 15, et determine la valeur du cap a ecrire dans le registre
void cap_SetValue(unsigned char cap_address, unsigned char cap_value)
{
	adjd_SetRegister(cap_address, cap_value);
}



// Permet de connaitre la valeur du CAP dans le registre
// address est une constante comme INTEGRATION_RED, ...
// retourne un int de la valeur
int integrationTime_GetValue(unsigned char address)
{
	int time_value;

	time_value = adjd_ReadRegister16(address);

	return time_value;
}


// Permet de changer la valeur du CAP dans le registre
// address est une constante comme INTEGRATION_RED, ...
// time_value est compris entre 0 et 4095
void integrationTime_SetValue(unsigned char address, int time_value)
{
	adjd_SetRegister16(address, time_value);
}


// Vous devez vous-meme modifier cette fonction tout dependamment de la sortie numerique utilisee
void led_TurnOff()
{
	// TODO : code a changer
	DIGITALIO_Write(9, 0);
}

// Vous devez vous-meme modifier cette fonction tout dependamment de la sortie numerique utilisee
void led_TurnOn()
{
	// TODO : code a changer
	DIGITALIO_Write(9, 1);
}

// permet de faire une lecture differentielle avec et sans eclairage de la led
void color_Read(int& data_red, int& data_blue, int& data_green, int& data_clear)
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

void color_ReadToCalibrate(int& data_red, int& data_blue, int& data_green, int& data_clear)
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
int color_Init(int& dev_handle)
{
	int error;
	error = armus::i2c_RegisterDevice(ADJD_S371_QR999_SADR, 100000, 1000, dev_handle);

	return error;
}

char DetecZone(void)
{
	char cZone;
	int red,blue,green,clear;

	color_Read(red, blue, green, clear);

	//Rouge
	if((red < 43) && (red > 26) && (blue < 16) && (blue > 0) && (green < 22) && (green > 4) && (clear < 52) && (clear > 33))
	{
		cZone = 1;
	}
	//Bleu
	if((red < 17) && (red > 0) && (blue < 31) && (blue > 12) && (green < 21) && (green > 3) && (clear < 43) && (clear > 25))
	{
		cZone = 2;
	}
	//Vert
	if((red < 16) && (red > 0) && (blue < 17) && (blue > 0) && (green < 21) && (green > 3) && (clear < 31) && (clear > 12))
	{
		cZone = 3;
	}
	//Jaune
	if((red < 77) && (red > 59) && (blue < 26) && (blue > 8) && (green < 71) && (green > 53) && (clear < 136) && (clear > 118))
	{
		cZone = 4;
	}
	//Blanc
	if((red < 75) && (red > 56) && (blue < 65) && (blue > 47) && (green < 80) && (green > 61) && (clear < 172) && (clear > 155))
	{
		cZone = 5;
	}
	//Gris
	if((red < 46) && (red > 28) && (blue < 42) && (blue > 24) && (green < 50) && (green > 31) && (clear < 102) && (clear > 82))
	{
		cZone = 6;
	}
	//Rose
	if((red < 62) && (red > 43) && (blue < 31) && (blue > 13) && (green < 37) && (green > 19) && (clear < 90) && (clear > 72))
	{
		cZone = 7;
	}

	switch(cZone)
			{
				case 1:
					LCD_ClearAndPrint("Zone : Rouge");
				break;

				case 2:
					LCD_ClearAndPrint("Zone : Bleu");
				break;

				case 3:
					LCD_ClearAndPrint("Zone : Vert");
				break;

				case 4:
					LCD_ClearAndPrint("Zone : Jaune");
				break;

				case 5:
					LCD_ClearAndPrint("Zone : Blanc");
				break;

				case 6:
					LCD_ClearAndPrint("Zone : Gris");
				break;

				case 7:
					LCD_ClearAndPrint("Zone : Rose");
				break;

				default:
					LCD_ClearAndPrint("Zone : Aucun");
				break;
			}

	LCD_Printf("R=%d, G=%d, B=%d, C=%d", red, green, blue, clear);

	return cZone;
}

char TestHue(void)
{
	char couleur;
	int red,blue,green,clear,r,g,b;
	float hue;

	color_Read(red, blue, green, clear);

	r = red/1024;
	g = blue/1024;
	b = green/1024;

	return couleur;
}

int main()
{
	int red, blue, green, clear;
	char Zone = 0;

	//initialisation du capteur
	ERROR_CHECK(color_Init(adjd_dev));

	cap_SetValue(CAP_RED, 15);
	cap_SetValue(CAP_GREEN, 15);
	cap_SetValue(CAP_BLUE, 15);
	cap_SetValue(CAP_CLEAR, 15);

	integrationTime_SetValue(INTEGRATION_RED, 255);
	integrationTime_SetValue(INTEGRATION_GREEN, 255);
	integrationTime_SetValue(INTEGRATION_BLUE, 255);
	integrationTime_SetValue(INTEGRATION_CLEAR, 255);

	while(1)
	{
		//color_Read(red, blue, green, clear);
		//LCD_ClearAndPrint("R=%d, G=%d, B=%d, C=%d", red, green, blue, clear);

		//Zone = ;
		//LCD_ClearAndPrint("%d",DetecZone());
		/*switch(DetecZone())
		{
		case 1:
			LCD_ClearAndPrint("Zone : Rouge");
		break;

		case 2:
			LCD_ClearAndPrint("Zone : Bleu");
		break;

		case 3:
			LCD_ClearAndPrint("Zone : Vert");
		break;

		case 4:
			LCD_ClearAndPrint("Zone : Jaune");
		break;

		case 5:
			LCD_ClearAndPrint("Zone : Blanc");
		break;

		case 6:
			LCD_ClearAndPrint("Zone : Gris");
		break;

		case 7:
			LCD_ClearAndPrint("Zone : Rose");
		break;

		default:
			LCD_ClearAndPrint("Zone : Aucun");
		break;

		}*/

		THREAD_MSleep(500);
	}

	return 0;
}

