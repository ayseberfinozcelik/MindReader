# Mind Reader - Tersine Muhendislik Sayi Tahmin Oyunu

Bu projede roller degisiyor: kullanici bir sayi tutuyor, bilgisayar Binary Search
(Ikili Arama) ile sayiyi minimum adimda buluyor.

## Oyun Akisi

1. Program senden `1-1000` arasi bir sayi tutmani ister.
2. Bilgisayar ortadaki sayiyi tahmin eder.
3. Sen su sekilde cevap verirsin:
   - `y`: Tuttugum sayi daha buyuk
   - `a`: Tuttugum sayi daha kucuk
   - `e`: Dogru tahmin
4. Bilgisayar araligi her adimda ikiye bolerek hizla sonuca gider.

## Ozellikler

- Binary Search ile en fazla 10-11 adimda sonucu bulma
- Gecersiz tuslama kontrolu (`y/a/e` disinda giris)
- Celiskili/yalan ipucu tespiti (imkansiz aralik olusursa oyun biter)
- Baslangicta ASCII logo
- GitHub Actions ile otomatik test (CI)

## Calistirma

```bash
python3 mind_reader.py
```

## Test

```bash
python3 -m unittest discover -s tests -v
```
