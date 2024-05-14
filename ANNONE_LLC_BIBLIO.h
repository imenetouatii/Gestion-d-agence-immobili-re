#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 256
#include <conio.h>
#include <time.h>
#define true 1
#define false 0

//________________________________________________STRUCTURES UTILISEES_____________________________________________________________

// Structure ANNONCE
typedef struct annonce
{
    char nom[20];
    char type[8];
    char category[10];
    char identifiant[20];
    int date[5];
    char prix[10];
    char localisation[20];
    char surface[10];
    char type_im[10];
    char texte[100];

    struct annonce *suiv;

} annonce;

// Structure ANNONCEUR
typedef struct annonceur
{
    char email[20];
    char identifiant[20];
    char MP[15];
    char tel[10];
    struct annonceur *adr;
    struct annonce *adr_annonce;
} annonceur;

//*******************************************************************************************************************************

//____________________________________ DECLARATION DES FONCTIONS ET PROCEDURES___________________________________________________

int *currant_time(int *tab);
annonceur *nouvel_annonceur(annonceur **tete);
annonce *nouvelle_annonce(annonceur *tete);
const char *ext_mot(char *mot, int num, char *delim);
void recharge_annonceur(annonceur **tete, char *buffer);
void charge_tous_annonceur(annonceur **tete);
void recharge_annonce(annonce **tete, char *buffer);
void charge_tous_annonce(annonceur *tete);
void init_annonce(annonceur *tete);
void copier_info(annonce *dis, annonce *src);
int nombre_annonceur(annonceur *tete);
int nombre_annonce(annonce *tete);
void permute(annonce *p, annonce *q);
void afficher_annonceur(annonceur *tete);
void affiche_annonce_une_par_une(annonceur *tete);
void affiche_annonce(annonce *tete);
void supprimer_annonce(annonceur **tete, annonceur *node, annonce **archive);
int supp_line_from_file(char *fname, int lno);
void recherche_type_imm(annonceur *tete);
void recherche_localisation(annonceur *tete);
void recherche_date(annonceur *tete);
void offreur_demandeur(annonceur *tete);
void offre_meme_annonceur(annonceur *tete);
void recherche_categorie_offre(annonce *tete);
void recherche_bien_offre(annonce *tete);
void recherche_surface_offre(annonce *tete);
void recherche_prix_offre(annonce *tete);
void fusion_LLC(annonce *fir, annonce *sec, annonce **result);
void eclat_offre_demande(annonceur *tete, annonce **offre, annonce **demande);
void tri_LLC(annonce *tete);
void histor_vie_annonce(void);
annonce *charge_tous_annonce_historique(annonce **tete, char *fname);
void plus_demande();
annonceur *compte_existant(annonceur **tete);
void interface();

//_______________________________________________PROCEDURES DE TEMPS_____________________________________________________________

//Fonction qui affiche la date actuelle locale => elle renvoie un tableau  qui contient 0-> jour   1->mois   2->année
int *Date(int *tab)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    tab[0] = (int)tm.tm_mday;
    tab[1] = (int)tm.tm_mon + 1;
    tab[2] = (int)tm.tm_year + 1900;
}

// Fonction qui affiche le temps actuel => elle renvoie un tableau qui contient 0-> hour | 1 -> min | 2-> second
int *currant_time(int *tab)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    tab[0] = (int)tm.tm_hour;
    tab[1] = (int)tm.tm_min;
    tab[2] = (int)tm.tm_sec;
    return tab;
}

//_______________________________________________PROCEDURES DE CREATION__________________________________________________________

// Procédure pour créer un nouvel annonceur => elle renvoie l'adresse du l'annonceur crée
annonceur *nouvel_annonceur(annonceur **tete)
{
    annonceur *p, *q;
    p = (annonceur *)malloc(sizeof(annonceur));
    FILE *file = fopen("annonceur.txt", "a+");
    printf("Entrez les informations suivantes : \n");
    printf("Nom d'utilisateur : ");
    scanf("%s", p->identifiant);
    fprintf(file, "\n%s   ", p->identifiant);
    printf("Mot de passe : ");
    scanf("%s", p->MP);
    fprintf(file, "%s   ", p->MP);
    printf("Adresse e-mail : ");
    scanf("%s", p->email);
    fprintf(file, "%s   ", p->email);
    printf("Numero de telephone : ");
    scanf("%s", p->tel);
    fprintf(file, "%s", p->tel);
    p->adr = NULL;
    p->adr_annonce = NULL;
    q = *tete;
    if (q == NULL)
    {
        *tete = p;
        return *tete;
    }
    else
    {
        while (q->adr != NULL)
        {
            q = q->adr;
        }
        q->adr = p;
        return p;
    }
}

// Procédure qui permet à l'utilisateur de l'adresse tete de créer une annonce => elle renvoie l'adresse actuelle de l'annonce
annonce *nouvelle_annonce(annonceur *tete)
{
    annonce *p, *q; // Création de deux pointeurs vers "annonceur"

    // Initialisation de deux variables qui vont déterminer le temps
    int time[2], date[3];
    Date(date);
    currant_time(time);

    // Allocation dynamique du maillon annonce
    p = (annonce *)malloc(sizeof(annonce));
    p->suiv = NULL;

    // Ouverture du fichier texte annonce.txt
    FILE *file = fopen("annonce.txt", "a+");

    // Remplir les information pour les écrire dans le fichier texte d'une facon déterminée (Les mots doivent être séparés par "-" ou "_")
    printf("\n\nIMPORTANT : DANS LES CHAMPS CONTENANT PLUS D'UN MOT, LES MOTS DOIVENT ETRE SEPARES PAR '-' ou '_' NON PAR UN BLANC (ESPACE)\n\n");
    fprintf(file, "\n%s ", tete->email);
    printf("Type de l'annonce (offre/demande) : ");
    ;
    scanf("%s", p->type);
    fprintf(file, "%s  ", p->type);
    printf("Categorie (location/vente) : ");
    scanf("%s", p->category);
    fprintf(file, "%s  ", p->category);
    printf("Nom de l'annonce : ");
    scanf("%s", p->nom);
    fprintf(file, "%s  ", p->nom);
    printf("Prix du bien : ");
    scanf("%s", p->prix);
    fprintf(file, "%s  ", p->prix);
    printf("Localisation du bien : ");
    scanf("%s", p->localisation);
    fprintf(file, "%s  ", p->localisation);
    printf("Type du bien : ");
    scanf("%s", p->type_im);
    fprintf(file, "%s  ", p->type_im);
    printf("Surface du bien : ");
    scanf("%s", p->surface);
    fprintf(file, "%s  ", p->surface);
    fprintf(file, "%02d %02d %02d %02d %d ", time[0], time[1], date[0], date[1], date[2]);
    printf("Ajoutez une petite description : ");
    scanf("%s", p->texte);
    fprintf(file, "%s", p->texte);
    if (tete->adr_annonce == NULL)
    {
        tete->adr_annonce = p;
        return tete->adr_annonce;
    }
    else
    {
        q = tete->adr_annonce;
        while (q->suiv != NULL)
        {
            q = q->suiv;
        }
        q->suiv = p;
        return p;
    }
}
//*******************************************************************************************************************************

//___________________________________________________PROCEDURES DIVERS___________________________________________________________

// Fonction qui permet d'extraire un mot dans une phrase en fonnction de sa position à un tel deliminateur => retourne le mot trouvé
const char *ext_mot(char *mot, int num, char *delim)
{
    char *token = NULL;
    int i = 1;
    static char phrase[100];
    strcpy(phrase, mot);
    token = strtok(phrase, delim);

    while (token != NULL && i < num)
    {
        token = strtok(NULL, delim);
        i++;
    }
    return token;
}

//*******************************************************************************************************************************

//______________________________________________PROCEDURES (RE)CHARGEMENT________________________________________________________

/*Procédure qui recharge les champs du maillon tete avec des informations de la phrase buffer avec une suite pariculière
 La suite en question : idenifiant motDePasse email telephone*/
void recharge_annonceur(annonceur **tete, char *buffer)
{
    annonceur *p, *q;
    p = (annonceur *)malloc(sizeof(annonceur));
    strcpy(p->identifiant, ext_mot(buffer, 1, " "));
    strcpy(p->MP, ext_mot(buffer, 2, " "));
    strcpy(p->email, ext_mot(buffer, 3, " "));
    strcpy(p->tel, ext_mot(buffer, 4, " "));
    p->adr = NULL;
    p->adr_annonce = NULL;
    q = *tete;
    if (q == NULL)
    {
        *tete = p;
    }
    else
    {
        while (q->adr != NULL)
        {
            q = q->adr;
        }
        q->adr = p;
    }
}

/*Procédure qui recharge les maillons annonceurs à partir du fichier texte annonceur.txt
 L'ordre : idenifiant motDePasse email telephone*/
void charge_tous_annonceur(annonceur **tete)
{
    FILE *file = fopen("annonceur.txt", "r");
    char buffer[100] = "";
    while (!feof(file))
    {
        fgets(buffer, 100, file);
        recharge_annonceur(tete, buffer);
    }
}

//
void recharge_annonce(annonce **tete, char *buffer)
{
    annonce *p, *q;
    p = (annonce *)malloc(sizeof(annonce));
    strcpy(p->type, ext_mot(buffer, 2, " "));
    strcpy(p->category, ext_mot(buffer, 3, " "));
    strcpy(p->nom, ext_mot(buffer, 4, " "));
    strcpy(p->prix, ext_mot(buffer, 5, " "));
    strcpy(p->localisation, ext_mot(buffer, 6, " "));
    strcpy(p->type_im, ext_mot(buffer, 7, " "));
    strcpy(p->surface, ext_mot(buffer, 8, " "));
    p->date[0] = atoi(ext_mot(buffer, 9, " "));
    p->date[1] = atoi(ext_mot(buffer, 10, " "));
    p->date[2] = atoi(ext_mot(buffer, 11, " "));
    p->date[3] = atoi(ext_mot(buffer, 12, " "));
    p->date[4] = atoi(ext_mot(buffer, 13, " "));
    p->suiv = NULL;
    q = *tete;
    if (q == NULL)
    {
        *tete = p;
    }
    else
    {
        while (q->suiv != NULL)
        {
            q = q->suiv;
        }
        q->suiv = p;
    }
}

// Procédure qui recharge les maillons annonces à partir du fichier texte annonce.txt
void charge_tous_annonce(annonceur *tete)
{
    FILE *file = fopen("annonce.txt", "r");
    annonceur *p = tete;
    char buffer[150] = "";
    while (p != NULL)
    {
        while (!feof(file))
        {
            fgets(buffer, 150, file);
            if (strcmp(ext_mot(buffer, 1, " "), p->email) == 0)
            {
                recharge_annonce(&p->adr_annonce, buffer);
            }
        }
        fseek(file, 0, SEEK_SET);
        p = p->adr;
    }
}

//*******************************************************************************************************************************

// Initialisation de toutes les annonces à NIL
void init_annonce(annonceur *tete)
{
    annonceur *p = tete;
    while (p != NULL)
    {
        p->adr_annonce = NULL;
    }
}

// Procédure qui copie les incformations du mallion source (src) dans le mallion destinataire (dis)
void copier_info(annonce *dis, annonce *src)
{
    strcpy(dis->category, src->category);
    strcpy(dis->type, src->type);
    strcpy(dis->identifiant, src->identifiant);
    strcpy(dis->localisation, src->localisation);
    strcpy(dis->nom, src->nom);
    strcpy(dis->prix, src->prix);
    strcpy(dis->surface, src->surface);
    strcpy(dis->texte, src->texte);
    strcpy(dis->type_im, src->type_im);
    dis->date[0] = src->date[0];
    dis->date[1] = src->date[1];
    dis->date[2] = src->date[2];
    dis->date[3] = src->date[3];
    dis->date[4] = src->date[4];
}

// Fonction qui compte le nombre d'utilisateurs dans la liste "tete"
int nombre_annonceur(annonceur *tete)
{
    annonceur *p = tete;
    int cpt = 0;
    while (p != NULL)
    {
        p = p->adr;
        cpt++;
    }
    return cpt;
}

// Fonction qui calcule le nombre d'annonce d'un utilisateur
int nombre_annonce(annonce *tete)
{
    annonce *p = tete;
    int cpt = 0;
    while (p != NULL)
    {
        cpt++;
        p = p->suiv;
    }
    return cpt;
}

// Procédure qui permet de permuter les informations des maillons sans les déplacer
void permute(annonce *p, annonce *q)
{
    annonce *tmp = (annonce *)malloc(sizeof(annonce));
    copier_info(tmp, p);
    copier_info(p, q);
    copier_info(q, tmp);
    free(tmp);
}

//*******************************************************************************************************************************

//_______________________________________________PROCEDURES D'AFFICHAGE_______________________________________________________

// Procédure pour afficher tous les annonceurs de la liste active
void afficher_annonceur(annonceur *tete)
{
    annonceur *q = tete;
    int i = 1;
    while (q != NULL)
    {
        printf("------------------------------------------- ANNONCEUR %d -----------------------------------------\n\n", i);
        printf("Identifiant : %s\n", q->identifiant);
        printf("Adresse E-mail : %s\n", q->email);
        printf("Mot de Passe : %s\n", q->MP);
        printf("Numero de telephone : %s\n\n", q->tel);
        q = q->adr;
        i++;
    }
}

// Fonction qui affiche toutes les annonces une par une selon le vouloir de l'utilisateur
void affiche_annonce_une_par_une(annonceur *tete)
{
    annonce *p = tete->adr_annonce;
    annonceur *q = tete;
    int choix;
    int i = 1;
    while (choix != 1)
    {
        p = q->adr_annonce;
        while (p != NULL)
        {
            printf(" \n\n\n---------------------------------------------------- ANNONCE %d -----------------------------------------------\n", i);
            i++;
            printf("   [   %s    ]     \n\n", p->nom);
            printf("L'adresse e-mail de l'annonceur : %s\n", q->email);
            printf("Type de l'annonce (offre/demande) : %s\n", p->type);
            printf("Categorie (location/vente) : %s\n", p->category);
            printf("Type du bien :%s \n", p->type_im);
            printf("Prix du bien : %s DA \n", p->prix);
            printf("Surface du bien : %s m2 \n", p->surface);
            printf("Localisation du bien : %s\n", p->localisation);
            printf("Date de publication de l'annonce : %d /", p->date[2]);
            printf(" %d /", p->date[3]);
            printf("%d \n", p->date[4]);
            printf("________________________________ \n");
            printf(" Que voulez-vous faire ?");
        jump:
            printf("\n0 : Suivant \n1 : Quitter\n");
            scanf("%d", &choix);
            printf("\n\n");
            if (choix == 0)
            {
                p = p->suiv;
            }
            else if (choix == 1)
            {
                return;
            }
            else
            {
                goto jump;
            }
        }
        q = q->adr;

        if (q->adr == NULL)
        {
            q = tete;
            i = 1;
        }
    }
}

// Fonction qui affiche toutes les annonces liée à un tel utilisateur
void affiche_annonce(annonce *tete)
{
    annonce *p = tete;
    int i = 1;
    while (p != NULL)
    {
        printf("\n\n--------------------------------------------   ANNONCE %d   ---------------------------------------------- \n", i);

        printf(" | %s  |\n", p->nom);
        printf("Type de l'annonce (offre/demande) : %s\n", p->type);
        printf("Categorie (location/vente) : %s\n", p->category);
        printf("Prix du bien : %s DA\n", p->prix);
        printf("Localisation du bien : %s\n", p->localisation);
        printf("Type du bien :%s\n", p->type_im);
        printf("Surface du bien : %s m2\n", p->surface);
        printf("Date de publication : %d /", p->date[2]);
        printf("%d /", p->date[3]);
        printf(" %d \n", p->date[4]);
        p = p->suiv;
        i++;
    }
}

//*******************************************************************************************************************************

//_______________________________________________PROCEDURES DE SUPPRESSION_______________________________________________________

// Procédure de supprission d'une annonce liée à un utlisateur tete en fonction de sa position, et la déplacer dans une liste archive
void supprimer_annonce(annonceur **tete, annonceur *node, annonce **archive)
{
    affiche_annonce(node->adr_annonce);
    int choix;
    int day[3], time[3];
    Date(day);
    currant_time(time);
    annonce *p = NULL, *q = node->adr_annonce;
    annonce *n = NULL, *s = NULL;
    printf("\n\n Entrez le numero de l'annonce que vous voulez supprimer\n Numero de l'annonce : ");
    scanf("%d", &choix);
    if (node != NULL)
    {
        annonce *m = (annonce *)malloc(sizeof(annonce));
        m->suiv = NULL;
        if (choix == 1)
        {
            p = node->adr_annonce;
            copier_info(m, p);
            node->adr_annonce = node->adr_annonce->suiv;
            free(p);
        }
        else
        {
            for (int i = 2; i < choix; i++)
            {
                q = q->suiv;
            }
            p = q->suiv;
            copier_info(m, p);
            q->suiv = q->suiv->suiv;
            free(p);
        }
        if (*archive == NULL)
        {
            *archive = m;
        }
        else
        {
            s = *archive;
            while (s->suiv != NULL)
            {
                s = s->suiv;
            }
            s->suiv = m;
        }
        annonceur *k = *tete;
        if (node->adr_annonce == NULL)
        {

            if (node == *tete)
            {
                *tete = node->adr;
                free(*tete);
            }
            else
            {
                while (k->adr != node)
                {
                    k = k->adr;
                }
                k->adr = node->adr;
                free(node);
            }
        }
        FILE *f = fopen("supp.txt", "a+");
        if (f != NULL)
        {
            fprintf(f, "\n%s  ", node->email);
            fprintf(f, "%s  ", m->type);
            fprintf(f, "%s  ", m->category);
            fprintf(f, "%s  ", m->nom);
            fprintf(f, "%s  ", m->prix);
            fprintf(f, "%s  ", m->localisation);
            fprintf(f, "%s  ", m->type_im);
            fprintf(f, "%s  ", m->surface);
            fprintf(f, "%02d %02d %02d %02d %d ", m->date[0], m->date[1], m->date[2], m->date[3], m->date[4]);
            fprintf(f, "%02d %02d %02d %02d %d ", time[0], time[1], day[0], day[1], day[2]);
        }
        else
        {
            printf("\nErreur lors de l'ouverture du fichier supp.txt");
        }
        fclose(f);
    }

    else
    {
        printf("\n L'annonceur ne dispose d'aucune annonce\n");
    }
}

// Supprission d'une ligne d'un fichier
int supp_line_from_file(char *fname, int lno)
{
    int ctr = 0;
    char ch;
    FILE *fptr1, *fptr2;
    char str[MAX], temp[] = "temp.txt";
    fptr1 = fopen(fname, "r");
    if (!fptr1)
    {
        printf(" File not found or unable to open the input file!!\n");
        return 0;
    }
    fptr2 = fopen(temp, "w"); // Ouvre le fichier temporaire en mode écriture
    if (!fptr2)
    {
        printf("Unable to open a temporary file to write!!\n");
        fclose(fptr1);
        return 0;
    }
    // Copier tout le contenu dans le fichier tomporaire sauf la ligne ciblée
    while (!feof(fptr1))
    {
        strcpy(str, "\0");
        fgets(str, MAX, fptr1);
        if (!feof(fptr1))
        {
            ctr++;
            /* Sauter de ligne au numéro de ligne donné */
            if (ctr != lno)
            {
                fprintf(fptr2, "%s", str);
            }
        }
    }
    fclose(fptr1);
    fclose(fptr2);
    remove(fname);       // Supprimer le fichier original
    rename(temp, fname); // Renomme le fichier temporaire au nom de l'original
}

//*******************************************************************************************************************************

//_______________________________________PROCEDURES DE RECHERCHE PUIS AFFICHAGE____________________________________________________

// Procédure qui recherche et affiche toutes les annonces qui correspondent à un type de bien immobilier demandé
void recherche_type_imm(annonceur *tete)
{
    annonceur *p = tete;
    annonce *q;
    char type[30];
    printf("\n Quel type de bien desirez-vous rechercher ? \n");
    scanf("%s", type);
    printf("\n");
    while (p != NULL)
    {
        q = p->adr_annonce;
        while (q != NULL)
        {
            if (strstr(strlwr(q->type_im), type) != NULL)
            {
                printf("_________________________________________ \n");
                printf("      [   %s   ]      ", q->nom);
                printf("\n Type de l'annonce (offre/demande) : %s\n", q->type);
                printf("Categorie (location/vente) : %s\n", q->category);
                printf("Type du bien :%s \n", q->type_im);
                printf("Prix du bien : %s DA \n", q->prix);
                printf("Surface du bien : %s m2\n", q->surface);
                printf("Localisation du bien : %s \n", q->localisation);
                printf("Description : %s\n", q->texte);
                printf("Date de publication de l'annonce : %02d /", q->date[2]);
                printf(" %02d /", q->date[3]);
                printf(" %02d \n", q->date[4]);
                printf("\n");
            }
            q = q->suiv;
        }
        p = p->adr;
    }
}

// Procédure qui permet de rechercher une annonce selon la localisation du bien
void recherche_localisation(annonceur *tete)
{
    annonceur *p = tete;
    annonce *q;
    char loc[30];
    printf("\nQuelle localisation voulez-vous rechercher ?\n\n");
    scanf("%s", loc);
    while (p != NULL)
    {
        q = p->adr_annonce;
        while (q != NULL)
        {
            if (strstr(strlwr(q->localisation), loc) != NULL)
            {
                printf("_______________________________________\n");
                printf("  [ %s  ]\n", q->nom);
                printf("\nType de l'annonce (offre/demande) : %s\n", q->type);
                printf("Categorie (location/vente) : %s\n", q->category);
                printf("Type du bien :%s \n", q->type_im);
                printf("Prix du bien : %s DA\n", q->prix);
                printf("Surface du bien : %s m2\n", q->surface);
                printf("Localisation du bien : %s \n", q->localisation);
                printf("Date de publication : %d /", q->date[2]);
                printf(" %d /", q->date[3]);
                printf(" %d \n", q->date[4]);
            }
            q = q->suiv;
        }
        p = p->adr;
    }
}

// Procédure qui permet de rechercher une annonce selon sa date de publication
void recherche_date(annonceur *tete)
{
    annonceur *p = tete;
    annonce *q;
    char choix;
    int jour, mois, an;
jump:
    printf("Rechercher par : \n1 : Jour precis (j/m/an)\n2 : Mois (m/an) \n3 : Annee ?\n4 : Quitter ");
    printf("Entrez votre choix : ");
    choix = getch();
    switch (choix)
    {
    case '1':
        printf("\nEntrez le jour : ");
        scanf("%d", &jour);
        printf("\nEntrez le mois : ");
        scanf("%d", &mois);
        printf("\nEntrez l'année : ");
        scanf("%d", &an);
        while (p != NULL)
        {
            q = p->adr_annonce;
            while (q != NULL)
            {
                if (q->date[4] == an && q->date[3] == mois && q->date[2] == jour)
                {
                    printf("________________________________________ \n");
                    printf("\n\n     [  %s  ]    \n", q->nom);
                    printf("Type de l'annonce (offre/demande) : %s\n", q->type);
                    printf("Categorie (location/vente) : %s\n", q->category);
                    printf("Type du bien :%s \n", q->type_im);
                    printf("Prix du bien : %s DA\n", q->prix);
                    printf("Surface du bien : %s m2\n", q->surface);
                    printf("Localisation du bien : %s \n", q->localisation);
                    printf("Description : %s \n", q->texte);
                    printf("Date de publication de l'annonce : %d /", q->date[2]);
                    printf(" %d /", q->date[3]);
                    printf(" %d \n", q->date[4]);
                }
                q = q->suiv;
            }
            p = p->adr;
        }
        break;
    case '2':
        printf("\n Entrez le mois : ");
        scanf("%d", &mois);
        printf("\nEntrez l'année : ");
        scanf("%d", &an);
        while (p != NULL)
        {
            q = p->adr_annonce;
            while (q != NULL)
            {
                if (q->date[4] == an && q->date[3] == mois)
                {
                    printf("\n\n    %s       ", q->nom);
                    printf("\n Type de l'annonce (offre/demande) : %s\n", q->type);
                    printf("Categorie (location/vente) : %s\n", q->category);
                    printf("Type du bien :%s \n", q->type_im);
                    printf("Prix du bien : %s\n", q->prix);
                    printf("Surface du bien : %s\n", q->surface);
                    printf("Localisation du bien : %s \n", q->localisation);
                    printf("Description : %s \n", q->texte);
                    printf("Date de publication de l'annonce : %d", q->date[2]);
                    printf(" %d ", q->date[3]);
                    printf(" %d \n", q->date[4]);
                }
                q = q->suiv;
            }
            p = p->adr;
        }
        break;
    case '3':

        printf("\nEntrez l'annee : ");
        scanf("%d", &an);
        while (p != NULL)
        {
            q = p->adr_annonce;
            while (q != NULL)
            {
                if (q->date[4] == an)
                {
                    printf("\n\n    %s      ", q->nom);
                    printf("\n Type de l'annonce (offre/demande) : %s\n", q->type);
                    printf("Categorie (location/vente) : %s\n", q->category);
                    printf("Type du bien :%s \n", q->type_im);
                    printf("Prix du bien : %s\n", q->prix);
                    printf("Surface du bien : %s\n", q->surface);
                    printf("Localisation du bien : %s \n", q->localisation);
                    printf("Description : %s \n", q->texte);
                    printf("Date de publication de l'annonce : %d", q->date[2]);
                    printf(" %d ", q->date[3]);
                    printf(" %d \n", q->date[4]);
                }
                q = q->suiv;
            }
            p = p->adr;
        }
        break;
    case '4':
        return;
        break;
    default:
        goto jump;
        break;
    }
}

// Procédure qui recherche et affiche les informations de tous les offreurs et demandeurs en même temps
void offreur_demandeur(annonceur *tete)
{
    char buffer[100];
    annonceur *p = tete;
    annonce *q;
    int i = 1;
    while (p != NULL)
    {
        if (p->adr_annonce == NULL)
        {
            break;
        }
        q = p->adr_annonce;
        strcpy(buffer, q->type);
        while (q != NULL)
        {
            if (strcmp(buffer, q->type) != 0)
            {
                printf("\n\nLes annonceurs qui sont offreurs et demandeurs en meme temps :");
                printf("\n    ___________________________________\n");
                printf("---------------------------------------------   ANNONCEUR %d    -----------------------------------------", i);
                i++;
                printf("\n\n Son adresse e-mail : %s \n", p->email);
                printf("Nom d'utilisateur : %s \n", p->identifiant);
                printf("Numero de telephone : %s \n\n", p->tel);
                goto jump;
            }
            q = q->suiv;
        }

    jump:
        if (p != NULL)
            p = p->adr;
    }
}

// Procedure qui recherche et affiche toutes les offres appartenant au même annonceur
void offre_meme_annonceur(annonceur *tete)
{
    annonceur *q = tete;
    int i = 1;
    char email[30] = "";
    printf("\n\n Entrez l'adresse e-mail de l'utilisateur dont vous voulez afficher les offres : ");
    scanf("%s", email);
    while (strcmp(q->email, email) != 0 && q->adr != NULL)
    {
        q = q->adr;
    }
    if (q->adr != NULL)
    {
        annonce *p = q->adr_annonce;
        while (p != NULL)
        {
            if (strcmp(p->type, "offre") == 0)
            {
                printf("\n\n\n----------------------------------  ANNONCE %d  ------------------------------------------\n\n", i);
                printf(" | %s  |\n", p->nom);
                printf("Categorie (location/vente) : %s\n", p->category);
                printf("Prix du bien : %s DA\n", p->prix);
                printf("Localisation du bien : %s\n", p->localisation);
                printf("Type du bien :%s\n", p->type_im);
                printf("Surface du bien : %s m2\n", p->surface);
                printf("Date de publication : %d /", p->date[2]);
                printf(" %d /", p->date[3]);
                printf(" %d \n", p->date[4]);
                i++;
            }

            p = p->suiv;
        }
    }
    else
    {
        printf("\n\nL'utilisateur n'existe pas \n");
    }
}

// Procédure qui recherche dans la liste des offre en fonction de la catégorie vente/location
void recherche_categorie_offre(annonce *tete)
{
    annonce *p = tete;
    char cat[30];
    printf("\n Souhaitez-vous rechercher des biens à vendre ou à louer ? ?\n Choisir vente ou location : ");
    scanf("%s", cat);
    while (p != NULL)
    {

        if (strstr(strlwr(p->category), strlwr(cat)) != NULL)
        {
            printf("\n__________________________________________\n");
            printf("\n\n | %s  |\n", p->nom);
            printf("Type de l'annonce (offre/demande) : %s\n", p->type);
            printf("Categorie (location/vente) : %s\n", p->category);
            printf("Prix du bien : %s DA\n", p->prix);
            printf("Localisation du bien : %s\n", p->localisation);
            printf("Type du bien :%s\n", p->type_im);
            printf("Surface du bien : %s m2\n", p->surface);
            printf("Description : %s\n", p->texte);
            printf("Date de publication : %d /", p->date[2]);
            printf(" %d /", p->date[3]);
            printf(" %d \n", p->date[4]);
        }
        p = p->suiv;
    }
}

// Procédure qui permet de rechercherdans la liste des offres en fonction du type de bien immobilier
void recherche_bien_offre(annonce *tete)
{
    annonce *p = tete;
    char cat[30];
    printf("\n Quel type de bien immobilier recherchez-vous ? ?\n Entrez votre recherche : ");
    scanf("%s", cat);
    while (p != NULL)
    {
        if (strstr(strlwr(p->type_im), strlwr(cat)) != NULL)
        {
            printf("_________________________________\n");
            printf("\n\n | %s  |\n", p->nom);
            printf("Type de l'annonce (offre/demande) : %s\n", p->type);
            printf("Categorie (location/vente) : %s\n", p->category);
            printf("Prix du bien : %s\ DAn", p->prix);
            printf("Localisation du bien : %s\n", p->localisation);
            printf("Type du bien :%s\n", p->type_im);
            printf("Surface du bien : %s m2 \n", p->surface);
            printf("Description : %s\n", p->texte);
            printf("Date de publication : %d /", p->date[2]);
            printf(" %d /", p->date[3]);
            printf(" %d \n", p->date[4]);
        }
        p = p->suiv;
    }
}

// Procédure qui recherche et affiche les offres en fonction d'une surface donnée (avec une marge de 10%)
void recherche_surface_offre(annonce *tete)
{
    annonce *p = tete;
    int surface;
    printf("\n Quelle surface recherchez vous ?\nEntrez votre choix : ");
    scanf("%d", &surface);
    while (p != NULL)
    {
        if (atoi(p->surface) >= (surface - surface / 10) && atoi(p->surface) <= (surface + surface / 10))
        {
            printf("________________________________\n");
            printf(" \n\n| %s  |\n", p->nom);
            printf("Type de l'annonce (offre/demande) : %s\n", p->type);
            printf("Categorie (location/vente) : %s\n", p->category);
            printf("Prix du bien : %s DA\n", p->prix);
            printf("Localisation du bien : %s\n", p->localisation);
            printf("Type du bien :%s\n", p->type_im);
            printf("Surface du bien : %s m2\n", p->surface);
            printf("Description : %s\n", p->texte);
            printf("Date de publication : %d /", p->date[2]);
            printf(" %d /", p->date[3]);
            printf(" %d \n", p->date[4]);
        }
        p = p->suiv;
    }
}

// Procédure qui recherche et affiche les offres selon un prix demandé avec une marge de 10%
void recherche_prix_offre(annonce *tete)
{
    annonce *p = tete;
    int prix;
    printf("\n Quel est votre budjet ?\n  Entrez votre recherche : ");
    scanf("%d", &prix);
    while (p != NULL)
    {
        if (atoi(p->prix) >= (prix - prix / 10) && atoi(p->prix) <= (prix + prix / 10))
        {
            printf("________________________________\n");
            printf(" \n\n| %s  |\n", p->nom);
            printf("Type de l'annonce (offre/demande) : %s\n", p->type);
            printf("Categorie (location/vente) : %s\n", p->category);
            printf("Prix du bien : %s DA\n", p->prix);
            printf("Localisation du bien : %s\n", p->localisation);
            printf("Type du bien :%s\n", p->type_im);
            printf("Surface du bien : %s m2\n", p->surface);
            printf("Description : %s\n", p->texte);
            printf("Date de publication : %d /", p->date[2]);
            printf(" %d /", p->date[3]);
            printf(" %d \n", p->date[4]);
        }
        p = p->suiv;
    }
}

//*******************************************************************************************************************************

//___________________________________________PROCEDURES ECLATEMENT/TRI/FUSION____________________________________________________

// Procédure qui fusionne deux liste fir et sec et donne le résultat dans la liste result
void fusion_LLC(annonce *fir, annonce *sec, annonce **result)
{
    annonce *p = fir, *q = sec;
    annonce *n = NULL;
    annonce *c = NULL;
    if (p == NULL && q == NULL)
        return;
    while (p != NULL && q != NULL)
    {
        long a = p->date[4] * 10000 + p->date[3] * 100 + p->date[2];
        long b = q->date[4] * 10000 + q->date[3] * 100 + q->date[2];
        n = (annonce *)malloc(sizeof(annonce));
        n->suiv = NULL;
        if (a > b)
        {
            copier_info(n, p);
            if (*result == NULL)
            {
                *result = n;
                c = *result;
            }
            else
            {
                c->suiv = n;
                c = c->suiv;
            }
            p = p->suiv;
        }
        else
        {
            copier_info(n, q);
            if (*result == NULL)
            {
                *result = n;
                c = *result;
            }
            else
            {
                c->suiv = n;
                c = c->suiv;
            }
            q = q->suiv;
        }
    }
    while (p != NULL)
    {
        n = (annonce *)malloc(sizeof(annonce));
        n->suiv = NULL;
        copier_info(n, p);
        c->suiv = n;
        c = c->suiv;
        p = p->suiv;
    }
    while (q != NULL)
    {
        n = (annonce *)malloc(sizeof(annonce));
        n->suiv = NULL;
        copier_info(n, q);
        c->suiv = n;
        c = c->suiv;
        q = q->suiv;
    }
}

// Procédure d'éclatement de la liste des annonces en deux listes offre et demande
void eclat_offre_demande(annonceur *tete, annonce **offre, annonce **demande)
{
    annonceur *p = tete;
    annonce *q;
    annonce *o = NULL;
    annonce *j = *offre, *k = *demande;
    while (p != NULL)
    {
        q = p->adr_annonce;
        while (q != NULL)
        {
            o = (annonce *)malloc(sizeof(annonce));
            o->suiv = NULL;
            copier_info(o, q);
            if (strcmp(q->type, "offre") == 0)
            {

                if (*offre == NULL)
                {
                    *offre = o;
                    j = *offre;
                }
                else
                {
                    j->suiv = o;
                    j = j->suiv;
                }
            }
            else
            {
                if (*demande == NULL)
                {
                    *demande = o;
                    k = *demande;
                }
                else
                {
                    k->suiv = o;
                    k = k->suiv;
                }
            }

            q = q->suiv;
        }
        p = p->adr;
    }
}

// Fonction qui permet de trier des annonces en fonction  de leur date de création
void tri_LLC(annonce *tete)
{
    annonce *p = tete;
    int isfinish;
    while (p->suiv != NULL)
    {
        isfinish = false;

        long a = p->date[4] * 10000 + p->date[3] * 100 + p->date[2];
        long b = p->suiv->date[4] * 10000 + p->suiv->date[3] * 100 + p->suiv->date[2];
        if (a < b)
        {
            permute(p, p->suiv);
            p = tete;
            isfinish = true;
        }

        if (isfinish == false)
        {
            p = p->suiv;
        }
    }
}

//*******************************************************************************************************************************

//_______________________________________________PROCEDURES HISTORIQUE___________________________________________________________

// Procédure qui affiche tout les annonces supprimées ayant une durée de vie donnée par l'utilisateur en heures
void histor_vie_annonce(void)
{
    FILE *file = fopen("./supp.txt", "r");
    char buffer[300];
    char choix;
    int time;
    int i = 1;
    if (file != NULL)
    {
    jump:
        printf("1 : Rechercher par heure\n");
        printf("2 : Rechercher par jour\n");
        printf("3 : Rechercher par mois\n");
        printf("4 : Rechercher par annee \n");
        printf("5 : Quitter\n Que voulez-vous faire ? : ");
        choix = getch();
        printf("\n");
        printf("Entrez une duree à rechercher : ");
        scanf("%d", &time);
        switch (choix)
        {
        case '1':
            break;
        case '2':
            time *= 100;
            break;
        case '3':
            time *= 10000;
            break;
        case '4':
            time *= 1000000;
            break;
        case '5':
            return;
            break;
        default:
            goto jump;
            break;
        }
        while (!feof(file))
        {
            fgets(buffer, 300, file);
            long a = atoi(ext_mot(buffer, 13, " ")) * 1000000 + atoi(ext_mot(buffer, 12, " ")) * 10000 + atoi(ext_mot(buffer, 11, " ")) * 100 + atoi(ext_mot(buffer, 9, " "));
            long b = atoi(ext_mot(buffer, 18, " ")) * 1000000 + atoi(ext_mot(buffer, 17, " ")) * 10000 + atoi(ext_mot(buffer, 16, " ")) * 100 + atoi(ext_mot(buffer, 15, " "));
            if (b - a <= time)
            {
                printf("\n---------------------------------------------  ANNONCE %d    ----------------------------------------\n", i);
                printf("\n\nType de l'annonce (offre/demande) : %s\n", ext_mot(buffer, 2, " "));
                printf("Categorie (location/vente) : %s\n", ext_mot(buffer, 3, " "));
                printf("Nom : %s\n", ext_mot(buffer, 4, " "));
                printf("Prix du bien : %s DA\n", ext_mot(buffer, 5, " "));
                printf("Localisation du bien : %s\n ", ext_mot(buffer, 6, " "));
                printf("Type du bien :%s \n", ext_mot(buffer, 7, " "));
                printf("Surface du bien : %s m2 \n", ext_mot(buffer, 8, " "));
                i++;
            }
        }
    }
    else
    {
        printf("\nERREUR LORS DE L'OUVERTURE DU FICHIER supp.txt");
    }
}

// Procédure qui permet de recharger toutes les annonces du fichier historique (supp.txt);
annonce *charge_tous_annonce_historique(annonce **tete, char *fname)
{
    FILE *file = fopen(fname, "r");
    annonce *p = *tete;
    char buffer[150] = "";
    if (file != NULL)
    {
        while (!feof(file))
        {
            fgets(buffer, MAX, file);
            recharge_annonce(&p, buffer);
        }
    }

    else
    {
        printf("\nErreur lors de l'ouverture du fichier texte %s", fname);
    }
    fclose(file);
    return p;
}

//Procédure qui affiche les annonces les plus demandées durant un période déterminée
void plus_demande()
{
    FILE *file = fopen("./supp.txt", "r");
    char buffer[300];
    char choix;
    int time;
    int i = 1;
    int c = 0, d = 0;

    if (file != NULL)
    {
    jump:
        printf("1 : une recherche par heure\n");
        printf("2 : une recherche par jour\n");
        printf("3 : une recherche par mois\n");
        printf("4 : une recherche par annee \n");
        printf("5 : Quitter\n votre choix est :");
        choix = getch();
        printf("\n");
        printf("Entrer la duree a rechercher : ");
        scanf("%d", &time);
        switch (choix)
        {
        case '1':
            break;
        case '2':
            time *= 100;
            break;
        case '3':
            time *= 10000;
            break;
        case '4':
            time *= 1000000;
            break;
        case '5':
            return;
            break;

        default:
            goto jump;
            break;
        }

        while (!feof(file))
        {
            fgets(buffer, 300, file);
            long a = atoi(ext_mot(buffer, 13, " ")) * 1000000 + atoi(ext_mot(buffer, 12, " ")) * 10000 + atoi(ext_mot(buffer, 11, " ")) * 100 + atoi(ext_mot(buffer, 9, " "));
            long b = atoi(ext_mot(buffer, 18, " ")) * 1000000 + atoi(ext_mot(buffer, 17, " ")) * 10000 + atoi(ext_mot(buffer, 16, " ")) * 100 + atoi(ext_mot(buffer, 15, " "));
            if (b - a <= time && strcmp(ext_mot(buffer, 7, " "), "terrain") == 0)
            {
                c++;
            }
            else
            {
                if (b - a <= time && strcmp(ext_mot(buffer, 7, " "), "appartement") == 0)
                {
                    d++;
                }
            }
        }
        if (c > d)
        {
            printf("\n\nLes annonces les plus demandees pendant cette periode est : 'TERRAIN'\n\n");
        }

        else if (c < d)
        {
            printf("\n\nLes annonces les plus demandees pendant cette periode est : 'appartement'\n\n");
        }

        else
        {
            printf("\n\nLes annonces les plus demandees pendant cette periode est : 'appartement' et 'terrain' en meme temps\n\n");
        }
    }
    else
    {
        printf("\nErreur lors de l'ouverture de fichier texte supp.txt\n");
    }
}

//*******************************************************************************************************************************

//_______________________________________________PROCEDURE GESTION DES COMPTES___________________________________________________

// Il y a aussi la procédure de création d'un nouvel annonceur citée plus haut

// Procédure pour enregistrer un annonceur déja inscrit dans l'application => elle renvoie l'adresse actuelle de l'annonceur
annonceur *compte_existant(annonceur **tete)
{
    char email[20], MP[20];
jump:
    printf("\n Entrez vos identifiannts : ");
    printf("\n Adresse e-mail : ");
    scanf("%s", email);
    printf("   Mot de passe : ");
    scanf("%s", MP);
    annonceur *p = *tete;
    while (p != NULL)
    {
        if (strcmp(p->email, email) == 0)
        {
            if (strcmp(p->MP, MP) == 0)
            {
                return p;
            }
            else
            {
                printf("\nLe mot de passe entre est incorrect ou inexistant\n");
                exit(0);
            }
        }
        p = p->adr;
    }
    if (p == NULL)
    {
        printf("L'utilisateur n'existe pas, veuillez vous inscrire afin de pouvoir acceder a nos services\n\n");
        goto jump;
    }
}

//_______________________________________________________INTERFACE____________________________________________________________________

void interface()
{
    printf("    c:                   --.--            oOo         ( )           --.--                oOo      \n");
    printf("        ________                                     (  )         oOo                                \n");
    printf("       /        \\     oOo         --.--             (  )                    _____________________          \n");
    printf("      /          \\                  _________________||__                  |  O       O       O  |          \n ");
    printf("    /____________\\                /\\                    \\                 |  O       O       O  |      ()       \n ");
    printf("    |   _    _  |                /  \\                    \\                |  O       O       O  |     (  )          \n ");
    printf("    |  | |  | | |       oo      / ___\\____________________\\               |  O       O       O  |    (    )        \n  ");
    printf("   |     _     |       /       |    |                    |               |  O       O       O  |    (    )    \n");
    printf("     |    | |    |      |        |    |         _          |               |         ___         |      ||           \n");
    printf(" ____|____|_|____| _____|________|____|________||__________|_______________|________|___|________|______||____________\n");

    printf("                  __     _  _   ____    _  _       _    _  _   _  _    ____    ___    _    _     _   ____   ____\n");
    printf("                 /__\\   | \\| | |  __)  | \\| |     | |  | \\/ | | \\/ |  | __ |  |  D)  | |  | |   | | |  __) |   D)   \n");
    printf("                //__\\\\  |  \\ | | |__   |  \\ |     | |  | -  | |    |  ||__||  |  )   | |  | |_  | | | |__  |   /  \n");
    printf("               //    \\\\ |_||_| |____|  |_||_|     |_|  |_/\\_| |_/\\_|  |____|  |___)  |_|  |___| |_| |____| |_|\\|\n");
}
