//Teiva Trillard 40001469
//Noah Morel 40006402


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "manipimage.h"

tImage initImage(int haut, int larg, char typ[3], int vmax){
    
    tImage image;
    image.hauteur = haut;
    image.largeur = larg;
    image.maxval = vmax;
    for (int i=0; i<3; i++)
    {
        image.type[i] = typ[i];
    }
    
    //Attribution de l'espace pour le tableau de pixels
    tPixel **img = (tPixel **)malloc(image.hauteur * sizeof(tPixel *));
    for(int i = 0; i < image.hauteur; i++){
        img[i] = (tPixel *)malloc(image.largeur * sizeof(tPixel));
    }

    image.img = img;

    //Remplissage du tableau par des pixels sans importance pour les tests
    for(int i=0; i<image.hauteur; i++){
        for(int j=0; j<image.largeur; j++){
            image.img[i][j].r = i;
            image.img[i][j].v = i;
            image.img[i][j].b = i;
        }
    }

    printf("INIT: %d %d %d %s\n", image.largeur, image.hauteur, image.maxval, image.type);

    return image;
}

tImage copieImage(tImage im){
    
    //Créer une nouvelle image
    //L'initialiser avec les paramètres de l'autre
    tImage imageCop = initImage(im.hauteur, im.largeur, im.type, im.maxval);
    //On copie l' "image"
    imageCop.img = im.img;

    return imageCop;
}

tImage chargePpm(char* fichier){

    printf("START Charge\n");

    //Ouvrir le fichier 
    tImage imageChargee;

    FILE* fich = NULL;

    fich = fopen(fichier, "r");

    char type[3] = "";
    int curseur = 5;

    if (fich != NULL){
        
        //Prendre le type
        fgets(type, 3, fich);
        curseur = getc(fich);
        curseur = getc(fich);
        
        //Passer les lignes de commentaires
        while(curseur == '#'){
            while(curseur != '\n'){
                curseur = getc(fich);
            }
            curseur = getc(fich);
        }

        //On remet le curseur au début de la ligne
        curseur = ungetc(curseur, fich);

        int largeur;
        int hauteur;
        int max;
        int i, j;

        //Prendre la longueur et largeur
        fscanf(fich, "%d %d", &largeur, &hauteur);

        curseur = getc(fich);
        
        //Prendre vmax
        fscanf(fich, "%d", &max);

        //Créer un objet image
        imageChargee = initImage(hauteur, largeur, type, max);

        curseur = getc(fich);

        //Mettre toutes les valeures dans le tableau
        for(i=0; i<hauteur; i++){
            for(j=0; j<largeur; j++){
                int r, v, b;
                
                fscanf(fich, "%d", &r);
                fscanf(fich, "%d", &v);
                fscanf(fich, "%d", &b);
                
                imageChargee.img[i][j].r = r;
                imageChargee.img[i][j].v = v;
                imageChargee.img[i][j].b = b;
            }
        }

        printf("DONE CHARGE\n");

        fclose(fich);

        return imageChargee;
    }

    return imageChargee;
}

void sauvePpm(char* nom, tImage im){

    printf("SAUV\n");

    //Création du fichier
    FILE *fich;
    fich = fopen(nom, "w");

    //Mettre le type
    fprintf(fich, "%s\n", im.type);

    //Mettre des commentaires
    fprintf(fich, "# Copyright TEIVA TRILLARD\n");

    //Mettre la taille
    fprintf(fich,"%d %d\n", im.largeur,  im.hauteur);

    //Mettre la valeur MAX
    fprintf(fich, "%d\n", im.maxval);

    //Mettre toutes les valeurs RVB
    for(int i=0; i<im.hauteur; i++){
        for(int j=0; j<im.largeur; j++){
            int r, v, b;
            r = im.img[i][j].r;
            v = im.img[i][j].v;
            b = im.img[i][j].b;

            fprintf(fich, "%d\n", r);
            fprintf(fich, "%d\n", v);
            fprintf(fich, "%d\n", b);
        }      
    }

    printf("Cree %s\n", nom);

    fclose(fich);

}

tImage niveauGris(tImage im){

    //Créé une copie de l'image en entrée, en la met en noir et blanc

    tImage nImage = copieImage(im);
    // On prends chaque pixel et on multiplie ces valeurs r, v, et b par la formule de niveau de gris.
    for (int i = 0; i < nImage.hauteur; i++){
        for (int j = 0; j < nImage.largeur; j++){
            int r = im.img[i][j].r;
            int v = im.img[i][j].v;
            int b = im.img[i][j].b;
            
            im.img[i][j].r = (r*0.2125)+(v*0.7174)+(b*0.0721);
            im.img[i][j].v = (r*0.2125)+(v*0.7174)+(b*0.0721);
            im.img[i][j].b = (r*0.2125)+(v*0.7174)+(b*0.0721);
        }
    }

    return nImage;
}

tPixel floumoy(tImage im, int i, int j, int r){
    
    //On prends un pixel en entée, en sortie on retourne le pixel dont la couleur est la moyenne
    //des couleurs des pixels dans un r rayon.

    tPixel moy;

    int moyr = 0;
    int moyv = 0;
    int moyb = 0;

    int minI = i-r;
    int minJ = j-r;
    int maxI = i+r;
    int maxJ = j+r;

    int nbPixels = 0;
    
    //On vérifie que le rayon de dépasse pas les bordures de l'image.
    if (minI < 0){
        minI = 0;
    }
    if (minJ < 0){
        minJ = 0;
    }
    if (maxI > im.hauteur){
        maxI = im.hauteur;
    }
    if (maxJ > im.largeur){
        maxJ = im.largeur;
    }

    int x, y;
    for(x = minI; x < maxI; x++){
        for(y = minJ; y < maxJ; y++){
            moyr += im.img[x][y].r;
            moyv += im.img[x][y].v;
            moyb += im.img[x][y].b;
            nbPixels += 1;
        }
    }

    //On fait la moyenne des valeures r, v, b.
    moy.r = moyr / nbPixels; 
    moy.v = moyv / nbPixels; 
    moy.b = moyb / nbPixels;

    return moy;
}

tImage flou(tImage im, int n){
    printf("FLOUTAGE\n");
    
    //En entrée, on prends une image, puis on retourne cettre image floutée.
    tImage imageFlout = initImage(im.hauteur, im.largeur, im.type, im.maxval);

    //On floute chaque pixel de l'image avec un rayon n de floutage.
    for(int i=0; i < imageFlout.hauteur; i++){
        for(int j=0; j < imageFlout.largeur; j++){
            tPixel pixelFlou = floumoy(im, i, j, n);
            imageFlout.img[i][j] = pixelFlou;
        }
    }

    printf("DONE\n");
    
    return imageFlout;
}

tImage contour(tImage im){
    printf("CONTOURAGE\n");

    //On prends une image en entrée puis on retourne ces countours.
    tImage imageContour = initImage(im.hauteur, im.largeur, im.type, im.maxval);
    
    for(int i=0; i < imageContour.hauteur; i++){
        for(int j=0; j < imageContour.largeur; j++){
            tPixel pixelContour = floumoy(im, i, j, 2);
            imageContour.img[i][j].r = 255 - abs(im.img[i][j].r - pixelContour.r);
            imageContour.img[i][j].v = 255 - abs(im.img[i][j].v - pixelContour.v);
            imageContour.img[i][j].b = 255 - abs(im.img[i][j].b - pixelContour.b);
        }
    }

    return imageContour;
}