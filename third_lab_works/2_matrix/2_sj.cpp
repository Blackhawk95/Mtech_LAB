#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<algorithm>
#include <iomanip>
#define P 1000
#define Q 1000
using namespace std;
bool sortbysec(const pair<int,int> &a,
              const pair<int,int> &b)
{
    return (a.second > b.second);
}
void swap(int mat[P][Q], int row1, int row2,
          int col)//mat row i rank
{
    for (int i = 0; i < col; i++)
    {
        int temp = mat[row1][i];
        mat[row1][i] = mat[row2][i];
        mat[row2][i] = temp;
    }
}
void ech(int mat[P][Q],int R,int C)
{
    int rank = C;
 
    for (int row = 0; row < rank; row++)
    {
        if (mat[row][row])
        {
           for (int col = 0; col < R; col++)
           {
               if (col != row)
               {
                 double mult = (double)mat[col][row] /
                                       mat[row][row];
                 for (int i = 0; i < rank; i++)
                   mat[col][i] -= mult * mat[row][i];
              }
           }
        }

        else
        {
            bool reduce = true;

            for (int i = row + 1; i < R;  i++)
            {
                if (mat[i][row])
                {
                    swap(mat, row, i, rank);
                    reduce = false;
                    break ;
                }
            }
            if (reduce)
            {
                rank--;
                for (int i = 0; i < R; i ++)
                    mat[i][row] = mat[i][rank];
            }
            row--;
        }
    }
}
int main()
{
	//INITIALIZATION
    	std::vector< pair <string,int> > words;
    	std::vector< pair <int,int> > r;
	std::ifstream file("text2.txt");
	std::ifstream file1("text2.txt");
	std::string theLine,line;
	int l=0,i,count=0,var;
	int mat[1000][1000];
	int arr[1000];


	//READING OF FILE 1
	while (file>>theLine)
	{
	  arr[count]+=1;
	  for (int j=0; j< theLine.length(); ++j)
	  {
	    theLine[j] = tolower(theLine[j]);
	  }
	  if(theLine=="."){
		count++;
		continue;}
	  for(i=0;i<words.size();i++)
	  {
	  	   if(words[i].first == theLine)
		   {
			l++;
			break;
		   }
	  }
	  if (l==0)
	  {
        	words.push_back( make_pair(theLine,1) );
	  }
	  l=0;
	}

	for(int i=0;i<count;i++)
	{
		for(int j=0;j<arr[i];j++)
		{	
			int q=0;
			int a=words.size();
			file1>>line;
			if(line==".")
				continue;
			for (int j=0; j< line.length(); ++j)
			{
			    line[j] = tolower(line[j]);
			}
			while(a!=0)
			{
				if(words[q].first==line)
					mat[i][q]=1;
				a--;
				q++;
			}
		}
	}
	ech(mat,count,words.size());
	
	for(int i=0;i<count;i++)
	{
		for(int j=0;j<words.size();j++)
		{	
			cout<<mat[i][j]<<"\t";
		}
		cout<<endl;
	}
	for(int i=0;i<words.size();i++)
	{
	int rank=0;
		for(int j=0;j<count;j++)
		{	
			if(mat[j][i]!=0)
				rank++;				
		}
		r.push_back( make_pair(i,rank) );
	}
	sort(r.begin(), r.end(), sortbysec);
	cout<<setw(15)<<"COLUMN\n";
	 	for (int i=0; i<r.size(); i++)
		    {
			cout<<setw(15)<<r[i].first<<endl;

		    }
	return 0;
}
