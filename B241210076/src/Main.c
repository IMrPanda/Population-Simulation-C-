#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Oyun.h"

/*
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p>
 * Programin giris noktasini barindiran baslik dosyasinin implementasyonu. Oyun motorunu baslatir.
 * </p>
 */

int main() {
    int turSayisi;

    printf("Oyunun calisacagi tur sayisini giriniz: ");
    if (scanf("%d", &turSayisi) != 1) {
        printf("Gecersiz bir tur sayisi girdiniz!\n");
        return 1;
    }

    // scanf'ten sonra enter tusunun bufferda kalmamasi icin temizlik 
    while (getchar() != '\n'); 

    // oyunu/simulasyonu baslat
    Oyun* oyun = createOyun(turSayisi);
    if (oyun != NULL) {
        oyun->Baslat(oyun);
        oyun->yoket(oyun); // oyun bitince tum belleki bosalt
    }

    return 0;
}