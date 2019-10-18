#include<stdio.h> 
#include<stdlib.h> 
#include<sys/types.h> 
#include<string.h> 
  
char* delete_tabs(char* new_str,char* text) {
    int j = 0;
    for (int i = 0; i<=strlen(text); ++i) {
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
        int read_count = read(STDIN_FILENO, input_str, 100);
        while (read_count == 100) {
            write(fd1[1], input_str, 100);
            read_count = read(STDIN_FILENO, input_str, 100);
        }
        if (read_count != 0) {
            write(fd1[1], input_str, read_count);
        }
        wait(0);
  
        close(fd2[1]); // Close writing end of second pipe 
  
        // Read string from child, print it and close 
        // reading end. 
        read(fd2[0], result_str, 100); 
        close(fd2[0]); 
        printf("%s\n",result_str); 

    } 
  
    // child process 
    else
    { 
  
        // Read a string using first pipe 
        char get_str[100]; 
        read(fd1[0], get_str, 100); 
       char* new_str;
        
        // Concatenate a fixed string with it 
        int read_count = read(fd1[0], get_str, 100);
        delete_tabs(new_str,get_str);

       for (int i = 0; i<strlen(new_str); ++i) {
           if (new_str[i]==' ') {
               new_str[i]='_';
           }
       }       
            while (read_count == 100) {
            write(STDOUT_FILENO, new_str, strlen(new_str));

            read_count = read(fd1[0], get_str, 100);
            delete_tabs(new_str,get_str);

       for (int i = 0; i<strlen(new_str); ++i) {
           if (new_str[i]==' ') {
               new_str[i]='_';
           }
       }
        }
        if (read_count != 0) {
            write(STDOUT_FILENO, new_str, strlen(new_str));
        }
        exit(0);

    } 
    return 0;
}
