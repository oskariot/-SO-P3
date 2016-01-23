#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

void rowBoat();
void boatBoard();
// generator_hackerów()
// generator_windowsów()

int waitingHackers, waitingWindowser; // dzielone zmienne
pthread_mutex_t Lock; // lock założony w celu korzystania z dzielonych zmiennych
pthread_cond_t windoserCanBoat;
pthread_cond_t hackerCanBoat;

int main()
{
  return 0;
}

void rowBoat()
{
  printf("The boat has left the dock");
  // blokada semafora na wsiadanie do łódki
}

void boatBoard()
{
  printf("I'm in!");
}
