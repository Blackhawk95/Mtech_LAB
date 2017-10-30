#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<iterator>
#include <algorithm> // for std::swap
#include <cstddef>
#include <cassert>
#include <unordered_map>
#include<iomanip>

using namespace std;

using namespace std;      
bool cmp(pair<int,int> const & a, pair<int,int> const & b){
	return a.second!=b.second ? a.second<b.second:a.first<b.first;
}

int main(){

	vector <string> g1;                                                 
  	vector <string> :: iterator i1; 
	
	vector <string> g2;
	vector <string> :: iterator i2;

	int m=0,n=0;
	//char file[] ="tests.txt";
	char file[] ="text2.txt";//given test case
	string line;	
	ifstream f (file);
	if(f.is_open()){	
		while(getline(f,line))
		{	                
      			istringstream iss(line);      
      			copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(g2));
				m++;//for counting no of rows
		}
	}
	f.close();

	for (i1 = g2.begin();i1!=g2.end();++i1)
	{	for (i2 = g2.begin();i2!=g2.end();++i2)
		{	if(i1!=i2)
			{	if(*i1==*i2)
					g2.erase(i2);
			}
		}
	}
	
	//for counting no of columns
	for (i1 = g2.begin();i1!=g2.end();++i1)
   	{	//cout<< *i1 << '\n'; //to show all words
		n++;		
	}

	cout<<"m : "<<m<<" n : "<<n<<endl; //row coumn indices
	int i=0,j=0;
	
	//two dim vector array
	vector<vector<float> > a(m, vector<float>(n));
	
	//read file second time
	ifstream f2 (file);
	if(f2.is_open())
	{	while(getline(f2,line))
			{	        
      				istringstream iss(line);      
      				copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(g1));

				for (i2 = g2.begin();i2!=g2.end();++i2)
   				{	int count=0;
					for (i1 = g1.begin();i1!=g1.end();++i1)
   					{	
						if(*i1==*i2)
						{	count++;
							
						}
					}
				a[i][j]=count;				
				
				cout<<i<<'\t'<<j<<endl;
				j++;//array column
				}
				i++;//array row
				j=0;//reseting array column

				g1.erase(g1.begin(),g1.end());	//clearing everything in a temp array	
			}
	}	
	
	f2.close();

	//to print matrics
	for(int k=0;k<m;k++)
	{
    		for(int l=0;l<n;l++)
    		{
        		cout<<a[k][l]<<" ";
    		}
    		cout<<endl;
	}

	
	//matrics operation -- based on rank program from gfg
	int lead = 0; //diag

    while (lead < m) {
        float d, mu;
	
        for (int r = 0; r < m; r++) { // for each row ...
            /* calculate divisor and multiplier */
            d = a[lead][lead];
            mu = (float)( a[r][lead] / a[lead][lead]);
			if(d!=0){
            	for (int c = 0; c < n; c++) { // for each column ...
					if (r == lead)
						a[r][c] /= d;               // make pivot = 1
                	else
						a[r][c] -= a[lead][c] * mu;  // make other = 0           
				}
			}
			else{
				//row swap if diagonal zero is detected
				bool reduce = true;
				for( int i3=r+1;i3<m;i3++)
				{	if(a[i3][r])
					{	for(int i5=0;i5<n;i5++)
						{	int temp=a[r][i5];
							a[r][i5]=a[i3][i5];
							a[i3][i5]=temp;
						} 
						reduce = false;
						break;
					}
				}
							
			}	
        }

        lead++;
 		//cout<<lead<<": lead "<<endl;
    }
	cout<<endl<<"THE ECHELON FORM IS:"<<endl;;
	//prinitng
	for(int k=0;k<m;k++)
	{
    		for(int l=0;l<n;l++)
    		{	if(a[k][l]==-0)
					a[k][l]=0; //simply converting -0 to 0
							
        		cout<<setw(4)<<a[k][l];//print echloen
    		}
    		cout<<endl;
	}

	//rank the matrics
	unordered_map<int,int> B;
	int count;

	//vector<vector<int> > B(2, vector<int>(n));
	for (int c = 0; c < n; c++) {
		for (int r = 0; r < m; r++) {
				if(a[r][c]==0)
					count=count+1;
		}
		B.insert(make_pair(c,count));
		count=0;	
	}	
	
	//Sort according to the rank
	vector<pair<int,int> > ele(B.begin(),B.end());
	sort(ele.begin(),ele.end(),cmp);

	cout<<"RANK"<<'\t'<<"COLUMN"<<'\t'<<"NO OF ZERO's"<<endl;
	//print
	int rank;
	for(vector<pair<int,int> >::iterator it2=ele.begin();it2!=ele.end();++it2,rank++)
        	cout<<rank+1<<'\t'<< it2->first <<'\t'<< it2->second << '\t'<<endl;

	return 0;

}



