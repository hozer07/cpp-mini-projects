#include <iostream>
#include <fstream>
#include <math.h>
#include <new>
#include <sstream>
#include <string>
using namespace std;
int main() {
	ifstream matrix("A.txt");			//Taking input matrix A. Since we need to read from it i used ifstream.
	string line;
	int size=0,vectorsize=0;			//Initialization of the variables that will store matrix and vector size.
	int i,j,k,r,m,a;					//Iterators
	double multiplier,sum,maxabsolute,temp;
	if(matrix.is_open()){				// Opening matrix A and finding the size by reading line by line.
		while (matrix.eof()==0){
			getline(matrix,line);
			size+=1;
	}
	matrix.seekg (0, ios::beg);			//Initializing the cursor position for next reading.
	
	}
	ifstream vector("b.txt");			 
	if(vector.is_open()){				//The same process of line by line reading in order to find vector size.
		while(vector.eof()==0){
			getline(vector,line);
			vectorsize+=1;
		}
		vector.seekg (0, ios::beg);
		
	}
	double** A = new double* [size];		//Code that generates dynamic 2D array that will store input matrix A.
	for(i = 0; i < size; ++i){
		A[i] = new double[size];
	}
	cout<<"Matrix A is:"<<endl;			//Shows the input matrix A.
	if(matrix.is_open()){
		for(int m=0;m<size;m++){
			for(int y=0;y<size;y++){
				matrix>>A[m][y];
				cout<<A[m][y]<<" ";
			}
			cout<<"\n";
		
	}matrix.close();
	
	}
	double* b = new double [vectorsize];	//Code that generates the dynamic array that will store input vector b.
	cout<<"\nVector b is:"<<endl;
	if(vector.is_open()){
		for(i=0;i<vectorsize;i++){		//Shows the input matrix b.
			vector>>b[i];
			cout<<b[i]<<endl;
		}
		vector.close();
		
	}
	double* vecx = new double [vectorsize];//Code that generates dynamic array that will store output vector x.
	//Partial pivoting process
	for(i=0;i<size;i++){				//Looping over columns.
		r=i;							//Variable that stores the row number of pivot.
		maxabsolute=fabs(A[i][i]);		//Initializes the diagonal element of the particular row as pivot.
		for(a=i+1;a<size;a++){			//Looping over the rows under pivot row.
			if(fabs(A[a][i])>maxabsolute){  //Code that checks the rows of the particular column for another pivot.If it finds a number that has higher absolute value than pivot, makes it new pivot.
				maxabsolute=fabs(A[a][i]);
				r=a;					//Row number of pivot element.
			}
		}
		if(r!=i){						//Checks if there is a new pivot.
			for(m=i;m<size;m++){
				temp=A[r][m];				//Makes the row exchanges of matrix A if it finds a new pivot.
				A[r][m]=A[i][m];
				A[i][m]=temp;
				
			}
			temp=b[r];						//Makes the row exchanges of vector b.
			b[r]=b[i];
			b[i]=temp;
			
		}
		
//		for(int x=0;x<vectorsize;x++){		//The checkpoint i controlled if the code makes row exchange correctly. I compared results here with my calculations.
//			cout<<b[x]<<endl;
//		}
//		cout<<"\n";
		
//		for(int x=0;x<size;x++){
//			for(int y=0;y<size;y++){
//				cout<<A[x][y]<<" ";
//			}
//			cout<<"\n";
//		}
//		cout<<"\n";
		
		for(k=i+1;k<size;k++){							//Looping over rows to make row reduction.
			multiplier=A[k][i]/A[i][i];					//Factor that is used for row reduction. First row element over pivot.
			for(m=0;m<size;m++){
				A[k][m]=A[k][m]-multiplier*A[i][m];		//Replaces the particular row's elements with the new ones one by one.
//				for(int x=0;x<size;x++){					//I used this comment as a checkpoint to look if my code makes row reduction correctly.
//			for(int y=0;y<size;y++){
//				cout<<A[x][y]<<" ";
//			}
//			cout<<"\n";
//		}
//      													//	I used this comment as a checkpoint to look if my code makes row reduction of b correctly.
//		for(int x=0;x<vectorsize;x++){
//			cout<<b[x]<<endl;
//		}
//		cout<<"\n";
			
		}
		b[k]=b[k]-multiplier*b[i];							// Replaces the row elements of vector b.
	}
}
	cout<<"\n"<<"Matrix A after elimination and pivoting:"<<endl;
	for(int x=0;x<size;x++){
		for(int y=0;y<size;y++){
			if(fabs(A[x][y])>(0.0000001)){					//Rounds the elements that has lower absolute value than 10^-7 to 0 when it shows upper triangular matrix.
				cout<<A[x][y]<<" ";
				}
			else{
				A[x][y]=0;
				cout<<0;
			}
			cout<<" ";
			}
		cout<<"\n";
		}
		cout<<"\nVector b after same process:"<<endl;
		int x;
		for(x=0;x<vectorsize;x++){
			if(fabs(b[x])>(0.0000001)){
				cout<<b[x]<<"\n";}
			else{
				b[x]=0;
				cout<<0<<"\n";
			}
		}
//		for(int x=0;x<size;x++){                      //I used this comment as a checkpoint
//			for(int y=0;y<size;y++){				  //to look if the program shows augmented matrix correctly.
//				cout<<A[x][y]<<" ";
//			}
//			cout<<"\n";}		
    for(int k=size-1;k>=0;k--){					//Implementing the back substution algorithm. I made use of that example: https://youtu.be/c0i8hFsOV3A?t=4m18s
      	sum=0.000000;
      	for(j=k+1;j<size;j++){
      		sum=sum+A[k][j]*vecx[j];
      }
        vecx[k]=(b[k]-sum)/A[k][k];
      }
      int checksingular=0;
      for(i=0;i<size;i++){						//Checks if there is a 0 element on the diagonal,i.e. if the matrix is singular.
      	if(A[i][i]==0){
      		checksingular+=1;
      		break;
		  }
	  }
      if(checksingular!=0){
      	cout<<"\nError! Matrix A is singular."<<endl;
	  }
	  else{
	  
      cout<<("\nSolution is:\n");
      for(int k=0;k<size;k++){						//Gives output vector x.
      	cout<<vecx[k]<<endl;
      }
      ofstream output("x.txt");
      for(int i=0;i<size;i++){						//Writes the content to x.txt
      	output<<vecx[i]<<endl;
      }
      if(size==2){									//Calculation of condition numbers for 2 by 2 matrices
      		ifstream matrix;
      		matrix.open("A.txt");
      		for(int i=0;i<2;i++){						//Reads the first A matrix again.
      			for(int j=0;j<2;j++){
      				matrix>>A[i][j];
				  }
			  }
			matrix.close();
	  		double maxrow=0,maxcolumn=0,rowsum=0,columnsum=0,maxrowinv=0,maxcolumninv=0;	//Computing condition numbers for 2x2 matrices.
      		for(int i=0;i<2;i++){
      			for(int j=0;j<2;j++){				//Calculates row and column sums
      				columnsum+=fabs(A[j][i]);		
					rowsum+=fabs(A[i][j]);     				
				  }
				if(columnsum>maxcolumn){			//Finds the max column sum
      					maxcolumn=columnsum;
					  }
				columnsum=0;
				if(rowsum>maxrow){					//Finds the max row sum
      					maxrow=rowsum;
					  }
				rowsum=0;
			  }
			double det;							
			det=A[1][1]*A[0][0]-A[1][0]*A[0][1];	//Determinant of 2x2 matrix
			temp=A[0][0];							//Rearrangement of numbers to calculate inverse matrix A.
			A[0][0]=A[1][1];
			A[1][1]=temp;
			A[1][0]*=-1;
			A[0][1]*=-1;
			
			for(int i=0;i<2;i++){					//Placing the numbers to inverse A.
				for(j=0;j<2;j++){
					A[i][j]=A[i][j]/det;
				}
			}
			for(int i=0;i<2;i++){						//Same process to calculate norms of inverse A.
      			for(j=0;j<2;j++){
      				columnsum+=fabs(A[j][i]);
					rowsum+=fabs(A[i][j]);     				
				  }
				if(columnsum>maxcolumninv){
      					maxcolumninv=columnsum;
					  }
				columnsum=0;
				if(rowsum>maxrowinv){
      					maxrowinv=rowsum;
					  }
				rowsum=0;
			  }
			  cout<<"\nNorm 1 condition number is "<<maxcolumn*maxcolumninv<<endl;
			  cout<<"Norm infinite condition number is "<<maxrow*maxrowinv<<endl;
			}
    // Now we can clear the memory since we do not have anything to do with them.
  }
  for(int i = 0; i < size; ++i) {
    delete [] A[i];
}
delete [] A;
delete [] b;
delete [] vecx;
  		
		return 0;
      }
