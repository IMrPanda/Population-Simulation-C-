#include "Yerlesim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	
 *
 * @author Sinan Ulusinan
 * @since  5 Mayıs 2026
 * <p> 
 * Yerlesim yapisi ve metotlarinin implementasyonu.
 * </p>
 */

 // kurucu fonksiyon
Yerlesim* createYerlesim(char* name) {
	if (name == NULL) return NULL;

    Yerlesim* this = (Yerlesim*)malloc(sizeof(Yerlesim)); // malloc ile bellekten yer ayiriyoruz
    if (this == NULL) return NULL;

    this->name = strdup(name); // sonra strdup ile kopyalayip hafizaya atiyoruz, bence hafiza yonetimi icin daha mantikli oluyor

	// hafizada yer yoksa temizleyip NULL dondur 
	if (this->name == NULL) {
        free(this);
        return NULL;
    }
    
    // getNufus ve yaslan miras alanlar tarafindan doldurulacak
    this->getNufus = NULL;
    this->yaslan = NULL;

	// copeAt tabanin kendi yikicisina bagli, miras alanlar override edecek
    this->copeAt = (void (*)(struct Yerlesim*))destroyYerlesim; 
    return this;
}

// yikici fonksiyon
void destroyYerlesim(Yerlesim* this) {
    if (this == NULL) return;

    // ismin kapladigi alani temizliyoruz
    if (this->name != NULL) {
        free(this->name);
		this->name = NULL; 
    }
    
    // sonra da yapinin kendisini
    free(this);
}