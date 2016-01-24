# Pracownia P3 - Systemy Operacyjne

## Problem przeprawy przez rzekę (*River crossing*)
Pewna przeprawa przez rzekę dzielona jest zarówno przez linuxowych Hakerów, jak i pracowników Microsoftu. Do przeprawy używana jest **jedna** łódka, która mieści tylko **czterech** programistów i zawsze płynie **w pełni** załadowana.
Aby zagwarantować bezpieczeństwo programistów w łódce nie może dojść do następujących sytuacji:

1. Trzech linuxowych hakerów przebywa w łódce z trzema pracownikami Microsoftu.
2. Trzech pracowników Microsoftu przebywa w łódce z trzema linuxowymi Hakerami.

Wszystkie pozostałe kombinacje są bezpieczne.
Dodatkowo 
Two procedures are needed: HackerArrives and EmployeeArrives, called by a hacker
or employee when he/she arrives at the river bank. The procedures arrange the
arriving hackers and employees into safe boatloads; once the boat is full, one thread
calls Rowboat and only after the call to Rowboat, the four threads representing the
people in the boat can return.
You can use either semaphores or condition variables to implement the solution. Any
order is acceptable and there should be no busy-waiting and no undue waiting –
hackers and employees should not wait if there are enough of them for a safe
boatload. Your code should be clearly commented, in particular, you should comment
each semaphore or condition variable operation to specify how correctness
properties are preserved.

W przypadku, gdy uzna pan, że temat nie nadaje się na pracownię P3, chciałbym wybrać temat powłoki, który wysłał pan jako propozycję i którego opis znajduje się pod adresem: http://pages.cs.wisc.edu/~dusseau/Classes/CS537-F07/Projects/P1/p1.html.
