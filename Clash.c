/*Project 1 Clash
Judsen Hembree
July 4th 2021
Module 6
*/

#define _GNU_SOURCE
#define MAX_ARGS 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){

    char *in=NULL;
    size_t len = 0;
	ssize_t nread;
    char* tokens[MAX_ARGS];

    void parse(char*, char**);
    void execute(char**);

    //error excessive args
    if(argc>2){
        fprintf(stderr,"%s","Invalid clash call.\nInteractive Usage: ./clash \nBatch Usage: ./clash example.txt\n");
        free(in);
        exit(EXIT_SUCCESS);
    }






    //interactive mode
    if(argc==1){
        fprintf(stdout,"%s","\nYou're in interactive mode.\n");
        fprintf(stdout,"%s", "\nClash> ");
        while((nread = getline(&in,&len,stdin)) !=-1){
            parse(in,tokens);
            
            if(strcmp(tokens[0],"exit")==0){
                //printf("detect exit");
                free(in);
                exit(EXIT_SUCCESS);
            }else if(strcmp(tokens[0],"cd")==0){

                char cdbuf[100];
                //printf("detect cd");
                if(tokens[2]!=NULL){
                    printf("***Error cd passed with too many arguements.\n");
                    free(in);
                    exit(EXIT_FAILURE);
                }

                if(tokens[1]==NULL){
                    printf("***Be advised passing cd without an arguement will set cwd to the HOME environment.\n");
                    chdir(getenv("HOME"));
                    printf("cwd: %s ",getcwd(cdbuf, 100));
                    printf("\n");

                }else{
                    
                    if(chdir(tokens[1])==-1){
                        fprintf(stderr,"That directory doesn't exist.\n");
                    }

                    printf("cwd: %s ",getcwd(cdbuf, 100));
                    printf("\n");
                }

            }else if(strcmp(tokens[0],"path")==0){
                unsigned tokenCount=1;

                while (tokens[tokenCount]!=NULL){
                    tokenCount++;
                }

                if(tokenCount < 2){
                    fprintf(stderr, "Usage: %s list-of-directories-for-path \n\n", argv[0]);
                    free(in);
                    exit(EXIT_FAILURE);
                }
                

                int new_string_size = 0;
                int counter=1; 

                while (tokens[counter] != NULL){
                // for each argument add the size of the argument plus 1 for the ":"

                new_string_size += strlen(tokens[counter]) + 1;
                counter++;
                }
                
                /* allocate memory for the new string */
                char * new_string = malloc(sizeof(char) * new_string_size);
                new_string[0] = '\0';
                counter=1;
                
                while (tokens[counter] != NULL){
                    strncat(new_string, tokens[counter], strlen(tokens[counter]) + 1);
                    counter++;
                    // and add a ":" to separate the elements of the string for all      
                    // but the last part
                    // You need 2 so you have room for the '\0'

                    if (tokens[counter] != NULL) strncat(new_string, ":",2);
                    }

                    // ... do whatever you're going to do with the string
                    //printf("You built this string: %s \n", new_string);

                    setenv("PATH",new_string,1);
                    printf("PATH: %s\n", getenv("PATH"));
                    // avoid a memory leak ... 
                    free(new_string);

                }else{
                   execute(tokens);
                }

                fprintf(stdout,"%s","\nClash> ");
            }
    









    //batch mode
    }else{
        fprintf(stdout,"%s","You're in batch mode\n\n");

        char *line_buf = NULL;
        size_t line_buf_size = 0;
        int line_count = 0;
        ssize_t line_size;
        char *filename=argv[1];
        FILE *fp;
        fp= fopen(filename, "r");

        if(!fp){
            fprintf(stderr,"Error opening file\n");
            free(line_buf);
            free(filename);
            free(fp);
            exit(EXIT_FAILURE);
        }

        /* Get the first line of the file. */
        line_size = getline(&line_buf, &line_buf_size, fp);


    /* Loop through until we are done with the file. */
        while (line_size >= 0){
            parse(line_buf, tokens);
            /* Increment our line count */
            line_count++;

            /* Show the line details */
            printf("Commands: ");
            unsigned i=0;

            while(tokens[i]!=NULL){
                printf("%s ", tokens[i]);
                i++;
            }

            printf("\n");
            
            if(strcmp(tokens[0],"exit")==0){
                //printf("detect exit");
                free(line_buf);
                free(fp);
                exit(EXIT_SUCCESS);

            }else if(strcmp(tokens[0],"cd")==0){

                char cdbuf[100];
                //printf("detect cd");

                if(tokens[2]!=NULL){
                    fprintf(stderr,"***Error cd passed with too many arguements.\n");
                    free(line_buf);
                    free(filename);
                    free(fp);
                    exit(EXIT_FAILURE);
                }

                if(tokens[1]==NULL){
                    //printf("***Be advised passing cd without an arguement will set cwd to the HOME environment.\n");
                    //not passing this printf cause its batch mode and it would be weird in my opinion.

                    chdir(getenv("HOME"));
                    printf("cwd: %s ",getcwd(cdbuf, 100));
                    printf("\n");

                }else{
                    if(chdir(tokens[1])==-1){
                        fprintf(stderr,"That directory doesn't exist.\n");
                    }

                    printf("cwd: %s ",getcwd(cdbuf, 100));
                    printf("\n");
                }

            }else if(strcmp(tokens[0],"path")==0){
                unsigned tokenCount=1;
                while (tokens[tokenCount]!=NULL){
                    tokenCount++;
                }

                if(tokenCount < 2){
                    fprintf(stderr, "Usage: %s list-of-directories-for-path \n\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                
                int new_string_size = 0;
                int counter=1; 

                while (tokens[counter] != NULL){
                // for each argument add the size of the argument plus 1 for the ":"
                new_string_size += strlen(tokens[counter]) + 1;
                counter++;
                }
                
                /* allocate memory for the new string */
                char * new_string = malloc(sizeof(char) * new_string_size);
                new_string[0] = '\0';
                counter=1;

                while (tokens[counter] != NULL){
                    //concatenate the argument onto the string you're building//  and leave space for a '\0' at the end //strncat is safer than strcat - it takes a parameter that limits the      //  number of characters copies from source to destination
                    strncat(new_string, tokens[counter], strlen(tokens[counter]) + 1);
                    counter++;

                    // and add a ":" to separate the elements of the string for all      
                    // but the last part
                    // You need 2 so you have room for the '\0'
                    if (tokens[counter] != NULL) strncat(new_string, ":",2);
                    }

                    //printf("You built this string: %s \n", new_string);
                    setenv("PATH",new_string,1);
                    printf("PATH: %s\n", getenv("PATH"));

                    // avoid a memory leak ... 
                    free(new_string);

                }else{
                   execute(tokens);
                }

                printf("\n");

            /* Get the next line */
            line_size = getline(&line_buf, &line_buf_size, fp);
        }

        /* Free the allocated line buffer */
        free(line_buf);
        line_buf = NULL;

        /* Close the file now that we are done with it */
        fclose(fp);
        }

    free(in);
    exit(EXIT_SUCCESS);
}





void parse(char* in, char* tokens[]){
    unsigned i=0;
    bzero(tokens,MAX_ARGS);
    char* token;
            /* get the first token */
            token = strtok(in, " \n");
            
            /* walk through other tokens */
            while( token != NULL ) {
                if(i>32){
                    fprintf(stderr,"%s\n", "Must use less than 32 sequential commands.");
                    free(in);
                    exit(EXIT_SUCCESS);
                }
                tokens[i]=token;

                token = strtok(NULL, " \n");
                i++;
            }
}




void execute(char **argv){
    pid_t return_pid = fork();
    pid_t wait_result;
    int stat_loc;

    if (return_pid < 0){
        fprintf(stderr, "***Error forking child process failed.\n");
        exit(EXIT_FAILURE);
    }

    if (return_pid == 0){
        if(execvp(argv[0], argv)==-1){
            fprintf(stderr, "***Error exec failed.\n");
            exit(EXIT_FAILURE);
        }else{
            exit(EXIT_SUCCESS);
        }
        
    }else{
        while(wait_result= waitpid(return_pid,&stat_loc,WUNTRACED)==0);
    }    
}