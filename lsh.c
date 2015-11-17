/* 
 * Main source code file for lsh shell program
 *
 * You are free to add functions to this file.
 * If you want to add functions in a separate file 
 * you will need to modify Makefile to compile
 * your additional functions.
 *
 * Add appropriate comments in your code to make it
 * easier for us while grading your assignment.
 *
 * Submit the entire lab1 folder as a tar archive (.tgz).
 * Command to create submission archive: 
      $> tar cvf lab1.tgz lab1/
 *
 * All the best 
 */


#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"


/*
 * Function declarations
 */

void PrintCommand(int, Command *);
void PrintPgm(Pgm *);
void stripwhite(char *);
void execute(Pgm *pl);

/* When non-zero, this global means the user is done using this program. */
int done = 0;


/*
 * Name: main
 *
 * Description: Gets the ball rolling...
 *
 */



int main(void)
{
  Command cmd;
  int n;

  while (!done) {

    char *line;
    line = readline("> ");

    if (!line) {
      /* Encountered EOF at top level */
      done = 1;
    }
    else {
      /*
       * Remove leading and trailing whitespace from the line
       * Then, if there is anything left, add it to the history list
       * and execute it.
       */
      stripwhite(line);

      if(*line) {
        add_history(line);
        /* execute it */
        n = parse(line, &cmd);
        execute(cmd.pgm);
        PrintCommand(n, &cmd);
      }
    }
    
    if(line) {
      free(line);
    }
  }
  return 0;
}

/*
 * Name: PrintCommand
 *
 * Description: Prints a Command structure as returned by parse on stdout.
 *
 */
void
PrintCommand (int n, Command *cmd)
{
  printf("\nParse returned %d:\n", n);
  printf("   stdin : %s\n", cmd->rstdin  ? cmd->rstdin  : "<none>" );
  printf("   stdout: %s\n", cmd->rstdout ? cmd->rstdout : "<none>" );
  printf("   bg    : %s\n", cmd->bakground ? "yes" : "no");
  PrintPgm(cmd->pgm);
}

/*
 * Name: PrintPgm
 *
 * Description: Prints a list of Pgm:s
 *
 */
void
PrintPgm (Pgm *p)
{
  int i = 0;
  if (p == NULL) {
    return;
  }
  else {
    char **pl = p->pgmlist;

    /* The list is in reversed order so print
     * it reversed to get right
     */
    PrintPgm(p->next);
    printf("    [");
    while (*pl) {
      printf("%s ", *pl++);
      i++;
    }

    printf("]\n");
  }
  printf("Number of arguments: %d\n", i);
}

/*
 * Name: stripwhite
 *
 * Description: Strip whitespace from the start and end of STRING.
 */
void
stripwhite (char *string)
{
  register int i = 0;

  while (isspace( string[i] )) {
    i++;
  }
  
  if (i) {
    strcpy (string, string + i);
  }

  i = strlen( string ) - 1;
  while (i> 0 && isspace (string[i])) {
    i--;
  }

  string [++i] = '\0';
}

void execute(Pgm *pl){
    pid_t pid; 
    int status; 
     char **p = pl->pgmlist;

   

    if((pid = fork()) < 0){       //If the fork returns -1, the fork failed somehow
        printf("Forking child process failed\n");
        exit(1);
    }

    else if (pid == 0){          //If forks() returns 0, the fork was successfull
        if((execvp(p[0], p)) < 0){       //If the execvp returns -1, the command failed to be executed
            printf("Executing the command failed\n");
            exit(1);
        }
    }
    else{
        while((wait(&status)) != pid);
    }
}
