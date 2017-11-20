#include <libarmus.h>
#include "cardDetector.h"


int CardDetector::getCouleur()
{

	//m_cap.DetecCarte();


		//m_cap.color_Read(m_red,m_green,m_blue,m_clear);


		//LCD_ClearAndPrint("R:%d  G:%d  B:%d  C:%d",m_red,m_green,m_blue,m_clear);


	return m_cap.DetecCarte();
}

int CardDetector::getPoint()
{


	return getCouleur();
}
