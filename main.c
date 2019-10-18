#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
char* delete_tabs(char* new_str,char* text) {
    int j = 0;
    for (int i = 0; i<strlen(text); ++i) {
        if (text[i]=='\t') {
            for (int k = 0; k<3; ++k) {
                new_str[j]=' ';
                ++j;
            }
        } else {
            new_str[j]=text[i];
            ++j;
        }
    }
           printf("%s\n",new_str);

    return new_str;
}

int main() 
{ 
  
    int fd1[2];  
    int fd2[2];  
    char result_str[100];
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        scanf ("%[^\n]%*c", input_str);
  
        close(fd1[0]);  // Close reading end of first pipe 
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        close(fd1[1]); 
  
        // Wait for child to send a string 
        wait(NULL); 
  
        close(fd2[1]); // Close writing end of second pipe 
  
        // Read string from child, print it and close 
        // reading end. 
        read(fd2[0], result_str, 100); 
        printf("%s\n",result_str); 
        close(fd2[0]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipe 
  
        // Read a string using first pipe 
        char get_str[100]; 
        read(fd1[0], get_str, 100); 
       char* new_str;
        delete_tabs(new_str,get_str);

       for (int i = 0; i<strlen(new_str); ++i) {
           if (new_str[i]==' ') {
               new_str[i]='_';
           }
       }
        // Concatenate a fixed string with it 
       
  
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
  
        // Write concatenated string and close writing end 
        write(fd2[1], new_str, strlen(new_str)+1); 
        close(fd2[1]); 
  
        exit(0); 
    } 
    return 0;
}
