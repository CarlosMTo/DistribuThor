#ifndef CARDDETECTOR_H__
#define CARDDETECTOR_H__
#include "capteurCouleur.h"
class CardDetector
{
public:
	int getPoint();


private:
	int getCouleur();
	CapteurCouleur m_cap;
	//CapteurCouleur JoueurUn;
	//CapteurCouleur JoueurDeux;
	//CapteurCouleur JoueurTrois;
	//CapteurCouleur JoueurQuatre;
	int m_red;
	int m_green;
	int m_blue;
	int m_clear;
	char CarteJoueur[4];


};


#endif //CARDDETECTOR_H__

