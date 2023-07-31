#include <stdio.h>
#include <stdlib.h>
#include "manipimage.c"

int main(){
    tImage c = chargePpm("image1.ppm");
    tImage b = initImage(768, 1024, "P3", 255);
    tImage a = copieImage(c);
    tImage d = niveauGris(c);
    tImage f = flou(c, 10);
    tImage g = contour(c);
    
    sauvePpm("image.ppm", c);
    sauvePpm("image-Copie.ppm",a);
    sauvePpm("image-gris.ppm", d);
    sauvePpm("image-flou.ppm", f);
    sauvePpm("image-contour.ppm", g);
    return 0;
}