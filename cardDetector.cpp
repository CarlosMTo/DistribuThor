#include <libarmus.h>
#include "cardDetector.h"
#include "capteurCouleur.h"

int CardDetector::getCouleur()
{

		m_cap.color_Read(m_red,m_green,m_blue,m_clear);


		LCD_ClearAndPrint("R:%d  G:%d  B:%d  C:%d",m_red,m_green,m_blue,m_clear);


	return 0;
}

int CardDetector::getPoint()
{


	return getCouleur();
}
