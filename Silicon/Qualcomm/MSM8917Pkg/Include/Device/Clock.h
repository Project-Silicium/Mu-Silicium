/*
 * Copyright (c) 2012, 2014, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of The Linux Foundation nor
 *     the names of its contributors may be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INT32ERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _CLOCK_H_
#define _CLOCK_H_

enum clk_reset_action {
  CLK_RESET_DEASSERT = 0,
  CLK_RESET_ASSERT   = 1
};

struct clk;

struct clk_ops {
  INT32 (*enable)(struct clk *clk);
  VOID (*disable)(struct clk *clk);
  VOID (*auto_off)(struct clk *clk);
  INT32 (*reset)(struct clk *clk, enum clk_reset_action action);
  INT32 (*set_rate)(struct clk *clk, UINT32 rate);
  INT32 (*set_min_rate)(struct clk *clk, UINT32 rate);
  INT32 (*set_max_rate)(struct clk *clk, UINT32 rate);
  INT32 (*set_flags)(struct clk *clk, UINT32 flags);
  UINT32 (*get_rate)(struct clk *clk);
  INT32 (*list_rate)(struct clk *clk, UINT32 n);
  INT32 (*is_enabled)(struct clk *clk);
  INT32 (*round_rate)(struct clk *clk, UINT32 rate);
  INT32 (*set_parent)(struct clk *clk, struct clk *parent);
  struct clk *(*get_parent)(struct clk *clk);
  BOOLEAN (*is_local)(struct clk *clk);
};

/**
 * struct clk
 * @count: enable refcount
 * @lock: protects clk_enable()/clk_disable() path and @count
 */
struct clk {
  UINT32 flags;
  UINT32 rate;
  struct clk_ops *ops;
  CONST CHAR16 *dbg_name;
  UINT32 count;
};

struct clk_lookup {
  CONST CHAR16    *con_id;
  struct clk    *clk;
};

struct clk_list {
  struct clk_lookup *clist;
  UINT32 num;
};

#define CLK_LOOKUP(con, c) { .con_id = con, .clk = &c }

#endif /* _CLOCK_H_ */