
#include <libarmus.h>
#include <detecteurSon.h>
#include <pthread.h>

#define PINSON 9

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

	//creation du thread
    if(pthread_create(&tson, NULL, ecouter, NULL) == -1) {
	perror("pthread_create");
	return EXIT_FAILURE;
    }

    //mock de travail sur le thread principal tant que 4 seconde de son !ecoule
	while(compteur != 21)
	{
		LCD_Printf ("je fait autre choses");
		THREAD_MSleep(200);
	}
    //potentiellement utile
    //pthread_join(tson, NULL);

	return 0;
}
