#ifndef ILCE_H
#define ILCE_H

#include "Yerlesim.h"
#include "Mahalle.h"

/* 
 *
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p> 
 * Ilce yapisi, Yerlesim yapisindan kalitim alan ve ilceye ozel ozellikleri (mahalleleri) tutan yapi.
 * </p>
 */

typedef struct ILCE Ilce;

struct ILCE {
    Yerlesim* super;  // kalitim

    // degiskenler
    Mahalle** mahalleler;    
    int mahalleSayisi;    
    int kapasite;      

    // metotlar
    void (*mahalleEkle)(Ilce*, Mahalle*);
    Mahalle* (*mahalleCikar)(Ilce*);
    void (*yaslan)(Ilce*);
    int (*getNufus)(Ilce*);
    void (*yoket)(Ilce*);
};

// kurucu ve yikici fonksiyonlar
Ilce* createIlce(char* ad);
void destroyIlce(Ilce* this);

#endif