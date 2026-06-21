#include "Sehir.h"
#include <stdlib.h>
#include <stdio.h>

#define BASLANGIC_KAPASITE 16 // bellek yönetimi icin baslangic kapasitesi

/*
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p>
 * Sehir yapisinin metotlarinin implementasyonu.
 * </p>
 */

        // ** sehir metotları **

static void ilceEkle_Sehir(Sehir* this, Ilce* ilce) {
    if (this == NULL || ilce == NULL) return;

    // kapasite dolduysa 2 katina cikariyoruz
    if (this->ilceSayisi == this->kapasite) {
        int yeniKapasite = this->kapasite * 2;
        
        // realloc patlarsa eski veriyi kaybetmemek icin temp kullaniyoruz
        Ilce** temp = (Ilce**)realloc(this->ilceler, yeniKapasite * sizeof(Ilce*));
        if (temp == NULL) {
            return; 
        }
        
        this->ilceler = temp;
        this->kapasite = yeniKapasite;
    }
    
    this->ilceler[this->ilceSayisi] = ilce;
    this->ilceSayisi++;
}

// nufusu 2'ye bolunce 2 yeni sehir olustururken ilceyi azaltmak icin kullanilacak metot
static Ilce* ilceCikar_Sehir(Sehir* this) {
    if (this == NULL || this->ilceSayisi == 0) return NULL;
    
    // dizideki son ilceyi bul, sayaci bir azalt ve o ilceyi geri dondur
    this->ilceSayisi--;
    return this->ilceler[this->ilceSayisi];
}

// yaslan metodu, sehirdeki her bir ilcenin yaslanmasi
static void yaslan_Sehir(Sehir* this) {
    if (this == NULL) return;
    
    for (int i = 0; i < this->ilceSayisi; i++) {
        // Her bir ilcenin kendi yaslan metodunu tetikliyoruz
        this->ilceler[i]->yaslan(this->ilceler[i]);
    }
}

// getNufus metodu, sehirdeki tum ilcelerin nufus toplamini dondurur
static int getNufus_Sehir(Sehir* this) {
    if (this == NULL) return 0;
    
    int toplamNufus = 0;
    for (int i = 0; i < this->ilceSayisi; i++) {
        toplamNufus += this->ilceler[i]->getNufus(this->ilceler[i]);
    }
    return toplamNufus;
}


        // ** kurucu ve yikici fonksiyonlar **

// kurucu fonksiyon 
Sehir* createSehir(char* ad) {
    if (ad == NULL) return NULL;

    Sehir* this = (Sehir*)malloc(sizeof(Sehir));
    if (this == NULL) return NULL;

    // kalitim 
    this->super = createYerlesim(ad);
    if (this->super == NULL) { 
        free(this); 
        return NULL; 
    }

    this->kapasite = BASLANGIC_KAPASITE;
    this->ilceSayisi = 0;
    
    // ilceler icin bellek ayirdigimiz yer
    this->ilceler = (Ilce**)malloc(this->kapasite * sizeof(Ilce*));
    if (this->ilceler == NULL) {
        this->super->copeAt(this->super); // superi super ile yok ediyoruz 
        free(this);                      // sonra sehiri siliyoruz
        return NULL;
    }

    // metotlari bagla
    this->ilceEkle = ilceEkle_Sehir;
    this->ilceCikar = ilceCikar_Sehir;
    this->yaslan = yaslan_Sehir;
    this->getNufus = getNufus_Sehir;
    this->yoket = destroySehir; 

    return this;
}

// yikici fonksiyon
void destroySehir(Sehir* this) {
    if (this == NULL) return;

    // once tum ilceleri yok ediyoruz 
    for (int i = 0; i < this->ilceSayisi; i++) {
        this->ilceler[i]->yoket(this->ilceler[i]);
    }
    
    // sonra ilce dizisini siliyoruz
    if (this->ilceler != NULL) {
        free(this->ilceler);
    }

    // super ile cagrilanlari super ile yok ediyoruz
    if (this->super != NULL) {
        this->super->copeAt(this->super);
    }

    // sehir structunu siliyoruz
    free(this);
}