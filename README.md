# -SO-P3

Jako P3 chciałbym w miarę możliwości zrobić bardziej skomplikowany problem synchronizacji (rozwinięcie P2).
Po przeszukaniu Internetu w poszukiwaniu trudnych problemów synchronizacji natrafiłem na następujący problem:

## River Crossing

```
A particular river crossing is shared by both Linux hackers and Microsoft employees. A boat is used to cross
the river, but it only seats four people, and must always carry a full load. In order to guarantee the safety
of the hackers, you cannot put three employees and one hacker in the same boat; similarly, you cannot put 
three hackers in the same boat as an employee. To further complicate matters, there is room to board only one
boat at a time; a boat must be taken across the river in order to start boarding the next boat. All other
combination are safe. Two procedures are needed, HackerArrives and EmployeeArrives, called by a hacker or 
employee when he/she arrives at the river bank. The procedures arrange the arriving hackers and employees 
into safe boatloads. To get into a boat, a thread calls BoardBoat(); once the boat is full, one thread calls 
RowBoat(). RowBoat() does not return until the boat has left the dock.
Assume BoardBoat() and RowBoat() are already written. Implement HackerArrives() and EmployeeArrives(). These
methods should not return until after RowBoat() has been called for the boatload. Any order is acceptable 
(again, don't worry about starvation), and there should be no busy-waiting and no undue waiting (hackers 
and employees should not wait if there are enough of them for a safe boatload).
```
