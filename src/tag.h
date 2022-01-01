/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _TAG_H_
#define _TAG_H_

#include <stdint.h>
#include <stdio.h>

enum tagtype {
	TAG_END,
	TAG_BYTE,
	TAG_SHORT,
	TAG_INT,
	TAG_LONG,
	TAG_FLOAT,
	TAG_DOUBLE,
	TAG_BYTE_ARRAY,
	TAG_STRING,
	TAG_LIST,
	TAG_COMPOUND,
	TAG_INT_ARRAY,
	TAG_LONG_ARRAY,
};

struct tag_payload_byte {
	char value;
};

struct tag_payload_short {
	short value;
};

struct tag_payload_int {
	int value;
};

struct tag_payload_long {
	long value;
};

struct tag_payload_float {
	float value;
};

struct tag_payload_double {
	double value;
};

struct tag_payload_byte_array {
	int size;
	char *array;
};

struct tag_payload_string {
	unsigned short len;
	char *str;
};

struct tag_payload_list {
	char tagid; /* type of list elements */
	int size;
	union payload *list;
};

struct tag_payload_compound {
	struct tag *head;
	struct tag *tail;
};

struct tag_payload_int_array {
	int size;
	int *array;
};

struct tag_payload_long_array {
	int size;
	long *array;
};

union payload {
	struct tag_payload_byte tp_byte;
	struct tag_payload_short tp_short;
	struct tag_payload_int tp_int;
	struct tag_payload_long tp_long;
	struct tag_payload_float tp_float;
	struct tag_payload_double tp_double;
	struct tag_payload_byte_array tp_byte_array;
	struct tag_payload_string tp_string;
	struct tag_payload_list tp_list;
	struct tag_payload_compound tp_compound;
	struct tag_payload_int_array tp_int_array;
	struct tag_payload_long_array tp_long_array;
};

struct tag {
	enum tagtype type;
	uint16_t namelen;
	char *name;
	union payload payload;
	struct tag *next;
};

struct tag *read_nbt_tag(FILE *stream);
void free_tag(struct tag *tag);

#endif /* _TAG_H_ */
