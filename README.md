# Pracownia P3 - Systemy Operacyjne

## Problem przeprawy przez rzekę (*River crossing*)
Pewna przeprawa przez rzekę dzielona jest zarówno przez linuxowych Hakerów, jak i pracowników Microsoftu. Do przeprawy używana jest **jedna** łódka, która mieści tylko **czterech** programistów i zawsze płynie **w pełni** załadowana. Łódka napędzana jest przez **jedną** osobę wiosłującą.<br />
Aby zagwarantować bezpieczeństwo programistów w łódce, nie może dojść do następujących sytuacji:

1. Trzech linuxowych Hakerów przebywa w łódce z trzema pracownikami Microsoftu.
2. Trzech pracowników Microsoftu przebywa w łódce z trzema linuxowymi Hakerami.

Wszystkie pozostałe kombinacje są bezpieczne.<br />
Dodatkowo programiści wprowadzili zasadę "kto pierwszy ten lepszy", a więc ostatnia zgłoszona do łódki osoba będzie wioślarzem. Łódka ma być gospodarowana **optymalnie**, tzn. powinna odpłynąć, gdy zgłoszone osoby będą mogły utworzyć poprawny ładunek. Ponieważ może się okazać, że wśród ostatnich programistów nie da się utworzyć ładunku (np. pozostanie trzech programistów), powinni oni zaniechać prób dostania się na łódź.
#### Zadanie
Zaimplementować rozwiązanie powyższego konfliktu zasobów z użyciem wątków. Każdy programista powinien być reprezentowany przez osobny wątek Hakera / pracownika Microsoftu wykonującym odpowiednio jedną z funkcji **_linHackerArrives()_** i **_windowserArrives()_**. Wątek może zakończyć działanie, gdy wystąpi jedna z sytuacji:

1. Programista przeprawi się przez rzekę.
2. Programista nie ma możliwości przeprawy przez rzekę (pula programistów skończyła się i nie może on już dobrać sobie odpowiedniej ekipy).

Wejście programisty na łódkę odbywa się z użyciem funkcji **_boardBoat()_**. Ostatni ze zgłoszonych programistów wywołuje funkcję **_boardAndRow()_** ("wejdź na pokład i wiosłuj"). Należy zadbać, żeby funkcja **_boardAndRow()_** została wywołana, gdy pozostali programiści zajęli już swoje miejsca w łódce. W czasie wodowania należy powstrzymać pozostałych programistów od wchodzenia do łódki. <br />
Rozwiązanie może korzystać z **tylko jednej** zmiennej typu *mutex*.

## Kompilacja
Aby skompilować program oraz tester należy uruchomić:
```Makefile
make
```
lub
```Makefile
make compile
```
## Uruchamianie
Program przyjmuje następujące parametry:
```Makefile
-s <wartość> # prędkość generowania programistów [(szybko) 0...12 (wolno)] (domyślnie 9)
-p <wartość> # ilość programistów do wygenerowania [0...] (domyślnie 100)
-t <wartość> # [0 lub 1] gdy 1 do pliku tekstowego drukuje raport do testów (domyślnie 0) 
```
Aby uruchomić program należy wykonać:
```Makefile
make run # uruchomienie z domyślnymi parametrami (-s 9, -p 100, -t 0)
```
Aby uruchomić program z parametrami użytkownika:
```Makefile
make run ARGS="-s 1 -p 300 -t 0"
```
lub
```Makefile
./riverCrossing -s 1 -p 300 -t 0
```
**Uwaga**: warto uruchomić program z prędkością 1 (parametr: -s 1), aby zaobserwować kończenie działania programu i zwracaną informację.
## Testowanie
Testowanie odbywa się za pomocą programu **_./tester_**. Komenda zbiorcza ```make test[nr]``` uruchamia program dla pewnych parametrów z zapisem raportu do pliku. Następnie **_./tester_** analizuje raport i drukuje podsumowanie poprawności na wyjście standardowe. Następnie raport jest kasowany.<br />
Ponieważ działanie generatora programistów zależy od działania generatora liczb losowych, testy warto uruchomić kilkukrotnie. Przygotowano pięć testów.
```Makefile
make test1 # uruchomienie testu dla 199 programistów
```
```Makefile 
make test2 # uruchomienie testu dla 200 programistów
```
```Makefile
make test3 # uruchomienie testu dla 201 programistów
```
```Makefile
make test4 # uruchomienie testu dla 202 programistów
```
```Makefile
make test5 # uruchomienie testu dla 1015 programistów
```
## Usuwanie
Aby usunąć program należy wykonać:
```Makefile
make clean
```


*Oskar Wieczorek,  24.01.2016*
