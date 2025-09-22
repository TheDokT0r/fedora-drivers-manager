#include <stdio.h>
#include <stdlib.h>
#include "drivers-manager.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

enum GPU detect_gpu()
{
    char *response = get_stdout_from_command("/lib/drivers-manager/detect-gpu.sh");
    if (response == NULL)
    {
        return -1; // Return an error code if the command fails
    }

    // Convert response to lowercase for easier comparison
    for (int i = 0; response[i]; i++)
    {
        response[i] = tolower((unsigned char)response[i]);
    }

    if (strstr(response, "nvidia") != NULL)
    {
        free(response);
        return NVIDIA_LATEST;
    }
}

char *get_stdout_from_command(const char *cmd)
{
    FILE *fp;
    char buffer[128];
    size_t output_size = 1; // Start with 1 for terminating null byte
    char *output = malloc(output_size);
    if (!output)
        return NULL;
    output[0] = '\0';

    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        perror("popen failed");
        free(output);
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t len = strlen(buffer);
        char *new_output = realloc(output, output_size + len);
        if (!new_output)
        {
            free(output);
            pclose(fp);
            return NULL;
        }
        output = new_output;
        strcpy(output + output_size - 1, buffer);
        output_size += len;
    }

    pclose(fp);
    return output;
}

int install_drivers(enum GPU required_drivers)
{
    int ret;

    if (required_drivers == NVIDIA_LATEST)
    {
        ret = system("sudo dnf install -y akmod-nvidia");

        if (ret == -1)
        {
            perror("system");
            return 1;
        }
        printf("Command executed successfully\n");
        return 0;
    }
    return -1;
}