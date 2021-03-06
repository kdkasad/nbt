/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _CFG_H_
#define _CFG_H_

#include <stdbool.h>
#include <stdio.h>

struct nbt_config {
	FILE *in;
	FILE *out;
	bool print_whitespace;
};

#endif /* _CFG_H_ */
