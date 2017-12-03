/*#include <libarmus.h>
//#include <detecteurSon.h>
#include <pthread.h>

#define PINSON 10

//detecteur de son : vrai ou faux
bool _son = false;
int compteur = 0;

//inutile pour le moment mais potentiellement utile alors dont delete
//pthread_cond_t halt = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


//pointeur de fonction pour le thread aka
//ecouter sur un autre fil d'execution
void *ecouter(void *arg)
{
	//si detection du son initiale
	if(DIGITALIO_Read(PINSON) == 1)
		_son = true;
	//detection 2
	else if(DIGITALIO_Read(PINSON) == 1 && _son)
	{
		//tant que temps < 4 sec, on aditione un compteur
		while(compteur < 21){
			compteur++;
			THREAD_MSleep(200);
		}
	}
	    // Pour enlever le warning
	    (void) arg;
	    pthread_exit(NULL);
}

int main()
{


	pthread_t tson;

	LCD_Printf("nigger\n");
	//creation du thread
    if(pthread_create(&tson, NULL, ecouter, NULL) == -1) {
	perror("pthread_create");
	return EXIT_FAILURE;
    }

    //mock de travail sur le thread principal tant que 4 seconde de son !ecoule
	while(compteur < 21)
	{
		LCD_Printf("je fait autre choses\n");
		THREAD_MSleep(200);
	}
    //potentiellement utile
    pthread_join(tson, NULL);

    LCD_Printf("nigger done \n");
	return 0;
}
*/

/*
============================================================================
 Name : Suiveur Test
 Author : Carlos Torres
 Version : A
 Description : test des entrées analogiques pour le suiveur
============================================================================
*/
// Include Files
#include <libarmus.h>

// Global Functions
int iSuiveur();
bool JoueurDetecte();
void DonnerCarte(); 
void LireBouton();

#define iAnalog_input_suiveur 8

int main()
{
	int iCompteur = 0;

	LCD_ClearAndPrint("\n\n\n\n\n\n\n");

	//MOTOR_SetSpeed(MOTOR_LEFT,40);
	//MOTOR_SetSpeed(MOTOR_RIGHT,42);


	while (1){
		/*for (int i = 30; i < 100; i++)
		{
			MOTOR_SetSpeed(6, i);
			THREAD_MSleep(100);
		}
		MOTOR_SetSpeed(6, 0);
		THREAD_MSleep(100);*/
		//LireBouton();
		if(DIGITALIO_Read(BMP_FRONT))
			DonnerCarte();
		/*if(JoueurDetecte())
		{
			iCompteur++;
			LCD_Printf("%d\n", iCompteur);
		}*/
	}

	return 0;
}
void LireBouton()
{
	if (ANALOG_Read(7) >= 3.5*1023/5)
		LCD_Printf("ALLO!!! \n");
	else
		LCD_Printf("AU REVOIR!!! \n");
}

void DonnerCarte()
{
	MOTOR_SetSpeed(6, 100);
	THREAD_MSleep(1200);
	MOTOR_SetSpeed(6,0);
	THREAD_MSleep(100);
}

bool JoueurDetecte()
{
	int iResSuiveur;
	iResSuiveur = iSuiveur();
	switch(iResSuiveur)
	{
	case 1:
		//LCD_Printf("Tout droit\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	case 2:
		//LCD_Printf("Tourner Gauche\n");
		MOTOR_SetSpeed(MOTOR_LEFT,0);
		MOTOR_SetSpeed(MOTOR_RIGHT,40);
		break;
	case 3:
		//LCD_Printf("Tourner droite\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,0);
		break;
	case 4:
		//LCD_Printf("Autre lecture\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	case 5:
		LCD_Printf("Joueur\n");
		MOTOR_SetSpeed(MOTOR_LEFT,0);
		MOTOR_SetSpeed(MOTOR_RIGHT,0);
		THREAD_MSleep(1000);
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		THREAD_MSleep(250);
		return true;
		break;
	case 6:
		//LCD_Printf("Erreur\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42);
		break;
	default:
		// LCD_Printf("Par défaut\n");
		MOTOR_SetSpeed(MOTOR_LEFT,40);
		MOTOR_SetSpeed(MOTOR_RIGHT,42 );
		break;
	}
	THREAD_MSleep(100);

	return false;
}

//*****************************************************************************
// Nom de la fct:   iSuiveur               Fait par: Carlos Torres
// Date:            2017-10-29
// Revision :       A
// Modification :
// Prototype:       int iSuiveur();
// Description:
//
// Parametres   sortie int pour donner l'état de lecture du suiveur.
//
//*****************************************************************************

int iSuiveur(){
	// Voltages Analogiques
	double dVoltDroite = 3.57;
	double dVoltCentre = 2.14;
	double dVoltGauche = 4.33;

	// marge d'erreur voltage
	double dMargeVolt = 0.10;

	// conversion double analogiques
	double dConvert_V_int = 1023.0/5.0;

	// lecture port analogique
	int iValSuiveur;
	iValSuiveur = ANALOG_Read(iAnalog_input_suiveur);

	// sortie
	int iResSuiveur = 0;

	if ((iValSuiveur <= (dVoltCentre + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltCentre - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 1;
	}
	else if ((iValSuiveur <= (dVoltGauche + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltGauche - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 2;
	}
	else if ((iValSuiveur <= (dVoltDroite + dMargeVolt) * dConvert_V_int) && (iValSuiveur >= (dVoltDroite - dMargeVolt) * dConvert_V_int)){
		iResSuiveur = 3;
	}
	else if (iValSuiveur >=  150){
		iResSuiveur = 4;
	}
	else if (iValSuiveur <=  10/*Valeur obtenue en pratique*/){
		iResSuiveur = 5;
	}
	else{
		iResSuiveur = 6;
	}

	return iResSuiveur;
}


