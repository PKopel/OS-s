## Zadanie 1 (10%)
Napisz program wypisujący w pętli nieskończonej zawartość bieżącego katalogu. 
Po odebraniu sygnału SIGTSTP (CTRL+Z) program zatrzymuje się, wypisując komunikat 
"Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu". Po ponownym 
wysłaniu SIGTSTP program powraca do pierwotnego wypisywania.
Program powinien również obsługiwać sygnał SIGINT. Po jego odebraniu program 
wypisuje komunikat "Odebrano sygnał SIGINT" i kończy działanie. W kodzie programu, 
do przechwycenia sygnałów użyj zarówno funkcji signal, jak i sigaction (np. SIGINT 
odbierz za pomocą signal, a SIGTSTP za pomocą sigaction).
