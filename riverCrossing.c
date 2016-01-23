#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int waitingLinHackers, waitingWindowsers; // dzielone zmienne
pthread_mutex_t Lock; // lock założony w celu korzystania z dzielonych zmiennych
pthread_cond_t linHackerCanBoat;
pthread_cond_t windowserCanBoat;

void rowBoat();
void boatBoard();
// generator_hackerów()
// generator_windowsów()
void linHackerArrives();

int main()
{
  // inicjowanie zmiennych POSIX-owych
  pthread_mutex_init(&Lock, NULL);
  pthread_cond_init(&linHackerCanBoat, NULL);
  pthread_cond_init(&windowserCanBoat, NULL);


  // destrukcja zmiennych POSIX-owych
  pthread_mutex_destroy(&Lock);
  pthread_cond_destroy(&linHackerCanBoat);
  pthread_cond_destroy(&windowserCanBoat);
  return 0;
}

void linHackerArrives()
{
  // sekcja krytyczna, użycie zmiennych globalnych waitingLinHackers i waitingWindowsers
  pthread_mutex_lock(&Lock);
  if (waitingLinHackers == 3)
  {
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    waitingLinHackers -= 3; // aktualizacja zmiennej
    rowBoat();
  }
  else if (waitingLinHackers >= 1 && waitingWindowsers >= 2)
  {
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&windowserCanBoat); // wybudzenia 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    waitingLinHackers--; // aktualizacja zmiennej
    waitingWindowsers -= 2; // aktualizacja zmiennej
  }
  else
  {
    waitingLinHackers++;
    pthread_cond_wait(&linHackerCanBoat, &Lock);
  }
  boatBoard();
  pthread_mutex_unlock(&Lock);
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
