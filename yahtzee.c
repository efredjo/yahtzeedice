#include <stdio.h>
#include <stdlib.h>

#define NO_OF_FACES 5

// #define DEBUG

enum SCORETYPES { ACES, TWOS, THREES, FOURS, FIVES, SIXES, THREEOAK, FOUROAK, FULLHOUSE, SMSTRAIGHT, LGSTRAIGHT, YAHTZEE, CHANCE };

struct ScoreUpper {
  int aces; // all add value
  int twos;
  int threes;
  int fours;
  int fives;
  int sixes;
  int bonus35; // if above is 63 or more
};

struct ScoreLower {
  int threeoak; // add value
  int fouroak; //add value
  int fullhouse; // 25 
  int smstraight; // 30
  int lgstraight; // 40
  int yahtzee; // 50 on first
  int chance; // add value
  int yahtzeebonus[3]; // 100 on each additional
};
  
struct Score {
  struct ScoreUpper upper;
  struct ScoreLower lower;
};

typedef int Dices[NO_OF_FACES];

const char* DiceNames[NO_OF_FACES] = { "A", "B", "C", "D", "E" };

const char* facerows[5][6] = {
    { " --- ", " --- ", " --- ", " --- ", " --- ", " --- " },
    { "|   |", "|  o|", "|  o|", "|o o|", "|o o|", "|ooo|" },
    { "| o |", "|   |", "| o |", "|   |", "| o |", "|ooo|" },
    { "|   |", "|o  |", "|o  |", "|o o|", "|o o|", "|ooo|" },
    { " --- ", " --- ", " --- ", " --- ", " --- ", " --- " }, 
};

void
prettyPrintDices(Dices& dices) {
  for(int r=0;r<5;r++) {
    for(int i=0;i<NO_OF_FACES;i++) {
      printf("%s ", facerows[r][dices[i]-1]);
    }
    printf("\n");
  }
}

void printScore(Score& score) {
}

void
swap(int *num1, int *num2) {
  int temp;
  temp = *num1;
  *num1 = *num2;
  *num2 = temp;
}
 
void
rollDices(Dices& dices)
{
  int i;
  for(i=0;i<NO_OF_FACES;i++) {
    dices[i]=1+rand()%6;
  }
}

void
printDices(Dices& dices) {
  int i;
  for(i=0;i<NO_OF_FACES;i++) {
    printf("%s=%d ", DiceNames[i], dices[i]);
  }
#ifdef DEBUG
  printf("\n");
#else
  printf("  ->  ");
#endif
  
}

void
sortHighDices(Dices& dices) {
  int i, j=0;
  for(i=0; i<NO_OF_FACES; i++) {
    for(j=i+1; j<NO_OF_FACES; j++) {
      if(dices[i]<dices[j]) {
	swap(&dices[i], &dices[j]);
      }
    }
  }	
#ifdef DEBUG
  printDices(dices);
#endif
}

void
sortLowDices(Dices& dices) {
  int i, j=0;
  for(i=0; i<NO_OF_FACES; i++) {
    for(j=i+1; j<NO_OF_FACES; j++) {
      if(dices[i]>dices[j]) {
	swap(&dices[i], &dices[j]);
      }
    }
  }	
#ifdef DEBUG
  printDices(dices);
#endif
}

int
twoofakind(Dices& dices)
{
  int i=0;

  for(i=0;i<NO_OF_FACES-1;i++) {
    if(dices[i]==dices[i+1])
      if(dices[i]!=0) return dices[i];
  }

  return 0;
}

int
threeofakind(Dices& dices)
{
  int i=0;

  for(i=0;i<NO_OF_FACES-2;i++) {
    if((dices[i]==dices[i+1])&&(dices[i]==dices[i+2]))
      if(dices[i]!=0) return dices[i];
  }

  return 0;
}

int
fourofakind(Dices& dices)
{
  int i=0;

  for(i=0;i<NO_OF_FACES-3;i++) {
    if((dices[i]==dices[i+1])&&(dices[i]==dices[i+2])&&(dices[i]==dices[i+3]))
      if(dices[i]!=0) return dices[i];
  }

  return 0;
}

int
fullhouse(Dices& dices) {
  sortHighDices(dices);
  if(int three=threeofakind(dices)) {
    sortLowDices(dices);
    if(int two=twoofakind(dices)) {
      if(two!=three) return 1;
    }
  }
  return 0;
}

int
straight(Dices& dices) {
  int i;
  sortLowDices(dices);
  for(i=0;i<NO_OF_FACES-1;i++) {
    if(dices[i]+1!=dices[i+1]) {
      return 0;
    }
  }
  return dices[0];
}

void
CheckDices(Dices& dices) {
  sortHighDices(dices);
  
  if(int value=straight(dices)) {
    if(value==2) printf("STRAIGHT FLUSH starting at %d\n", value);
    else printf("STRAIGHT starting at %d\n", value);
  } else if(int value=fullhouse(dices)) {
    printf("FULL HOUSE\n");
  } else if(int value=fourofakind(dices)) {
    printf("Four of a kind of %d\n", value);
  } else if(int value=threeofakind(dices)) {
    printf("Three of a kind of %d\n", value);
  } else if(int value=twoofakind(dices)) {
    printf("Pair of %d\n", value);
  }
#ifdef DEBUG
  printf("-----------------------------\n");
#endif  
}

int
main(int argc, char** argv)
{
  Dices me;

  for(int i=0;i<255;i++) {      
    rollDices(me);
    prettyPrintDices(me);
    CheckDices(me);
  }
  
  return 0;  
}

