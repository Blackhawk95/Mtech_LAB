#include <dirent.h>
#include <stdio.h>
int main(int argc,char *argv[]) //cmd line argument
{   DIR *d;

    struct dirent *dir;

    if(argc != 2)
    {   
        d = opendir(".");
    } // ls
    else
    {   
        d = opendir(argv[1]);
    } // ls hello/world/

    if (d) // if directory stream exist
    {   
        while ((dir = readdir(d)) != NULL) //until last file
        {
            printf("%s\t", dir->d_name); //print directory name
        }
    closedir(d); // close the dir stream
    }
    
    printf("\n");
    
    return(0);
}
