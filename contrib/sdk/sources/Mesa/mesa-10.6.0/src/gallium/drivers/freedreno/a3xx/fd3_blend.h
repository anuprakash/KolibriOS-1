/* -*- mode: C; c-file-style: "k&r"; tab-width 4; indent-tabs-mode: t; -*- */

/*
 * Copyright (C) 2013 Rob Clark <robclark@freedesktop.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Rob Clark <robclark@freedesktop.org>
 */

#ifndef FD3_BLEND_H_
#define FD3_BLEND_H_

#include "pipe/p_state.h"
#include "pipe/p_context.h"

struct fd3_blend_stateobj {
	struct pipe_blend_state base;
	struct {
		/* Blend control bits for color if there is an alpha channel */
		uint32_t blend_control_rgb;
		/* Blend control bits for color if there is no alpha channel */
		uint32_t blend_control_no_alpha_rgb;
		/* Blend control bits for alpha channel */
		uint32_t blend_control_alpha;
		uint32_t control;
	} rb_mrt[4];
};

static INLINE struct fd3_blend_stateobj *
fd3_blend_stateobj(struct pipe_blend_state *blend)
{
	return (struct fd3_blend_stateobj *)blend;
}

void * fd3_blend_state_create(struct pipe_context *pctx,
		const struct pipe_blend_state *cso);

#endif /* FD3_BLEND_H_ */
