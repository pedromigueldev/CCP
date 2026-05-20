#include <stdbool.h>
#include "vendor/mlib/mfile.h"
#include "vendor/mlib/mvector.h"
#include "vendor/mlib/mstr.h"
#include "vendor/mlib/mprint.h"

#define NL "\n"
MstrView create_mainc_buffer (MVecParamDefPtr(*pool, char)) {
	return MStrFmt(pool,
		"#include <stdio.h>			"NL
		"int main(void) {			"NL
		"	printf(\"Hello world\");"NL
		"	return 0;				"NL
		"}							"NL
	);
}

MstrView create_makefile_buffer (MVecParamDefPtr(*pool, char), char* project_name) {
	if (!project_name) return EMPTYVIEW(MstrView);

	return MStrFmt(pool,
		"CC = gcc 										"NL
		"CFLAGS = -Wall -Wextra -pedantic				"NL
		".PHONY: all clean								"NL
		NL
		"all: "$(project_name)							 NL
		$(project_name)": main.c 						"NL
		"	$(CC) $(CFLAGS) main.c -o "$(project_name)	 NL
		NL
		"clean:											"NL
		"	rm -f "$(project_name)						 NL
	);
}

int main(int argc, char** argv) { UNUSED(argc);
	MVecAllocDefault(pool, char);
    char* project_name = argv[1] ? argv[1] : NULL;
   	char* project_path = argv[2] ? argv[2] : NULL;
    MstrView path = {0};

    if (project_name)
    	path = MStrFmt(&pool, $(project_path != NULL ? project_path : ".")"/"$(project_name));
    else {
  		MPrintFmt("Ussage: <program-name> [project_path] <in_the_future: flags>");
    	return 1;
    }

	MRetEither(fcp, fcperr, mfile_mkdir_path(path, 0755)); 
	if (fcperr) {
		MPrintFmt("Create directory fail: "$(fcperr));
		return 1;
	}

	MstrView cmk_path = MStrFmt(&pool, MstrViewFmt(path)"/makefile");
	MstrView cmk_content = create_makefile_buffer(MVecParamRefPtr(&pool), project_name);
	MRetEither(cmk, cmkfail, mfile_create(cmk_path, cmk_content));
	if (cmkfail) {
		MPrintFmt("Create makefile fail: "$(cmkfail));
		return 1;
	}

    MstrView cmf_path = MStrFmt(&pool, MstrViewFmt(path)"/main.c");
    MRetEither(cmf, cmffail, mfile_create(cmf_path, create_mainc_buffer(MVecParamRefPtr(&pool))));
    if (cmffail) {
    	MPrintFmt("Create makefile fail: "$(cmffail));
		return 1;
    }

 	MPrintFmt("Project at "MstrViewFmt(path)" created successfully.\n");
	return 0;
}
