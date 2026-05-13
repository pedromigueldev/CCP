#ifndef MFILEH
#define MFILEH
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "./mstr.h"

#define ENSURE_NOT_NULL(ptr)                  \
    do {                                      \
        if ((ptr) == NULL) {                  \
            fprintf(stderr, "Error message not defined on error branch %s:%d\n",__FILE__, __LINE__);\
            exit(EXIT_FAILURE);               \
        }                                     \
    } while (0)
    
#define take_ptr(ptr)                    \
    ({                                   \
        ENSURE_NOT_NULL(ptr);            \
        ptr;                             \
    })
    
#define catch(fun, error, ...)                                                   \
    for (char *(error) = NULL, *__a = NULL; __a == NULL; __a = (char*)1) if(fun##_tryfail(&(error), __VA_ARGS__) && take_ptr((error)))

int mfile_mkdir_path_tryfail(char** error, char* path) {
    if (mkdir(path, 0755) == -1) {
        *error = strerror(errno);
        return 1;
    }
    
    return 0;
}
int read_file_tryfail (const char* filename, Mstr* buffer_out) {
	FILE* file = fopen(filename, "rb");
	
	if (!file) {
		char* error = strerror(errno);
		
		*buffer_out = (Mstr) {
			.raw = strdup(error),
			.length = strlen(error)
		};
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long buffer_len = ftell(file);
	rewind(file);
	char* buffer = malloc(buffer_len + 1);
	fread(buffer, 1, buffer_len, file);
	buffer[buffer_len] = '\0';
	fclose(file);

	*buffer_out = (Mstr) {
		.raw = buffer,
		.length = buffer_len
	};
	
	return 0;
}

#endif
