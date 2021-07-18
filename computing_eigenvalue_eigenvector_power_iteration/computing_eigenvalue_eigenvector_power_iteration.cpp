// HakanOzerProje2.cpp : Defines the entry point for the console application.
#include <iostream>
#include <fstream>
#include <math.h>
#include <new>
#include <sstream>
#include <string>
using namespace std;
class matrix{																//Defining the matrix class and its properties
		int row,column;												
		double** content;													//2D array that will store matrix elements
		double** eigenvector;												//Dominant eigenvector
		char *inputfile , *outputfile;										//Pointers that store names of input and output files
		double tolerance;
		double eigenvalue1,eigenvalue2;
	public:
		matrix(char *a,char *b,char *c){									//Constructor. a,b and c will come from command line parameters
			inputfile=a;													
			tolerance=atof(b);
			outputfile=c;
			findrow();														
			column=row;
			content  = new double* [row];
			for(int i=0; i<row; ++i){
				content[i] = new double[column];
				}
			takesquareinput();
			}
		matrix(int rowsize,int columnsize){									//Constructor for manually declaring matrix objects.This form will be used when we know
			row=rowsize;													//row and columns.
			column=columnsize;
			content  = new double* [row];
			for(int i=0; i<row; ++i){
				content[i] = new double[column];
				}
			}
		void takesquareinput();												//The function that takes matrix elements from text and copies them to matrix object.
		void setvalues();													//The function to assign matrix values manually.
		void showmatrix();													//The function that show any matrix object.
		void findrow();														//The function that calculates the row number of the matrix in text
		matrix operator * (const matrix&);									//Operation overload for matrix multiplication
		matrix operator - (const matrix&);									//Operation overload for matrix substraction
		void iteration();													//Iteration function.
		double infnorm();													//Returns infinite norm of vector(matrix actually) object.
		void normalizer();													//Divides all elements of a vector to infinite norm.
		void showdominants();												//The function that shows dominant eigenvalue and eigenvector after iteration.
		void secondeigen();													//Executes deflation and prints second eigenvalue
		matrix transpose();													//Transpose operation of any matrix
		void print();														//Prints to output file
};

void matrix::print(){
	ofstream out(outputfile);
	out<<"Dominant eigenvalue: "<<eigenvalue1<<endl;						//Just prints the output to text.
	out<<"Corresponding eigenvector:"<<endl;
	for(int i=0;i<row;i++){
		out<<eigenvector[i][0]<<endl;
	}
	out<<"Second eigenvalue: "<<eigenvalue2<<endl;
}

matrix matrix::transpose(){													//Standard transpose operation for any matrix.
	matrix result(column,row);
	for(int i=0;i<row;i++){
		for(int j=0;j<column;j++){
			result.content[j][i] = content[i][j];
		}
	}
	return result;
}
void matrix::secondeigen(){									//Deflation
	matrix I(row,column),B(row-1,column-1),H(row,column),transposev(1,column),v(column,1),vvt(column,column),vtv(1,1),e1(column,1),tempA(row,column);//row=column
	tempA.content=content;									//Copies content of the object that is used to call the function to a new object
	B.tolerance=tolerance;									//Tolerance is same for all square matrices, copied from object
	double norm2=0;									
	e1.content[0][0]=1;
	for(int i=1;i<column;i++){								//Generates e1 vector
		e1.content[i][0] = 0;
	}
	for(int i=0;i<row;i++){									//Generates identity matrix
		for(int j=0;j<column;j++){
			I.content[i][j]=0;
		}
	}
	
	for(int i=0;i<row;i++){
		I.content[i][i]=1;
	}
	for(int i=0;i<row;i++){									
		norm2 += eigenvector[i][0] * eigenvector[i][0];
	}
	norm2 = sqrt(norm2);									//Norm 2 of eigenvector
	if(eigenvector[0][0]>0){
		e1.content[0][0] = e1.content[0][0] * (-1) * norm2;
	}
	else if(eigenvector[0][0]<0){
		e1.content[0][0] *= norm2 * e1.content[0][0];
	}
	for(int i=0;i<column;i++){
		v.content[i][0] = eigenvector[i][0] - e1.content[i][0];
	}
	transposev = v.transpose();
	vvt = v*transposev;
	vtv = transposev*v;
	for(int i=0;i<column;i++){
		for(int j=0;j<column;j++){
			vvt.content[i][j] = vvt.content[i][j]*(2)/vtv.content[0][0];
		}
	}
	H = I - vvt;
	H = (H * tempA) * H;								//I just basically implemented householder transformation until here, but did not created new matrices
	for(int i=1;i<row;i++){								//rather overwrited them.
		for(int j=1;j<column;j++){
			B.content[i-1][j-1]=H.content[i][j];
		}
	}
	B.iteration();										//Executed iteration for new square matrix in the deflation method.
	eigenvalue2=B.eigenvalue1;
	cout<<"Second eigenvalue : "<<B.eigenvalue1<<endl;
}
matrix matrix::operator- (const matrix& param){			//General matrix substraction
	matrix temp(row,column);
	for(int i=0;i<row;i++){
		for(int j=0;j<column;j++){
			temp.content[i][j] = content[i][j] - param.content[i][j];
			}
	}
	return temp;
}
	

void matrix::takesquareinput(){							//This function just basically takes square matrix input from text when called.
	findrow();
	column=row;
	content  = new double* [row];
	for(int i=0; i<row; ++i){
	content[i] = new double[column];
	}
	ifstream inmatrix(inputfile);						
	if(inmatrix.is_open()){
		for(int m=0;m<row;m++){
			for(int y=0;y<column;y++){
				inmatrix>>content[m][y];
				}
				}
		inmatrix.seekg (0, ios::beg);
		inmatrix.close();
	}
}

void matrix::showdominants(){							//Shows dominant eigenvalue and eigenvector after iteration.
	cout<<"Dominant eigenvalue: "<<eigenvalue1<<endl;
	cout<<"Corresponding eigenvector:"<<endl;
	for(int i=0;i<row;i++){
		cout<<eigenvector[i][0]<<endl;
	}
}
void matrix::normalizer(){								//Divides all elements of a vector to its infinite norm
	double divider = infnorm();
	for(int i=0;i<row;i++){
		content[i][0] = content[i][0] / divider;
	}
}

double matrix::infnorm(){								//Returns infinite norm of a vector
	double max = 0;
	for (int i=0;i<row;i++){
		if(fabs(content[i][0])>max){
			max=fabs(content[i][0]);
		}
	}
	return max;
}
void matrix::iteration(){								
	matrix approxeigenvector(row,1),C(row,column),check(row,1);
	C.content=content;									//Copies content of the object that is used to call the function to a new object
	approxeigenvector.content[0][0]=1;					//I started iteration with [1 0 0 0] for a 4x4 matrix.First element is 1, rest is 0.
	for(int i=1;i<row;i++){
		approxeigenvector.content[i][0]=0;
	}
	double normx,norm;									//Norms variables whose difference will be compared to tolerance
	int kosul=1;
	while(kosul==1){
		norm = approxeigenvector.infnorm();				//Firstly, took the norm of the (n-1)th vector.
		check.content=approxeigenvector.content;		//Copied to another vector.This "check" vector will be used to determine if eigenvector is positive or negative.
		approxeigenvector.normalizer();					//Normalized before multiplication
		approxeigenvector = C*approxeigenvector ;		//Matrix multiplication to get new vector
		normx = approxeigenvector.infnorm();			//Norm of new vector
		if(fabs(normx-norm)<tolerance){					//Comparison of two norms
			if(check.content[0][0]*approxeigenvector.content[0][0]<0){		//This if will decide if eigenvalue is + or - signed. I noticed that if eigenvalue is-
				eigenvalue1=-1*approxeigenvector.infnorm();					//signs of vector elements change after every multiplication. So if (n-1)th vector's 
			}																//first element is - and nth vector's first element is +,then eigenvalue is - signed.
			else{eigenvalue1=approxeigenvector.infnorm();
			}
			approxeigenvector.normalizer();
			eigenvector = approxeigenvector.content;
			kosul=0;
			}
	}
}
	
matrix matrix::operator* (const matrix& param){			//General matrix multiplication
	matrix temp(row,param.column);
	for(int i=0;i<row;i++){
		for (int j = 0; j < param.column; j++){
			temp.content[i][j] = 0;
		}
	}
		for (int b = 0; b < param.column; b++){
			for (int i = 0; i < row; i++){
				for (int j = 0; j < column; j++){
					temp.content[i][b] += content[i][j] * param.content[j][b];
				}
		}
	}
	return temp;
}

void matrix::findrow(){									//Finds the row number of the square matrix in the text
	ifstream inmatrix(inputfile);				
	string line;
	int matrixrow=0;							
	if(inmatrix.is_open()){				
		while (inmatrix.eof()==0){
			getline(inmatrix,line);
			matrixrow+=1;
	}
	inmatrix.seekg (0, ios::beg);		
	}
	row=matrixrow;
	inmatrix.close();
}

void matrix::setvalues(){								//Manual input for matrices. This can be used when a matrix is declared like a(5,7) or b(23,10)
	cout << "Enter values for matrix:" << endl;
	for(int i=0;i<row;i++){
		for (int j = 0; j < column; j++){
			cin >> content[i][j];
			}
		}
}
void matrix::showmatrix(){								//Shows the content of the object that is used to call the function.
	for(int m=0;m<row;m++){
		for(int y=0;y<column;y++){
				cout<<content[m][y]<<" ";
			}
			cout<<endl;
		}
	}

int main(int argc, char** argv) {
	matrix A(argv[1],argv[2],argv[3]);
	A.showmatrix();				
	A.iteration();
	A.showdominants();
	A.secondeigen();
	A.print();
	}
