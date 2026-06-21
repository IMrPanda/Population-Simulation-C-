package odev.pdp;
import com.github.javafaker.Faker;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class Main {

	public static void main(String[] args) {
Faker faker = new Faker();
        
        System.out.println("Veri üretimi başlıyor... Lütfen bekleyin.");

        // Metotları sırayla çağırarak tüm dosyaları tek seferde üretiyoruz
        kisileriUret(faker, 10000);
        sehirleriUret(faker, 1000);
        ilceleriUret(faker, 2000);
        mahalleleriUret(faker, 5000);

        System.out.println("Bütün veriler başarıyla .txt dosyalarına yazıldı!");
    }

    private static void kisileriUret(Faker faker, int adet) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("kisiler.txt"))) {
            for (int i = 0; i < adet; i++) {
                // C dilinde %s %s %d ile rahat okumak için isimlerdeki olası boşlukları siliyoruz
                String ad = faker.name().firstName().replaceAll(" ", "");
                String soyad = faker.name().lastName().replaceAll(" ", "");
                int yas = faker.number().numberBetween(0, 51);
                
                writer.write(ad + " " + soyad + " " + yas);
                writer.newLine();
            }
            System.out.println("+ kisiler.txt oluşturuldu (" + adet + " satır).");
        } catch (IOException e) {
            System.out.println("Kişiler dosyası yazılırken hata oluştu: " + e.getMessage());
        }
    }

    private static void sehirleriUret(Faker faker, int adet) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("sehirler.txt"))) {
            for (int i = 0; i < adet; i++) {
                // Şehir isimlerinde boşluk olabilir (New York gibi), C'de fgets ile satır satır okumak daha mantıklı olur
                String sehir = faker.address().city();
                writer.write(sehir);
                writer.newLine();
            }
            System.out.println("+ sehirler.txt oluşturuldu (" + adet + " satır).");
        } catch (IOException e) {
            System.out.println("Şehirler dosyası yazılırken hata oluştu: " + e.getMessage());
        }
    }

    private static void ilceleriUret(Faker faker, int adet) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("ilceler.txt"))) {
            for (int i = 0; i < adet; i++) {
                String ilce = faker.address().state(); // Faker'da ilçe için state veya county kullanılabilir
                writer.write(ilce);
                writer.newLine();
            }
            System.out.println("+ ilceler.txt oluşturuldu (" + adet + " satır).");
        } catch (IOException e) {
            System.out.println("İlçeler dosyası yazılırken hata oluştu: " + e.getMessage());
        }
    }

    private static void mahalleleriUret(Faker faker, int adet) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("mahalleler.txt"))) {
            for (int i = 0; i < adet; i++) {
                String mahalle = faker.address().streetName() + " Mahallesi";
                writer.write(mahalle);
                writer.newLine();
            }
            System.out.println("+ mahalleler.txt oluşturuldu (" + adet + " satır).");
        } catch (IOException e) {
            System.out.println("Mahalleler dosyası yazılırken hata oluştu: " + e.getMessage());
        }
    }

}


