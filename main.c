#include <stdio.h>
#include "vendor/mlib/mfile.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Ussage: <program-name> [where] [..flags]\n");
    	return 1;
    }
	   
    char path[PATH_MAX];
    char* project_name = argv[1];
   	char* where = argc >= 3 ? argv[2] : nullptr;
    
    if (where) {
    	char *rpath = realpath(where, NULL);
        if (!rpath) {
            perror("realpath");
            return 1;
        }
		
        if (snprintf(path, PATH_MAX, "%s/%s", rpath, project_name) >= PATH_MAX) {
             fprintf(stderr, "Path was too long\n");
             return 1;
         }
    } else {
   		char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("cwd");
            return 1;
        }
    
        if (snprintf(path, PATH_MAX, "%s/%s", cwd, project_name) >= PATH_MAX) {
        	fprintf(stderr, "Path was too long\n");
            return 1;
        }
    }
    
	catch(mfile_mkdir_path, error, path) {
		fprintf(stderr, "Error: %s\n", error);
		return 1;
	}
	
    fprintf(stdout, "Project at '%s' created successfully.\n", path);
	return 0;
}
