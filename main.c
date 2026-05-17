#include <stdio.h>
#include "vendor/mlib/mfile.h"
#include "vendor/mlib/marr.h"

#define NL "\n"
DEFINE_FREE(charPtrFree, char*, free);

char* create_mainc_buffer () {
	return MPRINT_FMT_OUT(
		"#include <stdio.h>			"NL
		"int main(void) {			"NL
		"	printf(\"Hello world\");"NL
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

int main(int argc, char** argv) {
    char* project_name = argv[1] ? argv[1] : nullptr;
   	char* project_path = argv[2] ? argv[2] : nullptr;
    char* path __free(charPtrFree) = NULL;

    if (project_name)
    	path = MPRINT_FMT_OUT($(project_path != NULL ? project_path : ".")"/"$(project_name));
    else {
  		MPRINT_FMT("Ussage: <program-name> [project_path] <in_the_future: flags>");
    	return 1;
    }

	catch(mfile_mkdir_path, &path, 0755) {
		fprintf(stderr, "project: %s\n", unwrap_fail(path));
		return 1;
	}
   	
	char* out = NULL;	
	char* makefile __free(charPtrFree) = create_makefile_buffer(project_name);
	char* makefilePath __free(charPtrFree) = MPRINT_FMT_OUT($(path)"/makefile");
	catch(mfile_create, &out, makefilePath, makefile) {
		fprintf(stderr, "makefile: %s\n", unwrap_fail(out));
		return 1;
	};

   	char* mainc __free(charPtrFree) = create_mainc_buffer();
  	char* maincPath __free(charPtrFree) = MPRINT_FMT_OUT($(path)"/main.c");
   	catch(mfile_create,	&out, maincPath, mainc) {
   		fprintf(stderr, "main: %s\n", unwrap_fail(out));
   		return 1;
   	};
   	
	printf("Project at '%s' created successfully.\n", path);
	return 0;
}
