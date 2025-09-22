#ifndef FOO_H_
#define FOO_H_

enum GPU {
    NVIDIA_LATEST,
    NONE
};

enum GPU detect_gpu();

char *get_stdout_from_command(const char *cmd);

int install_drivers(enum GPU required_drivers);

#endif