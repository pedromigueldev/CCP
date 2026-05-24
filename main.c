#include <stdbool.h>
#include "vendor/mlib/mfile.h"
#include "vendor/mlib/marr.h"
#include "vendor/mlib/mstr.h"
#include "vendor/mlib/mprint.h"

#define NL "\n"
#define NLNL NL NL
MstrView create_mainc_buffer (MByteArray** pool) {
	return MStrFmt(pool,
		"#include <stdio.h>			"NL
		"int main(void) {			"NL
		"	printf(\"Hello world\");"NL
		"	return 0;				"NL
		"}							"NL
	);
}

MstrView create_makefile_buffer (MByteArray** pool, char* project_name) {
	if (!project_name) return EMPTYVIEW;

	return MStrFmt(pool,
		"CC = gcc"NL
		"DEBUG_FLAGS     = -Wpedantic -g -Wall -Wextra -Wconversion -Wsign-conversion -fsanitize=address -fsanitize=undefined -fsanitize=leak -std=c11"NL
		"CFLAGS          = $(DEBUG_FLAGS)"NLNL
		".PHONY: all clean								"NLNL
		"all: "$(project_name)							 NL
		$(project_name)": main.c 						"NL
		"	$(CC) $(CFLAGS) main.c -o "$(project_name)	 NLNL
		"clean:											"NL
		"	rm -f "$(project_name)						 NL
	);
}

int main(int argc, char** argv) { UNUSED(argc);
	__free(MByteArrayFree) MByteArray* pool = MByteArrayMalloc(400); 
    char* project_name = argv[1] ? argv[1] : NULL;
   	char* project_path = argv[2] ? argv[2] : NULL;
    MstrView path = {0};

    if (project_name)
    	path = MStrFmt(&pool, $(project_path != NULL ? project_path : ".")"/"$(project_name));
    else {
  		MPrintFmt("Ussage: <program-name> [project_path] <in_the_future: flags>");
    	return 1;
    }

	MPrintFmt(MstrViewFmt(path));
	if (IsEmptyView(MfileMkdir(path, 0755))) {
		MPrintFmt("Create directory fail: "$(strerror(errno)));
		return 1;
	}

	MstrView cmk_path = MStrFmt(&pool, MstrViewFmt(path)"/makefile");
	MstrView cmk_content = create_makefile_buffer(&pool, project_name);
	if (IsEmptyView(MfileWrite(cmk_path, cmk_content))) {
		MPrintFmt("Create makefile fail: "$(strerror(errno)));
		return 1;
	}

    MstrView cmf_path = MStrFmt(&pool, MstrViewFmt(path)"/main.c");
    if (IsEmptyView(MfileWrite(cmf_path, create_mainc_buffer(&pool)))) {
    	MPrintFmt("Create makefile fail: "$(strerror(errno)));
		return 1;
    }

 	MPrintFmt("Project at "MstrViewFmt(path)" created successfully.\n");
	return 0;
}
