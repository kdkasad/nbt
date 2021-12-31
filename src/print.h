/*
 * Copyright (c) 2021 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _PRINT_H_
#define _PRINT_H_

#include <stddef.h>
#include "tag.h"

size_t print_tag(struct tag *tag, FILE *stream);

#endif /* _PRINT_H_ */
