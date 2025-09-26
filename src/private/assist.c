#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "assist.h";
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>

char **str_split(const char *input, char delim)
{
    char *str = strdup(input); // Make a modifiable copy
    if (!str)
        return NULL;

    char **result = NULL;
    size_t count = 0;
    char *tmp = str;
    char *token;

    // Count number of tokens
    while (*tmp)
    {
        if (*tmp == delim)
            count++;
        tmp++;
    }
    count++;                                       // Last token
    result = malloc(sizeof(char *) * (count + 1)); // +1 for NULL terminator
    if (!result)
    {
        free(str);
        return NULL;
    }

    size_t idx = 0;
    token = strtok(str, &delim);
    while (token != NULL)
    {
        result[idx++] = strdup(token); // strdup each token
        token = strtok(NULL, &delim);
    }
    result[idx] = NULL;

    free(str);
    return result;
}

// Trims a string from whitespaces
void trim(char *s)
{
    // Pointer to the beginning of the trimmed string
    char *ptr = s;

    // Skip leading spaces
    while (*s == ' ')
        s++;

    // Shift remaining characters to the beginning
    while (*ptr++ = *s++)
        ;
}

char *get_stdout_from_command(const char *cmd)
{
    FILE *fp;
    char buffer[128];

    size_t output_size = 128; // Initial size
    size_t output_len = 0;    // How much data is in output

    char *output = malloc(output_size);
    if (!output)
        return NULL;

    output[0] = '\0'; // Null-terminate

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
        if (output_len + len + 1 > output_size)
        {
            size_t new_size = output_len + len + 1;
            char *new_output = realloc(output, new_size);
            if (!new_output)
            {
                free(output);
                pclose(fp);
                return NULL;
            }
            output = new_output;
            output_size = new_size;
        }

        strcpy(output + output_len, buffer);
        output_len += len;
    }

    pclose(fp);
    return output;
}

void detect_current_os() {
    #ifndef __linux__
        fprintf(stderr, "This application can only run on Linux.\n");
        exit(EXIT_FAILURE);
    #endif
}

bool file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

bool is_fedora() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (!fp) return false;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "ID=fedora", 9) == 0) {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

bool is_atomic_fedora() {
    return file_exists("/run/ostree-booted");
}

enum OPTIONAL_OS is_running_fedora_or_atomic() {
    if(is_atomic_fedora()) {
        return ATOMIC;
    } else if(is_fedora()) {
        return FEDORA;
    }

    return NOT_FEDORA;
}
