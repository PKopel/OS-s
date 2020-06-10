## Zadanie 3 (40%)
W problemie producenta i konsumenta występują dwa rodzaje procesów, które dzielą wspólny bufor dla produkowanych i konsumowanych jednostek. Zadaniem producenta jest wytworzenie surowca, umieszczenie go w buforze i rozpoczęcie pracy od nowa. Konsument pobiera surowiec z bufora i wykorzystuje go.
- - - - -
Przy pomocy potoków nazwanych zaimplementować problem Producenta i Konsumenta. Napisać dwa niezależne programy - Producent oraz Konsument, które będą komunikować się poprzez potok nazwany (kolejkę FIFO). Do potoku pisać będzie wiele procesów wykonujących program Producenta, a czytał będzie z niej jeden proces Konsumenta. Dla zademonstrowania, że nie doszło do utraty ani zwielokrotnienia towaru surowiec będzie pobierany z pliku przez Producenta i umieszczany w innym pliku przez Konsumenta.
- - - - -
Producent:

przyjmuje trzy argumenty: ścieżka do potoku nazwanego, ścieżka do pliku tekstowego z dowolną zawartością, N - liczba znaków odczytywanych jednorazowo z pliku
otwiera potok nazwany
wielokrotnie (aż do odczytania całego pliku):
odczekuje losową ilość czasu (np. 1-2 sekund)
zapisuje do potoku nazwanego linię zawierającą swój PID oraz odczytany fragment pliku w następującej formie: #PID#(N odczytanych znaków)
- - - - -
Konsument:

przyjmuje trzy argumenty: ścieżka do potoku nazwanego, ścieżka do pliku tekstowego (do którego będzie zapisywany odczytany tekst), N - liczba znaków odczytywanych jednorazowo z pliku
otwiera potok nazwany
wielokrotnie:
czyta kolejnych N znaków potoku nazwanego
umieszcza odczytane znaki w pliku tekstowym (różnym od plików, z których korzystają producenci)
Pliki tekstowe powinny być krótkie (na 5-10 odczytów) i umożliwiać sprawdzenie poprawności działania (brak utraty, zwielokrotnienia surowca). W szczególności każdy Producent powinien otrzymać wygenerowany w dowolny sposób plik tekstowy z dowolną zawartością, ale w istotny sposób różniącą się od zawartości plików innych Producentów. Na przykład jeden producent może otrzymać plik zawierający tylko konkretną literę, inny tylko liczby itd. 
- - - - -
Sprawdzić, że potoki nazwane działają dla niezależnych procesów - utworzyć potok z linii komend, a następnie uruchomić Producenta i Konsumenta w różnych terminalach. Dodatkowo należy napisać program, który tworzy potok nazwany, a następnie uruchamia program Konsumenta i pięciu Producentów (z różnymi argumentami).
