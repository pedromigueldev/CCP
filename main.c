#include <stdio.h>
#include "vendor/mlib/mfile.h"

int main(int argc, char** argv) {
	char* path = NULL;
	if (create_directory_tryfail(argc, argv, &path)) {
		fprintf(stderr, "ERROR: %s\n", path);
		return 1;
	}
	
    printf("Project at '%s' created successfully.\n", path);
	return 0;
}
