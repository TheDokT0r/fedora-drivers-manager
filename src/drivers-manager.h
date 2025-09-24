#ifndef DM
#define DM

enum GPU
{
    NVIDIA_LATEST,
    NVIDIA_9_SERIES,
    NVIDIA_200_SERIES,
    NVIDIA_300_SERIES,
    NVIDIA_400_SERIES,
    NVIDIA_500_SERIES,
    NVIDIA_600_SERIES,
    NVIDIA_700_SERIES,
    NONE
};

typedef struct NVIDIA_GPU_SERIES
{
    enum GPU series_id;
    char *keywords;
} NVIDIA_GPU_SERIES;

enum GPU detect_gpu();

int install_drivers(enum GPU required_drivers);

char *get_drivers_name(enum GPU gpu);

#endif