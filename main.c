#include <stdio.h>
#include "vendor/mlib/mfile.h"

int main(int argc, char** argv) {
	   
    char* project_name = argv[1] ? argv[1] : nullptr;
   	char* project_path = argv[2] ? argv[2] : nullptr;
    char path[PATH_MAX];

    if (project_path && project_name) 
    	snprintf(path, PATH_MAX, "%s/%s", project_path, project_name); 
    else if (project_name) 
    	snprintf(path, PATH_MAX, "./%s", project_name);
    else {
  		fprintf(stderr, "Ussage: <program-name> [project_path] [..flags]\n");
    	return 1;
    }
        
	catch(mfile_mkdir_path, error, path) {
		fprintf(stderr, "Error: %s\n", error);
		return 1;
	}
	
    printf("Project at '%s' created successfully.\n", path);
	return 0;
}
