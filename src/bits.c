/*
 * Copyright (c) 2021 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <netinet/in.h>
#include <stddef.h>

#include "bits.h"

uint16_t betoh16(uint16_t be)
{
	return ntohs(be);
}

uint32_t betoh32(uint32_t be)
{
	return ntohl(be);
}

uint64_t betoh64(uint64_t be)
{
	return (uint64_t) ntohl((uint32_t)(be & 0xffffffff)) << 32 |
		(uint64_t) ntohl((uint32_t)(be >> 32 & 0xffffffff));
}
