#include <stdio.h>
#include <stdlib.h>
#include "drivers-manager.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "private/assist.h"
#include <stdbool.h>

const char *detectCpuCmd =
    "lspci -mm | awk -F '\"|\" \"|\\\\(' "
    "'/\"Display|\"3D|\"VGA/ { "
    "a[$0] = $1 \" \" $3 \" \" ($(NF-1) ~ /^$|^Device [[:xdigit:]]+$/ ? $4 : $(NF-1)) "
    "} "
    "END { for (i in a) { "
    "if (!seen[a[i]]++) { "
    "sub(\"^[^ ]+ \", \"\", a[i]); "
    "print a[i] "
    "} "
    "} }'";

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
    tokens = str_split(checked_series.keywords, ',');
    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("Checking against keyword: %s\n", tokens[i]);
            if (strstr(response, *(tokens + i)))
            {
                for (int j = 0; tokens[j]; j++)
                {
                    free(tokens[j]);
                }
                free(tokens);
                return true;
            }
        }
    }

    free(tokens);
    return false;
}

enum GPU detect_gpu()
{
    NVIDIA_GPU_SERIES old_series[] = {
        new_series(NVIDIA_200_SERIES, "gt2,g2,gtx2"),
        new_series(NVIDIA_300_SERIES, "gt3"),
        new_series(NVIDIA_400_SERIES, "gt4,gtx4"),
        new_series(NVIDIA_500_SERIES, "gt5,gtx5"),
        new_series(NVIDIA_600_SERIES, "gt6,gtx6"),
    };
    char *response = get_stdout_from_command(detectCpuCmd);
    if (!response)
    {
        return -1; // Return an error code if the command fails
    }

    // Convert response to lowercase for easier comparison
    for (int i = 0; response[i]; i++)
    {
        response[i] = tolower((unsigned char)response[i]);
    }

    trim(response);
    printf("Response: %s\n", response);
    printf("Checking series...\n");

    enum GPU currect_series = NONE;

    int series_count = sizeof(old_series) / sizeof(old_series[0]);
    for (int i = 0; i < series_count; i++)
    {
        if (is_currect_nvidia_series(response, old_series[i]))
        {
            currect_series = old_series[i].series_id;
        }
    }

    if (currect_series == NONE && strstr(response, "nvidia") != NULL)
    {
        currect_series = NVIDIA_LATEST;
    }

    free(response);
    return currect_series;
}

int install_drivers(enum GPU required_drivers)
{
    get_stdout_from_command("pkexec dnf install https://mirrors.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://mirrors.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm -y");
    int ret;

    if (required_drivers == NVIDIA_LATEST)
    {
        ret = system("pkexec dnf install -y akmod-nvidia");

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

char *get_drivers_name(enum GPU gpu) {
    switch (gpu)
    {
    case NVIDIA_9_SERIES:
        return "NVIDIA 9 Series";
    case NVIDIA_200_SERIES:
        return "NVIDIA 200 Series";
    case NVIDIA_300_SERIES:
        return "NVIDIA 300 Series";
    case NVIDIA_400_SERIES:
        return "NVIDIA 400 Series";
    case NVIDIA_500_SERIES:
        return "NVIDIA 500 Series";
    case NVIDIA_600_SERIES:
        return "NVIDIA 600 Series";
    case NVIDIA_700_SERIES:
        return "NVIDIA 700 Series";
    case NONE:
        return "Unknown device";
    default:
        return "NVIDIA Latest";
    }
}