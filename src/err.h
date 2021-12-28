/*
 * Copyright (c) 2021 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _ERR_H_
#define _ERR_H_

enum retval {
	RET_NONE,
	RET_USER_ERROR,
	RET_RUNTIME_ERROR,
	RET_INVALID_INPUT,
};

#endif /* _ERR_H_ */
