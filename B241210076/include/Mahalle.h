#ifndef MAHALLE_H
#define MAHALLE_H

#include "Yerlesim.h"
#include "Kisi.h"

/*	
 *
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p> 
 * Mahalle yapisi, Yerlesim yapisindan kalitim alan ve mahalleye ozel ozellikleri ve metotlari tanimlayan yapi.
 * </p>
 */

typedef struct MAHALLE Mahalle;

struct MAHALLE {
    Yerlesim* super;  // kalitim

	// degiskenler
    Kisi** kisiler;    
    int kisiSayisi;    
    int kapasite;      

	// metotlar
    void (*kisiEkle)(Mahalle*, Kisi*);
    Kisi* (*kisiCikar)(Mahalle*);
    void (*yaslan)(Mahalle*);
    int (*getNufus)(Mahalle*);
    void (*yoket)(Mahalle*);
};

// kurucu ve yikici fonksiyonlar
Mahalle* createMahalle(char* ad);
void destroyMahalle(Mahalle* this);

#endif