/*mymatrix.h*/

// 
// << Jose M. Aguilar (jaguil61) >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and 
// cols).  Also, rows can be "jagged" --- i.e. rows can have 
// different column sizes, and thus the matrix is not necessarily 
// rectangular.  All elements are initialized to the default value
// for the given type T.  Example:
//
//   mymatrix<int>  M;  // 4x4 matrix, initialized to 0
//   
//   M(0, 0) = 123;
//   M(1, 1) = 456;
//   M(2, 2) = 789;
//   M(3, 3) = -99;
//
//   M.growcols(1, 8);  // increase # of cols in row 1 to 8
//
//   for (int r = 0; r < M.numrows(); ++r)
//   {
//      for (int c = 0; c < M.numcols(r); ++c)
//         cout << M(r, c) << " ";
//      cout << endl;
//   }
//
// Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class mymatrix
{
private:
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // dynamic array of ROWs
  int  NumRows;  // total # of rows (0..NumRows-1)

public:
  //
  // default constructor:
  //
  // Called automatically by C++ to construct a 4x4 matrix.  All 
  // elements are initialized to the default value of T.
  //
  mymatrix()
  {
    Rows = new ROW[4];  // an array with 4 ROW structs:
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (int r = 0; r < NumRows; ++r)
    {
      Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
      }
    }
  }

  //
  // parameterized constructor:
  //
  // Called automatically by C++ to construct a matrix with R rows, 
  // where each row has C columns. All elements are initialized to 
  // the default value of T.
  //
  mymatrix(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::constructor: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::constructor: # of cols");

    // TO DO: Set number of rows equal to R
	Rows = new ROW[R];
	NumRows = R;

	// TO DO: Initialize each row to have C columns
	for (int i = 0; i < NumRows; ++i)
	{
		Rows[i].Cols = new T[C];
		Rows[i].NumCols = C;

		// TO DO: Initialize the elements to their default value:
		for (int j = 0; j < Rows[i].NumCols; ++j)
			Rows[i].Cols[j] = T{};
	}
  }


  //
  // copy constructor:
  //
  // Called automatically by C++ to construct a matrix that contains a 
  // copy of an existing matrix.  Example: this occurs when passing 
  // mymatrix as a parameter by value
  //
  //   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
  //   { ... }
  //
  mymatrix(const mymatrix<T>& other)
  {
	// Set the "this" elements equal to the "other elements"
	  this->Rows = new ROW[other.NumRows];
	  this->NumRows = other.NumRows;

    // Copy over the "other" elements
	  for (int i = 0; i < other.NumRows; i++)
	  {
		  this->Rows[i].Cols = new T[other.Rows[i].NumCols];
		  this->Rows[i].NumCols = other.Rows[i].NumCols;

		  for (int j = 0; j < other.Rows[i].NumCols; j++)
		  {
			  this->Rows[i].Cols[j] = other.Rows[i].Cols[j];
		  }
	  }
  }


  //
  // numrows
  //
  // Returns the # of rows in the matrix.  The indices for these rows
  // are 0..numrows-1.
  //
  int numrows() const
  {
    return NumRows;
  }
  

  //
  // numcols
  //
  // Returns the # of columns in row r.  The indices for these columns
  // are 0..numcols-1.  Note that the # of columns can be different 
  // row-by-row since "jagged" rows are supported --- matrices are not
  // necessarily rectangular.
  //
  int numcols(int r) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");

    return Rows[r].NumCols;
  }


  //
  // growcols
  //
  // Grows the # of columns in row r to at least C.  If row r contains 
  // fewer than C columns, then columns are added; the existing elements
  // are retained and new locations are initialized to the default value 
  // for T.  If row r has C or more columns, then all existing columns
  // are retained -- we never reduce the # of columns.
  //
  // Jagged rows are supported, i.e. different rows may have different
  // column capacities -- matrices are not necessarily rectangular.
  //
  void growcols(int r, int C)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");

    //Return if the number new columns is the same as the current number of columns
	if (C <= Rows[r].NumCols)
		return;

	//Create a new pointer to a new row
	T* newRow = new T[C];

	//Copys everything from old row into the new row
	for (int i = 0; i < Rows[r].NumCols; i++)
		newRow[i] = Rows[r].Cols[i];

	//Initializes the remaining elements to default values
	for (int j = Rows[r].NumCols; j < C; j++)
		newRow[j] = T{};

	//Delete old row and install new one
	delete[] Rows[r].Cols;
	Rows[r].Cols = newRow;
	Rows[r].NumCols = C;
  }


  //
  // grow
  //
  // Grows the size of the matrix so that it contains at least R rows,
  // and every row contains at least C columns.
  // 
  // If the matrix contains fewer than R rows, then rows are added
  // to the matrix; each new row will have C columns initialized to 
  // the default value of T.  If R <= numrows(), then all existing
  // rows are retained -- we never reduce the # of rows.
  //
  // If any row contains fewer than C columns, then columns are added
  // to increase the # of columns to C; existing values are retained
  // and additional columns are initialized to the default value of T.
  // If C <= numcols(r) for any row r, then all existing columns are
  // retained -- we never reduce the # of columns.
  // 
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");

	int counter = 0; //Keeps track of the number of new rows

	if (R <= numrows()) //Try and grow the number of columns and return
	{
		for (int i = 0; i < NumRows; i++)
			growcols(i, C);

		return;
	}

	for (int j = 0; j < NumRows; j++) //Try and grow them if R is bigger than numrows()
		growcols(j, C);

	for (int k = NumRows; k < R; k++)
	{
		//Create a new pointer to a new row
		T* newRows = new T[C];

		//Initializes the remaining elements to default values
		for (int l = 0; l < C; l++)
			newRows[l] = T{};

		//New rows point to right index
		Rows[k].Cols = newRows;
		Rows[k].NumCols = C;

		counter++; 
	}

	NumRows = NumRows + counter; // Reflects the new size
  }


  //
  // size
  //
  // Returns the total # of elements in the matrix.
  //
  int size() const
  {
    // Loop through the rows and columns and add 1 to a counter for every element
	  int counter = 0;

	  for (int i = 0; i < NumRows; i++)
	  {
		  for (int j = 0; j < Rows[i].NumCols; j++)
			  counter++;
	  }

    return counter;
  }


  //
  // at
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M.at(r, c) = ...
  //    cout << M.at(r, c) << endl;
  //
  T& at(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");

    return Rows[r].Cols[c]; // access rth row, then cth column:
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M(r, c) = ...
  //    cout << M(r, c) << endl;
  //
  T& operator()(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return Rows[r].Cols[c]; // access rth row, then cth column:
  }

  //
  // scalar multiplication
  //
  // Multiplies every element of this matrix by the given scalar value,
  // producing a new matrix that is returned.  "This" matrix is not
  // changed.
  //
  // Example:  M2 = M1 * 2;
  //
  mymatrix<T> operator*(T scalar)
  {
    mymatrix<T> result;

    // TO DO: Loop through the original matrix and multiply it by the scalar
	for (int i = 0; i < NumRows; i++)
	{
		for (int j = 0; j < Rows[i].NumCols; j++)
		{
			result.Rows[i].Cols[j] = Rows[i].Cols[j] * scalar;
		}
	}

    return result;
  }

  //
  // matrix multiplication
  //
  // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
  // M2 is the "other" matrix.  This produces a new matrix, which is returned.
  // "This" matrix is not changed, and neither is the "other" matrix.
  //
  // Example:  M3 = M1 * M2;
  //
  // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
  // addition, the sizes of M1 and M2 must be compatible in the following sense:
  // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
  // multiplication can be performed, and the resulting matrix is of size RxC.
  //
  mymatrix<T> operator*(const mymatrix<T>& other)
  {
    //mymatrix<T> result;

	/*
	cout << endl << "This is the size: " << this->size() << endl; //For debugging
	cout << endl << "This is the numrows: " << this->numrows() << endl; //For debugging
	cout << endl << "This is the numcols: " << this->numcols(0) << endl; //For debugging

	cout << endl << "Other is the size: " << other.size() << endl; //For debugging
	cout << endl << "Other is the numrows: " << other.numrows() << endl; //For debugging
	cout << endl << "Other is the numcols: " << other.numcols(0) << endl; //For debugging
	*/

    // both matrices must be rectangular for this to work:
	int thisRows = this->numrows();
	int thisCols = this->numcols(0);
	int thisRec = -1; // Rectangular
	int thisNotRec = -2; // Not Rectangular
	int thisStat = 0; //Status if rectangular or not

	//Loop to check if "this" is a rectangle
	for (int i = 0; i < thisRows; i++)
	{
		if (this->Rows[i].NumCols > thisCols || this->Rows[i].NumCols < thisCols)
		{
			thisStat = thisNotRec;
			break;
		}
	}

	if (thisStat == 0)
		thisStat = thisRec;

	else
		thisStat = thisNotRec;

	int otherRows = other.numrows();
	int otherCols = other.numcols(0);
	int otherRec = -1; // Rectangular
	int otherNotRec = -2; // Not Rectangular
	int otherStat = 0; //Status if rectangular or not

	//Loop to check if "other" is a rectangle
	for (int j = 0; j < otherRows; j++)
	{
		if (other.Rows[j].NumCols > otherCols || other.Rows[j].NumCols < otherCols)
		{
			otherStat = otherNotRec;
			break;
		}
	}

	if (otherStat == 0)
		otherStat = otherRec;

	else
		otherStat = otherNotRec;

	//
	// TODO
	//
	if (thisStat != thisRec)
	   throw runtime_error("mymatrix::*: this not rectangular");
	
	if (otherStat != otherRec)
	   throw runtime_error("mymatrix::*: other not rectangular");

	//
	// Okay, both matrices are rectangular.  Can we multiply?  Only
	// if M1 is R1xN and M2 is NxC2.  This yields a result that is
	// R1xC2.
	// 
	// Example: 3x4 * 4x2 => 3x2
	//

	//
	// TODO
	//
	if (thisCols != otherRows)
	   throw runtime_error("mymatrix::*: size mismatch");

	// Okay, we can multiply:
	mymatrix<T> result(thisRows,otherCols);
	
	// TODO: Loop and pass everything to a function
	for (int i = 0; i < thisRows; i++)
	{
		for (int j = 0; j < otherCols; j++)
		{
			for (int k = 0; k < otherRows; k++)
				result.Rows[i].Cols[j] += this->Rows[i].Cols[k] * other.Rows[k].Cols[j];
		}
	}
	
	return result;
  }


  //
  // _output
  //
  // Outputs the contents of the matrix; for debugging purposes.
  //
  void _output()
  {
    for (int r = 0; r < this->NumRows; ++r)
    {
      for (int c = 0; c < this->Rows[r].NumCols; ++c)
      {
        cout << this->Rows[r].Cols[c] << " ";
      }
      cout << endl;
    }
  }

};
