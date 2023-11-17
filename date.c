#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
void *func(void *input)
{
    system(input);
    pthread_exit(NULL);
}
int token_to_cmd(char input[], char *cmds[32][16])
{
    char *tokens[256];
    tokens[0] = strtok(input, "  \n\0");
    int i = 0;
    while (tokens[i] != NULL)
    {
        tokens[i + 1] = strtok(NULL, "  \n\0");
        i++;
    }
    if (tokens[0] == NULL)
    {
        return 0;
    }

    int cmds_no = 0, row = 0, col = 0, token = 0;

    for (;;)
    {
        if (tokens[token] != NULL)
        {
            cmds[row][col] = tokens[token];
            col++;
            token++;
        }
        else
        {
            cmds[row][col] = NULL;
            cmds_no++;
            break;
        }
    }
    return cmds_no;
}

void execute(char *cmds[32][16], int ind)
{

    if (ind > 0)
    {
        pid_t pid;
        int file_descriptor[2];

        if (pipe(file_descriptor) == 1)
        {
            printf("Pipe error!\n");
        }
        else
        {

            if ((pid = fork()) < 0)
            {
                printf("Fork Error!");
            }
            else if (pid == 0)
            {
                close(1);
                dup(file_descriptor[1]);

                close(file_descriptor[0]);
                close(file_descriptor[1]);

                execute(cmds, ind - 1);
            }
            else
            {
                close(0);
                dup(file_descriptor[0]);

                close(file_descriptor[0]);
                close(file_descriptor[1]);
                if (strcmp("cd", cmds[0][0]) != 0)
                {
                    execvp(cmds[0][0], *cmds);
                    printf("%s: Command not found.\n", cmds[0][0]);
                }
            }
        }
    }
    else
    {
        if (strcmp("cd", cmds[0][0]) != 0)
        {
            execvp(cmds[0][0], *cmds);
            printf("%s: Command not found.\n", cmds[0][0]);
        }
    }
}
void dateexecute(char *cmds[32][16], int num_cmds)
{
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    if (cmds[0][1] == NULL)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("Fork Error!\n");
        }
        else if (pid == 0)
        {
            execute(cmds, num_cmds - 1);
            exit(0);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        count3++;
    }
    if (count3 == 0 && strstr("-u", cmds[0][1]) && cmds[0][2] == NULL)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("Fork Error!\n");
        }
        else if (pid == 0)
        {
            execute(cmds, num_cmds - 1);
            exit(0);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        count1++;
    }
    if (count3 == 0 && strstr("-r", cmds[0][1]) && cmds[0][3] == NULL)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("Fork Error!\n");
        }
        else if (pid == 0)
        {
            execute(cmds, num_cmds - 1);
            exit(0);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        count1++;
    }
    if (count3 == 0 && count1 == 0 && cmds[0][2] != NULL)
    {
        printf("Too many arguments\n");
        count2++;
    }
    if (count1 == 0 && count2 == 0 && count3 == 0)
    {
        printf("Unrecoganized Option\n");
    }
}
void siginthandler(int a) {}
int main()
{
    signal(SIGINT, siginthandler);
    int num_cmds;
    char input[1024];
    char a[1024];
    char *cmds[32][16];
    printf("\n");
    printf("         ********       ********             **               **             \n");
    printf("         *********      *********          * **             * **             \n");
    printf("         **     **      **     **        *   **           *   **             \n");
    printf("         **     **      **     **      *     **         *     **             \n");
    printf("         **     **      **     **            **               **             \n");
    printf("         *********      ********             **               **             \n");
    printf("         ********       ********             **               **             \n");
    printf("         **             **     **            **               **             \n");
    printf("         **             **     **            **               **             \n");
    printf("         **             **     **            **               **             \n");
    printf("         **             *********       ************     ************        \n");
    printf("         **             ********        ************     ************        \n");
    printf("\n");
    for (;;)
    {
        printf("PB11> $ ");
        input[0] = '\0';
        memset(a, '\0', 1024);
        fgets(input, 1024, stdin);
        for (int i = 3; i < strlen(input); i++)
        {
            a[i - 3] = input[i];
        }
        num_cmds = token_to_cmd(input, cmds);
        if (num_cmds > 0)
        {
            if (strstr("date", cmds[0][0]) || (strstr("&t", cmds[0][0]) && strstr("date", cmds[0][1])))
            {
                if (strstr("&t", cmds[0][0]))
                {
                    int count1 = 0;
                    int count2 = 0;
                    int count3 = 0;
                    if (cmds[0][2] == NULL)
                    {
                        pthread_t pid;
                        pthread_create(&pid, NULL, func, &a);
                        pthread_join(pid, NULL);
                        count3++;
                    }
                    if (count3 == 0 && strstr("-u", cmds[0][2]) && cmds[0][3] == NULL)
                    {
                        pthread_t pid;
                        pthread_create(&pid, NULL, func, &a);
                        pthread_join(pid, NULL);
                        count1++;
                    }
                    if (count3 == 0 && strstr("-r", cmds[0][2]) && cmds[0][4] == NULL)
                    {
                        pthread_t pid;
                        pthread_create(&pid, NULL, func, &a);
                        pthread_join(pid, NULL);
                        count1++;
                    }
                    if (count3 == 0 && count1 == 0 && cmds[0][3] != NULL)
                    {
                        printf("Too many arguments\n");
                        count2++;
                    }
                    if (count1 == 0 && count2 == 0 && count3 == 0)
                    {
                        printf("Unrecoganized Option\n");
                    }
                }
                if (strstr("date", cmds[0][0]))
                {
                    dateexecute(cmds, num_cmds);
                }
            }
            else
            {
                printf("Invalid Command\n");
            }
        }
    }
    return 0;
}