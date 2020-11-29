#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

// les constatntes
#define R 0
#define W 1
#define len 100


// defintions des fonctions du jeu



// permet de voir si le mot saisi existe dans la chaine
int rechercheLettre(char lettre, char motSecret[], int lettreTrouvee[])
{
    int i = 0;
    int bonneLettre = 0;

    // On parcourt motSecret pour vérifier si la lettre proposée y est 
    for (i = 0 ; motSecret[i] != '\0' ; i++)
    {
        if (lettre == motSecret[i]) // Si la lettre y est
        {
            bonneLettre = 1; // On mémorise que c'était une bonne lettre
            lettreTrouvee[i] = 1; // On met à 1 la case du tableau de booléens correspondant à la lettre actuelle
        }
    }

    return bonneLettre;
}


// verifie si le joueur a gagné
int gagne(int lettreTrouvee[])
{
    int i = 0;
    int joueurGagne = 1;

    for (i = 0 ; i < 6 ; i++)
    {
        if (lettreTrouvee[i] == 0)
            joueurGagne = 0;
    }

    return joueurGagne;
}



int main(int argc, char* argv[])
{	

	// declarations des variables
	int PversF[2],FversP[2];
	char *phrase = "Bonjour pere comment tu vas ?";
	char *phrase1 = "Bonjour fils je vais bien et toi? ";
	char message[6] ;
	int message1[6];
	
	// pendu variables 
	char lettre = 0; // Stocke la lettre proposée par l'utilisateur (retour du scanf)
       char motSecret[] = "TONTON"; // C'est le mot à trouver
       
       
       int i = 0; 
	
	char tableauLettres[26]; // contient toutes les lettres entrées par le fils
	
	
	
	
	if(pipe(PversF) ==-1 ){
	
		puts("erreur lors de la creation du tube PversF\n");
		exit(0);
	}if( pipe(FversP)== -1 ){
	
		puts("erreur lors de la creation du tube FversP\n");
		exit(0);
	}

	switch(fork()){
	
	case -1:
		{
			puts("erreur de creation du processs fils\n");
			exit(1);
		};break;
	case 0:
		{	// le processus fils
				
       			int lettreTrouvee[6] = {0};
       			int coupsRestants = 10;
       			
				close(PversF[W]); // on empeche le pere d'ecrire
				close(FversP[R]);// on empeche le fils de lire
				
				read(PversF[R],message,6);
				 
				 // generation d'une chaine de caractere aleatoirement
			   	srand(time(NULL));
				/*for(int i=0; i<26; i++){
				 	tableauLettres[i] = rand()%26+'A';
			    	}*/
				
				
				while (coupsRestants > 0 && !gagne(lettreTrouvee))
				{
					printf("\n\nIl vous reste %d coups a jouer", coupsRestants);
					printf("\nQuel est le mot secret ? ");

					/* On affiche le mot secret en masquant les lettres non trouvées
						Exemple : *A**ON */
					for (i = 0 ; i < 6 ; i++)
					{
					    if (lettreTrouvee[i]) // Si on a trouvé la lettre n° i
						printf("%c", message[i]); // On l'affiche
					    else
						printf("*"); // Sinon, on affiche une étoile pour les lettres non trouvées
					}
					
					printf("\nProposez une lettre : ");
					
					lettre = rand()%26+'A'; // generation du mot aleatoire
					
					printf(" %c  ",lettre);
					
					if (!rechercheLettre(lettre, message, lettreTrouvee))
					    {
						coupsRestants--; // On enlève un coup au joueur
					    }
				}
				
				
				write(FversP[W],lettreTrouvee,sizeof(tableauLettres) + 1);
				
				
				
		};break;
			
	default:
			{ // le processus pere
			
				close(FversP[W]); //on empeche le fils de d'ecrire
				close(PversF[R]); // on empeche le pere de lire
				
				write(PversF[W],motSecret, sizeof(tableauLettres) + 1);
				read(FversP[R],message1,6) ;
				
				if (gagne(message1))
			   	 printf("\n\nGagne ! Le mot secret etait bien : %s \n\n", motSecret);
				else
			   	 printf("\n\nPerdu ! Le mot secret etait : %s \n\n", motSecret);

				
				
				
			}
		}
	

	

    return 0;
}
