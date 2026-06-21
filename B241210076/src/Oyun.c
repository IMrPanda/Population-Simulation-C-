#include "Oyun.h"
#include "Sehir.h"     
#include "Ilce.h"
#include "Mahalle.h"
#include "Kisi.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BASLANGIC_KAPASITE 16 // bellek yönetimi icin baslangic kapasitesi

/*
 * @author Sinan Ulusinan
 * @since  7 Mayıs 2026
 * <p>
 * Oyun yapisinin metotlarinin implementasyonu.
 * </p>
 */

        // ** yardimci fonksiyonlar **

// dosyadan bir satir okuyup sonra \r\n temizle 
static void nRead_File(FILE* f, char* str, int size) {
    if (fgets(str, size, f) == NULL) {
        rewind(f); // Dosya biterse basa sar ve tekrar okuma kodu
        if (fgets(str, size, f) == NULL) { str[0] = '\0'; return; }
    }
    str[strcspn(str, "\r\n")] = '\0';
}

// kisiler.txt'den ad ve soyad oku 
static void kisiOku(FILE* f, char* ad, char* soyad) {
    char buffer[256];
    nRead_File(f, buffer, sizeof(buffer));
    if (sscanf(buffer, "%127s %127s", ad, soyad) < 2) {
        strcpy(ad, "Ad");
        strcpy(soyad, "Soyad");
    }
}

// konsolu temizle 
static void ekraniTemizle(void) {
    system("cls");
}

        // ** oyun metotları **

static void sehirEkle_Oyun(Oyun* this, Sehir* sehir) {
    if (this == NULL || sehir == NULL) return;

    // kapasite dolduysa 2 katina cikariyoruz
    if (this->sehirSayisi == this->kapasite) {
        int yeniKapasite = this->kapasite * 2;

        // realloc patlarsa eski veriyi kaybetmemek icin temp kullaniyoruz
        Sehir** temp = (Sehir**)realloc(this->sehirler, yeniKapasite * sizeof(Sehir*));
        if (temp == NULL) {
            return; 
        }
        
        this->sehirler = temp;
        this->kapasite = yeniKapasite;
    }
    
    this->sehirler[this->sehirSayisi] = sehir;
    this->sehirSayisi++;
}

// Her tur sonunda cagrilacak yeni sehir kontrolu
static void yeniSehirKontrol_Oyun(Oyun* this) {
    if (this == NULL) return;
    int mevcutSehirSayisi = this->sehirSayisi; 

	// mevcut sehirlerin nufusunu kontrol et, eger nufus 1000 veya uzeri ise yeni sehir olustur
    for (int i = 0; i < mevcutSehirSayisi; i++) {
        Sehir* mevcutSehir = this->sehirler[i];
        
        // Eger sehrin nufusu 1000 veya uzeriyse | 4 basamakliysa
        if (mevcutSehir->getNufus(mevcutSehir) >= 1000) {

            // yeni sehri olustur , txt dosyasindan isim cekiyoruz)
            char buffer[256];
            nRead_File(this->fileSehirler, buffer, sizeof(buffer));
            Sehir* yeniSehir = createSehir(buffer);
            if (yeniSehir == NULL) continue;

            int ilceSayisi = mevcutSehir->ilceSayisi;

            if (ilceSayisi > 1) {
                // mevcut sehrin ilcelerinin yarisini ilceCikar metoduyla al (2'ye bolunebilir hale getiriyoruz)
                int tasimaSayisi = ilceSayisi / 2;
                for (int j = 0; j < tasimaSayisi; j++) {
                    Ilce* mevcutIlce = mevcutSehir->ilceCikar(mevcutSehir);
                    // yeni sehre ilceEkle metoduyla ilceyi ekle
                    yeniSehir->ilceEkle(yeniSehir, mevcutIlce);
                }
            } else {
                // ilceSayisi == 1: yeni sehre yeni bir ilce olustur, mahalleleri tasi
                nRead_File(this->fileIlceler, buffer, sizeof(buffer));
                Ilce* yeniIlce = createIlce(buffer);
                if (yeniIlce == NULL) { yeniSehir->yoket(yeniSehir); continue; }

                Ilce* eskiIlce = mevcutSehir->ilceler[0];
                int mahalleSayisi = eskiIlce->mahalleSayisi;

                if (mahalleSayisi > 1) {
                    // Mahallelerin yarisini cikar ve yeni ilceye tasit
                    int tasincak = mahalleSayisi / 2;
                    for (int j = 0; j < tasincak; j++) {
                        Mahalle* mevcutMahalle = eskiIlce->mahalleCikar(eskiIlce);
                        yeniIlce->mahalleEkle(yeniIlce, mevcutMahalle);
                    }
                } else {
                    // mahalleSayisi == 1: yeni mahalle olustur, kisilerin yarisini tasi
                    nRead_File(this->fileMahalleler, buffer, sizeof(buffer));
                    Mahalle* yeniMahalle = createMahalle(buffer);
                    if (yeniMahalle == NULL) { destroyIlce(yeniIlce); yeniSehir->yoket(yeniSehir); continue; }

                    Mahalle* eskiMahalle = eskiIlce->mahalleler[0];
                    int tasincak = eskiMahalle->kisiSayisi / 2;
                    for (int j = 0; j < tasincak; j++) {
                        Kisi* mevcutKisi = eskiMahalle->kisiCikar(eskiMahalle);
                        yeniMahalle->kisiEkle(yeniMahalle, mevcutKisi);
                    }

                    yeniIlce->mahalleEkle(yeniIlce, yeniMahalle);
                }

                yeniSehir->ilceEkle(yeniSehir, yeniIlce);
            }

            // yeni sehri oyuna dahil et
            this->sehirEkle(this, yeniSehir);
        }
    }
}

//her turda nufus artisini ve yaslanmayi tetikleyen metot
static void turAtla_Oyun(Oyun* this) {
    if (this == NULL) return;

    for(int i = 0; i < this->sehirSayisi; i++) {
        Sehir* s = this->sehirler[i];
        
        // nufusun birler ve onlar basamagini topla ve artis katsayisini bul 
        int nufus = s->getNufus(s);
        int katsayi = (nufus / 10 % 10) + (nufus % 10);

        // her mahallenin mevcut nufusundan kac kisi dogacagini hesapla
        // her mahalleye bu yeni kisileri ekle
        for (int j = 0; j < s->ilceSayisi; j++) {
            Ilce* ilce = s->ilceler[j];
            for (int k = 0; k < ilce->mahalleSayisi; k++) {
                Mahalle* mahalle = ilce->mahalleler[k];
                int eskiNufus     = mahalle->getNufus(mahalle);
                int yeniDogacak; 

				// katsayi 0 ise nufus 1 artar kurali
                if (katsayi == 0) {
                    yeniDogacak = 1; 
                } else {
                    yeniDogacak = (eskiNufus * katsayi) - eskiNufus; 
                }

                for (int m = 0; m < yeniDogacak; m++) {
                    char ad[128], soyad[128];
                    kisiOku(this->fileKisiler, ad, soyad);
                    Kisi* yeniKisi = createKisi(ad, soyad, 0); // yeni dogan, yas=0 olarak baslar
                    if (yeniKisi != NULL) mahalle->kisiEkle(mahalle, yeniKisi);
                }
            }
        }

        // herkesi yaslandir
        s->yaslan(s);
    }
    
    // tur bitiminde yeni sehir kontrolu yap
    this->yeniSehirKontrol(this);
}

// Ekrana matris formatinda 5'li nufus yazdirma
static void ekraniYazdir_Oyun(Oyun* this) {
    int sayac = 0;
    for(int i = 0; i < this->sehirSayisi; i++) {
        printf("[%d]", this->sehirler[i]->getNufus(this->sehirler[i]));
        sayac++;
        if(sayac % 5 == 0) printf("\n");
        else if (i != this->sehirSayisi - 1) printf("-");
    }
    printf("\n");
}

// Baslangic kurulumlari ve ana dongu
static void Baslat_Oyun(Oyun* this) {
    srand((unsigned int)time(NULL));

    // kullanicidan baslangic sayilarini alma ve ilk sehirleri olusturma
    printf("Baslangic sayilarini giriniz (ornek: 18 25 79 37): ");
    char satir[1024];
    fgets(satir, sizeof(satir), stdin);
    satir[strcspn(satir, "\r\n")] = '\0';

    char* token = strtok(satir, " ");
    while (token != NULL) {
        int sayi       = atoi(token);
        int ilceSayisi = sayi / 10;
        int ilkMahalle = sayi % 10;

        if (ilceSayisi == 0) ilceSayisi = 1;

        // Mahalle sayisini ilce sayisina tam bolunur hale getir
        int n = (ilkMahalle == 0) ? 1 : ilkMahalle;
        while (n % ilceSayisi != 0) {
            n++;
            if (n > 9) n = 1;
        }
        int toplamMahalleCount = n;
        int mahallePerIlce     = toplamMahalleCount / ilceSayisi;
        int yeniSayi           = (ilceSayisi * 10) + toplamMahalleCount;
        int mahalleBasiKisi    = (int)ceil((double)yeniSayi / toplamMahalleCount);

        char buffer[256]; // txt dosyasindan sehir ismi oku , performans icin buffer kullaniyoruz
        nRead_File(this->fileSehirler, buffer, sizeof(buffer));
        Sehir* sehir = createSehir(buffer);

        for (int i = 0; i < ilceSayisi; i++) {
            nRead_File(this->fileIlceler, buffer, sizeof(buffer));
            Ilce* ilce = createIlce(buffer);

            for (int j = 0; j < mahallePerIlce; j++) {
                nRead_File(this->fileMahalleler, buffer, sizeof(buffer));
                Mahalle* mahalle = createMahalle(buffer);

                for (int k = 0; k < mahalleBasiKisi; k++) {
                    char ad[128], soyad[128];
                    kisiOku(this->fileKisiler, ad, soyad);
                    int yas = rand() % 51; // 0-50 arasi rastgele yas
                    Kisi* kisi = createKisi(ad, soyad, yas);
                    if (kisi != NULL) mahalle->kisiEkle(mahalle, kisi);
                }

                ilce->mahalleEkle(ilce, mahalle);
            }

            sehir->ilceEkle(sehir, ilce);
        }

        this->sehirEkle(this, sehir);
        token = strtok(NULL, " ");
    }

    // baslangic durumunu goster
    ekraniTemizle();
    printf("=== BASLANGIC DURUMU ===\n");
    this->ekraniYazdir(this);

    for (int i = 0; i < this->turSayisi; i++) {
        printf("\nDevam etmek icin Enter'a basiniz...\n");
        getchar();

        ekraniTemizle();
        this->turAtla(this);

        printf("=== TUR %d SONU ===\n", i + 1);
        this->ekraniYazdir(this);
    }

    // oyun sonu görüntüleme 
    printf("\n=== OYUN TAMAMLANDI - SON DURUM ===\n");
    this->ekraniYazdir(this); // nüfusları tekrar göster

    int satirSecim, sutunSecim;
    printf("\nGoruntulemek istediginiz sehrin koordinatlarini giriniz (Satir Sutun):\n");
    printf("Satir (0'dan baslar): "); 
    scanf("%d", &satirSecim);
    printf("Sutun (0'dan baslar): ");
    scanf("%d", &sutunSecim);

    // kullanicinin girdigi koordinata gore sehir secme
    int hedefIndex = (satirSecim * 5) + sutunSecim;

    if (hedefIndex >= 0 && hedefIndex < this->sehirSayisi) {
        Sehir* s = this->sehirler[hedefIndex];
        printf("\n-------------------------------------------\n");
        printf("Sehir: %s - Nufus: %d\n", s->super->name, s->getNufus(s));
        
        for (int i = 0; i < s->ilceSayisi; i++) {
            Ilce* ilce = s->ilceler[i];
            printf("  Ilce: %s - Nufus: %d\n", ilce->super->name, ilce->getNufus(ilce));
            
            for (int j = 0; j < ilce->mahalleSayisi; j++) {
                Mahalle* m = ilce->mahalleler[j];
                printf("    Mahalle: %s - Nufus: %d\n", m->super->name, m->getNufus(m));
                printf("    Kisiler:\n");
                
                for (int k = 0; k < m->kisiSayisi; k++) {
                    Kisi* kisi = m->kisiler[k];
                    // ID-Ad-Soyad-Yas 
                    char* info = kisi->toString(kisi);
                    if (info != NULL) {
                        printf("      %s\n", info);
                        free(info); // free ile bellegi temizle 
                    }
                }
            }
        }
        printf("-------------------------------------------\n");
    } else {
        printf("\nHATA: Girdiginiz koordinatta bir sehir bulunmuyor!\n");
    }

    printf("\nProgrami kapatmak icin herhangi bir tusa basiniz...\n");
    while(getchar() != '\n'); // Önceki tus girdisini temizleyip , 
    getchar(); // tusa basilmasi bekleniyor ki program kapanmadan once sonucu gorebilelim
}

        // ** kurucu ve yikici fonksiyonlar **

Oyun* createOyun(int turSayisi) {
    Oyun* this = (Oyun*)malloc(sizeof(Oyun));
    if (this == NULL) return NULL;

    this->kapasite = BASLANGIC_KAPASITE;
    this->sehirSayisi = 0;
    this->turSayisi = turSayisi;
    
    this->sehirler = (Sehir**)malloc(this->kapasite * sizeof(Sehir*));
    if (this->sehirler == NULL) {
        free(this);
        return NULL;
    }

    // TXT DOSYALARINI ACMA ISLEMI 
    this->fileSehirler   = fopen("sehirler.txt",   "r");
    this->fileIlceler    = fopen("ilceler.txt",    "r");
    this->fileMahalleler = fopen("mahalleler.txt", "r");
    this->fileKisiler    = fopen("kisiler.txt",    "r");

    if (!this->fileSehirler || !this->fileIlceler || !this->fileMahalleler || !this->fileKisiler) {
        printf("HATA: txt dosyalari bulunamadi!\n");
        // Dosyalardan biri bile acilmazsa program patlamasin diye guvenlik onlemi
        destroyOyun(this);
        return NULL;
    }

    // metotlari bagla
    this->sehirEkle          = sehirEkle_Oyun;
    this->Baslat             = Baslat_Oyun;
    this->turAtla            = turAtla_Oyun;
    this->yeniSehirKontrol   = yeniSehirKontrol_Oyun;
    this->ekraniYazdir       = ekraniYazdir_Oyun;
    this->yoket              = destroyOyun; 

    return this;
}

void destroyOyun(Oyun* this) {
    if (this == NULL) return;

    // tum sehirleri sirayla yok et bu da sira sira tüm yerlesimleri yok edecek
    for (int i = 0; i < this->sehirSayisi; i++) {
        this->sehirler[i]->yoket(this->sehirler[i]);
    }
    
    // sehirler dizisini temizle
    if (this->sehirler != NULL) free(this->sehirler);

    // ACIK OLAN DOSYALARI GUVENLE KAPAT
    if (this->fileSehirler)   fclose(this->fileSehirler);
    if (this->fileIlceler)    fclose(this->fileIlceler);
    if (this->fileMahalleler) fclose(this->fileMahalleler);
    if (this->fileKisiler)    fclose(this->fileKisiler);

    // oyun structunu sil
    free(this);
}