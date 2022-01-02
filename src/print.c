/*
 * Copyright (c) 2021-2022 Kian Kasad
 *
 * This file is distributed under a modified BSD license. See the provided
 * LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "diag.h"
#include "print.h"
#include "tag.h"

static size_t print_payload(enum tagtype type, union payload *payload, bool print_whitespace, FILE *stream);

static unsigned int indent_level = 0;
static char *indent_str = "    ";

static const char numtype_suffixes[] = {
	[TAG_BYTE] = 'b',
	[TAG_SHORT] = 's',
	[TAG_LONG] = 'l',
	[TAG_FLOAT] = 'f',
	[TAG_DOUBLE] = 'd',
};

size_t print_tag(struct tag *tag, bool print_whitespace, FILE *stream)
{
	size_t out = 0;

	if (print_whitespace) {
		/* print newline before tags with names and end tags */
		if ((tag->name && *tag->name) || tag->type == TAG_END) {
			out += fprintf(stream, "\n");
		}

		/* print indent. if tag is an end tag, print one less indent than the
		 * current indent level. */
		for (int i = 0; i < indent_level - (tag->type == TAG_END ? 1 : 0); i++)
			out += fprintf(stream, "%s", indent_str);
	}

	/* print tag name */
	if (tag->name && *tag->name) {
		out += fprintf(stream, "%s:%s", tag->name, (print_whitespace ? " " : ""));
	}

	out += print_payload(tag->type, &tag->payload, print_whitespace, stream);
	return out;
}

size_t print_payload(enum tagtype type, union payload *payload, bool print_whitespace, FILE *stream)
{
#define PRINT_INTTYPE(value) fprintf(stream, "%ld%c", (long) value, numtype_suffixes[type])
#define PRINT_FLOATTYPE(value) fprintf(stream, "%f%c", value, numtype_suffixes[type])
#define PRINT_ARRAY(payload_struct, suffix)  \
	{ \
		size_t out = 0; \
		out += fprintf(stream, "[B;"); \
		for (int i = 0; i < payload->payload_struct.size; i++) { \
			out += fprintf(stream, "%ld%s", (long) payload->payload_struct.array[i], suffix); \
			/* print comma after all but last element */ \
			if (i < payload->payload_struct.size - 1) \
			out += fprintf(stream, ",%s", (print_whitespace ? " " : "")); \
		} \
		out += fprintf(stream, "]"); \
		return out; \
	}

	switch (type) {
	case TAG_BYTE:
		return PRINT_INTTYPE(payload->tp_byte.value);
		break;

	case TAG_SHORT:
		return PRINT_INTTYPE(payload->tp_short.value);
		break;

	case TAG_INT:
		return fprintf(stream, "%d", payload->tp_int.value);
		break;

	case TAG_LONG:
		return PRINT_INTTYPE(payload->tp_long.value);
		break;

	case TAG_FLOAT:
		return PRINT_FLOATTYPE(payload->tp_float.value);
		break;

	case TAG_DOUBLE:
		return PRINT_FLOATTYPE(payload->tp_double.value);
		break;

	case TAG_STRING:
		{
			size_t out;
			char *escapedstr;
			unsigned short estrpos = 0;
			unsigned short nquotes = 0;

			/* count number of quotes in string */
			for (char *c = payload->tp_string.str; *c; c++)
				if (*c == '"')
					nquotes++;

			/* allocate a block of memory the size of the length of
			 * the string plus the number of quotes, so we can add
			 * a backslack before each quote in the string. we also
			 * add one byte at the end for a terminating null byte. */
			escapedstr = malloc(payload->tp_string.len + nquotes + 1);

			/* copy all characters from the original string to the
			 * new string, inserting a backslash before each quote
			 * character. */
			for (char *c = payload->tp_string.str; *c; c++) {
				if (*c == '"')
					escapedstr[estrpos++] = '\\';
				escapedstr[estrpos++] = *c;
			}
			escapedstr[estrpos] = '\0';

			out = fprintf(stream, "\"%s\"", escapedstr);

			free(escapedstr);
			return out;
		}
		break;

	case TAG_END:
		;
		indent_level--;
		return fprintf(stream, "}");
		break;


	case TAG_COMPOUND:
		{
			size_t out = 0;
			out += fprintf(stream, "{");
			indent_level++;

			/* print child tags */
			struct tag *c;
			/* skip last two */
			for (c = payload->tp_compound.head; c; c = c->next) {
				out += print_tag(c, print_whitespace, stream);
				/* if not either the second-to-last or last
				 * elements, append a comma */
				if ( !(c->type == TAG_END || c->next->type == TAG_END) )
					out += fprintf(stream, ",");
			}

			return out;
		}
		break;

	case TAG_LIST:
		{
			size_t out = 0;
			out += fprintf(stream, "[");

			for (int i = 0; i < payload->tp_list.size; i++) {
				out += print_payload(payload->tp_list.tagid, &payload->tp_list.list[i], print_whitespace, stream);
				/* print comma after all but last element */
				if (i < payload->tp_list.size - 1)
					out += fprintf(stream, ",");
			}

			out += fprintf(stream, "]");
			return out;
		}
		break;

	case TAG_BYTE_ARRAY:
		PRINT_ARRAY(tp_byte_array, "b");
		break;

	case TAG_INT_ARRAY:
		PRINT_ARRAY(tp_int_array, "");
		break;

	case TAG_LONG_ARRAY:
		PRINT_ARRAY(tp_long_array, "l");
		break;

	default:
		return 0;
		break;
	}

#undef PRINT_INTTYPE
#undef PRINT_FLOATTYPE
#undef PRINT_ARRAY
}
