#ifndef DEPLACEMENT_H__
#define DEPLACEMENT_H__

class Deplacement
{

public:
	int iPID(void);
	void vAvancer();
	void vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN);
	Deplacement(int speed);
private:
	int m_speed;
};


#endif //DEPLACEMENT_H__
