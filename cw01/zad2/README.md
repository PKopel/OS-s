## Zadanie 2. Program korzystający z biblioteki (25%)
Napisz program testujący działanie funkcji z biblioteki z zadania 1.

Jako argumenty przekaż liczbę elementów tablicy głównej (liczbę par plików) oraz listę zadań do wykonania. Zadania mogą stanowić zadania porównania wszystkich par w sekwencji lub zadania usunięcia bloku o podanym indeksie bądź usunięcia operacji o podanym indeksie.

Operacje mogą być specyfikowane w linii poleceń na przykład jak poniżej:

- create_table rozmiar — stworzenie tablicy o rozmiarze "rozmiar"
- compare_pairs file1A.txt:file1B.txt file2A.txt:file2B.txt … — porównanie para plików:  file1A.txt z file1B.txt, file2A.txt z file2B.txt, itd
- remove_block index — usuń z tablicy bloków o indeksie index
remove_operation block_index operation_index — usuń z bloku o indeksie block_index operację o indeksie operation_index


Program powinien stworzyć tablice bloków o zadanej liczbie elementów

W programie zmierz, wypisz na konsolę i zapisz  do pliku z raportem  czasy realizacji podstawowych operacji:

Przeprowadzenie porównania par plików — różna ilość elementów w sekwencji par (mała (np. 1-5), średnia oraz duża ilość par) oraz różny stopień podobieństwa plików w parze (pliki bardzo podobne do siebie, pliki w średnim stopniu niepodobne do siebie, pliki w znacznym stopniu niepodobne do siebie)
Zapisanie, w pamięci, bloków o różnych rozmiarach (odpowiadających rozmiarom różnych przeprowadzonych porównań)
Usunięcie zaalokowanych bloków o różnych rozmiarach  (odpowiadających rozmiarom różnych przeprowadzonych porównań)
Na przemian  kilkakrotne dodanie i usunięcie zadanej liczby bloków 
Mierząc czasy poszczególnych operacji, zapisz trzy wartości: czas rzeczywisty, czas użytkownika i czas systemowy. Rezultaty umieść pliku raport2.txt i dołącz do archiwum zadania.
