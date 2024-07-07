#ifndef DIRECTORY_UTILS_H
#define DIRECTORY_UTILS_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

void deletedir(const char *path);
void makedir(const char *name);
void changedir(const char *name);

#endif /* DIRECTORY_UTILS_H */
