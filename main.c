#include <stdio.h>
#include "vendor/mlib/mfile.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Ussage: <program-name> [where] [..flags]\n");
    	return 1;
    }
	   
	char* path = NULL;
    char* project_name = argv[1];
   	char* where = argc >= 3 ? argv[2] : NULL;
	
	if (mfile_mkdir_tryfail(where, project_name, &path)) {
		fprintf(stderr, "Error: %s\n", path);
		return 1;
	}
	
    fprintf(stdout, "Project at '%s' created successfully.\n", path);
	return 0;
}
