## Zadanie 1 (50%)
Napisz interpreter poleceń przechowywanych w pliku. Ścieżka do pliku to pierwszy argument wywołania programu.

Polecenia w pliku przechowywane są w kolejnych liniach w postaci: 
```
prog1 arg1 ... argn1 | prog2 arg1 ... argn2 | ... | progN arg1 ... argnN
```
Interpreter powinien uruchomić wszystkie N poleceń w osobnych procesach, zapewniając przy użyciu potoków nienazwanych oraz funkcji dup2, by wyjście standardowe procesu k było przekierowane do wejścia standardowego procesu (k+1)
Można założyć ograniczenie górne na ilość obsługiwanych argumentów oraz ilość połączonych komend w pojedynczym poleceniu (co najmniej 3).
Po uruchomieniu ciągu programów składających się na pojedyncze polecenie (linijkę) interpreter powinien oczekiwać na zakończenie wszystkich tych programów.
Program należy zaimplementować korzystając z funkcji pipe/fork/exec.
