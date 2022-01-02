/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <error.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg.h"
#include "err.h"
#include "print.h"
#include "tag.h"

static void print_usage(const char *argv0);

static struct nbt_config cfg = {
	.in = NULL,
	.out = NULL,
	.print_whitespace = true,
};

int main(int argc, char *argv[])
{
	int c;
	int err = 0; /* if nonzero, error ocurred parsing options */
	static const char *optstring = ":hs";
	while ((c = getopt(argc, argv, optstring)) != -1) {
		switch (c) {
		case 'h':
			print_usage(argv[0]);
			exit(RET_NONE);
			break;
		case 's':
			cfg.print_whitespace = false;
			break;
		case ':':
			error_at_line(RET_NONE, 0, __FILE__, __LINE__ - 1, "option '-%c' requires an argument", optopt);
			err++;
			break;
		case '?':
			error_at_line(RET_NONE, 0, __FILE__, __LINE__ - 1, "unrecognized option '-%c'", optopt);
			err++;
			break;
		}
	}

	/* exit after parsing all options so all errors are printed to user */
	if (err) {
		fputs("See the usage information with the '-h' option.\n", stderr);
		exit(RET_USER_ERROR);
	}

	/* set input file */
	if (optind < argc) {
		if (!(cfg.in = fopen(argv[optind], "rb")))
			error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 1, "%s", argv[optind]);
		optind++;
	} else {
		/* if none specified, default to stdin */
		cfg.in = stdin;
	}

	/* set output file */
	if (optind < argc) {
		if (!(cfg.out = fopen(argv[optind], "w")))
			error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 1, "%s", argv[optind]);
		optind++;
	} else {
		/* if none specified, default to stdout */
		cfg.out = stdout;
	}

	/* attempt to read one tag */
	struct tag *tag = read_tag(cfg.in);
	if (!tag)
		error_at_line(RET_RUNTIME_ERROR, 0, __FILE__, __LINE__ - 2, "no tag was read");

	print_tag(tag, cfg.print_whitespace, cfg.out);

	free(tag);
	return 0;
}

void print_usage(const char *argv0)
{
	printf(
"Usage:\n"
"\t%s [options] [infile [outfile]]\n"
"\n"
"Options:\n"
"	-h	Print this usage information\n"
"	-s	Don't print formatting whitespace\n"
"\n"
"Copyright (c) " COPYRIGHT_YEAR " " COPYRIGHT_NAME "\n"
"This program is distributed under a modified BSD license. See\n"
"the LICENSE file provided with the source code for details.\n"
	, argv0);
}
