#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define bool int
#define false 0
#define true 1

// zmienne poprawności
bool NoProgrammersDoubling = true;
bool EveryBoatCorrect = true;
bool CorrectBoating = true;
bool CorrectNumberOfBoats = true;
bool NoLoadsMore = true;

FILE * fin; // plik wejścia testowego
int * Programmers; // tablica do sprawdzenia czy programiści się nie powtarzają

void checkBoat(void);

int main()
{
  
  fin = fopen("output.txt", "r");
  
  int numberOfProgrammers;
  fscanf(fin, "%d", &numberOfProgrammers);
  
  Programmers = malloc(numberOfProgrammers * sizeof(int));
  for (int i = 0; i < numberOfProgrammers; i++)
    Programmers[i] = false;
    
  int numberOfBoatsReal = 0;
  
  // główna pętla wczytująca
  int boat;
  for (int i = 0; i < (numberOfProgrammers - 1) / 4; i++)
  {
    fscanf(fin, "%d", &boat);
    if (!boat) // łódka nie puszczona!
      CorrectBoating = false;
    else
    {
      numberOfBoatsReal++;
      checkBoat();
    }
  }
  
  // wczytanie przedostatniej linii
  // być może łódź nie mogła zabrać niedobitków
  int tempProgrammer;
  int hackers;
  fscanf(fin, "%d", &boat);
  int leftProgrammers;
  if (boat)
  {
    numberOfBoatsReal++;
    checkBoat();
  }
  else
  {
    leftProgrammers = numberOfProgrammers - 4 * numberOfBoatsReal;
    for (int i = 0; i < leftProgrammers; i++)
    {
      fscanf(fin, "%d", &tempProgrammer);
      if (Programmers[abs(tempProgrammer) - 1] == true)
        NoProgrammersDoubling = false; // zdublowany programista!
      if (tempProgrammer > 0)
        hackers++;
      Programmers[abs(tempProgrammer) - 1] = true;
    }
    if (leftProgrammers == 4 && (hackers == 2 || hackers == 4)) // nieprawidłowy ładunek!
      NoLoadsMore = false; // powinien być jeszcze jeden ładunek!
  }
  
  // sprawdzenie czy dobrze podsumowana ilość łódek
  int numberOfBoats;
  fscanf(fin, "%d", &numberOfBoats);
  if (numberOfBoats != numberOfBoatsReal)
    CorrectNumberOfBoats = false;
    
  // wypisanie wyników testu
  printf("%d%d%d%d%d\n", NoProgrammersDoubling, EveryBoatCorrect, CorrectBoating, CorrectNumberOfBoats, NoLoadsMore);
  // zamykanie
  free(Programmers);
  fclose(fin);
  return 0;
}

void checkBoat()
{
  int tempProgrammer;
  int hackers = 0; // zlicza hakerów w łódce
  for (int j = 0; j < 4; j++)
  {
    fscanf(fin, "%d", &tempProgrammer);
    if (Programmers[abs(tempProgrammer) - 1] == true)
      NoProgrammersDoubling = false; // zdublowany programista!
    if (tempProgrammer > 0)
      hackers++;
    Programmers[abs(tempProgrammer) - 1] = true;
  }
  if (hackers == 1 || hackers == 3) // nieprawidłowy ładunek!
    EveryBoatCorrect = false;
}
