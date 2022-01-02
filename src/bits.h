/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _BITS_H_
#define _BITS_H_

#include <stdint.h>

uint16_t betoh16(uint16_t be);
uint32_t betoh32(uint32_t be);
uint64_t betoh64(uint64_t be);

#endif /* _BITS_H_ */
