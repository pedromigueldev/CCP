#include <stdio.h>
#include "vendor/mlib/mfile.h"

int create_makefile_tryfail (char** out_buffer, char* prject_name, char* path);
int create_mainc_tryfail(char** out_buffer, char* path);

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

    char* path_out = strdup(path);   
	catch(mfile_mkdir_path, &path_out, 0755) {
		fprintf(stderr, "Error: %s\n", unwrap_fail(path_out));
		return 1;
	}
   	
	char* makefile = NULL;
   	catch(create_makefile, 	&makefile, project_name, path_out) {
   		fprintf(stderr, "Error: %s\n", unwrap_fail(makefile));
		return 1;
   	}
   	
   	catch(create_mainc,		&makefile, path_out) {
   		fprintf(stderr, "Error: %s\n", unwrap_fail(makefile));
   		return 1;
   	};
   	
	printf("Project at '%s' created successfully.\n", path_out);
   	free(makefile);
   	free(path_out);
	return 0;
}

#define NL "\n"
char* create_mainc_buffer () {
	return MPRINT_FMT_OUT(
		"#include <sdtio.h>			"NL
		"int main(void) {			"NL
		"	printf(\"Hello world\") "NL
		"	return 0;				"NL
		"}							"NL
	);
}

char* create_makefile_buffer (char* project_name) {
	if (!project_name) return NULL;

	return MPRINT_FMT_OUT(
		"CC = gcc 										"NL
		"CFLAGS = -Wall -Wextra -pedantic				"NL
		".PHONY: all clean								"NL
		"all: "$(project_name)							 NL
		$(project_name)": main.c 						"NL
		"	$(CC) $(CFLAGS) main.c -o "$(project_name)	 NL
		"clean:											"NL
		"	rm -f "$(project_name)						 NL
	);
}

int create_makefile_tryfail (char** out_buffer, char* project_name, char* path) {
	char* makefile = create_makefile_buffer(project_name);
	char* makefilePath = MPRINT_FMT_OUT($(path)"/makefile");
	FILE* file = fopen(makefilePath, "w");
	
    if (file == NULL) {
        wrap_fail(out_buffer) = strdup(strerror(errno));
        return 1;
    }
    
    fprintf(file, makefile);
    fclose(file);
    *out_buffer = makefile;
    free(makefilePath);
	return 0;
}

int create_mainc_tryfail(char** out_buffer, char* path) {
	char* mainc = create_mainc_buffer();
	char* maincPath = MPRINT_FMT_OUT($(path)"/main.c");
	FILE* file = fopen(maincPath, "w");
	
    if (file == NULL) {
        wrap_fail(out_buffer) = strdup(strerror(errno));
        return 1;
    }
    
    fprintf(file, mainc);
    fclose(file);
    *out_buffer = mainc;
    free(maincPath);
	return 0;
}
