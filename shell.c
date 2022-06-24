#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

char *helptext =
        "The following commands are available:\n"
        "  mycat      Prints the whole file. \n"
        "  myhead     Prints first 10 lines of the file. \n"
        "  mytail     Prints last 10 lines of the file. \n"
        "  myhistory  Prints last 50 commands. \n"
        "  help       Prints the manual.\n"
        "  exit       Exit the shell.\n"
        ;

void my_exit(char **args) {
    remove("history.txt");
    exit(0);
}

void my_head(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "my: head: missing argument\n");
    } else {
        FILE *f;
      	f=fopen(args[1],"r");
      	if(f==NULL)
      	{
      		printf("error in opening file");
          return;
      	}
      	int n=10;
      	char line[100];
      	while(fgets(line,sizeof(line),f))
      	{
      		if(n==0)
      		{
      			break;
      		}
      		printf("%s",line);
      		n--;
      		
      	}
      	fclose(f);
      }
        
}

void my_cat(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "my: cat: missing argument\n");
    } else {
        FILE *f;
        char c;
      	f=fopen(args[1],"r");
      	if(f==NULL)
      	{
      		printf("error in opening file");
          return;
      	}
      	// Read contents from file
        c = fgetc(f);
        while (c != EOF)
        {
            printf ("%c", c);
            c = fgetc(f);
        }
      	fclose(f);
      }
        
}

void my_tail(char **args) {
  if (args[1] == NULL && strcmp(args[0], "mytail")==0) {
        fprintf(stderr, "my: tail: missing argument\n");
  } else {
        FILE *f;
        char c;
        int n = 10;
        if (strcmp(args[0], "myhistory")==0) {
          n = 50;
          f=fopen("history.txt","r");
        } else {
          f=fopen(args[1],"r");
        }
      	if(f==NULL)
      	{
      		printf("error in opening file");
          return;
      	}
        int count = 0;
        unsigned long long pos;
        char str[2*100];

        if (fseek(f, 0, SEEK_END))
            perror("fseek() failed");
        else
        {
            pos = ftell(f);
            while (pos)
            {
                if (!fseek(f, --pos, SEEK_SET))
                {
                    if (fgetc(f) == '\n')
                        if (++count == n)
                            break;
                }
                else
                    perror("fseek() failed");
            }
            while (fgets(str, sizeof(str), f))
                printf("%s", str);
        }
    }
}

void my_help(char **args) {
    printf("%s", helptext);
}

struct builtin {
    char *name;
    void (*func)(char **args);
};

struct builtin builtins[] = {
    {"help", my_help},
    {"exit", my_exit},
    {"myhead", my_head},
    {"mycat", my_cat},
    {"mytail", my_tail},
    {"myhistory", my_tail},
};

int my_num_builtins() {
    return sizeof(builtins) / sizeof(struct builtin);
}

void my_exec(char **args) {
    for (int i = 0; i < my_num_builtins(); i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            builtins[i].func(args);
            return;
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        execvp(args[0], args);
        perror("my");
        exit(1);
    } else if (child_pid > 0) {
        int status;
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        perror("my");
    }
}

char** my_split_line(char *line) {
    int length = 0;
    int capacity = 16;

    char **tokens = malloc(capacity * sizeof(char*));
    if (!tokens) {
        perror("my");
        exit(1);
    }

    char *delimiters = " \t\r\n";
    char *token = strtok(line, delimiters);

    while (token != NULL) {
        tokens[length] = token;
        length++;

        if (length >= capacity) {
            capacity = (int) (capacity * 1.5);
            tokens = realloc(tokens, capacity * sizeof(char*));
            if (!tokens) {
                perror("my");
                exit(1);
            }
        }

        token = strtok(NULL, delimiters);
    }

    tokens[length] = NULL;
    return tokens;
}

char* my_read_line() {
    char *line = NULL;
    size_t buflen = 0;
    errno = 0;
    ssize_t strlen = getline(&line, &buflen, stdin);
    if (strlen < 0) {
        if (errno) {
            perror("my");
        }
        exit(1);
    }
    return line;
}

void store_history(char *line) {
    FILE *filePointer ;
    filePointer = fopen("history.txt", "a+") ;
    if ( filePointer == NULL )
    {
        printf( "failed to open history" ) ;
    }
    else
    {
        if ( strlen ( line ) > 0 )
        {
            fputs("\n", filePointer) ;
            fputs(line, filePointer) ;
        }
        fclose(filePointer) ;
    }
}

int main() {
    printf("Welcome to My Shell \n");
    printf("%s", helptext);
    while (true) {
        printf("\n> ");
        char *line = my_read_line();
        store_history(line);
        char **tokens = my_split_line(line);

        if (tokens[0] != NULL) {
            my_exec(tokens);
        }

        free(tokens);
        free(line);
    }
}