#include <stdio.h>
#include <stdlib.h>
#include "drivers-manager.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "private/assist.h"
#include <stdbool.h>

NVIDIA_GPU_SERIES new_series(enum GPU series_id, char *keywords)
{
    NVIDIA_GPU_SERIES series;
    series.keywords = keywords;
    series.series_id = series_id;

    return series;
}

static bool is_currect_nvidia_series(char *response, NVIDIA_GPU_SERIES checked_series)
{
    char **tokens;
    tokens = str_split(checked_series.keywords, ",");
    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            if (strstr(response, *(tokens + i)))
            {
                return true;

                free(*(tokens + i));
                free(tokens);
            }
        }
    }

    free(tokens);
    return false;
}

enum GPU detect_gpu()
{
    NVIDIA_GPU_SERIES old_series[] = {
        new_series(NVIDIA_200_SERIES, "gt2,gt 2,g2,gtx 2"),
        new_series(NVIDIA_300_SERIES, "gt 3"),
        new_series(NVIDIA_400_SERIES, "gt 4,gtx 4"),
        new_series(NVIDIA_500_SERIES, "gt 5,gtx 5"),
        new_series(NVIDIA_600_SERIES, "gt 6,gtx 6"),
    };
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

    enum GPU currect_series = NONE;

    int i = 0;
    while (&old_series[i])
    {
        if (is_currect_nvidia_series(response, old_series[i]))
        {
            currect_series = old_series[i].series_id;
        }

        i++;
    }

    if (currect_series = NONE && strstr(response, "nvidia") != NULL)
    {
        currect_series = NVIDIA_LATEST;
    }

    free(old_series);
    free(response);
    return currect_series;
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