#include "Kisi.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 *
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p> 
 * Kisi yapisinin metotlarinin implementasyonu.
 * </p>
 */

// sayac, her yeni Kisi olusturdugumuzde ID atamak icin 
static int sayac = 1;

		// ** kisi metotlari **

// yaslan metodu= Kisi'nin yasini bir arttirir
static void yaslan_Kisi(Kisi* this) {
	if (this != NULL) {
		this->age++;
	}
}

// toString metodu = Kisi'nin bilgilerini string formatinda dondurur
static char* toString_Kisi(Kisi* this) {
    if (this == NULL) return NULL;
    
    // 128 byte bellek acıyorum ki ID, isim, soyisim ve yas bilgilerini tutabilsin
    char* txtstr = (char*)malloc(128 * sizeof(char)); 
    if (txtstr != NULL) {
        // 182-Sinan Ulusinan - 21 
        sprintf(txtstr, "%d-%s %s - %d", this->id, this->name, this->surname, this->age);
    }
    return txtstr; 
}

		// ** kurucu ve yikici fonksiyonlar ** 

// kurucu fonksiyon 
Kisi* createKisi(char* name, char* surname, int age) {
    if (name == NULL || surname == NULL) return NULL;

    Kisi* this = (Kisi*)malloc(sizeof(Kisi));
    if (this == NULL) return NULL;

    // degiskenler 
    this->id = sayac++;
    this->name = strdup(name);
    this->surname = strdup(surname);
    this->age = age;

	// eger isim veya soyisim icin bellek ayrilamazsa, olusan bellekleri temizleyip NULL donduruyoruz
	if (this->name == NULL || this->surname == NULL) {
        free(this->name);
        free(this->surname);
        free(this);
        return NULL;
    }

    // metotlari atama
    this->yaslan = yaslan_Kisi;
    this->yoket = destroyKisi;
    this->toString = toString_Kisi;

    return this;
}

// yıkıcı fonksiyon
void destroyKisi(Kisi* this) {
    if (this == NULL) return;

    if (this->name != NULL) free(this->name);
    if (this->surname != NULL) free(this->surname);
    
    free(this);
}