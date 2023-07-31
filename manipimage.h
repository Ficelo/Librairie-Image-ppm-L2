//Teiva Trillard 40001469
//Noah Morel 40006402

//Le type tPixel
typedef struct pixel {
    int r;
    int v;
    int b;
} tPixel;


// Le type representant une image en memoire
typedef struct image {
    int hauteur;        // Hauteur en pixels
    int largeur;        // Largeur en pixels
    char type[3];       // Type de l'image ppm P1 P2 P3 P4 ou P6
    int maxval;         // Valeur maximale de l'intensite d'une couleur
    tPixel** img;       // Le tableau des pixels
} tImage;

tImage initImage(int haut, int larg, char typ[3], int vmax);
tImage copieImage(tImage im);
tImage chargePpm(char* fichier);
void sauvePpm(char* nom, tImage im);
tImage niveauGris(tImage im);
tPixel floumoy(tImage im, int i, int j, int r);
tImage flou(tImage im, int n);
tImage contour(tImage im);