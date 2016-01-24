# Pracownia P3 - Systemy Operacyjne

## Problem przeprawy przez rzekę (*River crossing*)
```
Pewna przeprawa przez rzekę jest dzielona zarówno przez linuxowych Hakerów, jak i 
pracowników Microsoftu. Do przeprawy używana jest łódka, która mieści tylko **czterech**
programistów i zawsze płynie **w pełni** załadowana.
Aby zagwarantować bezpieczeństwo programistów w łódce nie można dopuścić do 
następujących sytuacji:
A particular river crossing is shared by both Linux Hackers and Microsoft employees.
A boat is used to cross the river, but it only seats four people, and must always carry
a full load. In order to guarantee the safety of the hackers, you cannot put three
employees and one hacker in the same boat (because the employees would gang up
and convert the hacker). Similarly, you cannot put three hackers in the same boat as
an employee (because the hackers would gang up and convert the employee). All
other combinations are safe.
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
```

W przypadku, gdy uzna pan, że temat nie nadaje się na pracownię P3, chciałbym wybrać temat powłoki, który wysłał pan jako propozycję i którego opis znajduje się pod adresem: http://pages.cs.wisc.edu/~dusseau/Classes/CS537-F07/Projects/P1/p1.html.
