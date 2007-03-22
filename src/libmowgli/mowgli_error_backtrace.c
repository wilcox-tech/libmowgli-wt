/*
 * libmowgli: A collection of useful routines for programming.
 * mowgli_error_backtrace.c: Print errors and explain how they were reached.
 *
 * Copyright (c) 2007 William Pitcock <nenolod -at- sacredspiral.co.uk>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "mowgli.h"

void
mowgli_error_context_display(mowgli_error_context_t *e, const char *delim)
{
	mowgli_node_t *n, *tn;
	char *bt_msg;

	return_if_fail(e != NULL);
	return_if_fail(delim != NULL);

	if (MOWGLI_LIST_LENGTH(&e->bt) == 0)
		mowgli_throw_exception(mowgli.error_backtrace.no_backtrace);

	MOWGLI_LIST_FOREACH_SAFE(n, tn, e->bt.head)
	{
		bt_msg = (char *) n->data;

		mowgli_node_delete(n, &e->bt);

		printf("%s%s", bt_msg, n->next != NULL ? delim : "");

		mowgli_free(bt_msg);
	}
}

void
mowgli_error_context_push(mowgli_error_context_t *e, const char *msg, ...)
{
	char buf[65535];
	va_list va;

	return_if_fail(e != NULL);
	return_if_fail(msg != NULL);

	va_start(va, msg);
	vsnprintf(buf, 65535, msg, va);
	va_end(va);

	mowgli_node_add(strdup(buf), mowgli_node_create(), &e->bt);
}

void
mowgli_error_context_pop(mowgli_error_context_t *e)
{
	mowgli_queue_t *q;

	return_if_fail(e != NULL);

	mowgli_node_delete(e->bt.tail, &e->bt);
}

mowgli_error_context_t *
mowgli_error_context_create(void)
{
	mowgli_error_context_t *out = mowgli_alloc(sizeof(mowgli_error_context_t));

	return out;
}