#ifndef CAPTEURCOULEUR_H__
#define CAPTEURCOULEUR_H__

class CapteurCouleur
{

public:
	char DetecZone(void);
	int  color_Init(int& dev_handle);
	void cap_SetValue(unsigned char cap_address, unsigned char cap_value);


private:
	void adjd_SetRegister(unsigned char reg, unsigned char val);
	void adjd_SetRegister16(unsigned char reg, int val);
	unsigned char adjd_ReadRegister(unsigned char reg);
	int adjd_ReadRegister16(unsigned char reg);
	void integrationTime_SetValue(unsigned char address, int time_value);
	int integrationTime_GetValue(unsigned char address);
	unsigned char cap_GetValue(unsigned char cap_address);
	void led_TurnOff();
	void led_TurnOn();
	void color_Read(int& data_red, int& data_blue, int& data_green, int& data_clear);
	void color_ReadToCalibrate(int& data_red, int& data_blue, int& data_green, int& data_clear);

};


#endif //CAPTEURCOULEUR_H__
