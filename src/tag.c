/*
 * Copyright (c) 2021 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <error.h>
#include <errno.h>
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "print.h"
#include "tag.h"

static void read_payload(enum tagtype type, union payload *dest, FILE *stream);

/*
 * Read a tag from a file.
 *
 * Returns a dynamically-allocated tag. The returned tag must be freed by the
 * caller.
 */
struct tag *read_tag(FILE *stream)
{
	/* attempt to read the new tag's type */
	int c = fgetc(stream);
	if (c == EOF)
		return NULL;

	/* allocate a tag */
	struct tag *tag = malloc(sizeof(*tag));
	if (!tag)
		error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 2, "error allocating memory");

	/* set the new tag's type */
	enum tagtype type = (char)c;
	tag->type = type;

	/* there is no next tag yet */
	tag->next = NULL;

	if (tag->type == TAG_END)
		return tag;

	/* read name length */
	fread(&tag->namelen, sizeof(tag->namelen), 1, stream);
	tag->namelen = be16toh(tag->namelen);

	/* read name */
	tag->name = malloc(tag->namelen + 1);
	fread(tag->name, 1, tag->namelen, stream);
	tag->name[tag->namelen] = '\0';

	read_payload(tag->type, &tag->payload, stream);

	return tag;
}

void read_payload(enum tagtype type, union payload *dest, FILE *stream)
{
#define READ_INT_VALUE(payload_type) \
	fread(&dest->payload_type.value, sizeof(dest->payload_type.value), 1, stream);

	/* temporary byte array for reading float/double types */
	uint8_t bytes[8] = { 0 };

	switch (type) {
	case TAG_END:
		break;

	case TAG_BYTE:
		READ_INT_VALUE(tp_byte);
		break;

	case TAG_SHORT:
		READ_INT_VALUE(tp_short);
		dest->tp_short.value = be16toh(dest->tp_short.value);
		dest->tp_short.value = le16toh(dest->tp_short.value);
		break;

	case TAG_INT:
		READ_INT_VALUE(tp_int);
		dest->tp_int.value = be32toh(dest->tp_short.value);
		break;

	case TAG_LONG:
		READ_INT_VALUE(tp_long);
		dest->tp_long.value = be64toh(dest->tp_short.value);
		break;

	case TAG_FLOAT:
		fread(bytes, 4, 1, stream);
		// shift bytes to their respective positions
		*((uint32_t*)&dest->tp_float.value) = 0
			| ((uint32_t) bytes[0] << 24)
			| ((uint32_t) bytes[1] << 16)
			| ((uint32_t) bytes[2] << 8)
			| ((uint32_t) bytes[3] << 0);
		break;

	case TAG_DOUBLE:
		fread(bytes, 8, 1, stream);
		// shift bytes to their respective positions
		*((uint64_t*)&dest->tp_double.value) = 0
						      | ((uint64_t) bytes[0] << 56)
						      | ((uint64_t) bytes[1] << 48)
						      | ((uint64_t) bytes[2] << 40)
						      | ((uint64_t) bytes[3] << 32)
						      | ((uint64_t) bytes[4] << 24)
						      | ((uint64_t) bytes[5] << 16)
						      | ((uint64_t) bytes[6] << 8)
						      | ((uint64_t) bytes[0] << 0);
		break;

	case TAG_STRING:
		/* read length */
		fread(&dest->tp_string.len, sizeof(dest->tp_string.len), 1, stream);
		dest->tp_string.len = be16toh(dest->tp_string.len);
		/* allocate & read string */
		dest->tp_string.str = malloc(dest->tp_string.len);
		if (!dest->tp_string.str)
			error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 2, "failed to allocate memory");
		if (fread(dest->tp_string.str, 1, dest->tp_string.len, stream) < dest->tp_string.len) {
			if (feof(stream))
				error_at_line(RET_INVALID_INPUT, 0, __FILE__, __LINE__ - 2, "reached end of file");
			else
				error_at_line(RET_RUNTIME_ERROR, errno, __FILE__, __LINE__ - 4, "error reading input");
		}
		break;

	case TAG_COMPOUND:
		{
			/* for convenience, p points to payload */
			struct tag_payload_compound *p = &dest->tp_compound;
			/* read tags until an end tag is reached */
			struct tag *t;
			p->tail = p->head = t = read_tag(stream);
			while (t->type != TAG_END) {
				t = read_tag(stream);
				p->tail->next = t;
				p->tail = p->tail->next;
			}
		}
		break;

	case TAG_LIST:
		{
			/* for convenience, p points to payload */
			struct tag_payload_list *p = &dest->tp_list;
			/* read tag list type */
			fread(&p->tagid, sizeof(p->tagid), 1, stream);
			/* read list size */
			fread(&p->size, sizeof(p->size), 1, stream);
			p->size = be32toh(p->size);
			/* allocate payload array */
			p->list = calloc(p->size, sizeof(union payload));
			/* read p->size elements */
			for (int i = 0; i < p->size; i++) {
				read_payload(p->tagid, &p->list[i], stream);
			}
		}
		break;

	case TAG_BYTE_ARRAY:
		{
			/* for convenience, p points to payload */
			struct tag_payload_byte_array *p = &dest->tp_byte_array;
			/* read array size */
			fread(&p->size, sizeof(p->size), 1, stream);
			p->size = be32toh(p->size);
			/* allocte array */
			p->array = calloc(p->size, sizeof(*p->array));
			/* read array elements */
			fread(p->array, sizeof(*p->array), p->size, stream);
		}
		break;

	case TAG_INT_ARRAY:
		{
			/* for convenience, p points to payload */
			struct tag_payload_int_array *p = &dest->tp_int_array;
			/* read array size */
			fread(&p->size, sizeof(p->size), 1, stream);
			p->size = be32toh(p->size);
			/* allocte array */
			p->array = calloc(p->size, sizeof(*p->array));
			/* read array elements */
			fread(p->array, sizeof(*p->array), p->size, stream);
			/* correct byte order of elements */
			for (int i = 0; i < p->size; i++)
				p->array[i] = be32toh(p->array[i]);
		}
		break;

	case TAG_LONG_ARRAY:
		{
			/* for convenience, p points to payload */
			struct tag_payload_long_array *p = &dest->tp_long_array;
			/* read array size */
			fread(&p->size, sizeof(p->size), 1, stream);
			p->size = be32toh(p->size);
			/* allocte array */
			p->array = calloc(p->size, sizeof(*p->array));
			/* read array elements */
			fread(p->array, sizeof(*p->array), p->size, stream);
			/* correct byte order of elements */
			for (int i = 0; i < p->size; i++)
				p->array[i] = be64toh(p->array[i]);
		}
		break;
	}
}

void free_tag(struct tag *tag)
{
	switch (tag->type) {
		case TAG_STRING:
			free(tag->payload.tp_string.str);
			break;

		case TAG_COMPOUND:
			for (struct tag *c = tag->payload.tp_compound.head; c; /* nothing */) {
				/* use payload's head as a temporary variable */
				tag->payload.tp_compound.head = c;
				c = c->next;
				free(tag->payload.tp_compound.head);
			}
			break;

		case TAG_LIST:
			free(tag->payload.tp_list.list);
			break;

		case TAG_BYTE_ARRAY:
			free(tag->payload.tp_byte_array.array);
			break;

		case TAG_INT_ARRAY:
			free(tag->payload.tp_int_array.array);
			break;

		case TAG_LONG_ARRAY:
			free(tag->payload.tp_long_array.array);
			break;

		case TAG_END:
		case TAG_BYTE:
		case TAG_SHORT:
		case TAG_INT:
		case TAG_LONG:
		case TAG_FLOAT:
		case TAG_DOUBLE:
			/* These tags don't hold any dynamically-allocated
			 * payloads, so do nothing.
			 * These cases are still included here though so that
			 * the compiler can warn about missing cases if new tag
			 * types are added. */
			break;
	}

	// free the name string if it exists
	if (tag->name)
		free(tag->name);

	// free the tag structure
	free(tag);
}
