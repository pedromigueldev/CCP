#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
 
#define HOME_C "%s/%s"
#define PROJECT_C "%s/Projects/%s"

int create_directory_tryfail(int arc, char** argv, char** path);

int main(int argc, char** argv) {
	char* path = NULL;
	if (create_directory_tryfail(argc, argv, &path)) {
		fprintf(stderr, "ERROR: %s\n", path);
		return 1;
	}
	
    printf("Project at '%s' created successfully.\n", path);
	return 0;
}

int create_directory_tryfail(int argc, char** argv, char** path) {
	if (argc < 2) {
		*path = strdup("Not enough arguments\n");
        return 1;
    }
    char* project_name = argv[1];
    char cwd[PATH_MAX];
    char newProjectFile[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        *path = strerror(errno);
        return 1;
    }

    if (snprintf(newProjectFile, sizeof(newProjectFile), "%s/%s", cwd, project_name) >= sizeof(newProjectFile)) {
        *path = strdup("Path too long\n");
        return 1;
    }

    if (mkdir(newProjectFile, 0755) == -1) {
        *path = strerror(errno);
        return 1;
    }

    *path = strdup(newProjectFile);
    return 0;
}
