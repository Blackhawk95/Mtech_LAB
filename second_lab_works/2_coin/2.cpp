#include<iostream>

using namespace std;
//m is the size, n is the amt
int count(int coin_array[],int size_of_coin_array,int amt){

	if(amt==0){
		return 1;
	}
	if(amt<0){
		return 0;
	}
	if(size_of_coin_array <=0 && amt>=1){
	  return 0;
	}
	      

	return count(coin_array,size_of_coin_array-1,amt) + count(coin_array,size_of_coin_array,amt-coin_array[size_of_coin_array-1]);
}

int main(){
        int i,j,amt;
	int coin_array[]={1,5,10,20,50,100};
	int size_of_coin_array = sizeof(coin_array)/sizeof(coin_array[0]);
	cout<<"Enter Amt: ";
	cin>>amt;
	cout<<"Possible No of solution: ";
	cout<<count(coin_array,size_of_coin_array,amt)<<endl;
	return 0;
}
