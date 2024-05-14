#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include "ANNONE_LLC_BIBLIO.h"
#define true 1
#define false 0

// PROGRAMME PRINCIPAL
int main()
{

    interface();

    /*
    -----------------------------------------------  LES DECLARATIONS  -------------------------------------------------------------
     */

    annonceur *tete = NULL;                 // Créer un pointeur pour les listes des annonceurs
    annonce *offre = NULL, *demande = NULL; // Créer des pointeurs pour les listes des annonces
    annonce *archive = NULL;                // Créer un pointeur vers la liste archive
    annonce *fusion = NULL;                 // Créer un pointeur vers la liste fusion
    char choix;                             // Permet de sélectionner si l'utlisateur est nouveau ou déja inscrit "pour la premiere partie"
    annonceur *p;                           // Pointeur vers annonceur pour sauvegarder l'adresse courante de l'utilisateur
    char choix2;                            // Pour faire le choix entre les fonctions de l'utilisateur en partie deux
    char choix2a;
    char choix3;
    char choix4; // permet de sélectionner si l'utlisateur est nouveau ou déja inscrit "pour la troisieme partie"

    /*

    ----------------------------------------------- PREMIERE PARTIE ---------------------------------------------------------------

                                     NB : ELLE CONCERNE LES COMPTES D'UTILISATEURS
    */

    // Saluer l'utilisateur
    printf("\n\n-------------------------------------- Bienvenue dans l'application ANEN immobilier-------------------------------------\n\n");

    // Informer l'utilisateur de patienter quelques instants
    printf("                           POUR ACCEDER A L'APPLICATION ANEN,VOUS DEVEZ DISPOSER D'UN COMPTE                        \n\n");
    printf("\n");
    // Chargement des données des fichiers textes annonceur.txt et annonce.txt
    charge_tous_annonceur(&tete);
    charge_tous_annonce(tete);

    /*PREMIERE PARTIE : Les comptes utilisateurs (interface)

                              ===  Afficher les options pour accéder à l'application === */

jump:
    printf("1 : Inscription (nouvel utilisateur)\n2 : Connexion (compte existant)\n3 : Afficher les annonceurs\n4 : Afficher les annonces (une par une)\n5 : Quitter\n\n");
    printf("Que voulez-vous faire ? : ");
    choix = getch();
    printf("\n");
    switch (choix)
    {
    case '1':
        //cas num 1 pour creer un nouvel utilateur non enregistrer et l'ecrire dans le fichier annonceur.txt
        p = nouvel_annonceur(&tete);
        break;
    case '2':
        //cas num 2 pour enregistrer un  utilateur deja incrit dans l'aaplication

        p = compte_existant(&tete);
        break;
    case '3':
        //cas num 3 pour affihcer tous les incformation sur les annonceurs enregistrés dans le fichier annonceur.txt
        afficher_annonceur(tete);
        //apres l'affichage il retourne a jump pour créer son compte "compte est obligatoire"
        goto jump;
        break;
    case '4':
        //fonction qui affiche toutes les annonces une par une selon le vouloir de l'utilisateur
        affiche_annonce_une_par_une(tete);
        goto jump;
        break;
    case '5':
        goto fin;
        break;
    default:
        goto jump;
        break;
    }
    /*
    ---------------------------------------------- DEUXIEME PARTIE --------------------------------------------------------------------

                                      NB :  Les fonctinnalités de l'application

    */

    printf("\n \n----------------  Bienvenue ! vous êtes maintenant membre de notre famille ANEN  ----------------------\n\n");

jump2:
    // Afficher les options disponibles à l'utilisateur

    printf("\n1 : Creer une nouvelle annonce\n2 : Supprimer une annonce \n3 : Afficher les annonces\n4 : Afficher les offres d'un annonceur \n5 : Rechercher une annonce\n6 : Lister les annonceurs qui sont offreur et demandeurs en meme temps\n7 : Eclater les annonces en offres et demandes \n8 : Quitter et consulter l'historique \n");
    printf("\n Que voulez-vous faire ? : ");

    //demander a l'utilsateur de saisir son choix

    choix2 = getch();
    printf("\n");

    /*
    -------------------------    un menu simple des options dans l'application ANEN ----------------------------------------
    */
    switch (choix2)
    {
    case '1':
        // Permet à l'utilisateur de créer une nouvelle annonce
        nouvelle_annonce(p);
        break;
    case '2':
        // Permet à l'utilisateur de supprimer une annonce
        supprimer_annonce(&tete, p, &archive);
        break;
    case '3':
        // Afficher toutes les annonces d'un certain utilisateur
        affiche_annonce(p->adr_annonce);
        break;
    case '4':
        // Afficher toutes les offres appartenant au même annonceur
        offre_meme_annonceur(tete);
        break;

    case '5':
    jump2a:
        printf("\n\n1 : Rechercher selon la date de publication \n");
        printf("2 : Rechercher selon le type du bien \n");
        printf("3 : Rechercher selon la localisation \n");
        printf("4 : QUITTER\n");
        printf("Que voulez-vous faire ? ");
        choix2a = getch();
        switch (choix2a)
        {
        case '1':
            // Rechercher une annonce selon la date
            recherche_date(tete);
            break;
        case '2':
            // Rechercher une annonce selon le type immobilier
            recherche_type_imm(tete);
            break;
        case '3':
            // Reechercher une annonce selon la localisation
            recherche_localisation(tete);
            break;
        case '4':
            goto jump2;
            break;
        }

        goto jump2a;
        break;
    case '6':
        // Lister les utilisateurs qui sont offreurs et demandeurs en même temps
        offreur_demandeur(tete);
        break;
    case '7':
        // Eclater la liste active en deux listes : offres et demandes
        eclat_offre_demande(tete, &offre, &demande);
    jump3:
        printf("\n\n1 : Afficher la liste des offres \n");
        printf("2 : Afficher la liste des demandes \n");
        printf("3 : Afficher la liste des offres et des demandes \n");
        printf("4 : Rechercher les offres en fonction du type du bien immobilier \n");
        printf("5 : Rechercher les offres en fonction de la categorie du bien \n");
        printf("6 : Rechercher les offres en fonction du prix du bien  \n");
        printf("7 : Rechercher les offres en fonction de la surface du bien\n");
        printf("8 : Trier puis afficher la liste des offre  \n");
        printf("9 : Trier puis afficher la liste des demandes \n");
        printf("a : Fusionner les liste des offres et des demandes triees \n");
        printf("b : Quitter\n\n");
        choix3 = getch();
        switch (choix3)
        {
        case '1':
            affiche_annonce(offre);
            break;
        case '2':
            affiche_annonce(demande);
            break;
        case '3':
            affiche_annonce(offre);
            affiche_annonce(demande);
            break;
        case '4':
            recherche_bien_offre(offre);
            break;
        case '5':
            recherche_categorie_offre(offre);
            break;
        case '6':
            recherche_prix_offre(offre);
            break;
        case '7':
            recherche_surface_offre(offre);
            break;
        case '8':
            tri_LLC(offre);
            affiche_annonce(offre);
            break;
        case '9':
            tri_LLC(demande);
            affiche_annonce(demande);
            break;
        case 'a':
            tri_LLC(offre);
            tri_LLC(demande);

            // Fusion des listes des offres et des demandes et mettre le résultat dans la liste fusion
            fusion_LLC(offre, demande, &fusion);
            affiche_annonce(fusion);
            break;
        case 'b':
            goto jump2;
            break;
        default:
            break;
        }
        goto jump3;
        break;

    case '8':
        // Quitter le programme
        goto jump4;
        break;
    default:
        break;
    }
    goto jump2;
jump4:
    /*

    ----------------------------------------------- TROISIEME PARTIE ---------------------------------------------------------------

                                            NB : L'HISTORIQUE DES RECHERCHES
    */
historique:
    printf("\n\n -------------------------------------CONSULTER L'HISTORIQUE DES RECHERCHE-------------------------------------\n\n");
    printf("1 : Consulter les annonces en fonction de leur duree de vie \n");
    printf("2 : Consulter les annonces les plus demandees\n3 : Quitter le programme\nvotre choix est : ");
    printf("3 : Quitter le programme ");
    choix4 = getch();
    printf("\n\n");
    switch (choix4)
    {
    case '1':
        histor_vie_annonce();
        break;
    case '2':
        plus_demande();
        break;
    case '3':
        goto fin;
        break;
    default:
        break;
    }
    goto historique;

fin:
    return 0;
};
