// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>


using namespace std;      
bool cmp(pair<string,int> const & a, pair<string,int> const & b){
	return a.second!=b.second ? a.second>b.second:a.first>b.first;
}
int main () {
  string line;
  ifstream myfile1 ("example2.txt");
  ifstream myfile2 ("example1.txt");	 			 
  //vector things
  vector <string> g1;
  vector <string> :: iterator i1;
  vector <string> :: reverse_iterator ir1;
	
  vector <string> g2;
  vector <string> :: iterator i2;
  vector <string> :: reverse_iterator ir2;	


  if (myfile1.is_open())
  {
    while ( getline (myfile1,line) )
    { //output each file
      //cout << line << '\n';
      istringstream iss(line);
      copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(g1));
    }

myfile1.close();
//output the contents of vector
//for (i1 = g1.begin();i1!=g1.end();++i1)
   //cout<< *i1 << '\n';
   //cout<<' ';
}
 else cout << "Unable to open file";

	//Second file
	line="";
	if(myfile2.is_open())
{	
	while ( getline (myfile2,line) )
    {
      //cout << line << '\n';
        istringstream iss2(line);
      copy(istream_iterator<string>(iss2),istream_iterator<string>(),back_inserter(g2));
        //g2.push_back(line);
    }
    myfile2.close();
   //output the contents of vec 2
    //for (i2 = g2.begin(); i2!=g2.end(); ++i2)
   //cout << *i2 << '\n';
   //cout<<' ';  
}
  else cout << "Unable to open file"; 

//create third vector
vector<string> g3;
vector<string> :: iterator i3;
vector<string> :: iterator i4;

//sort 1st and 2nd vector for using set_symmetric_difference()
sort(g1.begin(), g1.end());
sort(g2.begin(), g2.end());

//to find difference
set_symmetric_difference(g1.begin(), g1.end(), g2.begin(), g2.end(), back_inserter(g3));

//code to avoid common		
 for (i3 = g3.begin();i3!=g3.end();++i3)
	{	 for (i1 = g1.begin(); i1!=g1.end(); ++i1)
	        { 	if(*i1==*i3)
			{	for(i2 = g2.begin(); i2!=g2.end(); ++i2)
				{	if(*i2==*i3)
					{
						g3.erase(i3);		
					}
      				}
      			}         
		}
	}

//map to rank and sort
unordered_map<string,int> mymap;
int count;

//done the count and created a map with count 
for (i3 = g3.begin(); i3!=g3.end(); ++i3)
  {        //cout << *i3 << '\n';
	 count=0;
         for(i4 = g3.begin();i4!=g3.end(); ++i4)
	{	if(*i3==*i4)
			count++;
		}
	mymap.insert(make_pair(*i3,count));
}
	
//Sort according to the rank
vector<pair<string,int>> ele(mymap.begin(),mymap.end());
sort(ele.begin(),ele.end(),cmp);

//
for(vector<pair<string,int>>::iterator it2=ele.begin();it2!=ele.end();++it2)
        cout<< it2->first <<" " << it2->second << '\n';

  return 0;
}
