/*Jose M. Aguilar
  Jaguil61
  Project 2: mymatrix<T>*/

#include <iostream>
#include <time.h>
#include "mymatrix.h"

int randNumGen() //Gives a random number 1 - 10
{
	int randNum;

	srand(clock());

	randNum = rand() % 11;

	return randNum;
}

int main()
{
	
	mymatrix<int>  M;  // 4x4 matrix, initialized to 0

	int randomNum = randNumGen(); // Random number 0-10
	int randomNumR = randomNum % M.numrows(); // Random number 0-3

	cout << "Random Number 0-10: " << randomNum << endl;
	cout << "Random Number 0-4: " << randomNumR << endl;

	cout << endl;
	
	M(0, 0) = 123;
	M(1, 1) = 456;
	M(2, 2) = 789;
	M(3, 3) = -99;
	
	cout << "[matrix M]:" << endl;
	M._output();

	cout << endl << "[at TEST]: " << M.at(randomNumR, randomNumR) << " at (" << randomNumR << ", " << randomNumR << ")"<< endl; //Checks at function
	cout << endl << "[numrows TEST] Number of Rows: " << M.numrows() << endl; //Checks numrows function
	cout << endl << "[numcols TEST] " << M.numcols(randomNumR) << " columns in row index " << randomNumR << endl; //Checks numcols function
	cout << endl << "[size TEST] Size of matrix: " << M.size() << endl; //Checks the size function

	cout << endl << "[grow TEST]: " << endl;
	M.growcols(randomNumR, randomNum); //grow row (1-4) to have (1-10) columns

	cout << "[matrix M]:" << endl;
	M._output();

	cout << endl << endl << "[mymatrix TEST]: " << endl;
	
	mymatrix<int> M2(6,6);

	M2(0, 0) = 1;
	M2(1, 1) = 2;
	M2(2, 2) = 3;
	M2(3, 3) = 4;
	M2(4, 4) = 5;
	M2(5, 5) = 6;

	cout << "[matrix M2]:" << endl;
	M2._output();

	cout << endl << "[grow TEST]: " << endl;
	M2.grow(randomNum,randomNum);

	cout << "[matrix M2]:" << endl;
	M2._output();

	cout << endl << "[numrows TEST] Number of Rows: " << M2.numrows() << endl; //Checks numrows function
	cout << endl << "[numcols TEST] " << M2.numcols(randomNumR) << " columns in row index " << randomNumR << endl; //Checks numcols function
	cout << endl << "[size TEST] Size of matrix: " << M2.size() << endl; //Checks the size function

	cout << endl << "[operator* TEST] Matrix M1 * 2 : " << endl;

	mymatrix<int> M3;

	M3 = M * 2;

	cout << endl << "[matrix M3]:" << endl;
	M3._output();
	
	cout << endl << "[matrix multiplication TEST]: " << endl << endl;
	mymatrix<int> M4(3,4);
	mymatrix<int> M5(4,2);
	mymatrix<int> M6;

	M4(0, 0) = randomNum;
	M4(1, 1) = randomNumR;
	M4(2, 2) = randomNum;

	cout << "[matrix M4]:" << endl;
	M4._output();

	M5(0, 0) = randomNum;
	M5(1, 1) = randomNumR;
	M5(2, 0) = randomNum;

	cout << endl <<"[matrix M5]:" << endl;
	M5._output();

	M6 = M4 * M5;

	cout << endl << "[matrix M6 (M4 * M5)]:" << endl;
	M6._output();

	cout << endl;
	system("pause");
	return 0;
}