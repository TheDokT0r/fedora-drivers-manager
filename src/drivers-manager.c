#include <stdio.h>
#include <stdlib.h>
#include "drivers-manager.h"
#include <string.h>

enum GPU detect_gpu()
{
    char *response = get_stdout_from_command("/lib/drivers-manager/detect-gpu.sh");
    if(strstr(response, "RTX") != NULL) {
     return RTX;   
    } else {
        return NONE;
    }
}

char *get_stdout_from_command(const char *cmd) {
    FILE *fp;
    char buffer[128];
    size_t output_size = 1;   // Start with 1 for terminating null byte
    char *output = malloc(output_size);
    if (!output) return NULL;
    output[0] = '\0';

    fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("popen failed");
        free(output);
        return NULL;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        char *new_output = realloc(output, output_size + len);
        if (!new_output) {
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