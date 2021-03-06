#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
//priority_queue <node,vector<node>,greater<node> > pq;

struct node{
	char letter;
	int freq;
	node *right,*left;
	node(char letter,int freq){
		right=left=NULL;
		this->letter=letter;
		this->freq=freq;
	}
};

class CompareNode {
	public:
	bool operator()(node* n1, node* n2)//return true
	{
		if(n1->freq >  n2->freq)
			return true;
		else
			return false;
	}
};


priority_queue <node*,vector<node*>,CompareNode > pq;

//insert into pq
void insert_PQ(){
	char l;
	int f;
	cout<<"enter character"<<endl;
	cin>>l;
	cout<<"enter freq"<<endl;
	cin>>f;
	pq.push(new node(l,f));
	
}

//to print pq
void output_PQ(){
	while(! pq.empty()) {
	node* n = pq.top();
	cout<<n->letter<<'\t'<<n->freq<<endl;
	pq.pop();
	}
}
// Prints huffman codes from the root of Huffman Tree.
void printCodes(struct node* root, string s)
{
    if (!root)
        return;
 
    if (root->letter != '#')
        cout << root->letter << ": " << s << "\n";
 
    printCodes(root->left, s + "0");
    printCodes(root->right, s + "1");
}

void Huffmann(){
	// Iterate while size of heap doesn't become 1
	while (pq.size() != 1)
	{	node *l,*r;
		l=pq.top();
		pq.pop();
		r=pq.top();
		pq.pop();
		node* A=new node('#',l->freq+r->freq);
		A->left=l;
		A->right=r;
		pq.push(A);
	}
	cout<<"done"<<endl;
}

int  main(){
	int i=0;
	cout<<"enter no of elements"<<endl;
	cin>>i;
	while(i>0){
		insert_PQ();
		i--;
	}	
	Huffmann();
	//output_PQ();
	printCodes(pq.top(),"");
}
