# MindReader

Bu projede roller değişiyor: kullanıcı bir sayı tutuyor, bilgisayar ikili arama
mantığıyla sayıyı az adımda buluyor.

Bu sürüm C++ ile yazıldı.

## Oyun Akışı

1. Program senden `1-1000` arası bir sayı tutmanı ister.
2. Mod seçersin: `1` = Klasik İkili Arama (garantili hızlı), `2` = Rastgele Pivot (daha az tekrar eden tahminler).
3. Bilgisayar tahmin eder.
4. Sen şu şekilde cevap verirsin:
   - `b`: Tuttuğum sayı daha büyük
   - `k`: Tuttuğum sayı daha küçük
   - `d`: Doğru tahmin
   - `u`: Son adımı geri al
5. Bilgisayar seçilen moda göre aralığı daraltarak sonuca gider.

## Özellikler

- Klasik İkili Arama modu (en hızlı)
- Rastgele Pivot modu (daha sürprizli tahmin akışı)
- Geçersiz tuşlama kontrolü (`b/k/d` dışında giriş)
- Yanlış tuşlama için `u` ile son adımı geri alma
- Çelişkili ipucu tespiti (imkansız aralık oluşursa oyun biter)
- Başlangıçta ASCII logo
- GitHub Actions ile otomatik test (sürekli bütünleştirme)
- İş mantığı (`next_guess`, `update_range`) ayrı modülde olduğu için test edilebilir yapı

## Çalıştırma

```bash
make run
```

## Test

```bash
make test
```

## Elle Derleme

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src/main.cpp src/mind_reader_logic.cpp -o build/mind_reader
```
