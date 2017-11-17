#ifndef CARDDETECTOR_H__
#define CARDDETECTOR_H__

class CardDetector
{
public:
	int getPoint();


private:
	int getCouleur();
	CapteurCouleur m_cap;
	int m_red;
	int m_green;
	int m_blue;
	int m_clear;



};


#endif //CARDDETECTOR_H__

