## Zadanie 2. Równoległe mnożenie macierzy (50%)
Napisz program macierz do równoległego mnożenia macierzy — zawartość każdej z macierzy (wejściowej lub wynikowej) znajduje się w osobnych plikach. Argumenty operacji mnożenia są treścią pliku lista, będącego pierwszym argumentem wywołania programu. Plik lista zawiera, w pojedynczej linii:

Nazwę pliku z  pierwszą macierzą wejściową. 
Nazwę pliku z  drugą macierzą wejściową. 
Nazwę pliku z macierzą wyjściową.
Na samym początku program (Manager Process) tworzy podaną (w drugim argumencie programu) liczbę procesów potomnych (Worker Process).
 Model manager - Worker
Dla każdego wpisu z pliku  lista proces potomny  mnoży odpowiednie wiersze macierzy A przez odpowiednie  kolumny macierzy B,  a wynik zapisuje w odpowiednie miejsce macierzy C  — każdy z procesów potomnych korzysta z całej macierzy A, a w przypadku macierzy B, tylko z określonych jej kolumn — patrz rysunek: Mnozenie 

Przykładowo, dla dwóch procesów potomnych, pierwszy z nich mnoży odpowiednie wiersze macierzy A przez kolumny macierzy B oznaczone kolorem zielonym; drugi proces potomny, mnoży odpowiednie wiersze macierzy A przez kolumny macierzy B oznaczone kolorem niebieskim.
Czas działania procesu potomnego nie może przekroczyć podanej liczby sekund — trzeci argument programu. Po upłynięciu tego czasu każdy z procesów potomnych kończy swoje działanie, zwracając do procesu macierzystego poprzez kod wyjścia procesu liczbę wykonanych mnożeń — wymnożenie  fragmentu macierzy A przez fragment macierzy B traktujemy jako jedno mnożenie. Proces, który zakończył mnożenie fragmentów, nie kończy działania — może być użyty do kolejnej operacji mnożenia. Program macierzysty pobiera statusy procesów potomnych, wypisuje raport: "Proces PID wykonał n mnożeń macierzy" i  kończy swoje działanie. UWAGA! Nie używamy sygnałów, które są tematem następnych zajęć. Ponadto zakładamy, że jedynym dostępnym sposobem komunikacji procesów jest, w tym momencie, komunikacja poprzez plik — właściwe sposoby komunikacji procesów poznamy na dalszych ćwiczeniach.

Tworzenie pliku wynikowego może się odbywać na dwa sposoby — czwarty argument programu:

Procesy potomne  zapisują wynik mnożenia w odpowiednie miejsce wspólnego pliku wynikowego — pamiętaj o odpowiednim użyciu blokady FLOCK.
Procesy potomne zapisują wynik mnożenia do odrębnych plików; osobny proces wywołuje jedną z funkcji rodziny exec*() w celu wykonania komendy paste — połączenie zawartości plików.
 
Napisz pomocniczy program, który:

Tworzy określoną liczbę plików z treścią macierzy. Rozmiar tworzonych macierzy, dla odrębnych par macierzy, jest losowy ∈ [Min, Max], gdzie: Min, Max są argumentami programu, przy czym należy zadbać, aby (dla danej pary) liczba kolumn macierzy A była równa liczbie wierszy macierzy B.
Umożliwia sprawdzenie poprawności otrzymanych wyników mnożenia — implementacja własna lub  przy wykorzystaniu dowolnej biblioteki do wykonywania testów jednostkowych

# To rozwiązanie nie działa poprawnie dla więcej niż 1 procesu liczącego
