# Rubiks-Cube-Solver

This program takes in the data of a shuffled 3*3 Rubiks cube and then provides the moves need to be done to solve the cube. 

# How-the-program-works

The program asks the user how they want to input the data of the shuffled cube. Two choices are given:<br>
1. By inputting the colors of the shuffled cube
2. By providing the information of the moves used to shuffle the cube
<br>
Once the user inputs the data of the shuffled cube the program shows the user what the shuffled cube looks like and then tells the user to makes the moves to solve the cube in the following steos: <br><br>

1. Making the white cross<br>
2. Solving the white color corners<br>
3. Solving the colored edges (other than white or yellow)<br>
4. Making the yellow cross<br>
5. Solving the yellow colored edges<br>
6. Placing the yellow colored corners in their right positions<br>
7. Solving the yellow colored corners by correcting their orientation<br>
<br>
For each step, the program shows the user the cube movements to complete the step and what the cube should look like after applying those moves, helping users verify they are following the instructions correctly. The program conveys cube movements using algorithms. For more information on Rubik's Cube algorithms, please visit my website: https://cubemaster262075683.wordpress.com/algorithms/. Once all seven steps are completed, the shuffled cube will be solved!