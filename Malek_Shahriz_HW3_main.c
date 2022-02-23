/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Name: Shahriz Malek
* Student ID: 920378989
* GitHub ID: ShahrizSchool
* Project: Assignment 3 – Simple Shell
*
* File: <simpleshell.c>
*
* Description: For this assignment we are getting userInput from command prompt so the shell we create
* will = work on top of the regular command-line interpreter for linux. 
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void userInput();

int main() {

        char* filePath = "/bin/"; //this is the path will be used for execvp and to copy files to child
        char fullPath[20]; 
        char commandLineBuffer[1024]; //this will used for the userInput to specify the buffer size
        char* limitCMDArgument[100]; //limit for how much will fit in command prompt arguments

        while (1) {

                int errno; // this integer will be used for the error status
                // When I looked up errno it said I need this variable declaration here but it was 
                // working without it

                printf("Prompt$ ");

                userInput(commandLineBuffer, limitCMDArgument); //this is to read userinput using the 
                // buffer var and limit argument

                if (strcmp(commandLineBuffer, "exit") == 0) { //This will only work if the userInput
                // is "exit" and will exit out of the program
                        break;
                }

                pid_t IDChild;

                int pid = fork(); 
                /* 
                this creates a new child and will be used for file path and printing the childID 

                */



                int status; // this integer status will be used for checking if exit. 

                if (pid == 0) {

                        strcpy(fullPath, filePath); 
                        strcat(fullPath, limitCMDArgument[0]);

                        /* if the execvp fails run the if statement, so if invalid userInput is sumbitted
                        run this code.
                        */
                        if (execvp(fullPath, limitCMDArgument) < 0) { 
                                printf("Invalid user input. Exit with error %d\n", errno);
                                exit(2);
                        }

                } else {
                        IDChild = wait(&status); //this will make it so that we are waiting for parent

                        if (WIFEXITED(status)) {
                                printf("Child %d, exited with %d\n", IDChild, WEXITSTATUS(status));
                        }
                }
        }

        return 0;

}

void userInput(char commandArray[], char * cmdPointer[]) { //this userInput function will be used
// in the main function

        if (fgets(commandArray, 1024, stdin)) { //it will save the userinput 

                size_t inputLength = strlen(commandArray); //takes the size of userinput length 

                if (commandArray[inputLength - 1] == '\n') {
                        commandArray[inputLength - 1] = '\0'; 
                }

                char *tok; //this token pointer will be used to tokenize the string 

                tok = strtok(commandArray, " "); 
                int count = 0;

                while (tok != NULL) {
                        cmdPointer[count] = tok;
                        tok = strtok(NULL, " ");
                        count++;
                }
                cmdPointer[count] = NULL;
        } else {
                exit(0);
        }
}