#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<iterator>
#include <algorithm> // for std::swap
#include <cstddef>
#include <cassert>
using namespace std;



void swap(vector<float,vector<float> > *mat, int row1, int row2, int col)
{
    for (int i = 0; i < col; i++)
    {
        vector<float,vector<float> > *temp = *mat[row1][i];
        *mat[row1][i] = *mat[row2][i];
        *mat[row2][i] = *temp;
    }
}

/* function for finding rank of matrix */
int rankOfMatrix(vector<float,vector<float> > *mat,int R,int C)
{
    int rank = C;
 
    for (int row = 0; row < rank; row++)
    {
        // Before we visit current row 'row', we make
        // sure that *mat[row][0],....*mat[row][row-1]
        // are 0.
 
        // Diagonal element is not zero
        if (*mat[row][row]!=0)
        {
           for (int col = 0; col < R; col++)
           {
               if (col != row)
               {
                 // This makes all entries of current
                 // column as 0 except entry '*mat[row][row]'
                 double mult = (double)*mat[col][row] /
                                       *mat[row][row];
                 for (int i = 0; i < rank; i++)
                   *mat[col][i] -= mult * *mat[row][i];
              }
           }
        }
 
        // Diagonal element is already zero. Two cases
        // arise:
        // 1) If there is a row below it with non-zero
        //    entry, then swap this row with that row
        //    and process that row
        // 2) If all elements in current column below
        //    *mat[r][row] are 0, then remvoe this column
        //    by swapping it with last column and
        //    reducing number of columns by 1.
        else
        {
            bool reduce = true;
 
            /* Find the non-zero element in current
                column  */
            for (int i = row + 1; i < R;  i++)
            {
                // Swap the row with non-zero element
                // with this row.
                if (*mat[i][row])
                {	
							
                    swap(&*mat, row, i, rank);
                    reduce = false;
                    break ;
                }
            }
 
            // If we did not find any row with non-zero
            // element in current columnm, then all
            // values in this column are 0.
            if (reduce)
            {
                // Reduce number of columns
                rank--;
 
                // Copy the last column here
                for (int i = 0; i < R; i ++)
                    *mat[i][row] = *mat[i][rank];
            }
 
            // Process this row again
            row--;
        }
 
       // Uncomment these lines to see intermediate results
       // display(*mat, R, C);
       // printf("\n");
    }
    return rank;
}

int main(){

	vector <string> g1;                                                 
  	vector <string> :: iterator i1; 
	
	vector <string> g2;
	vector <string> :: iterator i2;

	int m=0,n=0;
	char file[] ="text1.txt";
	string line;	
	ifstream f (file);
	if(f.is_open()){	
		while(getline(f,line))
		{	                
      				istringstream iss(line);      
      				copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(g1));
				for (i1 = g1.begin();i1!=g1.end();++i1)
   				{	g2.push_back(g1.back());
					g1.pop_back();
				}
				//g1.erase(g1.begin(),g1.end());
				//if unique
				//g2.insert( g2.end(), g1.begin(), g1.end() );

				for (i1 = g2.begin();i1!=g2.end();++i1)
   				{	for (i2 = g2.begin();i2!=g2.end();++i2)
   					{	if(i1!=i2)
						{	if(*i1==*i2)
								g2.erase(i1);
						}
					}
				}
				
				//g2.insert( g2.end(), g1.begin(), g1.end() );
				//for (i1 = g1.begin();i1!=g1.end();++i1)
   				//{	cout<< *i1 << '\n';
				//}
				g1.erase(g1.begin(),g1.end());
				m++;
						
		}
	
	}
	f.close();
	
	cout<<"BLAAAH             "<<endl;

	for (i1 = g2.begin();i1!=g2.end();++i1)
   	{	cout<< *i1 << '\n';
		n++;		
	}
	cout<<"m : "<<m<<" n : "<<n<<endl; 
	int i=0,j=0;
	vector<vector<float> > a(m, vector<float>(n));
	ifstream f2 (file);
	if(f2.is_open())
	{	while(getline(f2,line))
			{	        
      				istringstream iss(line);      
      				copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(g1));
		
				//g1.erase(g1.begin(),g1.end());
				//if unique
				//g2.insert( g2.end(), g1.begin(), g1.end() );
				for (i2 = g2.begin();i2!=g2.end();++i2)
   				{	int count=0;
					for (i1 = g1.begin();i1!=g1.end();++i1)
   					{	
						if(*i1==*i2)
						{	count++;
							
						}
						//cout<<"HELLO"<<endl;
					}
				a[i][j]=count;				
				
				cout<<i<<'\t'<<j<<endl;
				j++;
				}
				i++;
				j=0;
				//g2.insert( g2.end(), g1.begin(), g1.end() );
				//for (i1 = g1.begin();i1!=g1.end();++i1)
   				//{	cout<< *i1 << '\n';
				//}
				g1.erase(g1.begin(),g1.end());		
			}
	}	
	
	f2.close();

	for(int k=0;k<m;k++)
	{
    		for(int l=0;l<n;l++)
    		{
        		cout<<a[k][l]<<" ";
    		}
    		cout<<endl;
	}

	float **b = new float*[m];  // surrogate
    for (size_t i = 0; i < m; ++i)
    {
        b[i] = new float[n];
		//b[i] = a[i];
    }
	//matrics operation -- rank program from gfg
	printf("Rank of the matrix is : %d",
         rankOfMatrix(&a,m,n));

	for(int k=0;k<m;k++)
	{
    		for(int l=0;l<n;l++)
    		{
        		cout<<a[k][l]<<" ";
    		}
    		cout<<endl;
	}


	return 0;

}



