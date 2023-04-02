// 
// 21070006210
// Metehan Ugus
// 
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

const char EMPTY_SYMBOL = ' '; // Define an empty symbol for the game area
const char HERO_SYMBOL = 'H'; // Define a symbol for the hero
const char MONSTER_SYMBOL = 'M'; // Define a symbol for the monster
const int STAMINA = 10; // Define the initial stamina of the hero
const int MONSTER_REWARD = 5; // Define the reward for killing a monster
const int FIRST_DIMENSION = 8; // Define the number of rows in the game area
const int SECOND_DIMENSION = 8; // Define the number of columns in the game area

struct Coordinate {
  int row; // The row of the coordinate
  int column; // The column of the coordinate
};

char gameArea[FIRST_DIMENSION][SECOND_DIMENSION]; // Define the game area

// Function to fill the game area with empty symbols
void fillGameArea() {
  for (int row = 0; row < FIRST_DIMENSION; ++row) {
    for (int column = 0; column < SECOND_DIMENSION; ++column) {
      gameArea[row][column] = EMPTY_SYMBOL;
    }
  }
}

// Function to print the game area
void printGameArea() {
  std::cout << "  ";
  for (int column = 0; column < SECOND_DIMENSION; ++column) {
    std::cout << " " << column << " ";
  }
  std::cout << std::endl;
  for (int row = 0; row < FIRST_DIMENSION; ++row) {
    std::cout << row << " ";
    for (int column = 0; column < SECOND_DIMENSION; ++column) {
      std::cout << "[" << gameArea[row][column] << "]";
    }
    std::cout << std::endl;
  }
}

// Function to check if a given coordinate is within the game area
bool isValidCoordinate(int row, int column) {
  return row >= 0 && row < FIRST_DIMENSION && column >= 0 && column < SECOND_DIMENSION;
}

// Function to check if a given position is a valid position for a monster
bool validMonsterPosition(int row, int column) {
  return isValidCoordinate(row, column) && gameArea[row][column] == EMPTY_SYMBOL &&
    isValidCoordinate(row - 1, column) && gameArea[row - 1][column] != MONSTER_SYMBOL &&
    isValidCoordinate(row + 1, column) && gameArea[row + 1][column] != MONSTER_SYMBOL &&
    isValidCoordinate(row, column - 1) && gameArea[row][column - 1] != MONSTER_SYMBOL &&
    isValidCoordinate(row, column + 1) && gameArea[row][column + 1] != MONSTER_SYMBOL &&
    isValidCoordinate(row - 1, column - 1) && gameArea[row - 1][column - 1] != MONSTER_SYMBOL &&
    isValidCoordinate(row - 1, column + 1) && gameArea[row - 1][column + 1] != MONSTER_SYMBOL &&
    isValidCoordinate(row + 1, column - 1) && gameArea[row + 1][column - 1] != MONSTER_SYMBOL &&
    isValidCoordinate(row + 1, column + 1) && gameArea[row + 1][column + 1] != MONSTER_SYMBOL;
}

// Function to randomly assign monsters to the game area
void assignMonsters() {
  srand(time(NULL)); // random generator
  int monstersPlaced = 0;
  //Place monsters until we have placed 5
  while (monstersPlaced < 5) {
    int row = rand() % FIRST_DIMENSION; //Generate a random row
    int column = rand() % SECOND_DIMENSION; // Generate a random column
    if (validMonsterPosition(row, column)) { // Check if the position is valid for a monster
      gameArea[row][column] = MONSTER_SYMBOL; // Place the monster on the game area
      ++monstersPlaced; // Increment the number of monsters placed
    }
  }
}

// Function to assign the hero to a random empty position in the game area
Coordinate * assignHero() {
  srand(time(NULL)); // random generator
  Coordinate * heroPosition = new Coordinate(); // Create a new Coordinate for the hero
  while (true) {
    int row = rand() % FIRST_DIMENSION; // Generate a random row
    int column = rand() % SECOND_DIMENSION; // Generate a random column
    if (gameArea[row][column] == EMPTY_SYMBOL) { // Check if the position is empty
      heroPosition -> row = row; //Set the row of the hero's position
      heroPosition -> column = column; // Set the column of the hero's position
      gameArea[row][column] = HERO_SYMBOL; // Place the hero on the game area
      return heroPosition;
    }
  }
}

//Finding the distance between two points. I used Manhattan Distance here.
int distance(int row1, int column1, int row2, int column2) {
  return std::abs(row1 - row2) + std::abs(column1 - column2);
}

// Function to find the nearest monster to a given position
Coordinate * nearestMonster(int row, int column) {
  Coordinate * nearestMonsterPosition = NULL; // Create a null pointer for the nearest 
  int nearestMonsterDistance = INT_MAX; // Set the nearest monster distance to the maximum possible integer value
  for (int r = 0; r < FIRST_DIMENSION; ++r) {
    for (int c = 0; c < SECOND_DIMENSION; ++c) {
      if (gameArea[r][c] == MONSTER_SYMBOL) { // Check if the current position is a monster
        int monsterDistance = distance(row, column, r, c); // Calculate the distance between the current position and the monster
        if (monsterDistance < nearestMonsterDistance) { // Check if the current monster is closer than the previous closest monster
          nearestMonsterDistance = monsterDistance; // Update the nearest monster distance
          nearestMonsterPosition = new Coordinate(); // Create a new Coordinate for the nearest monster position
          nearestMonsterPosition -> row = r; // Set the row of the nearest monster position
          nearestMonsterPosition -> column = c; // Set the column of the nearest monster position
        }
      }
    }
  }
  return nearestMonsterPosition;
}

// Function to move the hero from its current position to the next position
void moveHero(Coordinate * heroPosition, Coordinate * nextPosition) {
  gameArea[heroPosition -> row][heroPosition -> column] = EMPTY_SYMBOL; // Set the hero's current position to empty
  gameArea[nextPosition -> row][nextPosition -> column] = HERO_SYMBOL; // Place the hero on the next position
  heroPosition -> row = nextPosition -> row; // Update the row of the hero's position
  heroPosition -> column = nextPosition -> column; // Update the column of the hero's position
}

// Function to remove a monster from the game area
void slayMonster(Coordinate * heroPosition) {
  gameArea[heroPosition -> row][heroPosition -> column] = EMPTY_SYMBOL; // Set the monster's position to empty
}

// Function to check if the hero can move from its current position to the next position
bool canMoveTo(Coordinate * currentPosition, Coordinate * nextPosition, int & goldEarned) {
  if (gameArea[nextPosition -> row][nextPosition -> column] == MONSTER_SYMBOL) { // Check if the next position has a monster
    goldEarned += MONSTER_REWARD; // Increment the gold earned by the monster reward
    slayMonster(nextPosition); // Remove the monster from the game area
  }
  int newStamina = STAMINA - distance(currentPosition -> row, currentPosition -> column,
    nextPosition -> row, nextPosition -> column); // Calculate the new stamina after moving
  if (newStamina >= 0) { // Check if the hero has enough stamina to move
    moveHero(currentPosition, nextPosition); // Move the hero to the next position
    return true; // Return true to indicate that the hero has successfully moved
  } else {
    return false; // Return false to indicate that the hero cannot move to the next position due to lack of stamina
  }
}

// Function to print the best path found by the brute force algorithm
void printPath(std::vector < char > path) {
  std::cout << "\nBest path : ";
  for (int i = 0; i < path.size(); ++i) {
    std::cout << "-" << path[i];
  }
  std::cout << std::endl;
}

// Recursive function to perform brute force search for the best path
void bruteForce(Coordinate * heroPosition, int goldEarned, int remainingStamina,
  std::vector < char > path, std::vector < char > & bestPath, int & bestGoldEarned) {
  if (remainingStamina == 0) { // Check if the hero has run out of stamina
    if (goldEarned > bestGoldEarned) { // Check if the gold earned in this path is more than the previous best path
      bestGoldEarned = goldEarned; // Update the best gold earned
      bestPath = path; // Update the best path
      printPath(bestPath);  // Print the best path
    }
    return;
  }
  Coordinate * nearestMonsterPosition = nearestMonster(heroPosition -> row, heroPosition -> column); // Find the nearest monster to the hero's position
  if (nearestMonsterPosition == NULL) { // Check if there are no monsters left in the game area
    if (goldEarned > bestGoldEarned) { // Check if the gold earned in this path is more 
      bestGoldEarned = goldEarned; // Update the best gold earned
      bestPath = path; 
      printPath(bestPath);
    }
    return;
  }
  Coordinate * nextPosition = new Coordinate(); // Create a new Coordinate for the next position
  if (nearestMonsterPosition -> row < heroPosition -> row) { // Check if the nearest monster is above the hero
    nextPosition -> row = heroPosition -> row - 1; // Set the row of the next position to above the hero
    nextPosition -> column = heroPosition -> column; // Set the column of the next position to the same as the hero
    if (canMoveTo(heroPosition, nextPosition, goldEarned)) { // Check if the hero can move to the next position
      path.push_back('U'); // Add up 'U' to the path 
      bruteForce(heroPosition, goldEarned, remainingStamina - 1, path, bestPath, bestGoldEarned); // Recursive call to the bruteForce function with updated parameters
      path.pop_back(); // Remove the last direction from the path
      moveHero(heroPosition, nextPosition); // Move the hero back to its original position
	  goldEarned -= MONSTER_REWARD; // Deduct the monster reward from the gold earned
	  slayMonster(nextPosition); // Remove the monster from the game area
    }
  }
  if (nearestMonsterPosition->row > heroPosition->row) { // Check if the nearest monster is below the hero
    nextPosition->row = heroPosition->row + 1; // Set the row of the next position to below the hero
    nextPosition->column = heroPosition->column;
    if (canMoveTo(heroPosition, nextPosition, goldEarned)) { 
        path.push_back('D'); // Add 'D' (down) to the path
        bruteForce(heroPosition, goldEarned, remainingStamina - 1, path, bestPath, bestGoldEarned);
        path.pop_back();
        moveHero(heroPosition, nextPosition); 
        goldEarned -= MONSTER_REWARD; 
        slayMonster(nextPosition); 
    }
  }
if (nearestMonsterPosition->column < heroPosition->column) { // Check if the nearest monster is to the left of the hero
    nextPosition->row = heroPosition->row; // Set the row of the next position to the same as the hero
    nextPosition->column = heroPosition->column - 1; // Set the column of the next position to the left of the hero
    if (canMoveTo(heroPosition, nextPosition, goldEarned)) { 
        path.push_back('L'); // Add left 'L' to the path
        bruteForce(heroPosition, goldEarned, remainingStamina - 1, path, bestPath, bestGoldEarned); 
        path.pop_back(); 
        moveHero(heroPosition, nextPosition); 
        goldEarned -= MONSTER_REWARD; 
        slayMonster(nextPosition);
    }
  }
if (nearestMonsterPosition->column > heroPosition->column) { // Check if the nearest monster is to the right of the hero
    nextPosition->row = heroPosition->row; // Set the row of the next position to the same as the hero
    nextPosition->column = heroPosition->column + 1; // Set the column of the next position to the right of the hero
    if (canMoveTo(heroPosition, nextPosition, goldEarned)) {
      path.push_back('R'); // Add right 'R' to the path
      bruteForce(heroPosition, goldEarned, remainingStamina - 1, path, bestPath, bestGoldEarned);
      path.pop_back();
      moveHero(heroPosition, nextPosition);
      goldEarned -= MONSTER_REWARD;
      slayMonster(nextPosition);
    }
  }
}

int main() {
  fillGameArea(); // Fill the game area with empty symbols
  assignMonsters(); // Assign monsters to the game area
  Coordinate * heroPosition = assignHero(); // Assign the hero to the game area
  printGameArea(); // Print the game area
  std::vector < char > path; // Initialize a vector to store the path taken by the hero
  std::vector < char > bestPath; // Initialize a vector to store the best path taken by the hero
  int goldEarned = 0;
  int bestGoldEarned = 0;
  bruteForce(heroPosition, goldEarned, STAMINA, path, bestPath, bestGoldEarned);
  std::cout << "Maximum possible gold : " << bestGoldEarned << std::endl;
  std::cout << "\n21070006210 Metehan Ugus\n";
  std::cout << "SE2228 Lab Assignment 1\n";
  return 0;
}
