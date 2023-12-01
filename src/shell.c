#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


char data[512];
FILE *pf;
#define MAXINPUT 80

void cd(char* token)
{
    char currDirectory[512];

    //getcwd(currDirectory, sizeof(currDirectory)); //Get the current directory and store in currDirectory
    if((token = strtok(NULL, " \n")) != NULL) //Get next token
    {
        if (chdir(("%s", /*currDirectory,*/ token)) == 1) //Change directory and check for error
        {
            perror("cd Error: "); //Print error
        }
    } else
    {
        if (chdir("/nfs/pihome/scm22qwu/") == 1)
        {
            perror("cd Error: ");
        }
    }
}

void execCommand(char* token)
{
    char* args[MAXINPUT];
    char* inputCommand = token;
    int i = 0;
    args[i++] = token; //Getting first token and incrementing i


    while ((token = strtok(NULL, " \n")) != NULL) //Checking token isn't NULL and getting the next token
    {
        if (strcmp(token, "") != 0)
        {
            args[i++] = token; //Adding token to args and incrementing i
        }
    }

    args[i] = NULL; //NULL terminating args so that C knows its end of list

    pid_t pid = fork(); //Create a new child process, if not using child process it will never move focus back to c program

    if(pid == -1) //Check if the child process creation failed
    {
        perror("Fork Error: "); //Print fork error and return
        return;
    } else if (pid == 0) //If fork sucessfull
    {

        if(execvp(inputCommand, args) == -1) //Run command and check if there is error
        {
            perror("exec");
        }
    }else
    {
        wait(NULL); //Make the parent process wait for the child process to finish
    }
}


int main()
{

    char currDirectory[80];
    char input[100];
    char* token;
    const char s[2] = " \n"; //Delimeter is space and newline character otherwise function pointer isn't recognised
    printf("-- Charlie Flux Shell -- \n");

    while (1)
    {

        getcwd(currDirectory, sizeof(currDirectory)); //Get current directory and sotre in currDirectory
        printf("%s$ : ", currDirectory);
        fgets(input, sizeof(input), stdin); //Getting the input string and placing it in input
        token = strtok(input, s); //Tokenising the first word of the input using strtok

            if (strcmp(token, "exit") == 0) //If input is exit then return
            {
                return 0;
            }
            else if (strcmp(token, "cd") == 0)
            {
                cd(token);
            }
            else
            {
                execCommand(token);
            }

    }
    return 0;
}