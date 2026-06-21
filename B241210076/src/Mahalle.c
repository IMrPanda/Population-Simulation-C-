#include "Mahalle.h"
#include <stdlib.h>
#include <stdio.h>

#define BASLANGIC_KAPASITE 16 // bellek yönetimi icin baslangic kapasitesini 2^n yaparak belirledim ki RAM kullanimi verimi artsin

/*
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p>
 * Mahalle yapisinin metotlarinin implementasyonu.
 * </p>
 */

		// ** mahalle metotları **

static void kisiEkle_Mahalle(Mahalle* this, Kisi* kisi) {
    if (this == NULL || kisi == NULL) return;

    // kapasite dolduysa 2 katina cikariyoruz
    if (this->kisiSayisi == this->kapasite) {
        int yeniKapasite = this->kapasite * 2;
        

        // realloc patlarsa eski veriyi kaybetmemek pointer ile yeni bir bellek alani acmaya calisiyoruz 
		// yoksa realloc programi patlatabiliyor bazen 
        Kisi** temp = (Kisi**)realloc(this->kisiler, yeniKapasite * sizeof(Kisi*));
        if (temp == NULL) {
            return; // eger acamazsak geri donüp eski veriyi koruyoruz.
        }
        
        this->kisiler = temp;
        this->kapasite = yeniKapasite;
    }
    
    this->kisiler[this->kisiSayisi] = kisi;
    this->kisiSayisi++;
}

// nufusu 2'ye bolunce 2 yeni mahalle olustururken nufusu azaltmak icin kullanilacak metot
static Kisi* kisiCikar_Mahalle(Mahalle* this) {
    if (this == NULL || this->kisiSayisi == 0) return NULL;
    
    // dizideki son kisiyi bul, sayaci bir azalt ve o kisiyi geri dondur
    this->kisiSayisi--;
    return this->kisiler[this->kisiSayisi];
}

// yaslan metodu, mahalledeki her bir kisinin yaslanmasi
static void yaslan_Mahalle(Mahalle* this) {
    if (this == NULL) return;
    
    for (int i = 0; i < this->kisiSayisi; i++) {
        // Her bir kişinin kendi yaslan metodunu tetikliyoruz
        this->kisiler[i]->yaslan(this->kisiler[i]);
    }
}

// getNufus metodu, mahalledeki kisi sayisini dondurur
static int getNufus_Mahalle(Mahalle* this) {
    if (this == NULL) return 0;
    return this->kisiSayisi;
}


		// ** kurucu ve yikici fonksiyonlar **

// kurucu fonksiyon 
Mahalle* createMahalle(char* ad) {
    if (ad == NULL) return NULL;

    Mahalle* this = (Mahalle*)malloc(sizeof(Mahalle));
    if (this == NULL) return NULL;

	// kalitim 
    this->super = createYerlesim(ad);
    if (this->super == NULL) { 
        free(this); 
        return NULL; 
    }

    this->kapasite = BASLANGIC_KAPASITE;
    this->kisiSayisi = 0;
    
    // kisiler icin bellek ayirdigimiz yer
    this->kisiler = (Kisi**)malloc(this->kapasite * sizeof(Kisi*));
    if (this->kisiler == NULL) {
        this->super->copeAt(this->super); //  superi super ile yok ediyoruz 
        free(this);                      // sonra mahalleyi siliyoruz
        return NULL;
    }

    // metotlari bagla
    this->kisiEkle = kisiEkle_Mahalle;
    this->kisiCikar = kisiCikar_Mahalle;
    this->yaslan = yaslan_Mahalle;
    this->getNufus = getNufus_Mahalle;
    this->yoket = destroyMahalle; 

    return this;
}

// yikici fonksiyon
void destroyMahalle(Mahalle* this) {
    if (this == NULL) return;

    // once tum kisileri yok ediyoruz 
    for (int i = 0; i < this->kisiSayisi; i++) {
        this->kisiler[i]->yoket(this->kisiler[i]);
    }
    
    // sonra kisi dizisini siliyoruz
    if (this->kisiler != NULL) {
        free(this->kisiler);
    }

    // super ile cagrilanlari super ile yok ediyoruz
    if (this->super != NULL) {
        this->super->copeAt(this->super);
    }

    // mahalle structunu siliyoruz
    free(this);
}