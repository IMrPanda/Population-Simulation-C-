#ifndef OYUN_H
#define OYUN_H

#include "Sehir.h"
#include <stdio.h>

/* *
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p> 
 * Oyun yapisi, tum simulasyonu yoneten, dosya okumalarini yapan ve tur mantigini isleyen Manager sinif.
 * </p>
 */

typedef struct OYUN Oyun;

struct OYUN {
    // degiskenler
    Sehir** sehirler;    
    int sehirSayisi;    
    int kapasite;
    int turSayisi;      

    // txt dosyalari icin pointerlar , okuma islemleri bu pointerlar uzerinden yapilacak
    FILE* fileSehirler;
    FILE* fileIlceler;
    FILE* fileMahalleler;
    FILE* fileKisiler;

    // metotlar
    void (*sehirEkle)(Oyun*, Sehir*);
    void (*Baslat)(Oyun*);
    void (*turAtla)(Oyun*);
    void (*yeniSehirKontrol)(Oyun*);
    void (*ekraniYazdir)(Oyun*);
    void (*yoket)(Oyun*);
};

// kurucu ve yikici fonksiyonlar
Oyun* createOyun(int turSayisi);
void destroyOyun(Oyun* this);

#endif