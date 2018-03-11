/*
main.c

*/
/// Il faut appuyer sur d pour lancer le jeu et pour afficher chaque tour il faut appuyer sur d
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#define TAILLE_BLOC         30 // Taille d'un bloc (carr�) en pixels
#define SIZE                20
#define LARGEUR_FENETRE     TAILLE_BLOC * SIZE
#define HAUTEUR_FENETRE     TAILLE_BLOC * SIZE
#define joueurs 2

enum {VIDE, MUR};

int rand_a_b(int a, int b){
    //srand(time(NULL));
    return rand()%(b-a)+ a;  //retourne un entier al�atoire compris entre les nombres donn�s
                            //lors de l'appel de la fonction
}

void alea_carte(int carte[SIZE][SIZE])  //pose un nombre d�fini de 1 dans un carteleau
{
    int i,j;
    int nbrblocs;
    nbrblocs =200; //le nombre de 1 � mettre
    printf("%d\n", nbrblocs);
    while(nbrblocs>0)
    {
        for(i=0; i<SIZE; i++)
        {
            for(j=0; j<SIZE; j++)
            {
                if(nbrblocs!=0)
                {
                    carte[i][j]=rand()%2;
                }
                if(carte[i][j]==1)
                {
                    nbrblocs=nbrblocs-1;
                }
            }
        }
    }
    carte[(SIZE/2)-1][(SIZE/2)-1] = 2;
    carte[13][13]=3;

}
void init_carte(int carte[SIZE][SIZE]){ //pose al�atoirement les joueurs sur la map (on ne l'utilise car elle ne marche pas sous sdl)
    int i,j, compt = 0, aleaX, aleaY, switcher;
    int nbjoueurs[SIZE*SIZE]; //liste des joueurs � placer( de m�me taille que le carteleau)

    for(i=0; i < SIZE*SIZE; i++) nbjoueurs[i] = 0; //on vide le carteleau pour initialiser nbjoueurs

    for(i=1; i < joueurs; i++) nbjoueurs[i] = i + 2; //on y met les joueurs

    for(i = 0; i < SIZE; i++){ //on recopie les joueurs dans le carteleau
        for(j = 0; j < SIZE; j++){
           carte[i][j] = nbjoueurs[compt];
            compt++;
        }
    }
    for(i = 0; i < SIZE; i++){   //Algo de Knuth pour m�langer
        for(j = 0; j < SIZE; j++){
            aleaX = rand_a_b(0,SIZE);
            aleaY = rand_a_b(0,SIZE);
            switcher = carte[i][j];
            carte[i][j] = carte[aleaX][aleaY];
            carte[aleaX][aleaY] = switcher;
        }
    }
}


int ligne_joueur(int carte[SIZE][SIZE], int n) {//retourne la ligne d'un joueur n et le nmero du joueur
    int i, j;
    for(i=0; i<SIZE; i++){
        for (j=0; j<SIZE; j++){
            if (carte[i][j]==n+1){ /*on retourne si le carteleau vaut le nombre au-dessus du joueur car
                                   la case o� se trouve le J1 vaut 2, celle du J2 vaut 3,...
                                   puisqu'une case avec un 1 correspond � un mur*/
                return(i);
            }
        }
    }
}
int col_joueur(int carte[SIZE][SIZE], int n) {//retourne la colonne d'un joueur, n est le nimeo du joue
    int i,j;
    for(i=0; i<SIZE; i++){
        for (j=0; j<SIZE; j++){
            if (carte[i][j]==n+1){ /*m�me chose que pour la ligne*/
                return(j);
            }
        }
    }
}

void avancer(int carte[SIZE][SIZE], int n){

int direction,cote;
direction=(rand()%5)+1;       //ici on d�cide que les ia jouent al�atoirement c'est � dire qu'elle choisisse al�atoirement la direction o� elles vont
switch (direction) //switch pour diff�rencier les actions
{
case 1 :
    if (ligne_joueur(carte,n)==0){   //Si l'ia est sur la premi�re ligne du carteleau et qu'elle d�cide d'aller en haut, elle ne pourra pas
        break;}
    if (carte[ligne_joueur(carte,n)-1][col_joueur(carte,n)]==1){ //M�me chose s'il y a un mur, elle ne pourra pas aller sur la case
        break;}
    carte[ligne_joueur(carte,n)-1][col_joueur(carte,n)]=n+1;  //Si l'espace est libre et � l'int�rieur du carteleau, elle se d�place
    carte[ligne_joueur(carte,n)+1][col_joueur(carte,n)]=0;
    break;
case 2 :
    if (ligne_joueur(carte,n)==SIZE-1){       /*Si l'ia est sur la derni�re ligne, elle ne peut pas aller en bas. S'il y a un mur, pareil.
                                                Si espace libre alors elle se d�place*/
        break;}
    if (carte[ligne_joueur(carte,n)+1][col_joueur(carte,n)]==1){
        break;}
    carte[ligne_joueur(carte,n)+1][col_joueur(carte,n)]=n+1;
    carte[ligne_joueur(carte,n)][col_joueur(carte,n)]=0;
    break;
case 3 :
    if(col_joueur(carte,n)==SIZE-1){  /*Si l'ia est sur la derni�re colonne ou s'il y a un mur � droite, d�placement � droite
                                    impossible, sinon d�placement effectu�*/
        break;}
    if (carte[ligne_joueur(carte,n)][col_joueur(carte,n)+1]==1){
        break;}

    carte[ligne_joueur(carte,n)][col_joueur(carte,n)+1]=n+1;
    carte[ligne_joueur(carte,n)][col_joueur(carte,n)]=0;
    break;
case 4 :      /*M�me chose qu'avant mais pour la gauche (1�re colonne ou mur = d�placement impossible*/
    if (col_joueur(carte,n)==0){
        break;}
    if (carte[ligne_joueur(carte,n)][col_joueur(carte,n)-1]==1){
        break;}
    carte[ligne_joueur(carte,n)][col_joueur(carte,n)-1]=n+1;
    carte[ligne_joueur(carte,n)][col_joueur(carte,n)+1]=0;
    break;

case 5 : //apr�s avoir choisi de casser un mur, on choisit la direction. Cette action permet de tuer un joueur
    cote=(rand()%4)+1;
    switch(cote)
    {
        case 1: //pour d�truire un bloc au-dessus
            carte[ligne_joueur(carte,n)-1][col_joueur(carte,n)]=0;

            break;
        case 2 : //d�truire un bloc en dessous
            carte[ligne_joueur(carte,n)+1][col_joueur(carte,n)]=0;

            break;
        case 3 : //permet de d�truire un bloc � droite
            carte[ligne_joueur(carte,n)][col_joueur(carte,n)+1]=0;

            break;
        case 4 : //permet de d�truire un bloc � gauche
            carte[ligne_joueur(carte,n)][col_joueur(carte,n)-1]=0;

        default :
            break;
    }

default:

    break;}}


void ia(int carte[SIZE][SIZE],int joueur){ //une ia qui choisit al�atoirement sa direction
avancer(carte, joueur);
//pause();
}

void jouer(SDL_Surface* ecran)
{   // on initialise les pointeurs SDL qui nous permettront d'afficher , les murs...
    SDL_Surface *j1= NULL;
    SDL_Surface *j2= NULL;
    SDL_Surface *mur = NULL;
    SDL_Surface *blanc = NULL;
    SDL_Rect position, positionJoueur;
    SDL_Event event;


    int continuer = 1, i = 0, j = 0 , k=0;
    int carte[SIZE][SIZE];
    int numerojoueur=0;
    mur = SDL_LoadBMP("murr.bmp");// on affecte un pointeur aux differentes textures qu'on devra afficher
    j1=SDL_LoadBMP("J2.bmp");
    j2=SDL_LoadBMP("J1.bmp");
    blanc=SDL_LoadBMP("blanc.bmp");



    alea_carte(carte);// chargement de la carte (on la remplit de 0 et de 1 aleatoirement)


    while (continuer ==1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)// utilisation des keycode
            {
            case SDLK_ESCAPE:// echap pour revenir au menu
                continuer = 0;
                break;
            case SDLK_d:// appuyer sur d pour afficher chaque tour(appuyer sur d achaque tour)




// Effacement de l'�cran
                SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
                // on remet un ecran blanc par dessus le menu pour avoir la surface de jeu

// Placement des objets � l'�cran


                for (i = 0 ; i < SIZE ; i++)
                {
                    for (j = 0 ; j < SIZE ; j++)
                    {
                        position.x = i * TAILLE_BLOC;
                        position.y = j * TAILLE_BLOC;

                        switch(carte[i][j])
                        {
                        case 1:
                            SDL_BlitSurface(mur, NULL, ecran, &position);//affiche un mur
                            break;

                        case 2 :
                            SDL_BlitSurface(j1, NULL, ecran, &position);// affiche le joueur 1
                            break;
                        case 3 :
                            SDL_BlitSurface(j2, NULL, ecran, &position);// affiche le joueur 2
                            break;
                        default:
                            break;

                        }
                    }
                }
                for(i=0;i<(joueurs*10);i++){
                        numerojoueur=numerojoueur+1;
                    while(carte[ligne_joueur(carte,numerojoueur)][col_joueur(carte,numerojoueur)]==0){
                numerojoueur=numerojoueur+1;
            }
                   ia(carte,numerojoueur);
                    for (i = 0 ; i < SIZE ; i++)
                {
                    for (j = 0 ; j < SIZE ; j++)
                    {
                        position.x = i * TAILLE_BLOC;
                        position.y = j * TAILLE_BLOC;

                        switch(carte[i][j])
                        {
                        case 0 :
                            SDL_BlitSurface(blanc, NULL, ecran, &position);// permet de pas avoir de dedoublement du personnage dans l'affichage
                            break;
                        case 1:
                            SDL_BlitSurface(mur, NULL, ecran, &position);// par court le tableau et a chaque fois qu'il croise un 1 il blitte un mur
                            break;

                        case 2 :
                            SDL_BlitSurface(j1, NULL, ecran, &position);// affiche joueur 1 sur la carte
                            break;
                        case 3 :
                            SDL_BlitSurface(j2, NULL, ecran, &position);// affiche joueur 2 sur la carte
                            break;
                        default:
                            break;

                        }
                    }
                }
                              if(numerojoueur==joueurs){
            numerojoueur=0; //Si on atteint le dernier joueur, on recommence � 1
           }
                }

                SDL_Flip(ecran);
                continuer=1;
                break;
            default :
                break;
            }
        default :
            break;

        }
    }
}


int main(int argc, char *argv[])  ///Menu
{
    srand(time(NULL));
    SDL_Surface *ecran = NULL, *menu = NULL;// initialisation des pointeurs
    SDL_Rect positionMenu;
    SDL_Event event;

    int continuer = 1;

    SDL_Init(SDL_INIT_VIDEO);

    ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);// on genere la fenetre d'ecran avec la bonne taille
    SDL_WM_SetCaption("Project Laby", NULL);// nom de la fenetre SDL

    menu = SDL_LoadBMP("menuu.bmp");// affichage du menu
    positionMenu.x = 0;
    positionMenu.y = 0;

    while (continuer)// permet d'attendre que le joueur appuie sur d pour lancer le jeu
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT: // pour quitter le jeu
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)// utilisation des keycode
            {
            case SDLK_ESCAPE: // echap pour quitter jeu
                continuer = 0;
                break;
            case SDLK_d: // appuyer sur d pour lancer le jeu
                jouer(ecran);
                break;
            }
        default:
            break;
        }

        // Effacement de l'�cran
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
        SDL_Flip(ecran);
    }

    SDL_FreeSurface(menu);// on libere l'ecran
    SDL_Quit();

    return EXIT_SUCCESS;
}
