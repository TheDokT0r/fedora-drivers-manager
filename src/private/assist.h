#include <stdbool.h>

#ifndef ASSIST
#define ASSIST

enum OPTIONAL_OS {
    FEDORA,
    ATOMIC,
    NOT_FEDORA
};

char **str_split(const char *input, char delim);
void trim(char *s);

char *get_stdout_from_command(const char *cmd);

void detect_current_os();

// Helper function to check if a file exists
bool file_exists(const char *path);

// Check if the system is Fedora by reading /etc/os-release
bool is_fedora();

// Check if it's an OSTree/Atomic-based Fedora (Silverblue, Kinoite, CoreOS)
bool is_atomic_fedora();

enum OPTIONAL_OS is_running_fedora_or_atomic();

#endif