#ifndef SEHIR_H
#define SEHIR_H

#include "Yerlesim.h"
#include "Ilce.h"

/* *
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p> 
 * Sehir yapisi, Yerlesim yapisindan kalitim alan ve sehire ozel ozellikleri (ilceleri) tutan yapi.
 * </p>
 */

typedef struct SEHIR Sehir;

struct SEHIR {
    Yerlesim* super;  // kalitim

    // degiskenler
    Ilce** ilceler;    
    int ilceSayisi;    
    int kapasite;      

    // metotlar
    void (*ilceEkle)(Sehir*, Ilce*);
    Ilce* (*ilceCikar)(Sehir*);
    void (*yaslan)(Sehir*);
    int (*getNufus)(Sehir*);
    void (*yoket)(Sehir*);
};

// kurucu ve yikici fonksiyonlar
Sehir* createSehir(char* ad);
void destroySehir(Sehir* this);

#endif