#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// c-owa imitacja boola
#define true 1
#define false 0

// kolory
#define HACKER_COLOR "\033[0;33m"
#define WINDOWSER_COLOR "\033[0;34m"
#define WARNING_COLOR "\033[0;31m"
#define BOAT_INFO_COLOR "\033[0;32m"
#define RESET_COLOR "\033[0m"
#define SUCCESS_COLOR "\033[1;32m"

// makro withState() zwraca hakera z plusem, windowsera z minusem
// jest pomocne przy zapisie do pliku testowego
#define withState(id, type) ((type) ? ((id) + 1) : ((-(id)) - 1))

// ustawienia
unsigned int PROGNUM = 100;
unsigned int SPEED = 9;
unsigned int TEST = false;

// zmienne wspódzielone
int waitingLinHackers, waitingWindowsers;
int boarding; // true, gdy łódka wodowana
int inBoat; // liczba programistów w łódce
int programmersArrived; // liczba przybyłych programistów na przystań
int noHope; // true, gdy nie da się zabrać pozostałych programistów
int boatsShipped; // liczba łódek, które przepłynęły

FILE * fout; // plik outputowy dla testów

// zmienne POSIX-owe
pthread_t * Programmers; // tablica wątków-programistów
pthread_mutex_t mainLock; // lock dla wszystkich dzielonych zmiennych
pthread_cond_t linHackerCanBoard;
pthread_cond_t windowserCanBoard;
pthread_cond_t boardingDone;
pthread_cond_t readyToRow;

// funkcje
void programmersRandomGenerator(int iter, int speed);
void * linHackerArrives(void *args);
void * windowserArrives(void *args);
void boardBoat(int id, int type); // wejście na łódkę
void boardAndRow(int id, int type); // wejście na łódkę i wiosłowanie
void failInfo(void); // funkcja pomocnicza

int main(int argc, char *argv[])
{
  srand(time(NULL));

  // wczytanie parametrów programu
  for (int i = 1; i < argc; i += 2)
  {
    if (!strcmp(argv[i], "-p"))
      PROGNUM = atoi(argv[i+1]);
    if (!strcmp(argv[i], "-s"))
      SPEED = atoi(argv[i+1]);
    if (!strcmp(argv[i], "-t"))
      TEST = atoi(argv[i+1]);
  }

  // normalizacja prędkości wykonania
  if (SPEED > 12)
    SPEED = 12;
  else if (SPEED < 0)
    SPEED = 0;

  Programmers = malloc(PROGNUM * sizeof(pthread_t));

  if (TEST) // otwarcie pliku do testów
  {
    fout = fopen("output.txt", "w");
    fprintf(fout, "%d\n", PROGNUM);
  }

  // info
  printf("%sPARAMS(speed => %d, programmers => %d, test => %s)%s\n",
        BOAT_INFO_COLOR, SPEED, PROGNUM, TEST ? "true" : "false", RESET_COLOR);

  // inicjowanie zmiennych dzielonych
  waitingLinHackers = 0;
  waitingWindowsers = 0;
  boarding = false;
  inBoat = 0;
  programmersArrived = 0;
  noHope = false;
  boatsShipped = 0;

  // inicjowanie zmiennych POSIX-owych
  pthread_mutex_init(&mainLock, NULL);
  pthread_cond_init(&linHackerCanBoard, NULL);
  pthread_cond_init(&windowserCanBoard, NULL);
  pthread_cond_init(&boardingDone, NULL);
  pthread_cond_init(&readyToRow, NULL);

  // startowanie wątków
  programmersRandomGenerator(PROGNUM, SPEED);

  // kończenie wątków
  int tret; // wartość zwracana przez różne pthread_funkcja()
  for (int i = 0; i < PROGNUM; i++)
    if (tret = pthread_join(Programmers[i], NULL))
    {
      fprintf(stderr, "Error from pthread_join(). Returned %i instead of 0.", tret); // wyświetlanie kodu błędu
      exit(EXIT_FAILURE);
    }
  if (noHope)
  {
    printf("\n");
    if (TEST) fprintf(fout, "\n");
  }
  else
  {
    printf("%s___________________________________________%s\n", SUCCESS_COLOR, RESET_COLOR);
    printf("%sFinished!\nAll programmers transported.\n%s", SUCCESS_COLOR, RESET_COLOR);
  }
  printf("%sSent %d boats.\n%s", SUCCESS_COLOR, boatsShipped, RESET_COLOR);
  if (TEST) fprintf(fout, "%d\n", boatsShipped);

  // destrukcja zmiennych POSIX-owych
  pthread_mutex_destroy(&mainLock);
  pthread_cond_destroy(&linHackerCanBoard);
  pthread_cond_destroy(&windowserCanBoard);
  pthread_cond_destroy(&boardingDone);
  pthread_cond_destroy(&readyToRow);

  if (TEST) // zamknięcie pliku
    fclose(fout);
  return 0;
}

void * linHackerArrives(void * args)
{
  int id = *((int *)args);
  free(args);
  int iWillRow = false;
  // sekcja krytyczna
  pthread_mutex_lock(&mainLock);
  while (boarding) // czekanie aż łódka będzie dostępna
    pthread_cond_wait(&boardingDone, &mainLock);
  programmersArrived++;
  printf("%sLinux hacker #%d arrives...%s\n", HACKER_COLOR, id, RESET_COLOR);
  if (waitingLinHackers == 3) // można wypłynąć
  {
    boarding = true;
    pthread_cond_signal(&linHackerCanBoard); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoard); // wybudzenie 1 hakera
    pthread_cond_signal(&linHackerCanBoard); // wybudzenie 1 hakera
    waitingLinHackers -= 3; // aktualizacja zmiennej
    iWillRow = true; // ostatni == ten wątek == będzie wiosłował
    if (TEST) fprintf(fout, "1"); // początek łodki w pliku testowym
    pthread_cond_wait(&readyToRow, &mainLock); // oczekiwanie aż wsiądą do łódki
  }
  else if (waitingLinHackers >= 1 && waitingWindowsers >= 2) // można wypłynąć
  {
    boarding = true;
    pthread_cond_signal(&linHackerCanBoard); // wybudzenie 1 hakera
    pthread_cond_signal(&windowserCanBoard); // wybudzenia 1 windowsera
    pthread_cond_signal(&windowserCanBoard); // wybudzenie 1 windowsera
    waitingLinHackers--; // aktualizacja zmiennej
    waitingWindowsers -= 2; // aktualizacja zmiennej
    iWillRow = true; // ostatni == ten wątek == będzie wiosłował
    if (TEST) fprintf(fout, "1"); // początek łodki w pliku testowym
    pthread_cond_wait(&readyToRow, &mainLock); // oczekiwanie aż wsiądą do łódki
  }
  else // uśpienie
  {
    waitingLinHackers++;
    // wypisywanie sytuacji "3 na 1"
    if (waitingLinHackers == 1 && waitingWindowsers == 3)
      printf("%sAwaiting 1 Linux hacker and 3 Windowsers. Can't load the boat!%s\n",
            WARNING_COLOR, RESET_COLOR);
    if (waitingLinHackers == 3 && waitingWindowsers == 1)
      printf("%sAwaiting 3 Linux hackers and 1 Windowser. Can't load the boat!%s\n",
            WARNING_COLOR, RESET_COLOR);
    if (programmersArrived == PROGNUM) // gdy ostatni, wybudzenie reszty
    {
      noHope = true; // zakomunikuj pozostałym, że już nie przepłyną
      failInfo();
      pthread_cond_broadcast(&linHackerCanBoard); // wybudzenie pozostałych
      pthread_cond_broadcast(&windowserCanBoard); // wybudzenie pozostałych
    }
    else
      pthread_cond_wait(&linHackerCanBoard, &mainLock); // oczekiwanie możliwość wejścia
  }
  if (noHope)
  {
    printf("%s#%d%s ", HACKER_COLOR, id, RESET_COLOR);
    if (TEST) fprintf(fout, " %d", withState(id, 1));
  }
  else if (iWillRow)
    boardAndRow(id, 1);
  else
    boardBoat(id, 1);
  pthread_mutex_unlock(&mainLock);
}

void * windowserArrives(void * args)
{
  int id = *((int *)args);
  free(args);
  int iWillRow = false;
  // sekcja krytyczna
  pthread_mutex_lock(&mainLock);
  while (boarding) // czekanie aż łódka będzie dostępna
    pthread_cond_wait(&boardingDone, &mainLock);
  programmersArrived++;
  printf("%sWindowser #%d arrives...%s\n", WINDOWSER_COLOR, id, RESET_COLOR);
  if (waitingWindowsers == 3) // można wypłynąć
  {
    boarding = true;
    pthread_cond_signal(&windowserCanBoard); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoard); // wybudzenie 1 windowsera
    pthread_cond_signal(&windowserCanBoard); // wybudzenie 1 windowsera
    waitingWindowsers -= 3; // aktualizacja zmiennej
    iWillRow = true; // ostatni == ten wątek == będzie wiosłował
    if (TEST) fprintf(fout, "1"); // początek łodki w pliku testowym
    pthread_cond_wait(&readyToRow, &mainLock); // oczekiwanie aż wsiądą do łódki
  }
  else if (waitingWindowsers >= 1 && waitingLinHackers >= 2) // można wypłynąć
  {
    boarding = true;
    pthread_cond_signal(&windowserCanBoard); // wybudzenie 1 windowsera
    pthread_cond_signal(&linHackerCanBoard); // wybudzenia 1 hackera
    pthread_cond_signal(&linHackerCanBoard); // wybudzenie 1 hackera
    waitingWindowsers--; // aktualizacja zmiennej
    waitingLinHackers -= 2; // aktualizacja zmiennej
    iWillRow = true; // ostatni == ten wątek == będzie wiosłował
    if (TEST) fprintf(fout, "1"); // początek łodki w pliku testowym
    pthread_cond_wait(&readyToRow, &mainLock); // oczekiwanie aż wsiądą do łódki
  }
  else // uśpienie
  {
    waitingWindowsers++;
    // wypisywanie sytuacji "3 na 1"
    if (waitingLinHackers == 1 && waitingWindowsers == 3)
      printf("%sAwaiting 1 Linux hacker and 3 Windowsers. Can't load the boat!%s\n",
            WARNING_COLOR, RESET_COLOR);
    if (waitingLinHackers == 3 && waitingWindowsers == 1)
      printf("%sAwaiting 3 Linux hackers and 1 Windowser. Can't load the boat!%s\n",
            WARNING_COLOR, RESET_COLOR);
    if (programmersArrived == PROGNUM) // gdy ostatni, wybudzenie reszty
    {
      noHope = true; // zakomunikuj pozostałym, że już nie przepłyną
      failInfo();
      pthread_cond_broadcast(&linHackerCanBoard); // wybudzenie pozostałych
      pthread_cond_broadcast(&windowserCanBoard); // wybudzenie pozostałych
    }
    else
      pthread_cond_wait(&windowserCanBoard, &mainLock); // oczekiwanie możliwość wejścia
  }
  if (noHope)
  {
    printf("%s#%d%s ", WINDOWSER_COLOR, id, RESET_COLOR);
    if (TEST) fprintf(fout, " %d", withState(id, 0));
  }
  else if (iWillRow)
    boardAndRow(id, 0);
  else
    boardBoat(id, 0);
  pthread_mutex_unlock(&mainLock);
}

void programmersRandomGenerator(int iter, int speed)
{
  int tret; // wartość zwracana przez różne pthread_funkcja()
  int random;
  int * id;
  for (int i = 0; i < iter; i++)
  {
    random = rand() % (int)pow(10, speed);
    while (random-- >= 1);
    id = malloc(sizeof(int));
    *id = i;
    // w zależności od losowania wątek hackera lub windowsiarza
    if (tret = pthread_create(&Programmers[i], NULL, rand() % 2 ? linHackerArrives : windowserArrives, (void *)id))
    {
      fprintf(stderr,"Error from pthread_create(). Returned %i instead of 0.", tret); // wyświetlanie kodu błędu
      exit(EXIT_FAILURE);
    }
  }
}

void boardAndRow(int id, int type)
{
  boarding = false;
  inBoat = 0;
  boatsShipped++;
  printf("%s %d%s  /\n", type ? HACKER_COLOR : WINDOWSER_COLOR , id, RESET_COLOR);
  printf("%sAwaiting hackers: %d, awaiting windowsers: %d%s\n",
          BOAT_INFO_COLOR, waitingLinHackers, waitingWindowsers, RESET_COLOR);
  if (TEST) fprintf(fout, " %d\n", withState(id, type));
  pthread_cond_broadcast(&boardingDone); // rozpoczęcie nowych naborów do łódki
  if (programmersArrived == PROGNUM && waitingLinHackers + waitingWindowsers > 0) // ostatni kurs
  {
    noHope = true; // zakomunikuj pozostałym, że już nie przepłyną
    failInfo();
    pthread_cond_broadcast(&linHackerCanBoard); // wybudzenie pozostałych
    pthread_cond_broadcast(&windowserCanBoard); // wybudzenie pozostałych
  }
}

void boardBoat(int id, int type)
{
  if (inBoat == 0)
    printf("%sBoat starts: \t%s \\ ", BOAT_INFO_COLOR, RESET_COLOR);
  inBoat++;
  if (inBoat == 3)
    pthread_cond_signal(&readyToRow); // ostatni może zacząć wiosłować
  printf("%s %d %s | ", type ? HACKER_COLOR : WINDOWSER_COLOR, id, RESET_COLOR);
  if (TEST) fprintf(fout, " %d", withState(id, type));
}

void failInfo()
{
  printf("%s___________________________________________%s\n", SUCCESS_COLOR, RESET_COLOR);
  printf("%sFinished!\n%d programmers left on shore: %s", SUCCESS_COLOR, (PROGNUM - 1) % 4 + 1, RESET_COLOR);
  if (TEST) fprintf(fout, "0 ");
}
