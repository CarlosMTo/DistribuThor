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

// Fonction dection zone fonction couleur.

char DetecZone(void)
{
	char cZone;
	int red,blue,green,clear;

	color_Read(red, blue, green, clear);

	//Rouge
	if((red < 632) && (red > 387) && (blue < 140) && (blue > 60) && (green < 230) && (green > 128) && (clear < 660) && (clear > 470))
	{
		cZone = 1;
	}
	//Bleu
	if((red < 130) && (red > 60) && (blue < 333) && (blue > 228) && (green < 185) && (green > 129) && (clear < 461) && (clear > 363))
	{
		cZone = 2;
	}
	//Vert
	if((red < 111) && (red > 55) && (blue < 131) && (blue > 73) && (green < 211) && (green > 145) && (clear < 347) && (clear > 235))
	{
		cZone = 3;
	}
	//Jaune
	if((red < 906) && (red > 790) && (blue < 300) && (blue > 213) && (green < 906) && (green > 773) && (clear < 906) && (clear > 886))
	{
		cZone = 4;
	}
	//Blanc
	if((red < 906) && (red > 886) && (blue < 937) && (blue > 746) && (green < 906) && (green > 886) && (clear < 906) && (clear > 886))
	{
		cZone = 5;
	}
	//Gris
	if((red < 541) && (red > 390) && (blue < 487) && (blue > 379) && (green < 588) && (green > 420) && (clear < 935) && (clear > 886))
	{
		cZone = 6;
	}
	//Rose
	if((red < 875) && (red > 703) && (blue < 380) && (blue > 293) && (green < 490) && (green > 370) && (clear < 906) && (clear > 886))
	{
		cZone = 7;
	}

	return cZone;
}

// Fonction Main

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

	integrationTime_SetValue(INTEGRATION_RED, 4095);
	integrationTime_SetValue(INTEGRATION_GREEN, 4095);
	integrationTime_SetValue(INTEGRATION_BLUE, 4095);
	integrationTime_SetValue(INTEGRATION_CLEAR, 4095);

	MOTOR_SetSpeed(MOTOR_LEFT,46);
	MOTOR_SetSpeed(MOTOR_RIGHT,50);

	while(1)
	{
		switch(DetecZone())
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

		THREAD_MSleep(50);
	}

	return 0;
}

