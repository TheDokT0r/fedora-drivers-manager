#ifndef FOO_H_
#define FOO_H_

enum GPU {
    RTX,
    GTX1080,
    NONE,
};

enum GPU detect_gpu();

char *get_stdout_from_command(const char *cmd);

#endif