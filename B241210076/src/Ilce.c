#include "Ilce.h"
#include <stdlib.h>
#include <stdio.h>

#define BASLANGIC_KAPASITE 16 // bellek yönetimi icin baslangic kapasitesi

/*
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p>
 * Ilce yapisinin metotlarinin implementasyonu.
 * </p>
 */

        // ** ilce metotları **

static void mahalleEkle_Ilce(Ilce* this, Mahalle* mahalle) {
    if (this == NULL || mahalle == NULL) return;

    // kapasite dolduysa 2 katina cikariyoruz
    if (this->mahalleSayisi == this->kapasite) {
        int yeniKapasite = this->kapasite * 2;
        
        // realloc patlarsa eski veriyi kaybetmemek icin temp kullaniyoruz
        Mahalle** temp = (Mahalle**)realloc(this->mahalleler, yeniKapasite * sizeof(Mahalle*));
        if (temp == NULL) {
            return; 
        }
        
        this->mahalleler = temp;
        this->kapasite = yeniKapasite;
    }
    
    this->mahalleler[this->mahalleSayisi] = mahalle;
    this->mahalleSayisi++;
}

// nufusu 2'ye bolunce 2 yeni ilce olustururken mahalleyi azaltmak icin kullanilacak metot
static Mahalle* mahalleCikar_Ilce(Ilce* this) {
    if (this == NULL || this->mahalleSayisi == 0) return NULL;
    
    // dizideki son mahalleyi bul, sayaci bir azalt ve o mahalleyi geri dondur
    this->mahalleSayisi--;
    return this->mahalleler[this->mahalleSayisi];
}

// yaslan metodu, ilcedeki her bir mahallenin yaslanmasi
static void yaslan_Ilce(Ilce* this) {
    if (this == NULL) return;
    
    for (int i = 0; i < this->mahalleSayisi; i++) {
        // Her bir mahallenin kendi yaslan metodunu tetikliyoruz
        this->mahalleler[i]->yaslan(this->mahalleler[i]);
    }
}

// getNufus metodu, ilcedeki tum mahallelerin nufus toplamini dondurur 
static int getNufus_Ilce(Ilce* this) {
    if (this == NULL) return 0;
    
    int toplamNufus = 0;
    for (int i = 0; i < this->mahalleSayisi; i++) {
        toplamNufus += this->mahalleler[i]->getNufus(this->mahalleler[i]);
    }
    return toplamNufus;
}


        // ** kurucu ve yikici fonksiyonlar **

// kurucu fonksiyon 
Ilce* createIlce(char* ad) {
    if (ad == NULL) return NULL;

    Ilce* this = (Ilce*)malloc(sizeof(Ilce));
    if (this == NULL) return NULL;

    // kalitim 
    this->super = createYerlesim(ad);
    if (this->super == NULL) { 
        free(this); 
        return NULL; 
    }

    this->kapasite = BASLANGIC_KAPASITE;
    this->mahalleSayisi = 0;
    
    // mahalleler icin bellek ayirdigimiz yer
    this->mahalleler = (Mahalle**)malloc(this->kapasite * sizeof(Mahalle*));
    if (this->mahalleler == NULL) {
        this->super->copeAt(this->super); // superi super ile yok ediyoruz 
        free(this);                      // sonra ilceyi siliyoruz
        return NULL;
    }

    // metotlari bagla
    this->mahalleEkle = mahalleEkle_Ilce;
    this->mahalleCikar = mahalleCikar_Ilce;
    this->yaslan = yaslan_Ilce;
    this->getNufus = getNufus_Ilce;
    this->yoket = destroyIlce; 

    return this;
}

// yikici fonksiyon
void destroyIlce(Ilce* this) {
    if (this == NULL) return;

    // once tum mahalleleri yok ediyoruz 
    for (int i = 0; i < this->mahalleSayisi; i++) {
        this->mahalleler[i]->yoket(this->mahalleler[i]);
    }
    
    // sonra mahalle dizisini siliyoruz
    if (this->mahalleler != NULL) {
        free(this->mahalleler);
    }

    // super ile cagrilanlari super ile yok ediyoruz
    if (this->super != NULL) {
        this->super->copeAt(this->super);
    }

    // ilce structunu siliyoruz
    free(this);
}