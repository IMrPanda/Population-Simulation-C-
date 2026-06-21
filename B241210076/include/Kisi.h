#ifndef KISI_H
#define KISI_H

/*	
 *
 * @author Sinan Ulusinan
 * @since  6 Mayıs 2026
 * <p> 
 * Kisi yapisi, kisiye ait ozellikleri ve metotlari tanimlayan yapi.
 * Kisi yapisi, yaslanma, yoketme ve toString gibi metotlara sahip olacak.
 * </p>
 */

typedef struct KISI Kisi;

struct KISI {
    int id;
    char* name;
    char* surname;
    int age;

	// Kisiye ait metotlar
    void (*yaslan)(Kisi*);
    void (*yoket)(Kisi*);
    char* (*toString)(Kisi* this); 
};

// Kurucu ve yikici fonksiyonlar
Kisi* createKisi(char* name, char* surname, int age);
void destroyKisi(Kisi* this);

#endif