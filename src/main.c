#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "print.h"
#include "tag.h"

int main(int argc, char *argv[])
{
	FILE *f = stdin;
	if (argc >= 2) {
		f = fopen(argv[1], "rb");
		if (!f)
			error_at_line(RET_USER_ERROR, errno, __FILE__, __LINE__ - 2, "error opening file");
	}

	struct tag *tag = read_tag(f);
	if (!tag)
		error_at_line(RET_RUNTIME_ERROR, 0, __FILE__, __LINE__ - 2, "no tag was read");
	print_tag(tag);
	free(tag);

	return 0;
}
