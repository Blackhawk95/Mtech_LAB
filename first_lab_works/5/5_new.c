#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>

int main(int argc,char* argv[])
{   ssize_t bytes; // This data type is used to represent the sizes of blocks that can be read or written in a single operation. It is similar to size_t,but signed.
    
    int filedesc = open(argv[1], O_RDONLY); //get the file descriptor
    
    if(filedesc < 0)
        return 1; // error in opening the file
    
    off_t fileLength = lseek(filedesc, 0, SEEK_END );  // moving the read/write pointer to end of file to measure the size of file 
    
    if ( fileLength < 0 )
        return 1; // error in getting file length
    
    char content[fileLength]; // char array the size of file
    
    if(lseek(filedesc,0,SEEK_SET) < 0) 
        return 1; // moving the pointer back to the start
    
    bytes = read (filedesc,content,sizeof(content)-1); //reading the text and storing in content, bytes will give the size in blocks
    
    write(1,content,bytes); //printing out the data

    close(filedesc); //closing the file
    
    printf("\n"); // for the last escape character( for easy viewing)
    
    return 0;
}
    
