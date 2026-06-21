#ifndef YERLESIM_H
#define YERLESIM_H

/*	
 *
 * @author Sinan Ulusinan
 * @since  5 Mayıs 2026
 * <p> 
 * Yerlesim yapisi, sehir, mahalle ve ilcenin ortak ozelliklerini ve metotlarini tanimlayan yapi.
 * Her bir yerlesim turu (sehir, mahalle, ilce) bu yapiyi miras alarak kendi özelliklerini ve metotlarini ekleyecek.
 * </p>
 */

struct Yerlesim;

typedef struct Yerlesim
{
	// sehir mahalle ve ilcenin ortak özelligi = isim verisine sahipler
	char* name;

	// ortak metotlari = nufus alma / yaslandirma / yoketme(cop toplayici yok diye)
	int (*getNufus)(struct Yerlesim*);
	void (*yaslan)(struct Yerlesim*);
	void (*copeAt)(struct Yerlesim*);
} Yerlesim;



Yerlesim* createYerlesim(char* name);
void destroyYerlesim(Yerlesim* this); 



#endif