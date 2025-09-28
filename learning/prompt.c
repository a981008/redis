#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

void enable_canonical_mode()
{
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    orig_termios.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

char *prompt(char *line, int size)
{
    char *retval;

    do
    {
        printf(">> ");
        retval = fgets(line, size, stdin);
    } while (retval && *line == '\n');
    line[strlen(line) - 1] = '\0';

    return retval;
}

int main(int argc, char *argv[])
{
    enable_canonical_mode();

    int size = 1024, max = size >> 1;
    char buffer[size];
    char *line = buffer;

    while (prompt(line, size))
    {
        argc = 0;
        char **ap, *args[max];

        for (ap = args; (*ap = strsep(&line, " \t")) != NULL;)
        {
            if (**ap != '\0')
            {
                if (argc >= max)
                    break;
                if (strcasecmp(*ap, "quit") == 0 || strcasecmp(*ap, "exit") == 0)
                    exit(0);
                ap++;
                argc++;
            }
        }
        line = buffer;

        for (int i = 0; i < argc; i++)
        {
            printf("args[%d]: %s\n", i, args[i]);
        }
    }

    return 0;
}
