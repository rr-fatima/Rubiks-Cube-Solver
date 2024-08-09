#include <stdbool.h>
#include <stdio.h>

#define SIZE 3

// defines data structure of a rubiks cube face
typedef struct {
  char grid[SIZE][SIZE];
} Face;

// define data structure of a whole rubiks cube consisting of 6 faces
typedef struct {
  Face faces[6];  // 0: Down (D), 1: Top (T), 2: Front (F), 3: Left(L), 4: Back
                  // (B), 5: Right (R)
} RubiksCube;

// intializing functions for the rubik's cube movements
void rotate_down_clockwise(RubiksCube *cube);
void rotate_down_anticlockwise(RubiksCube *cube);
void rotate_top_clockwise(RubiksCube *cube);
void rotate_top_anticlockwise(RubiksCube *cube);
void rotate_front_clockwise(RubiksCube *cube);
void rotate_front_anticlockwise(RubiksCube *cube);
void rotate_left_clockwise(RubiksCube *cube);
void rotate_left_anticlockwise(RubiksCube *cube);
void rotate_back_clockwise(RubiksCube *cube);
void rotate_back_anticlockwise(RubiksCube *cube);
void rotate_right_clockwise(RubiksCube *cube);
void rotate_right_anticlockwise(RubiksCube *cube);

// initializing functions for solving the shuffled cube

void moveEdgeFromYellowToWhite(RubiksCube *cube, int faceNumber);
void fixWhiteEdge(RubiksCube *cube, int face, int row, int col, bool firstStep);
bool checkColorEdge(RubiksCube *cube, int face, char color);
void solveWhiteCross(RubiksCube *cube);

void fixWhiteCorner(RubiksCube *cube, int cornerNum, char colorA, char colorB);
void solveWhiteCorners(RubiksCube *cube);

int findColoredEdgeTop(RubiksCube *cube);
int allignEdgeToCenter(RubiksCube *cube, int faceNumber);
void fixColoredEdge(RubiksCube *cube, int faceNumber);
void solveColoredEdges(RubiksCube *cube);

int yellowEdgesFound(RubiksCube *cube);
void allignTop(RubiksCube *cube, int caseNumber);
void algorithmYellowCross(RubiksCube *cube);
void solveYellowCross(RubiksCube *cube);

void allignRed(RubiksCube *cube);
void solveYellowColorEdges(RubiksCube *cube);
void allignForAlgorithm(RubiksCube *cube);
void algorithmYellowColorEdges(RubiksCube *cube);

void algorithmAllignYellowCorners(RubiksCube *cube, int caseNumber);
int findCorrectCorners(RubiksCube *cube);
void solveAllignYellowCorners(RubiksCube *cube);

bool checkFullySolvedCorner(RubiksCube *cube);
void algorithmOrientationYellowCorner(RubiksCube *cube);
void solveOrientationYellowCorners(RubiksCube *cube);

// function for initializing the cube
void initialize_cube(RubiksCube *cube) {
  char colors[6] = {'W', 'Y', 'R', 'B', 'O', 'G'};
  for (int face = 0; face < 6; face++) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        cube->faces[face].grid[i][j] = colors[face];
      }
    }
  }
}

// prints the rubik's cube
void print_cube(const RubiksCube *cube) {
  printf("    The cube map looks like: \n");

  // for yellow centre face
  for (int row = 0; row < 3; row++) {
    printf("\n                  ");
    for (int col = 0; col < 3; col++) {
      printf("%c ", cube->faces[1].grid[row][col]);
    }
  }

  printf("\n");

  // for orange, blue, red and green centre faces
  for (int row = 0; row < 3; row++) {
    printf("\n");
    printf("    %c %c %c  ", cube->faces[4].grid[row][0],
           cube->faces[4].grid[row][1], cube->faces[4].grid[row][2]);
    printf("%c %c %c  ", cube->faces[3].grid[row][0],
           cube->faces[3].grid[row][1], cube->faces[3].grid[row][2]);
    printf("%c %c %c  ", cube->faces[2].grid[row][0],
           cube->faces[2].grid[row][1], cube->faces[2].grid[row][2]);
    printf("%c %c %c  ", cube->faces[5].grid[row][0],
           cube->faces[5].grid[row][1], cube->faces[5].grid[row][2]);
  }

  printf("\n");

  // for white centre face
  for (int row = 0; row < 3; row++) {
    printf("\n                  ");
    for (int col = 0; col < 3; col++) {
      printf("%c ", cube->faces[0].grid[row][col]);
    }
  }

  printf("\n\n    The seperate faces looks like the following: \n\n");

  for (int face = 0; face < 6; face++) {
    if (face == 0) {
      printf("    Down ");
    } else if (face == 1) {
      printf("    Top ");
    } else if (face == 2) {
      printf("    Front ");
    } else if (face == 3) {
      printf("    Left ");
    } else if (face == 4) {
      printf("    Back ");
    } else {
      printf("    Right ");
    }

    printf("face:\n");

    for (int i = 0; i < SIZE; i++) {
      printf("    ");
      for (int j = 0; j < SIZE; j++) {
        printf("%c ", cube->faces[face].grid[i][j]);
      }
      printf("\n");
    }

    printf("\n");
  }
}

// it rotates the face clockwise
void rotate_face_clockwise(Face *face) {
  // makes a 2D array having the copy of the rotated face array
  char temp[SIZE][SIZE];

  // fill the temp array with the rotated face values
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      temp[j][SIZE - 1 - i] = face->grid[i][j];
    }
  }

  // copy the temp array face values to the original array values
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      face->grid[i][j] = temp[i][j];
    }
  }
}

// it rotates the face clockwise
void rotate_face_anticlockwise(Face *face) {
  // makes a 2D array having the copy of the rotated face array
  char temp[SIZE][SIZE];

  // fill the temp array with the rotated face values
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      temp[SIZE - 1 - j][i] = face->grid[i][j];
    }
  }

  // copy the temp array face values to the original array values
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      face->grid[i][j] = temp[i][j];
    }
  }
}

// takes the agorithm used for shuffling the cube
void enterShuffleAlgorthm(RubiksCube *cube) {
  int choice = 0;
  printf(
      "\n\nWhile shuffling the cube please hold it such that the red face of "
      "the "
      "cube is in front of you. The white faces is facing the floor and the "
      "yellow face is facing the roof. Please use the following convention to "
      "enter the algorithm of the movements used to shuffle the cube.\n");
  printf("0: End the shuffle. \n");
  printf("1: Turn the right face 90 degrees clockwise. (R) \n");
  printf("2: Turn the right face 90 degrees counterclockwise. (R') \n");
  printf("3: Turn the left face 90 degrees clockwise. (L) \n");
  printf("4: Turn the left face 90 degrees counterclockwise. (L') \n");
  printf("5: Turn the top face 90 degrees clockwise. (U)\n");
  printf("6: Turn the top face 90 degrees counterclockwise. (U')\n");
  printf("7: Turn the bottom face 90 degrees clockwise. (D)\n");
  printf("8: Turn the bottom face 90 degrees counterclockwise. (D')\n");
  printf("9: Turn the front face 90 degrees clockwise. (F)\n");
  printf("10: Turn the front face 90 degrees counterclockwise. (F')\n");
  printf("11: Turn the back face 90 degrees clockwise. (B)\n");
  printf("12: Turn the back face 90 degrees counterclockwise. (B')\n\n");

  do {
    printf("Enter the algorithm: ");
    scanf("%d", &choice);

    if (choice < 0 || choice > 12) {
      printf("Wrong input! \n");
    } else if (choice == 1) {
      rotate_right_clockwise(cube);
    } else if (choice == 2) {
      rotate_right_anticlockwise(cube);
    } else if (choice == 3) {
      rotate_left_clockwise(cube);
    } else if (choice == 4) {
      rotate_left_anticlockwise(cube);
    } else if (choice == 5) {
      rotate_top_clockwise(cube);
    } else if (choice == 6) {
      rotate_top_anticlockwise(cube);
    } else if (choice == 7) {
      rotate_down_clockwise(cube);
    } else if (choice == 8) {
      rotate_down_anticlockwise(cube);
    } else if (choice == 9) {
      rotate_front_clockwise(cube);
    } else if (choice == 10) {
      rotate_front_anticlockwise(cube);
    } else if (choice == 11) {
      rotate_back_clockwise(cube);
    } else if (choice == 12) {
      rotate_back_anticlockwise(cube);
    }

  } while (choice != 0);
}

// helps in taking the input of the colors
void enterFace(RubiksCube *cube, int faceNumber) {
  char color = '-';
  int number = 1;

  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      if (faceNumber == 0) {
        printf("W%d: ", number);
      } else if (faceNumber == 1) {
        printf("Y%d: ", number);
      } else if (faceNumber == 2) {
        printf("R%d: ", number);
      } else if (faceNumber == 3) {
        printf("B%d: ", number);
      } else if (faceNumber == 4) {
        printf("O%d: ", number);
      } else if (faceNumber == 5) {
        printf("G%d: ", number);
      }

      scanf(" %c", &color);
      cube->faces[faceNumber].grid[row][col] = color;
      number++;
    }
  }
}

// takes the input of the colors of the shuffled cube
void enterShuffleCube(RubiksCube *cube) {
  printf(
      "\nHold the cube such that the red face of the cube is in front of you. "
      "The white faces is facing the floor and the yellow face is facing the "
      "roof. Use the following map when inoutting the colors.");
  printf("\n                    Y1 Y2 Y3 ");
  printf("\n                    Y4 Y5 Y6 ");
  printf("\n                    Y7 Y8 Y9 ");
  printf("\n                                ");
  printf("\nO1 O2 O3  B1 B2 B3  R1 R2 R3  G1 G2 G3");
  printf("\nO4 O5 O6  B4 B5 B6  R4 R5 R6  G4 G5 G6");
  printf("\nO7 O8 O9  B7 B8 B9  R7 R8 R9  G7 G8 G9");
  printf("\n                                ");
  printf("\n                    W1 W2 W3 ");
  printf("\n                    W4 W5 W6 ");
  printf("\n                    W7 W8 W9 \n");
  printf(
      "Use the following convention when inputting the colors.\nYellow color - "
      "Y \nWhite color - W\nRed color - R\nBlue color - B\nOrange color - "
      "O\nGreen color - G\n");
  printf("\nEnter the colors for the yellow centre face. \n");
  enterFace(cube, 1);
  printf("\nEnter the colors for the white centre face. \n");
  enterFace(cube, 0);
  printf("\nEnter the colors for the red centre face. \n");
  enterFace(cube, 2);
  printf("\nEnter the colors for the blue centre face. \n");
  enterFace(cube, 3);
  printf("\nEnter the colors for the orange centre face. \n");
  enterFace(cube, 4);
  printf("\nEnter the colors for the green centre face. \n");
  enterFace(cube, 5);
} 

int main() {
  int choice = 0;
  RubiksCube cube;
  initialize_cube(&cube);

  printf("To input the shuffled cube either one of the following options: \n");
  printf("1: Enter the colors of the shuffled cube\n");
  printf("2: Enter the algorithm of the movements used to shuffle the cube \n");
  printf("Enter the choice: ");
  scanf("%d", &choice);

  while (choice < 1 || choice > 2) {
    printf("Wrong Input! \n");
    printf("Enter the choice: ");
    scanf("%d", &choice);
  }

  if (choice == 1) {
    enterShuffleCube(&cube);
  } else {
    enterShuffleAlgorthm(&cube);
  }

  printf("\nTHE SHUFFLED CUBE LOOKS LIKE THE FOLLOWING! \n\n");
  print_cube(&cube);

  printf(
      "The solution provided by the program is in the form of algorithm. "
      "Algorithms are a finite sequence of well defined instructions used to "
      "convey the sequence of turns of a cube through writing. For more "
      "information on 3*3 Rubiks cube algorithms, please visit my website: "
      "https://cubemaster262075683.wordpress.com/algorithms/ \n\n");

  printf("TO SOLVE THE WHITE CROSS MAKE THE FOLLOWING MOVES: \n\n");

  solveWhiteCross(&cube);

  printf("\n\nTHE SOLVED WHITE CROSS LOOKS LIKE THE FOLLOWING! \n\n");
  print_cube(&cube);

  printf("\nTO SOLVE THE WHITE CORNERS MAKE THE FOLLOWING MOVES: \n\n");
  solveWhiteCorners(&cube);

  printf("\n\nTHE SOLVED WHITE CORNERS LOOKS LIKE THE FOLLOWING! \n\n");
  print_cube(&cube);

  printf("\nTO SOLVE THE COLORED EDGES MAKE THE FOLLOWING MOVES: \n\n");
  solveColoredEdges(&cube);

  printf("\n\nTHE SOLVED COLORED EDGES LOOKS LIKE THE FOLLOWING! \n\n");
  print_cube(&cube);

  printf("\nTO SOLVE THE YELLOW CROSS MAKE THE FOLLOWING MOVES: \n\n");
  solveYellowCross(&cube);

  printf("\n\nTHE SOLVED YELLOW CROSS LOOKS LIKE THE FOLLOWING! \n\n");
  print_cube(&cube);

  printf("\nTO SOLVE THE YELLOW EDGES MAKE THE FOLLOWING MOVES: \n\n");
  solveYellowColorEdges(&cube);

  printf("\n\nTHE SOLVED YELLOW EDGES LOOKS LIKE THE FOLLOWING! \n\n");
  print_cube(&cube);

  printf(
      "\nTO SOLVE THE YELLOW CORNER IN THEIR RIGHT POSITIONS MAKE THE "
      "FOLLOWING MOVES: \n\n");
  solveAllignYellowCorners(&cube);

  printf(
      "\n\nTHE SOLVED YELLOW CORNERS IN THEIR RIGHT POSITIONS LOOKS LIKE THE "
      "FOLLOWING! \n\n");
  print_cube(&cube);

  printf(
      "\nTO SOLVE THE YELLOW CORNER IN THEIR RIGHT ORIENTATION MAKE THE "
      "FOLLOWING MOVES: \n\n");
  solveOrientationYellowCorners(&cube);

  printf(
      "\n\nTHE SOLVED YELLOW CORNERS IN THEIR RIGHT ORIENTATION LOOKS LIKE THE "
      "FOLLOWING! \n\n");
  print_cube(&cube);

  return 0;
}

/*
declaration of the functiions for the rubiks cube movements
*/

void rotate_down_clockwise(RubiksCube *cube) {
  // Rotate the down face
  rotate_face_clockwise(&cube->faces[0]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[2].grid[SIZE - 1][i];

  // Left -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[2].grid[SIZE - 1][i] = cube->faces[3].grid[SIZE - 1][i];
  }

  // Bottom -> Left
  for (int i = 0; i < SIZE; i++) {
    cube->faces[3].grid[SIZE - 1][i] = cube->faces[4].grid[SIZE - 1][i];
  }

  // Right -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[4].grid[SIZE - 1][i] = cube->faces[5].grid[SIZE - 1][i];
  }

  // Temp -> Right
  for (int i = 0; i < SIZE; i++) cube->faces[5].grid[SIZE - 1][i] = temp[i];
}

void rotate_down_anticlockwise(RubiksCube *cube) {
  // Rotate the down face
  rotate_face_anticlockwise(&cube->faces[0]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[2].grid[SIZE - 1][i];

  // Right -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[2].grid[SIZE - 1][i] = cube->faces[5].grid[SIZE - 1][i];
  }

  // Bottom -> Right
  for (int i = 0; i < SIZE; i++) {
    cube->faces[5].grid[SIZE - 1][i] = cube->faces[4].grid[SIZE - 1][i];
  }

  // Left -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[4].grid[SIZE - 1][i] = cube->faces[3].grid[SIZE - 1][i];
  }

  // Temp -> Left
  for (int i = 0; i < SIZE; i++) cube->faces[3].grid[SIZE - 1][i] = temp[i];
}

void rotate_top_clockwise(RubiksCube *cube) {
  // Rotate the down face
  rotate_face_clockwise(&cube->faces[1]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[4].grid[0][i];

  // Left -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[4].grid[0][i] = cube->faces[3].grid[0][i];
  }

  // Bottom -> Left
  for (int i = 0; i < SIZE; i++) {
    cube->faces[3].grid[0][i] = cube->faces[2].grid[0][i];
  }

  // Right -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[2].grid[0][i] = cube->faces[5].grid[0][i];
  }

  // Temp -> Right
  for (int i = 0; i < SIZE; i++) cube->faces[5].grid[0][i] = temp[i];
}

void rotate_top_anticlockwise(RubiksCube *cube) {
  // Rotate the down face
  rotate_face_anticlockwise(&cube->faces[1]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[4].grid[0][i];

  // Right -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[4].grid[0][i] = cube->faces[5].grid[0][i];
  }

  // Bottom -> Right
  for (int i = 0; i < SIZE; i++) {
    cube->faces[5].grid[0][i] = cube->faces[2].grid[0][i];
  }

  // Left -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[2].grid[0][i] = cube->faces[3].grid[0][i];
  }

  // Temp -> Left
  for (int i = 0; i < SIZE; i++) cube->faces[3].grid[0][i] = temp[i];
}

void rotate_front_clockwise(RubiksCube *cube) {
  // Rotate the front face
  rotate_face_clockwise(&cube->faces[2]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[SIZE - 1][i];

  // Left -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[SIZE - 1][i] =
        cube->faces[3].grid[SIZE - 1 - i][SIZE - 1];
  }

  // Bottom -> Left
  for (int i = 0; i < SIZE; i++) {
    cube->faces[3].grid[i][SIZE - 1] = cube->faces[0].grid[0][i];
  }

  // Right -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[0][SIZE - 1 - i] = cube->faces[5].grid[i][0];
  }

  // Temp -> Right
  for (int i = 0; i < SIZE; i++) cube->faces[5].grid[i][0] = temp[i];
}

void rotate_front_anticlockwise(RubiksCube *cube) {
  // Rotate the front face
  rotate_face_anticlockwise(&cube->faces[2]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[SIZE - 1][i];

  // Right -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[SIZE - 1][i] = cube->faces[5].grid[i][0];
  }

  // Bottom -> Right
  for (int i = 0; i < SIZE; i++) {
    cube->faces[5].grid[i][0] = cube->faces[0].grid[0][SIZE - 1 - i];
  }

  // Left -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[0][i] = cube->faces[3].grid[i][SIZE - 1];
  }

  // Temp -> Left
  for (int i = 0; i < SIZE; i++)
    cube->faces[3].grid[SIZE - 1 - i][SIZE - 1] = temp[i];
}

void rotate_left_clockwise(RubiksCube *cube) {
  // Rotate the left face
  rotate_face_clockwise(&cube->faces[3]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[i][0];

  // Left -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[i][0] = cube->faces[4].grid[SIZE - 1 - i][SIZE - 1];
  }

  // Bottom -> Left
  for (int i = 0; i < SIZE; i++) {
    cube->faces[4].grid[i][SIZE - 1] = cube->faces[0].grid[SIZE - 1 - i][0];
  }

  // Right -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[i][0] = cube->faces[2].grid[i][0];
  }

  // Temp -> Right
  for (int i = 0; i < SIZE; i++) cube->faces[2].grid[i][0] = temp[i];
}

void rotate_left_anticlockwise(RubiksCube *cube) {
  // Rotate the left face
  rotate_face_anticlockwise(&cube->faces[3]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[i][0];

  // Right -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[i][0] = cube->faces[2].grid[i][0];
  }

  // Bottom -> Right
  for (int i = 0; i < SIZE; i++) {
    cube->faces[2].grid[i][0] = cube->faces[0].grid[i][0];
  }

  // Left -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[i][0] = cube->faces[4].grid[SIZE - 1 - i][SIZE - 1];
  }

  // Temp -> Left
  for (int i = 0; i < SIZE; i++)
    cube->faces[4].grid[SIZE - 1 - i][SIZE - 1] = temp[i];
}

void rotate_back_clockwise(RubiksCube *cube) {
  // Rotate the back face
  rotate_face_clockwise(&cube->faces[4]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[0][i];

  // Left -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[0][i] = cube->faces[5].grid[i][SIZE - 1];
  }

  // Bottom -> Left
  for (int i = 0; i < SIZE; i++) {
    cube->faces[5].grid[i][SIZE - 1] =
        cube->faces[0].grid[SIZE - 1][SIZE - 1 - i];
  }

  // Right -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[SIZE - 1][i] = cube->faces[3].grid[i][0];
  }

  // Temp -> Right
  for (int i = 0; i < SIZE; i++) cube->faces[3].grid[SIZE - 1 - i][0] = temp[i];
}

void rotate_back_anticlockwise(RubiksCube *cube) {
  // Rotate the back face
  rotate_face_anticlockwise(&cube->faces[4]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[0][i];

  // Right -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[0][i] = cube->faces[3].grid[SIZE - 1 - i][0];
  }

  // Bottom -> Right
  for (int i = 0; i < SIZE; i++) {
    cube->faces[3].grid[i][0] = cube->faces[0].grid[SIZE - 1][i];
  }

  // Left -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[SIZE - 1][i] =
        cube->faces[5].grid[SIZE - 1 - i][SIZE - 1];
  }

  // Temp -> Left
  for (int i = 0; i < SIZE; i++) cube->faces[5].grid[i][SIZE - 1] = temp[i];
}

void rotate_right_clockwise(RubiksCube *cube) {
  // Rotate the right face
  rotate_face_clockwise(&cube->faces[5]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[i][SIZE - 1];

  // Left -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[i][SIZE - 1] = cube->faces[2].grid[i][SIZE - 1];
  }

  // Bottom -> Left
  for (int i = 0; i < SIZE; i++) {
    cube->faces[2].grid[i][SIZE - 1] = cube->faces[0].grid[i][SIZE - 1];
  }

  // Right -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[i][SIZE - 1] = cube->faces[4].grid[SIZE - 1 - i][0];
  }

  // Temp -> Right
  for (int i = 0; i < SIZE; i++) cube->faces[4].grid[SIZE - 1 - i][0] = temp[i];
}

void rotate_right_anticlockwise(RubiksCube *cube) {
  // Rotate the right face
  rotate_face_anticlockwise(&cube->faces[5]);

  // Temporarily store the edges that will be affected
  char temp[SIZE];

  // Top -> temp
  for (int i = 0; i < SIZE; i++) temp[i] = cube->faces[1].grid[i][SIZE - 1];

  // Right -> Top
  for (int i = 0; i < SIZE; i++) {
    cube->faces[1].grid[i][SIZE - 1] = cube->faces[4].grid[SIZE - 1 - i][0];
  }

  // Bottom -> Right
  for (int i = 0; i < SIZE; i++) {
    cube->faces[4].grid[i][0] = cube->faces[0].grid[SIZE - 1 - i][SIZE - 1];
  }

  // Left -> Bottom
  for (int i = 0; i < SIZE; i++) {
    cube->faces[0].grid[i][SIZE - 1] = cube->faces[2].grid[i][SIZE - 1];
  }

  // Temp -> Left
  for (int i = 0; i < SIZE; i++) cube->faces[2].grid[i][SIZE - 1] = temp[i];
}

/*
declaration of the functiions for the solving the shuffled cube
*/

// helps the fixWhiteFunction in bringing the white edges to the white face AKA
// bottom face
void moveEdgeFromYellowToWhite(RubiksCube *cube, int faceNumber) {
  if (cube->faces[faceNumber].grid[0][1] == 'B') {
    // for alligning
    if (faceNumber == 2) {
      rotate_top_clockwise(cube);
      printf("U ");
    } else if (faceNumber == 4) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
    } else if (faceNumber == 5) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
    }

    // for rotating
    rotate_left_clockwise(cube);
    rotate_left_clockwise(cube);
    printf("L2 ");

  } else if (cube->faces[faceNumber].grid[0][1] == 'G') {
    // for alligning
    if (faceNumber == 2) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
    } else if (faceNumber == 3) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
    } else if (faceNumber == 4) {
      rotate_top_clockwise(cube);
      printf("U ");
    }

    // for rotating
    rotate_right_clockwise(cube);
    rotate_right_clockwise(cube);
    printf("R2 ");

  } else if (cube->faces[faceNumber].grid[0][1] == 'O') {
    // for alligning
    if (faceNumber == 2) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
    } else if (faceNumber == 3) {
      rotate_top_clockwise(cube);
      printf("U ");
    } else if (faceNumber == 5) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
    }

    // for rotating
    rotate_back_clockwise(cube);
    rotate_back_clockwise(cube);
    printf("B2 ");

  } else if (cube->faces[faceNumber].grid[0][1] == 'R') {
    // for alligning
    if (faceNumber == 3) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
    } else if (faceNumber == 4) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
    } else if (faceNumber == 5) {
      rotate_top_clockwise(cube);
      printf("U ");
    }

    // for rotating
    rotate_front_clockwise(cube);
    rotate_front_clockwise(cube);
    printf("F2 ");
  }
}

// helps the main function of solveWhiteCross by fixing the white edges
void fixWhiteEdge(RubiksCube *cube, int face, int row, int col,
                  bool firstStep) {
  if (firstStep == true) {  // not on yellow face
    if (face == 2) {        // red face
      if (row == 1 && col == 0) {
        rotate_left_anticlockwise(cube);  // L'
        rotate_top_clockwise(cube);       // U
        rotate_left_clockwise(cube);      // L
        printf("L' U L ");
      } else if (row == 0 && col == 1) {
        rotate_front_clockwise(cube);      // F
        rotate_right_clockwise(cube);      // R
        rotate_top_anticlockwise(cube);    // U'
        rotate_right_anticlockwise(cube);  // R'
        rotate_front_anticlockwise(cube);  // F'
        printf("F R U' R' F' ");
      } else if (row == 1 && col == 2) {
        rotate_right_clockwise(cube);      // R
        rotate_top_clockwise(cube);        // U
        rotate_right_anticlockwise(cube);  // R'
        printf("R U R' ");
      } else if (row == 2 && col == 1) {
        rotate_front_clockwise(cube);      // F
        rotate_left_anticlockwise(cube);   // L'
        rotate_top_clockwise(cube);        // U
        rotate_left_clockwise(cube);       // L
        rotate_front_anticlockwise(cube);  // F'
        printf("F L' U L F' ");
      }
    } else if (face == 3) {  // blue face
      if (row == 1 && col == 0) {
        rotate_back_anticlockwise(cube);  // B'
        rotate_top_clockwise(cube);       // U
        rotate_back_clockwise(cube);      // B
        printf("B' U B ");
      } else if (row == 0 && col == 1) {
        rotate_left_clockwise(cube);       // L
        rotate_front_clockwise(cube);      // F
        rotate_top_anticlockwise(cube);    // U'
        rotate_front_anticlockwise(cube);  // F'
        rotate_left_anticlockwise(cube);   // L'
        printf("L F U' F' L' ");
      } else if (row == 1 && col == 2) {
        rotate_front_clockwise(cube);      // F
        rotate_top_clockwise(cube);        // U
        rotate_right_anticlockwise(cube);  // F'
        printf("F U F' ");
      } else if (row == 2 && col == 1) {
        rotate_left_anticlockwise(cube);   // L'
        rotate_front_clockwise(cube);      // F
        rotate_top_anticlockwise(cube);    // U'
        rotate_front_anticlockwise(cube);  // F'
        rotate_left_clockwise(cube);       // L
        printf("L' F U' F' L ");
      }
    } else if (face == 4) {  // orange face
      if (row == 1 && col == 0) {
        rotate_right_anticlockwise(cube);  // R'
        rotate_top_clockwise(cube);        // U
        rotate_right_clockwise(cube);      // R
        printf("R' U R ");
      } else if (row == 0 && col == 1) {
        rotate_back_clockwise(cube);      // B
        rotate_left_clockwise(cube);      // L
        rotate_top_anticlockwise(cube);   // U'
        rotate_left_anticlockwise(cube);  // L'
        rotate_back_anticlockwise(cube);  // B'
        printf("B L U' L' B' ");
      } else if (row == 1 && col == 2) {
        rotate_left_clockwise(cube);      // L
        rotate_top_clockwise(cube);       // U
        rotate_left_anticlockwise(cube);  // L'
        printf("L U L' ");
      } else if (row == 2 && col == 1) {
        rotate_back_anticlockwise(cube);  // B'
        rotate_left_clockwise(cube);      // L
        rotate_top_anticlockwise(cube);   // U'
        rotate_left_anticlockwise(cube);  // L'
        rotate_back_clockwise(cube);      // B
        printf("B' L U' L' B ");
      }
    } else if (face == 5) {  // green face
      if (row == 1 && col == 0) {
        rotate_front_anticlockwise(cube);  // F'
        rotate_top_clockwise(cube);        // U
        rotate_front_clockwise(cube);      // F
        printf("F' U F ");
      } else if (row == 0 && col == 1) {
        rotate_right_anticlockwise(cube);  // R'
        rotate_front_anticlockwise(cube);  // F'
        rotate_top_clockwise(cube);        // U
        rotate_front_clockwise(cube);      // F
        rotate_right_clockwise(cube);      // R
        printf("R' F' U F R ");
      } else if (row == 1 && col == 2) {
        rotate_back_clockwise(cube);      // B
        rotate_top_clockwise(cube);       // U
        rotate_back_anticlockwise(cube);  // B'
        printf("B U B' ");
      } else if (row == 2 && col == 1) {
        rotate_right_clockwise(cube);      // R
        rotate_front_anticlockwise(cube);  // F'
        rotate_top_clockwise(cube);        // U
        rotate_front_clockwise(cube);      // F
        rotate_right_anticlockwise(cube);  // R'
        printf("R F' U F R' ");
      }
    }
  }

  // for white edge piece on the top face
  if (cube->faces[1].grid[1][0] == 'W') {
    moveEdgeFromYellowToWhite(cube, 3);
  }

  // for white edge piece on the top face
  if (cube->faces[1].grid[0][1] == 'W') {
    moveEdgeFromYellowToWhite(cube, 4);
  }

  // for white edge piece on the top face
  if (cube->faces[1].grid[2][1] == 'W') {
    moveEdgeFromYellowToWhite(cube, 2);
  }

  // for white edge piece on the top face
  if (cube->faces[1].grid[1][2] == 'W') {
    moveEdgeFromYellowToWhite(cube, 5);
  }
}

// for checking if the edges of the face of the cube has the color passed to it
bool checkColorEdge(RubiksCube *cube, int face, char color) {
  bool present = false;

  if (cube->faces[face].grid[1][0] == color) {
    present = true;
  } else if (cube->faces[face].grid[0][1] == color) {
    present = true;
  } else if (cube->faces[face].grid[2][1] == color) {
    present = true;
  } else if (cube->faces[face].grid[1][2] == color) {
    present = true;
  }

  return present;
}

// main function responsible for solving the white cross
void solveWhiteCross(RubiksCube *cube) {
  bool done = true;

  do {
    done = true;

    fixWhiteEdge(cube, 1, 0, 0,
                 false);  // the white edges found on the top face

    for (int face = 2; face < 6; face++) {
      if (checkColorEdge(cube, face, 'W')) {
        done = false;
        if (cube->faces[face].grid[1][0] == 'W') {
          fixWhiteEdge(cube, face, 1, 0, true);
        } else if (cube->faces[face].grid[0][1] == 'W') {
          fixWhiteEdge(cube, face, 0, 1, true);
        } else if (cube->faces[face].grid[2][1] == 'W') {
          fixWhiteEdge(cube, face, 2, 1, true);
        } else if (cube->faces[face].grid[1][2] == 'W') {
          fixWhiteEdge(cube, face, 1, 2, true);
        }
      }
    }

    if (done) {
      if (cube->faces[0].grid[1][0] == 'W' &&
          cube->faces[3].grid[2][1] != 'B') {
        done = false;
        rotate_left_clockwise(cube);
        rotate_left_clockwise(cube);
        printf("L2 ");
      } else if (cube->faces[0].grid[0][1] == 'W' &&
                 cube->faces[2].grid[2][1] != 'R') {
        done = false;
        rotate_front_clockwise(cube);
        rotate_front_clockwise(cube);
        printf("F2 ");
      } else if (cube->faces[0].grid[1][2] == 'W' &&
                 cube->faces[5].grid[2][1] != 'G') {
        done = false;
        rotate_right_clockwise(cube);
        rotate_right_clockwise(cube);
        printf("R2 ");
      } else if (cube->faces[0].grid[2][1] == 'W' &&
                 cube->faces[4].grid[2][1] != 'O') {
        done = false;
        rotate_back_clockwise(cube);
        rotate_back_clockwise(cube);
        printf("B2 ");
      }
    }

  } while (!done);
}

// helps the main function of solveWhiteCorners by
void fixWhiteCorner(RubiksCube *cube, int cornerNum, char colorA, char colorB) {
  int newCornerNum = 0;

  // for alligning the corner in its right position
  if (cornerNum == 1) {
    if ((colorA == 'R' && colorB == 'G') || (colorA == 'G' && colorB == 'R')) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      newCornerNum = 2;
    } else if ((colorA == 'B' && colorB == 'O') ||
               (colorA == 'O' && colorB == 'B')) {
      rotate_top_clockwise(cube);
      printf("U ");
      newCornerNum = 3;
    } else if ((colorA == 'G' && colorB == 'O') ||
               (colorA == 'O' && colorB == 'G')) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      newCornerNum = 4;
    } else {
      newCornerNum = cornerNum;
    }

  } else if (cornerNum == 2) {
    if ((colorA == 'R' && colorB == 'B') || (colorA == 'B' && colorB == 'R')) {
      rotate_top_clockwise(cube);
      printf("U ");
      newCornerNum = 1;
    } else if ((colorA == 'B' && colorB == 'O') ||
               (colorA == 'O' && colorB == 'B')) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      newCornerNum = 3;
    } else if ((colorA == 'G' && colorB == 'O') ||
               (colorA == 'O' && colorB == 'G')) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      newCornerNum = 4;
    } else {
      newCornerNum = cornerNum;
    }

  } else if (cornerNum == 3) {
    if ((colorA == 'R' && colorB == 'B') || (colorA == 'B' && colorB == 'R')) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      newCornerNum = 1;
    } else if ((colorA == 'R' && colorB == 'G') ||
               (colorA == 'G' && colorB == 'R')) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      newCornerNum = 2;
    } else if ((colorA == 'G' && colorB == 'O') ||
               (colorA == 'O' && colorB == 'G')) {
      rotate_top_clockwise(cube);
      printf("U ");
      newCornerNum = 4;
    } else {
      newCornerNum = cornerNum;
    }

  } else if (cornerNum == 4) {
    if ((colorA == 'R' && colorB == 'B') || (colorA == 'B' && colorB == 'R')) {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      newCornerNum = 1;
    } else if ((colorA == 'R' && colorB == 'G') ||
               (colorA == 'G' && colorB == 'R')) {
      rotate_top_clockwise(cube);
      printf("U ");
      newCornerNum = 2;
    } else if ((colorA == 'B' && colorB == 'O') ||
               (colorA == 'O' && colorB == 'B')) {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      newCornerNum = 3;
    } else {
      newCornerNum = cornerNum;
    }
  }

  // to finally put the corner in the last layer
  bool fixed = true;

  do {
    fixed = true;

    if (newCornerNum == 1) {
      rotate_left_anticlockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_left_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("L' U' L U ");

      if (cube->faces[0].grid[0][0] == 'W' &&
          cube->faces[3].grid[2][2] == cube->faces[3].grid[1][1] &&
          cube->faces[2].grid[2][0] == cube->faces[2].grid[1][1]) {
      } else {
        fixed = false;
      }

    } else if (newCornerNum == 2) {
      rotate_right_clockwise(cube);
      rotate_top_clockwise(cube);
      rotate_right_anticlockwise(cube);
      rotate_top_anticlockwise(cube);
      printf("R U R' U' ");

      if (cube->faces[0].grid[0][2] == 'W' &&
          cube->faces[2].grid[2][2] == cube->faces[2].grid[1][1] &&
          cube->faces[5].grid[2][0] == cube->faces[5].grid[1][1]) {
      } else {
        fixed = false;
      }

    } else if (newCornerNum == 3) {
      rotate_back_anticlockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_back_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("B' U' B U ");

      if (cube->faces[0].grid[2][0] == 'W' &&
          cube->faces[4].grid[2][2] == cube->faces[4].grid[1][1] &&
          cube->faces[3].grid[2][0] == cube->faces[3].grid[1][1]) {
      } else {
        fixed = false;
      }

    } else if (newCornerNum == 4) {
      rotate_back_clockwise(cube);
      rotate_top_clockwise(cube);
      rotate_back_anticlockwise(cube);
      rotate_top_anticlockwise(cube);
      printf("B U B' U' ");

      if (cube->faces[0].grid[2][2] == 'W' &&
          cube->faces[5].grid[2][2] == cube->faces[5].grid[1][1] &&
          cube->faces[4].grid[2][0] == cube->faces[4].grid[1][1]) {
      } else {
        fixed = false;
      }
    }

  } while (!fixed);
}

// main function responsible for solving the white corners
void solveWhiteCorners(RubiksCube *cube) {
  int cornerNumber = 0;
  char otherColorA, otherColorB;
  bool done = true;

  do {
    done = true;

    if (cube->faces[3].grid[0][2] == 'W' || cube->faces[2].grid[0][0] == 'W' ||
        cube->faces[1].grid[2][0] == 'W') {
      if (cube->faces[3].grid[0][2] == 'W') {
        otherColorA = cube->faces[2].grid[0][0];
        otherColorB = cube->faces[1].grid[2][0];
      } else if (cube->faces[2].grid[0][0] == 'W') {
        otherColorA = cube->faces[3].grid[0][2];
        otherColorB = cube->faces[1].grid[2][0];
      } else {
        otherColorA = cube->faces[2].grid[0][0];
        otherColorB = cube->faces[3].grid[0][2];
      }

      done = false;
      cornerNumber = 1;

    } else if (cube->faces[2].grid[0][2] == 'W' ||
               cube->faces[5].grid[0][0] == 'W' ||
               cube->faces[1].grid[2][2] == 'W') {
      if (cube->faces[2].grid[0][2] == 'W') {
        otherColorA = cube->faces[5].grid[0][0];
        otherColorB = cube->faces[1].grid[2][2];
      } else if (cube->faces[5].grid[0][0] == 'W') {
        otherColorA = cube->faces[2].grid[0][2];
        otherColorB = cube->faces[1].grid[2][2];
      } else {
        otherColorA = cube->faces[2].grid[0][2];
        otherColorB = cube->faces[5].grid[0][0];
      }

      done = false;
      cornerNumber = 2;

    } else if (cube->faces[4].grid[0][2] == 'W' ||
               cube->faces[3].grid[0][0] == 'W' ||
               cube->faces[1].grid[0][0] == 'W') {
      if (cube->faces[4].grid[0][2] == 'W') {
        otherColorA = cube->faces[3].grid[0][0];
        otherColorB = cube->faces[1].grid[0][0];
      } else if (cube->faces[3].grid[0][0] == 'W') {
        otherColorA = cube->faces[4].grid[0][2];
        otherColorB = cube->faces[1].grid[0][0];
      } else {
        otherColorA = cube->faces[4].grid[0][2];
        otherColorB = cube->faces[3].grid[0][0];
      }

      done = false;
      cornerNumber = 3;

    } else if (cube->faces[5].grid[0][2] == 'W' ||
               cube->faces[4].grid[0][0] == 'W' ||
               cube->faces[1].grid[0][2] == 'W') {
      if (cube->faces[5].grid[0][2] == 'W') {
        otherColorA = cube->faces[4].grid[0][0];
        otherColorB = cube->faces[1].grid[0][2];
      } else if (cube->faces[4].grid[0][0] == 'W') {
        otherColorA = cube->faces[5].grid[0][2];
        otherColorB = cube->faces[1].grid[0][2];
      } else {
        otherColorA = cube->faces[5].grid[0][2];
        otherColorB = cube->faces[4].grid[0][0];
      }

      done = false;
      cornerNumber = 4;
    }

    if (done == false) {
      fixWhiteCorner(cube, cornerNumber, otherColorA, otherColorB);

    } else {  // if the white corner is in the bottom layer but in the wrong
              // position

      if (cube->faces[3].grid[2][2] == 'W' ||
          cube->faces[2].grid[2][0] == 'W' ||
          cube->faces[0].grid[0][0] == 'W') {
        if (cube->faces[0].grid[0][0] == 'W' &&
            cube->faces[3].grid[2][2] == cube->faces[3].grid[1][1] &&
            cube->faces[2].grid[2][0] ==
                cube->faces[2]
                    .grid[1][1]) {  // if its not in the right position
        } else {
          rotate_left_anticlockwise(cube);
          rotate_top_anticlockwise(cube);
          rotate_left_clockwise(cube);
          rotate_top_clockwise(cube);
          printf("L' U' L U ");
          done = false;
        }
      }

      if (done == true && (cube->faces[2].grid[2][2] == 'W' ||
                           cube->faces[5].grid[2][0] == 'W' ||
                           cube->faces[0].grid[0][2] == 'W')) {
        if (cube->faces[0].grid[0][2] == 'W' &&
            cube->faces[2].grid[2][2] == cube->faces[2].grid[1][1] &&
            cube->faces[5].grid[2][0] ==
                cube->faces[5]
                    .grid[1][1]) {  // if its not in the right position
        } else {
          rotate_right_clockwise(cube);
          rotate_top_clockwise(cube);
          rotate_right_anticlockwise(cube);
          rotate_top_anticlockwise(cube);
          printf("R U R' U' ");
          done = false;
        }
      }

      if (done == true && (cube->faces[4].grid[2][2] == 'W' ||
                           cube->faces[3].grid[2][0] == 'W' ||
                           cube->faces[0].grid[2][0] == 'W')) {
        if (cube->faces[0].grid[2][0] == 'W' &&
            cube->faces[4].grid[2][2] == cube->faces[4].grid[1][1] &&
            cube->faces[3].grid[2][0] ==
                cube->faces[3]
                    .grid[1][1]) {  // if its not in the right position
        } else {
          rotate_back_anticlockwise(cube);
          rotate_top_anticlockwise(cube);
          rotate_back_clockwise(cube);
          rotate_top_clockwise(cube);
          printf("B' U' B U ");
          done = false;
        }
      }

      if (done == true && (cube->faces[5].grid[2][2] == 'W' ||
                           cube->faces[4].grid[2][0] == 'W' ||
                           cube->faces[0].grid[2][2] == 'W')) {
        if (cube->faces[0].grid[2][2] == 'W' &&
            cube->faces[5].grid[2][2] == cube->faces[5].grid[1][1] &&
            cube->faces[4].grid[2][0] ==
                cube->faces[4]
                    .grid[1][1]) {  // if its not in the right position
        } else {
          rotate_back_clockwise(cube);
          rotate_top_clockwise(cube);
          rotate_back_anticlockwise(cube);
          rotate_top_anticlockwise(cube);
          printf("B U B' U' ");
          done = false;
        }
      }
    }

  } while (!done);
}

// alligns the colored edges in the first layer to their corressponding coloured
// centre and return the new face number that has that edge after alligning
int allignEdgeToCenter(RubiksCube *cube, int faceNumber) {
  if (faceNumber == 2) {  // front face
    if (cube->faces[2].grid[0][1] == 'B') {
      rotate_top_clockwise(cube);
      printf("U ");
      return 3;
    } else if (cube->faces[2].grid[0][1] == 'G') {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      return 5;
    } else if (cube->faces[2].grid[0][1] == 'O') {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      return 4;
    } else {
      return 2;
    }
  } else if (faceNumber == 3) {  // left face
    if (cube->faces[3].grid[0][1] == 'O') {
      rotate_top_clockwise(cube);
      printf("U ");
      return 4;
    } else if (cube->faces[3].grid[0][1] == 'R') {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      return 2;
    } else if (cube->faces[3].grid[0][1] == 'G') {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      return 5;
    } else {
      return 3;
    }
  } else if (faceNumber == 4) {  // back face
    if (cube->faces[4].grid[0][1] == 'G') {
      rotate_top_clockwise(cube);
      printf("U ");
      return 5;
    } else if (cube->faces[4].grid[0][1] == 'B') {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      return 3;
    } else if (cube->faces[4].grid[0][1] == 'R') {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      return 2;
    } else {
      return 4;
    }
  } else {  // right face
    if (cube->faces[5].grid[0][1] == 'O') {
      rotate_top_anticlockwise(cube);
      printf("U' ");
      return 4;
    } else if (cube->faces[5].grid[0][1] == 'R') {
      rotate_top_clockwise(cube);
      printf("U ");
      return 2;
    } else if (cube->faces[5].grid[0][1] == 'B') {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
      return 3;
    } else {
      return 5;
    }
  }
}

// find the face number that has the colored edge on the top
int findColoredEdgeTop(RubiksCube *cube) {
  if (cube->faces[2].grid[0][1] != 'W' && cube->faces[2].grid[0][1] != 'Y' &&
      cube->faces[1].grid[2][1] != 'W' && cube->faces[1].grid[2][1] != 'Y') {
    return 2;
  } else if (cube->faces[3].grid[0][1] != 'W' &&
             cube->faces[3].grid[0][1] != 'Y' &&
             cube->faces[1].grid[1][0] != 'W' &&
             cube->faces[1].grid[1][0] != 'Y') {
    return 3;
  } else if (cube->faces[4].grid[0][1] != 'W' &&
             cube->faces[4].grid[0][1] != 'Y' &&
             cube->faces[1].grid[0][1] != 'W' &&
             cube->faces[1].grid[0][1] != 'Y') {
    return 4;
  } else if (cube->faces[5].grid[0][1] != 'W' &&
             cube->faces[5].grid[0][1] != 'Y' &&
             cube->faces[1].grid[1][2] != 'W' &&
             cube->faces[1].grid[1][2] != 'Y') {
    return 5;
  } else {
    return -1;
  }
}

// fix the aleady alligned colored edges
void fixColoredEdge(RubiksCube *cube, int faceNumber) {
  if (faceNumber == 2) {  // for the edge piece with the main color RED
    if (cube->faces[1].grid[2][1] == 'B') {  // if the otherside is blue

      rotate_top_anticlockwise(cube);
      rotate_left_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_left_clockwise(cube);

      rotate_top_clockwise(cube);
      rotate_front_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_front_anticlockwise(cube);

      printf("U' L' U L U F U' F' ");

    } else {  // if the other side is green

      rotate_top_clockwise(cube);
      rotate_right_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_right_anticlockwise(cube);

      rotate_top_anticlockwise(cube);
      rotate_front_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_front_clockwise(cube);

      printf("U R U' R' U' F' U F ");
    }
  } else if (faceNumber == 3) {  // fpr the edge piece with the main color BLUE
    if (cube->faces[1].grid[1][0] == 'R') {  // if the other side is red

      rotate_top_clockwise(cube);
      rotate_front_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_front_anticlockwise(cube);

      rotate_top_anticlockwise(cube);
      rotate_left_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_left_clockwise(cube);

      printf("U F U' F' U' L' U L ");

    } else {  // if the other side is orange

      rotate_top_anticlockwise(cube);
      rotate_back_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_back_clockwise(cube);

      rotate_top_clockwise(cube);
      rotate_left_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_left_anticlockwise(cube);

      printf("U' B' U B U L U' L' ");
    }
  } else if (faceNumber == 5) {  // for the edge piece with the main color GREEN
    if (cube->faces[1].grid[1][2] == 'O') {  // if the other side is orange

      rotate_top_clockwise(cube);
      rotate_back_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_back_anticlockwise(cube);

      rotate_top_anticlockwise(cube);
      rotate_right_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_right_clockwise(cube);

      printf("U B U' B' U' R' U R ");

    } else {  // if the other side is red

      rotate_top_anticlockwise(cube);
      rotate_front_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_front_clockwise(cube);

      rotate_top_clockwise(cube);
      rotate_right_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_right_anticlockwise(cube);

      printf("U' F' U F U R U' R' ");
    }
  } else if (faceNumber == 4) {  // for the edgepiece with the main color ORANGE
    if (cube->faces[1].grid[0][1] == 'G') {  // if the other side is green

      rotate_top_anticlockwise(cube);
      rotate_right_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_right_clockwise(cube);

      rotate_top_clockwise(cube);
      rotate_back_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_back_anticlockwise(cube);

      printf("U' R' U R U B U' B' ");

    } else {  // if the other side is blue

      rotate_top_clockwise(cube);
      rotate_left_clockwise(cube);
      rotate_top_anticlockwise(cube);
      rotate_left_anticlockwise(cube);

      rotate_top_anticlockwise(cube);
      rotate_back_anticlockwise(cube);
      rotate_top_clockwise(cube);
      rotate_back_clockwise(cube);

      printf("U L U' L' U' B' U B ");
    }
  }
}

// main function responsible for solving the colored edges AKA the second layer
void solveColoredEdges(RubiksCube *cube) {
  bool done = true;
  int faceNum;
  int newFace;

  do {
    done = true;
    faceNum = -2;
    newFace = -2;
    faceNum = findColoredEdgeTop(cube);

    if (faceNum != -1) {
      done = false;
      newFace = allignEdgeToCenter(cube, faceNum);
      fixColoredEdge(cube, newFace);

    } else {  // to check if there are colored edges in the second layer that
              // are positioned wrong

      if (cube->faces[2].grid[1][0] != 'W' &&
          cube->faces[2].grid[1][0] != 'Y' &&
          cube->faces[2].grid[1][0] != 'R' &&
          cube->faces[3].grid[1][2] != 'W' &&
          cube->faces[3].grid[1][2] != 'B' &&
          cube->faces[3].grid[1][2] != 'Y') {  // for edge 5

        done = false;

        rotate_top_anticlockwise(cube);
        rotate_left_anticlockwise(cube);
        rotate_top_clockwise(cube);
        rotate_left_clockwise(cube);

        rotate_top_clockwise(cube);
        rotate_front_clockwise(cube);
        rotate_top_anticlockwise(cube);
        rotate_front_anticlockwise(cube);

        printf("U' L' U L U F U' F' ");
      }

      if (done && cube->faces[2].grid[1][2] != 'W' &&
          cube->faces[2].grid[1][2] != 'Y' &&
          cube->faces[2].grid[1][2] != 'R' &&
          cube->faces[5].grid[1][0] != 'W' &&
          cube->faces[5].grid[1][0] != 'G' &&
          cube->faces[5].grid[1][0] != 'Y') {  // for edge 6

        done = false;

        rotate_top_clockwise(cube);
        rotate_right_clockwise(cube);
        rotate_top_anticlockwise(cube);
        rotate_right_anticlockwise(cube);

        rotate_top_anticlockwise(cube);
        rotate_front_anticlockwise(cube);
        rotate_top_clockwise(cube);
        rotate_front_clockwise(cube);

        printf("U R U' R' U' F' U F ");
      }

      if (done && cube->faces[5].grid[1][2] != 'W' &&
          cube->faces[5].grid[1][2] != 'Y' &&
          cube->faces[5].grid[1][2] != 'G' &&
          cube->faces[4].grid[1][0] != 'W' &&
          cube->faces[4].grid[1][0] != 'O' &&
          cube->faces[4].grid[1][0] != 'Y') {  // for edge 7

        done = false;

        rotate_top_clockwise(cube);
        rotate_back_clockwise(cube);
        rotate_top_anticlockwise(cube);
        rotate_back_anticlockwise(cube);

        rotate_top_anticlockwise(cube);
        rotate_right_anticlockwise(cube);
        rotate_top_clockwise(cube);
        rotate_right_clockwise(cube);

        printf("U B U' B' U' R' U R ");
      }

      if (done && cube->faces[4].grid[1][2] != 'W' &&
          cube->faces[4].grid[1][2] != 'Y' &&
          cube->faces[4].grid[1][2] != 'O' &&
          cube->faces[3].grid[1][0] != 'W' &&
          cube->faces[3].grid[1][0] != 'B' &&
          cube->faces[3].grid[1][0] != 'Y') {  // for edge 8

        done = false;

        rotate_top_anticlockwise(cube);
        rotate_back_anticlockwise(cube);
        rotate_top_clockwise(cube);
        rotate_back_clockwise(cube);

        rotate_top_clockwise(cube);
        rotate_left_clockwise(cube);
        rotate_top_anticlockwise(cube);
        rotate_left_anticlockwise(cube);

        printf("U' B' U B U L U' L' ");
      }
    }

  } while (!done);
}

// finds the number of yellow edges in the top layer of the cube
int yellowEdgesFound(RubiksCube *cube) {
  int count = 0;
  if (cube->faces[1].grid[0][1] == 'Y') {
    count++;
  }

  if (cube->faces[1].grid[1][2] == 'Y') {
    count++;
  }

  if (cube->faces[1].grid[2][1] == 'Y') {
    count++;
  }

  if (cube->faces[1].grid[1][0] == 'Y') {
    count++;
  }

  return count;
}

// alligns the top layer such that the algorithm can be applied
void allignTop(RubiksCube *cube, int caseNumber) {
  if (caseNumber ==
      1) {  // when the yellow top edges are opposite to each other
    if (cube->faces[1].grid[0][1] == 'Y' && cube->faces[1].grid[2][1] == 'Y') {
      rotate_top_clockwise(cube);
      printf("U ");
    }
  } else {  // when the yellow top edges are consecutive to each other
    if (cube->faces[1].grid[0][1] == 'Y' && cube->faces[1].grid[1][2] == 'Y') {
      rotate_top_clockwise(cube);
      printf("U ");
    } else if (cube->faces[1].grid[1][0] == 'Y' &&
               cube->faces[1].grid[2][1] == 'Y') {
      rotate_top_anticlockwise(cube);
      printf("U' ");
    } else if (cube->faces[1].grid[0][1] == 'Y' &&
               cube->faces[1].grid[1][0] == 'Y') {
      rotate_top_clockwise(cube);
      rotate_top_clockwise(cube);
      printf("U2 ");
    }
  }
}

// has the algorithm that was used in solving the yellow cross
void algorithmYellowCross(RubiksCube *cube) {
  rotate_front_clockwise(cube);
  rotate_right_clockwise(cube);
  rotate_top_clockwise(cube);
  rotate_right_anticlockwise(cube);
  rotate_top_anticlockwise(cube);
  rotate_front_anticlockwise(cube);
  printf("F R U R' U' F' ");
}

// main function responsible for making a yellow cross in the top layer
void solveYellowCross(RubiksCube *cube) {
  bool done = true;
  do {
    done = true;
    int yellowEdges = yellowEdgesFound(cube);

    if (yellowEdges == 0) {
      algorithmYellowCross(cube);
      done = false;
    } else if (yellowEdges == 2) {
      if ((cube->faces[1].grid[0][1] == 'Y' &&
           cube->faces[1].grid[2][1] == 'Y') ||
          (cube->faces[1].grid[1][2] == 'Y' &&
           cube->faces[1].grid[1][0] ==
               'Y')) {  // when the edges are opposite to each other
        allignTop(cube, 1);
      } else {
        allignTop(cube, 2);
      }
      algorithmYellowCross(cube);
      done = false;
    }

  } while (!done);
}

// has the algorithm that was used for alligning the yellow edges right
void algorithmYellowColorEdges(RubiksCube *cube) {
  rotate_right_clockwise(cube);
  rotate_top_clockwise(cube);
  rotate_top_clockwise(cube);
  rotate_right_anticlockwise(cube);
  rotate_top_anticlockwise(cube);
  rotate_right_clockwise(cube);
  rotate_top_anticlockwise(cube);
  rotate_right_anticlockwise(cube);
  printf("R U2 R' U' R U' R' ");
}

// for alligning the yellow-red edge with the red centre
void allignRed(RubiksCube *cube) {
  if (cube->faces[3].grid[0][1] == 'R') {
    rotate_top_anticlockwise(cube);
    printf("U' ");
  } else if (cube->faces[4].grid[0][1] == 'R') {
    rotate_top_clockwise(cube);
    rotate_top_clockwise(cube);
    printf("U2 ");
  } else if (cube->faces[5].grid[0][1] == 'R') {
    rotate_top_clockwise(cube);
    printf("U ");
  }
}

// alligns the upper face so that the algorithm can be applied
void allignForAlgorithm(RubiksCube *cube) {
  if (cube->faces[3].grid[0][1] == 'B' && cube->faces[4].grid[0][1] == 'G' &&
      cube->faces[5].grid[0][1] == 'O') {
    rotate_top_clockwise(cube);
    printf("U ");
  } else if (cube->faces[3].grid[0][1] == 'G' &&
             cube->faces[4].grid[0][1] == 'B' &&
             cube->faces[5].grid[0][1] == 'O') {
    rotate_top_anticlockwise(cube);
    printf("U' ");
  } else if (cube->faces[3].grid[0][1] == 'O' &&
             cube->faces[4].grid[0][1] == 'B' &&
             cube->faces[5].grid[0][1] == 'G') {
    rotate_top_clockwise(cube);
    rotate_top_clockwise(cube);
    printf("U2 ");
  }
}

// main function responsible for alligning the yellow edges with the centers
void solveYellowColorEdges(RubiksCube *cube) {
  bool done = true;

  do {
    done = true;
    allignRed(cube);
    if (cube->faces[3].grid[0][1] == 'G' && cube->faces[4].grid[0][1] == 'O' &&
        cube->faces[5].grid[0][1] == 'B') {
      algorithmYellowColorEdges(cube);
      done = false;
    } else if (cube->faces[3].grid[0][1] == 'B' &&
               cube->faces[4].grid[0][1] == 'O' &&
               cube->faces[5].grid[0][1] == 'G') {
    } else {
      allignForAlgorithm(cube);
      algorithmYellowColorEdges(cube);
      done = false;
    }
  } while (!done);
}

// has the algorithms used for alligning the yellow corners in the right
// position
void algorithmAllignYellowCorners(RubiksCube *cube, int caseNumber) {
  if (caseNumber == 1) {
    rotate_top_clockwise(cube);
    rotate_front_clockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_back_anticlockwise(cube);
    rotate_top_clockwise(cube);
    rotate_front_anticlockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_back_clockwise(cube);
    printf("U F U' B' U F' U' B ");
  } else if (caseNumber == 2) {
    rotate_top_clockwise(cube);
    rotate_right_clockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_left_anticlockwise(cube);
    rotate_top_clockwise(cube);
    rotate_right_anticlockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_left_clockwise(cube);
    printf("U R U' L' U R' U' L ");
  } else if (caseNumber == 3) {
    rotate_top_clockwise(cube);
    rotate_left_clockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_right_anticlockwise(cube);
    rotate_top_clockwise(cube);
    rotate_left_anticlockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_right_clockwise(cube);
    printf("U L U' R' U L' U' R ");
  } else if (caseNumber == 4) {
    rotate_top_clockwise(cube);
    rotate_back_clockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_front_anticlockwise(cube);
    rotate_top_clockwise(cube);
    rotate_back_anticlockwise(cube);
    rotate_top_anticlockwise(cube);
    rotate_front_clockwise(cube);
    printf("U B U' F' U B' U' F ");
  }
}

// count the yellow corners that are in their correct positions and if only one
// corner is in the right position returns the corner no.
int findCorrectCorners(RubiksCube *cube) {
  char otherColorA;
  char otherColorB;
  int correct = 0;
  int onlyCorrectCorner = 0;

  // if corner 1 is right
  if (cube->faces[1].grid[2][0] == 'Y') {
    otherColorA = cube->faces[2].grid[0][0];
    otherColorB = cube->faces[3].grid[0][2];
  } else if (cube->faces[2].grid[0][0] == 'Y') {
    otherColorA = cube->faces[1].grid[2][0];
    otherColorB = cube->faces[3].grid[0][2];
  } else {
    otherColorA = cube->faces[1].grid[2][0];
    otherColorB = cube->faces[2].grid[0][0];
  }

  if ((otherColorA == 'R' && otherColorB == 'B') ||
      (otherColorA == 'B' && otherColorB == 'R')) {
    correct++;
    onlyCorrectCorner = 5;
  }

  // if corner 2 is right
  if (cube->faces[1].grid[2][2] == 'Y') {
    otherColorA = cube->faces[2].grid[0][2];
    otherColorB = cube->faces[5].grid[0][0];
  } else if (cube->faces[2].grid[0][2] == 'Y') {
    otherColorA = cube->faces[1].grid[2][2];
    otherColorB = cube->faces[5].grid[0][0];
  } else {
    otherColorA = cube->faces[1].grid[2][2];
    otherColorB = cube->faces[2].grid[0][2];
  }

  if ((otherColorA == 'R' && otherColorB == 'G') ||
      (otherColorA == 'G' && otherColorB == 'R')) {
    correct++;
    onlyCorrectCorner = 6;
  }

  // if corner 3 is right
  if (cube->faces[1].grid[0][0] == 'Y') {
    otherColorA = cube->faces[3].grid[0][0];
    otherColorB = cube->faces[4].grid[0][2];
  } else if (cube->faces[3].grid[0][0] == 'Y') {
    otherColorA = cube->faces[1].grid[0][0];
    otherColorB = cube->faces[4].grid[0][2];
  } else {
    otherColorA = cube->faces[1].grid[0][0];
    otherColorB = cube->faces[3].grid[0][0];
  }

  if ((otherColorA == 'B' && otherColorB == 'O') ||
      (otherColorA == 'O' && otherColorB == 'B')) {
    correct++;
    onlyCorrectCorner = 7;
  }

  // if corner 4 is right
  if (cube->faces[1].grid[0][2] == 'Y') {
    otherColorA = cube->faces[4].grid[0][0];
    otherColorB = cube->faces[5].grid[0][2];
  } else if (cube->faces[4].grid[0][0] == 'Y') {
    otherColorA = cube->faces[1].grid[0][2];
    otherColorB = cube->faces[5].grid[0][2];
  } else {
    otherColorA = cube->faces[1].grid[0][2];
    otherColorB = cube->faces[4].grid[0][0];
  }

  if ((otherColorA == 'G' && otherColorB == 'O') ||
      (otherColorA == 'O' && otherColorB == 'G')) {
    correct++;
    onlyCorrectCorner = 8;
  }

  if (correct == 1) {
    return onlyCorrectCorner;
  } else {
    return correct;
  }
}

// main function responsible for alligning the yellow corners in their right
// position
void solveAllignYellowCorners(RubiksCube *cube) {
  int correct = 0;
  correct = findCorrectCorners(cube);

  if (correct == 0) {  // no corners are in their right place
    do {
      algorithmAllignYellowCorners(cube, 2);
      correct = findCorrectCorners(cube);
    } while (correct == 0);
  }

  if (correct > 4) {  // only one corner is in the right place so the index of
                      // the corner is returned
    if ((correct - 4) == 1) {  // corner 1 is right
      do {
        algorithmAllignYellowCorners(cube, 1);
        correct = findCorrectCorners(cube);
      } while (correct != 4);
    } else if ((correct - 4) == 2) {  // corner 2 is right
      do {
        algorithmAllignYellowCorners(cube, 2);
        correct = findCorrectCorners(cube);
      } while (correct != 4);
    } else if ((correct - 4) == 3) {  // corner 3 is right
      do {
        algorithmAllignYellowCorners(cube, 3);
        correct = findCorrectCorners(cube);
      } while (correct != 4);
    } else if ((correct - 4) == 4) {  // corner 4 is right
      do {
        algorithmAllignYellowCorners(cube, 4);
        correct = findCorrectCorners(cube);
      } while (correct != 4);
    }
  }
}

// check if the right top corner is in the correct orientation
bool checkFullySolvedCorner(RubiksCube *cube) {
  bool solve = false;
  if (cube->faces[1].grid[2][2] == 'Y') {
    solve = true;
  }
  return solve;
}

// has the algorithm for solving the yellow corners by putting them in the right
// orientation
void algorithmOrientationYellowCorner(RubiksCube *cube) {
  bool done = true;
  do {
    done = true;
    rotate_right_anticlockwise(cube);
    rotate_down_anticlockwise(cube);
    rotate_right_clockwise(cube);
    rotate_down_clockwise(cube);
    printf("R' D' R D ");
    done = checkFullySolvedCorner(cube);
  } while (!done);
}

// main function rtesponsible for solving the yellow corners by putting them in
// correct orientation
void solveOrientationYellowCorners(RubiksCube *cube) {
  for (int i = 1; i <= 4; i++) {
    if (!checkFullySolvedCorner(
            cube)) {  // if the right top corner is not solved
      algorithmOrientationYellowCorner(cube);
    }

    rotate_top_anticlockwise(
        cube);  // rotates the left top corner to the right top corner
    printf("U' ");
  }
}
