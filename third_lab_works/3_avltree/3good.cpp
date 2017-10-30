    #include<iostream>
    #include<cstdio>
    #include<sstream>
    #include<algorithm>
    #define pow2(n) (1 << (n))
    using namespace std;
    
    //Node
    struct avl_node
    {   int data;
        struct avl_node *left;
        struct avl_node *right;
    }*root;
	//Tree
    class avlTree
    {   public:
            int height(avl_node *);
            int diff(avl_node *);
			avl_node *minValueNode(avl_node *);
            avl_node *rr_rotation(avl_node *);
            avl_node *ll_rotation(avl_node *);
            avl_node *lr_rotation(avl_node *);
            avl_node *rl_rotation(avl_node *);
            avl_node* balance(avl_node *);
            avl_node* insert(avl_node *, int );
			avl_node* remove(avl_node *, int );
            void display(avl_node *, int);
            avlTree()
            {
                root = NULL;

            }
    };
    /*
     * Main Contains Menu
     */
    int main()
    {   
		int choice, item;
        avlTree avl;
        while (1)
        {
            cout<<"\n------------"<<endl;
            cout<<"AVL Tree Menu"<<endl;
            cout<<"\n------------"<<endl;
            cout<<"1.Insert Element into the tree"<<endl;
            cout<<"2.Display Balanced AVL Tree"<<endl;
			cout<<"3.Delete an element from tree"<<endl;
            cout<<"4.Exit"<<endl;
            cout<<"Enter your Choice: ";
            cin>>choice;
			cout<<endl;
            switch(choice)
            {
            case 1:
                cout<<"Enter value to be inserted: ";
                cin>>item;
                root = avl.insert(root, item);
                break;
            case 2:
                if (root == NULL)
                {	cout<<"Tree is Empty"<<endl;
                    continue;
                }
                cout<<"Balanced AVL Tree:"<<endl;
                avl.display(root, 1);
                break;
            case 3:
                cout<<"Enter value to be removed: ";
                cin>>item;
                root = avl.remove(root, item);    
                break;
			case 4:
				exit(1);
				break;
            default:
                cout<<"Wrong Choice"<<endl;
            }
        }
        return 0;
    }
    
    //Height of AVL Tree
     
    int avlTree::height(avl_node *temp)
    {
        int h = 0;
        if (temp != NULL)
        {
            int l_height = height (temp->left);
            int r_height = height (temp->right);
            int max_height = max (l_height, r_height);
            h = max_height + 1;
        }
        return h;
    }

    
     //Height Difference 
     
    int avlTree::diff(avl_node *temp)
    {
        int l_height = height (temp->left);
        int r_height = height (temp->right);
        int b_factor= l_height - r_height;
        return b_factor;
    }
     
    
     //Right- Right Rotation
  
    avl_node *avlTree::rr_rotation(avl_node *parent)
    {
        avl_node *temp;
        temp = parent->right;
        parent->right = temp->left;
        temp->left = parent;
        return temp;
    }

    
     //Left- Left Rotation

    avl_node *avlTree::ll_rotation(avl_node *parent)

    {

        avl_node *temp;

        temp = parent->left;

        parent->left = temp->right;
        temp->right = parent;
        return temp;
    }
    
     //Left - Right Rotation
 
    avl_node *avlTree::lr_rotation(avl_node *parent)
    {
        avl_node *temp;
        temp = parent->left;
        parent->left = rr_rotation (temp);
        return ll_rotation (parent);
    }
    
     //Right- Left Rotation
     
    avl_node *avlTree::rl_rotation(avl_node *parent)
    {
        avl_node *temp;
        temp = parent->right;
        parent->right = ll_rotation (temp);
        return rr_rotation (parent);
    }
    
     //Balancing AVL Tree
     

    avl_node *avlTree::balance(avl_node *temp)
    {
        int bal_factor = diff (temp);
        if (bal_factor > 1)
        {
            if (diff (temp->left) > 0)
                temp = ll_rotation (temp);
            else
                temp = lr_rotation (temp);
        }
        else if (bal_factor < -1)
        {
            if (diff (temp->right) > 0)
                temp = rl_rotation (temp);
            else
                temp = rr_rotation (temp);
        }
        return temp;
    }

  	//Insert Element into the tree
    avl_node *avlTree::insert(avl_node *root, int value)
    {
        if (root == NULL)
        {
            root = new avl_node;
            root->data = value;
            root->left = NULL;
            root->right = NULL;
            return root;
        }
        else if (value < root->data)
        {
            root->left = insert(root->left, value);
            root = balance (root);
        }
        else if (value >= root->data)
        {
            root->right = insert(root->right, value);
            root = balance (root);
        }
        return root;
    }
     
     //Display AVL Tree
 
    void avlTree::display(avl_node *ptr, int level)
    {
        int i;
        if (ptr!=NULL)
        {
            display(ptr->right, level + 1);
            printf("\n");
            if (ptr == root)
            	cout<<"Root -> ";
            for (i = 0; i < level && ptr != root; i++)
                cout<<"        ";
            cout<<ptr->data;
            display(ptr->left, level + 1);
        }
    }

    //minvalueNode
	avl_node *avlTree::minValueNode(avl_node *node)
	{	avl_node* current = node;
		while(current->left !=NULL)
			current=current->left;
		
		return current;
	} 

    //recursive fn to delete node
    avl_node *avlTree::remove(avl_node *root, int value)    
    {	if( root == NULL)
		return root;
	
		if( value < root-> data )
			root->left = remove(root->left, value);
	
		else if(value > root->data )
			root->right = remove(root->right, value);

		else
		{	if( (root->left == NULL) || (root->right == NULL))//left or right is empty
			{	avl_node *temp = root->left ? root->left : root->right;	
    			
				if(temp==NULL)
				{	temp = root;
					root = NULL;
				}
				else
					*root = *temp;
	
				free(temp);
			}
			else//if left and right child exist
			{	avl_node *temp = minValueNode(root->right);
				root->data=temp->data;
				root->right = remove(root->right,temp->data);
			}
		}
	
		if(root == NULL)
			return root;
	
		root = balance(root);
		return root;
	}
    			
    
