#include<iostream>
#include<string>

using namespace std;

int main(){
	string line;	
	fstream f;
	int a = f.open("text.txt",'r');	
	while(getline(line,a))
	{	
		cout<<line;				
	}
	return 0;

}



