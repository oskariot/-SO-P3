#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define true 1
#define false 0

int waitingLinHackers, waitingWindowsers; // dzielone zmienne
int boarding;
int inBoat;

pthread_t Programmers[12];
pthread_mutex_t mainLock; // lock dla: waitingLinHackers/Windowsers, boarding
pthread_cond_t linHackerCanBoat;
pthread_cond_t windowserCanBoat;
pthread_cond_t boardingDone;
pthread_cond_t readyToRow;

void boatAndRow(int);
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
  boarding = false;
  inBoat = 0;

  // inicjowanie zmiennych POSIX-owych
  pthread_mutex_init(&mainLock, NULL);
  pthread_cond_init(&linHackerCanBoat, NULL);
  pthread_cond_init(&windowserCanBoat, NULL);
  pthread_cond_init(&boardingDone, NULL);
  pthread_cond_init(&readyToRow, NULL);

  int tret; // wartość zwracana przez różne pthread_funkcja()

  // startowanie wątków
  for (int i = 0; i < 12; i++)
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
  for (int i = 0; i < 12; i++)
    if (tret = pthread_join(Programmers[i], NULL))
    {
      fprintf(stderr, "Error from pthread_join(). Returned %i instead of 0.", tret); // wyświetlanie kodu błędu
      exit(EXIT_FAILURE);
      printf("hello!2\n");
    }

  // destrukcja zmiennych POSIX-owych
  pthread_mutex_destroy(&mainLock);
  pthread_cond_destroy(&linHackerCanBoat);
  pthread_cond_destroy(&windowserCanBoat);
  pthread_cond_destroy(&boardingDone);
  pthread_cond_destroy(&readyToRow);
  return 0;
}

void * linHackerArrives(void * args)
{
  int id = *((int *)args);
  free(args);
  int iWillRow = false;
  // sekcja krytyczna, użycie zmiennych globalnych waitingLinHackers i waitingWindowsers
  pthread_mutex_lock(&mainLock);
  while (boarding)
    pthread_cond_wait(&boardingDone, &mainLock);
  printf("Hey I'm linux hacker %d!\n", id);
  if (waitingLinHackers == 3)
  {
    boarding = true;
    printf("Me[%d] Found 3 other linux hackers! Let's board!\n", id);
    // zaczynamy boarding
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    waitingLinHackers -= 3; // aktualizacja zmiennej
    iWillRow = true;

    pthread_cond_wait(&readyToRow, &mainLock); // czekaj aż wsiądą do łódki

    printState();
  }
  else if (waitingLinHackers >= 1 && waitingWindowsers >= 2)
  {
    boarding = true;
    printf("Me[%d] Found 1 other linux hacker and 2 windowsers. Let's board!\n", id);
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&windowserCanBoat); // wybudzenia 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    waitingLinHackers--; // aktualizacja zmiennej
    waitingWindowsers -= 2; // aktualizacja zmiennej
    iWillRow = true;

    pthread_cond_wait(&readyToRow, &mainLock); // czekaj aż wsiądą do łódki

    printState();
  }
  else
  {
    waitingLinHackers++;
    printf("Linux hacker %d waiting!\n", id);
    printState();
    pthread_cond_wait(&linHackerCanBoat, &mainLock);
  }
  if (iWillRow)
    boatAndRow(id);
  else
    boatBoard(id);
  pthread_mutex_unlock(&mainLock);
}

void * windowserArrives(void * args)
{
  int id = *((int *)args);
  free(args);
  int iWillRow = false;
  // sekcja krytyczna, użycie zmiennych globalnych waitingLinHackers i waitingWindowsers
  pthread_mutex_lock(&mainLock);
  while (boarding)
    pthread_cond_wait(&boardingDone, &mainLock);
  printf("Hey I'm windowser %d!\n", id);
  if (waitingWindowsers == 3)
  {
    boarding = true;
    printf("Me[%d] Found 3 other windowsers! Let's board!\n", id);
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    waitingWindowsers -= 3; // aktualizacja zmiennej
    iWillRow = true;

    pthread_cond_wait(&readyToRow, &mainLock); // czekaj aż wsiądą do łódki

    printState();
  }
  else if (waitingWindowsers >= 1 && waitingLinHackers >= 2)
  {
    boarding = true;
    printf("Me[%d] Found 1 other windowsers and 2 hackers. Let's board!\n", id);
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenia 1 hackera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hackera
    waitingWindowsers--; // aktualizacja zmiennej
    waitingLinHackers -= 2; // aktualizacja zmiennej
    iWillRow = true;

    pthread_cond_wait(&readyToRow, &mainLock); // czekaj aż wsiądą do łódki

    printState();
  }
  else
  {
    waitingWindowsers++;
    printf("Windowser %d waiting!\n", id);
    printState();
    pthread_cond_wait(&windowserCanBoat, &mainLock);
  }
  if (iWillRow)
    boatAndRow(id);
  else
    boatBoard(id);
  pthread_mutex_unlock(&mainLock);
}

void printState()
{
  printf("waiting hackers: %d, waiting windowsers: %d\n", waitingLinHackers, waitingWindowsers);
}

void boatAndRow(int id)
{
  // wsiądź i odepchnij łódkę
  boarding = false;
  inBoat = 0;
  printf("%d: The boat has left the dock! ", id);
  pthread_cond_broadcast(&boardingDone);
  // odblokuj lock
}

void boatBoard(int id)
{
  inBoat++;
  if (inBoat == 3)
    pthread_cond_signal(&readyToRow);
  printf("%d is in!\n", id);
}
