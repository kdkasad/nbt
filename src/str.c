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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "str.h"

/*
 * Append text to a vstr object using printf format string.
 *
 * Usage is identical to sprintf(3) except a vstr is used as the destination
 * instead of a char pointer.
 *
 * Returns the number of characters appended to the vstr.
 */
size_t vstr_append_printf(struct vstr *vstr, const char *fmt, ...)
{
#define DEFAULT_SIZE 64

	va_list ap;
	size_t added = 0;
	size_t tlen = DEFAULT_SIZE;
	char *tmp = malloc(tlen);
	if (!tmp)
		error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 2, "malloc");

	/* Repeatedly attempt to write to temporary string. Doubles the size of
	 * the temporary string each time it is too small. */
	va_start(ap, fmt);
	while ((added = vsnprintf(tmp, tlen, fmt, ap)) >= tlen) {
		tlen *= 2;
		free(tmp);
		tmp = malloc(tlen);
		if (!tmp)
			error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 2, "malloc");
	}
	va_end(ap);

	/* Append the temporary string to the vstr, resizing if necessary */
	if (vstr->len < vstr->pos + added + 1) {
		vstr->len = vstr->pos + added + 1;
		vstr->s = realloc(vstr->s, vstr->len);
	}
	strcat(vstr->s, tmp);

	free(tmp);
	return added;

#undef DEFAULT_SIZE
}

/*
 * Allocate and initialize a vstr object.
 *
 * initsize: The initial size to use for the string. If 0, a preset default is
 * used.
 */
struct vstr *vstr_create(size_t initsize)
{
#define DEFAULT_SIZE 64

	struct vstr *vstr = malloc(sizeof(struct vstr));
	vstr->len = initsize;
	vstr->pos = 0;
	vstr->s = calloc(vstr->len, sizeof(char));
	return vstr;

#undef DEFAULT_SIZE
}

/*
 * Free a vstr object and its contents
 */
void vstr_free(struct vstr *vstr)
{
	free(vstr->s);
	free(vstr);
}
