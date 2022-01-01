/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "str.h"

int test_str_h(void)
{
	struct vstr *s = vstr_create(0);

	vstr_append(s, "Hello, world!\n");
	assert(!strcmp("Hello, world!\n", s->s));
	vstr_free(s);

	s = vstr_create(10);
	vstr_append(s, "This is more than %d characters\n", 10);
	assert(!strcmp("This is more than 10 characters\n", s->s));
	vstr_free(s);

	return 0;
}

int main(int argc, char **argv)
{
#define DO_TEST(name) \
	printf("Test for " #name ".h returned: %d\n", test_##name##_h());

	DO_TEST(str);

#undef DO_TEST
}
