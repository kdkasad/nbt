/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _STR_H_
#define _STR_H_

#include <stddef.h>

/*
 * Variable-length string.
 * Use the vstr_*(...) functions to handle the string.
 */
struct vstr {
	size_t len;
	size_t pos;
	char *s;
};

size_t vstr_append_printf(struct vstr *vstr, const char *fmt, ...);
struct vstr *vstr_create(size_t initsize);
void vstr_free(struct vstr *vstr);

#endif /* _STR_H_ */
