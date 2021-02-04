#include <LedControl.h>
//12= data pin(DIn), 11= CLK Pin, 10= Load/CS Pin, 1 = num of devices
LedControl lc88=LedControl(12,11,10,1);

const int x = 8;
bool cells[x][x];

void setup(){
  Serial.begin(57600);
  lc88.shutdown(0,false); // Wake up! 0= index of first device;
  lc88.setIntensity(0,2);
  lc88.clearDisplay(0);
  createRandomCells(20);
  printCells();
  delay(2000);
}

void loop(){
  //createRandomCells(5);
  updateCells();
  printCells();
  delay(1000); 
}

void updateMatrix() {
  for (int row = 0; row < x; row++) {
    for (int col = 0; col < x; col++) {
      lc88.setLed(0,row,col,cells[row][col]);
    }
  }
}

void updateCells() {
  bool newCells[x][x];
  for (int row = 0; row < x; row++) {
    for (int col = 0; col < x; col++) {
      newCells[row][col] = checkCell(row, col);
    }
  }
  for (int row = 0; row < x; row++) {
    for (int col = 0; col < x; col++) {
      if (newCells[row][col]) {
        lc88.setLed(0,row,col,true);
      }
    }
  }
  for (int row = 0; row < x; row++) {
    for (int col = 0; col < x; col++) {
      cells[row][col] = newCells[row][col];
    }
  }
  delay(250);
  updateMatrix();
}

bool checkCell(int row, int col) {
  int neighbours = getNeighbours(row, col);
  if (cells[row][col] == true && (neighbours == 2 || neighbours == 3)) {
    return true;
  } else if (cells[row][col] == false && neighbours == 3) {
    return true;
  } else {
    return false;
  }
}

int getNeighbours(int cellRow, int cellCol) {
  int neighbours = 0;
  for (int row = cellRow - 1; row <= cellRow + 1; row++) {
    for (int col = cellCol - 1; col <= cellCol + 1; col++) {
      bool test = cells[((row + x) % x)][((col + x) % x)];
      if (row == cellRow && col == cellCol) {
      } else if (cells[((row + x) % x)][((col + x) % x)]) {
        neighbours++;
      }
    }
  }
  return neighbours;
}

void createRandomCells(long spawnProb) {
  for (int row = 0; row < x; row++) {
    for (int col = 0; col < x; col++) {
      cells[row][col] = randomBool(20);
    }
  }
}

bool randomBool(int prob) {
  int randomNumber = getRandom(100);
  if (prob > randomNumber) {
    return true;
  } else {
    return false;
  }
}

long getRandom(long maxNumber) {
  long randomized = ((analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5)) 
  * (analogRead(A0) + analogRead(A1) + analogRead(A2) + analogRead(A3) + analogRead(A4) + analogRead(A5))) % maxNumber;
  return abs(randomized);
}

void printCells() {
  for (int row = 0; row < x; row++) {
    //char thisRow[10];
    for (int col = 0; col < x; col++) {
      Serial.print(cells[row][col]);
      delay(3);
    }
    Serial.print('\n');
  }
  Serial.print('\n');
}
