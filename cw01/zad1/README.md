## Zadanie 1. Alokacja tablicy ze wskaźnikami na bloki pamięci zawierające  tablicę wskaźników (25%)

Zaprojektuj i przygotuj zestaw funkcji (bibliotekę) do zarządzania tablicą bloków, w których to blokach pamięci zapisywane są rezultaty operacji porównywania plików wiersz po wierszu (poleceniem diff) sekwencji par plików  — sekwencja ich nazw jest parametrem funkcji. 

Biblioteka powinna umożliwiać: 

Utworzenie tablicy wskaźników (tablicy głównej) — w tej tablicy będą przechowywane wskaźniki na bloki operacji edycyjnych — pierwszy element tablicy głównej zawiera wykaz operacji edycyjnych dla pierwszej pary plików, drugi element dla drugiej pary, itd. Pojedynczy blok operacji edycyjnych (element wskazywany z tablicy głównej), to tablica wskaźników na poszczególne operacje edycyjne
Definiowanie sekwencji par plików
Przeprowadzenie porównania (dla każdego elementu sekwencji) oraz zapisanie wyniku porównania do pliku tymczasowego
Utworzenie, na podstawie zawartość pliku tymczasowego, bloku operacji edycyjnych — tablicy wskaźników na operacje edycyjne, ustawienie w tablicy głównej (wskaźników) wskazania na ten blok; na końcu, funkcja powinna zwrócić indeks elementu tablicy (głównej), który zawiera wskazanie na utworzony blok  — dla  pokazanego (niżej) przykładu powinna więc zwrócić 0
Zwrócenie informacji o ilości operacji w danym bloku operacji edycyjnych — dla przykładu,  pokazanego poniżej, zwracaną wartością powinno być 3
Usunięcie, z pamięci, bloku (operacji edycyjnych) o zadanym indeksie
Usunięcie, z pamięci, określonej operacji dla podanego bloku operacji edycyjnych
Przykład — załóżmy, że sekwencja nazw plików zawiera tylko jedną parę ('a.txt', 'b.txt').

Zawartość pliku a.txt:
```
aaa
bbb ccc
ddd
eee
hhh iii 
```
Zawartość pliku b.txt:
```
jjj kkk
aaa
fff ccc
eee
bbb ggg
```
Wynik wykonania diff a.txt b.txt:
```
0a1
> jjj kkk
2,3c3
< bbb ccc
< ddd
---
> fff ccc
5c5
< hhh iii 
---
> bbb ggg
```
W tym przypadku tablica główna powinna zawierać tylko jeden wskaźnik na  blok operacji edycyjnych (bo mamy tylko jedną parę plików).  Blok operacji edycyjnych powinien być trzyelementową tablicą wskaźników na napisy z treścią operacji edycyjnych.
Struktura danych

Tablice / bloki powinny być alokowane przy pomocy funkcji calloc() (alokacja dynamiczna).

Przygotuj plik Makefile, zawierający polecenia kompilujące pliki źródłowe biblioteki oraz tworzące biblioteki w dwóch wersjach: statyczną i współdzieloną.
