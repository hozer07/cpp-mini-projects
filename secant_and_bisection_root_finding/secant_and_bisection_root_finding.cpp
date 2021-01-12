#include <iostream>
#include <fstream>
#include <math.h>
#include <new>
#include <sstream>
#include <string>
using namespace std;
class equation{
	double* factors;													//Dynamic array that will store coefficients of polynomial
	double* exponents;													//The exponents of each x term
	int factornumber;													//The variable that will store how many coefficients there are.
	double tolerance,guess1,guess2,root;								
	int bisectioniteration,secantiteration,hybriditeration;				//Iteration numbers for each method
	public:
		void takedata(int,char**);										//Takes the command line arguments and binds them to necessary class properties.
		void printequation();											//Shows the polynomial.
		void intervalbisection();										//Executes bisection method
		double operate(double&);										//calculates f(a) for an a number.
		double sign(const double&);										//Returns 1 for positive numbers and -1 for negative numbers
		void printbisection();											//Prints the results of bisection method
		void secant();													//Executes secant method
		void printsecant();												//Prints the results of secant method
		void hybrid();													//Executes hybrid method
		void printhybrid();												//Prints the results of hybrid method
};
void equation::printhybrid(){
	cout<<"Hybrid result is "<<root<<endl;
	cout<<"Hybrid result found after "<<hybriditeration<<" iterations."<<endl;
}
void equation::hybrid(){
	int cnt=0,kosul=1;
	double a=guess1,b=guess2,fa,fb,c,fc;
	for(int i=0;i<2;i++){												//Executes bisection number just twice for i=0 and i=1
		fa=operate(a);													//Checks if one of the two initial guesses is a root
		if(fa==0){
			root=a;
			kosul=0;
			break;
		}
		fb=operate(b);													//Checks if one of the two initial guesses is a root
		if(fb==0){
			root=b;
			kosul=0;
			break;
		}
		c=(a+b)/2;														
		cnt+=1;	
		fc=operate(c);
		if(fc==0){ 
			root=c;
			kosul=0;
			break;
		}
		if(sign(fa)==sign(fc)){
			a=c;
		}
		else{
			b=c;
		}
		if(b-a<tolerance){
			root=c;
			kosul=0;
		}
		}
		while(kosul==1){												//Executes secant method for the rest.
		if(operate(a)==0){
			root=a;
			cnt+=1;
			kosul=0;
		}
		if(operate(b)==0){
			root=b;
			cnt+=1;
			kosul=0;
		}
		c=b-operate(b)*(b-a)/(operate(b)-operate(a));					//Formula to find (k+1)th x term in the slide.
		cnt+=1;
		if(fabs(c-b)<tolerance){										//Checks if the difference is smaller than tolerance
			root=c;
			kosul=0;
		}
		else{
			a=b;
			b=c;
		}
	}
	hybriditeration=cnt;
}
void equation::printsecant(){
	cout<<"Secant result is "<<root<<endl;
	cout<<"Secant result found after "<<secantiteration<<" iterations."<<endl;
}
void equation::secant(){
	double a=guess1,b=guess2,c,fa,fb;
	int cnt=0,kosul=1;
	while(kosul==1){
		if(operate(a)==0){														//Checks if one of the two initial guesses is a root
			root=a;
			cnt+=1;
			secantiteration=cnt;
			kosul=0;
		}
		if(operate(b)==0){														//Checks if one of the two initial guesses is a root
			root=b;
			cnt+=1;
			secantiteration=cnt;
			kosul=0;
		}
		c=b-operate(b)*(b-a)/(operate(b)-operate(a));							//Formula to find (k+1)th x term in the slide.
		cnt+=1;
		if(fabs(c-b)<tolerance){
			root=c;
			secantiteration=cnt;
			kosul=0;
		}
		else{
			a=b;
			b=c;
		}
	}
}
void equation::printbisection(){
	cout<<"Bisection result is "<<root<<endl;
	cout<<"Bisection result found after "<<bisectioniteration<<" iterations."<<endl;
}
double equation::operate(double& m){											//Function to substitute a number a to the polynomial
	double result=0;
	for(int i=0;i<factornumber;i++){
		result += factors[i]*pow(m,exponents[i]);
	}
	return result;
}
double equation::sign(const double& x){											//Function to check the signs of number to use in bisection method
	if(x<0){
		return -1;
	}
	else{
		return 1;
	}
}
void equation::intervalbisection(){
	int cnt=0,kosul=1;
	double a=guess1,b=guess2,fa,fb,c,fc;
	while(kosul==1){
		fa=operate(a);	
		if(fa==0){																//Checks if one of the two initial guesses is a root
			root=a;
			kosul=0;
		}
		fb=operate(b);
		if(fb==0){																//Checks if one of the two initial guesses is a root
			root=b;
			kosul=0;
		}
		c=(a+b)/2;															
		cnt+=1;
		fc=operate(c);
		if(fc==0){																//Checks if (a+b)/2 is a root.
			root=c;
			kosul=0;
		}
		if(sign(fa)==sign(fc)){													//Main formula of bisection method. If f(a) and f(c) have same signs, a gets closer to b.
			a=c;
		}
		else{																	//If f(a) and f(b) have different signs, b gets closer to a.
			b=c;
		}
		if(fabs(b-a)<tolerance){												//Checks if difference is smaller than tolerance.
			root=c;
			kosul=0;
		}
		}
		bisectioniteration=cnt;
	}
void equation::takedata(int b,char** a){										//Takes the command line arguments and binds them to coefficients,guesses and tolerance.
	factornumber=b-4;
	factors=new double[factornumber];
	for(int i=0;i<factornumber;i++){
		factors[i]=atof(a[i+1]);
	}
	exponents=new double[factornumber];
	for(int i=0;i<factornumber;i++){
		exponents[i]=factornumber-1-i;
	}
	tolerance=atof(a[b-1]);
	guess1=atof(a[b-3]);
	guess2=atof(a[b-2]);
}
void equation::printequation(){													//Prints the polynomial.
	for(int i=0;i<factornumber;i++){
		if(factors[i]!=0){
		cout<<"("<<factors[i]<<"x^"<<exponents[i]<<")";
		if(i<factornumber){
			cout<<"+";
		}
		}
	}
	cout<<"\b=0"<<endl;
};
int main(int argc, char** argv) {
	equation eq1;
	eq1.takedata(argc,argv);								
	eq1.printequation();
	eq1.intervalbisection();
	eq1.printbisection();
	eq1.secant();
	eq1.printsecant();
	eq1.hybrid();
	eq1.printhybrid();
	return 0;
}
