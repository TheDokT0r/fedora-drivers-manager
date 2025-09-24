#ifndef ASSIST
#define ASSIST

char **str_split(const char *input, char delim);
void trim(char *s);

char *get_stdout_from_command(const char *cmd);

#endif