## Zadanie 2. Operacje na strukturze katalogów (45%)
Napisz prosty odpowiednik programu find — program powinien implementować następujące opcje: '-mtime', '-atime' oraz '-maxdepth'.  W przypadku dwóch pierwszych, podobnie jak w przypadku find, argumentem może być: liczba (bezwzględna), liczba poprzedzonej znakiem '+' lub liczba poprzedzona znakiem '-'. Program ma wypisać na standardowe wyjście następujące informacje o znalezionych plikach:

Ścieżka bezwzględna pliku,
Liczbę dowiązań
Rodzaj pliku (zwykły plik - file, katalog - dir, urządzenie znakowe - char dev, urządzenie blokowe - block dev, potok nazwany - fifo, link symboliczny - slink, soket - sock) 
Rozmiar w bajtach,
Datę ostatniego dostępu,
Datę ostatniej modyfikacji.
Ścieżka podana jako argument wywołania może być względna lub bezwzględna.  Program nie powinien podążać za dowiązaniami symbolicznymi do katalogów.

Program należy zaimplementować w dwóch wariantach:

- Korzystając z funkcji opendir(), readdir() oraz funkcji z rodziny stat (25%)
- Korzystając z funkcji nftw() (20%)

- - - - - 
W ramach testowania funkcji:

Utwórz w badanej strukturze katalogów jakieś dowiązania symboliczne, zwykłe pliki i katalogi.
Porównaj wynik szukania (własna implementacja) z wynikiem szukania za pomocą polecenia find — wywołaj polecenie find z opcjami '-mtime', '-atime' lub '-maxdepth' — przykłady:
```find /etc -mtime 0  2> /dev/null | wc -l```
```find /usr -atime -7 -maxdepth 2 2> /dev/null | wc -l```
Następnie wywołaj swój program z takimi samymi opcjami i sprawdź, czy liczba znalezionych plików jest taka sama.
