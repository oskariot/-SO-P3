#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int waitingLinHackers, waitingWindowsers; // dzielone zmienne

pthread_t Programmers[10];
pthread_mutex_t Lock; // lock założony w celu korzystania z dzielonych zmiennych
pthread_cond_t linHackerCanBoat;
pthread_cond_t windowserCanBoat;

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

void * linHackerArrives(void * args)
{
  int id = *((int *)args);
  free(args);
  // pthread_cond_wait(&Lock);
  // sekcja krytyczna, użycie zmiennych globalnych waitingLinHackers i waitingWindowsers
  pthread_mutex_lock(&Lock);
  printf("Hey I'm linux hacker %d!\n", id);
  if (waitingLinHackers == 3)
  {
    printf("Me[%d] Found 3 other linux hackers! Let's board!\n", id);
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    waitingLinHackers -= 3; // aktualizacja zmiennej
    boatAndRow(id);
    printState();
  }
  else if (waitingLinHackers >= 1 && waitingWindowsers >= 2)
  {
    printf("Me[%d] Found 1 other linux hacker and 2 windowsers. Let's board!\n", id);
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hakera
    pthread_cond_signal(&windowserCanBoat); // wybudzenia 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    waitingLinHackers--; // aktualizacja zmiennej
    waitingWindowsers -= 2; // aktualizacja zmiennej
    boatAndRow(id);
    printState();
  }
  else
  {
    waitingLinHackers++;
    printf("Linux hacker %d waiting!\n", id);
    printState();
    pthread_cond_wait(&linHackerCanBoat, &Lock);
  }
  boatBoard(id);
  pthread_mutex_unlock(&Lock);
}

void * windowserArrives(void * args)
{
  int id = *((int *)args);
  free(args);
  // sekcja krytyczna, użycie zmiennych globalnych waitingLinHackers i waitingWindowsers
  pthread_mutex_lock(&Lock);
  printf("Hey I'm windowser %d!\n", id);
  if (waitingWindowsers == 3)
  {
    printf("Me[%d] Found 3 other windowsers! Let's board!\n", id);
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    waitingWindowsers -= 3; // aktualizacja zmiennej
    // czekaj aż wsiądą do łódki
    boatAndRow(id); // + return
    printState();
  }
  else if (waitingWindowsers >= 1 && waitingLinHackers >= 2)
  {
    printf("Me[%d] Found 1 other windowsers and 2 hackers. Let's board!\n", id);
    pthread_cond_signal(&windowserCanBoat); // wybudzenie 1 windowsera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenia 1 hackera
    pthread_cond_signal(&linHackerCanBoat); // wybudzenie 1 hackera
    waitingWindowsers--; // aktualizacja zmiennej
    waitingLinHackers -= 2; // aktualizacja zmiennej
    // czekaj aż wsiądą do łódki
    boatAndRow(id); // + return
    printState();
  }
  else
  {
    waitingWindowsers++;
    printf("Windowser %d waiting!\n", id);
    printState();
    pthread_cond_wait(&windowserCanBoat, &Lock);
  }
  boatBoard(id);
  pthread_mutex_unlock(&Lock);
}

void printState()
{
  printf("waiting hackers: %d, waiting windowsers: %d\n", waitingLinHackers, waitingWindowsers);
}

void boatAndRow(int id)
{
  printf("%d: The boat has left the dock! ", id);
  // wsiądź i odepchnij łódkę
  // broadcast do czekającyh na przystani, że mogą się szykować
}

void boatBoard(int id)
{
  // weź lock
  // zwiększ liczbę w łódce
  // sprawdź czy == 3, jeśli tak, obudź odpychającego
  // zdejmij lock
  printf("%d is in!\n", id);
}
