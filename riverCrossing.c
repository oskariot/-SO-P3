#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int waitingLinHackers, waitingWindowsers; // dzielone zmienne

pthread_t Programmers[10];
pthread_mutex_t Lock; // lock założony w celu korzystania z dzielonych zmiennych
pthread_cond_t linHackerCanBoat;
pthread_cond_t windowserCanBoat;

void rowBoat(int);
void boatBoard(int);
// generator_hackerów()
// generator_windowsów()
void * linHackerArrives(void * args);
void * windowserArrives(void * args);

void printState();

int main()
{
  srand(time(NULL));

  // inicjowanie zmiennych dzielonych
  waitingLinHackers = 0;
  waitingWindowsers = 0;

  // inicjowanie zmiennych POSIX-owych
  pthread_mutex_init(&Lock, NULL);
  pthread_cond_init(&linHackerCanBoat, NULL);
  pthread_cond_init(&windowserCanBoat, NULL);

  int tret; // wartość zwracana przez różne pthread_funkcja()

  // startowanie wątków
  for (int i = 0; i < 10; i++)
  {
    int * id = malloc(sizeof(int));
    *id = i;
    if (rand() % 2)
    {
      if (tret = pthread_create(&Programmers[i], NULL, linHackerArrives, (void *)id))
      {
        fprintf(stderr,"Error from pthread_create(). Returned %i instead of 0.", tret); // wyświetlanie kodu błędu
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      if (tret = pthread_create(&Programmers[i], NULL, windowserArrives, (void *)id))
      {
        fprintf(stderr,"Error from pthread_create(). Returned %i instead of 0.", tret); // wyświetlanie kodu błędu
        exit(EXIT_FAILURE);
      }
    }
  }

  // kończenie wątków
  for (int i = 0; i < 10; i++)
  if (tret = pthread_join(Programmers[i], NULL))
  {
    fprintf(stderr, "Error from pthread_join(). Returned %i instead of 0.", tret); // wyświetlanie kodu błędu
    exit(EXIT_FAILURE);
    printf("hello!2\n");
  }


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

void windowserArrives()
{
  // sekcja krytyczna, użycie zmiennych globalnych waitingLinHackers i waitingWindowsers
  pthread_mutex_lock(&Lock);
  if (waitingWindowsers == 3)
  {
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    waitingWindowsers -= 3; // aktualizacja zmiennej
    rowBoat();
  }
  else if (waitingWindowsers >= 1 && waitingLinHackers >= 2)
  {
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenia 1 hackera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hackera
    waitingWindowsers--; // aktualizacja zmiennej
    waitingLinHackers -= 2; // aktualizacja zmiennej
  }
  else
  {
    waitingWindowsers++;
    pthread_cond_wait(&windowserCanBoat, &Lock);
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
