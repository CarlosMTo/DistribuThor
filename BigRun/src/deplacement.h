#ifndef DEPLACEMENT_H__
#define DEPLACEMENT_H__

class Deplacement
{

public:
	int iPID(void);
	void vAvancer(int distance,int ucVitesse);
	void vTourner(int iAngle, int iMoteur_STOP, int iMoteur_RUN, int iEncodeur_RUN);
//private:

};


#endif DEPLACEMENT_H__
