## Zadanie 3. Testy i pomiary (50%)
- - - - 
(25%) Przygotuj plik Makefile, zawierający polecenie uruchamiania testów oraz polecenia kompilacji programu z zad 2 na trzy sposoby:
Z wykorzystaniem bibliotek statycznych,
Z wykorzystaniem bibliotek dzielonych (dynamiczne, ładowane przy uruchomieniu programu),
Z wykorzystaniem bibliotek ładowanych dynamicznie (dynamiczne, ładowane przez program).
Wyniki pomiarów zbierz w pliku results3a.txt. Otrzymane wyniki krótko skomentuj.
- - - - 
(25%) Rozszerz plik Makefile z punktu 3a) dodając możliwość skompilowania programu na trzech różnych  poziomach optymalizacji — -O0…-Os. Przeprowadź ponownie pomiary, kompilując i uruchamiając program dla różnych poziomów optymalizacji.
Wyniki pomiarów dodaj do pliku results3b.txt. Otrzymane wyniki krótko skomentuj.
Wygenerowane pliki z raportami załącz jako element rozwiązania.

Uwaga: Do odczytania pliku można użyć funkcji read() (man read), do wywołania zewnętrznego polecenia Unixa można użyć funkcji system() (man system).

# To rozwiązanie nie działa poprawnie przy dynamicznym ładowaniu bibliotek
