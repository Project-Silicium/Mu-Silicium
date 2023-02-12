/* Copyright (c) 2018-2020, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 *  with the distribution.
 *   * Neither the name of The Linux Foundation nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef DDRDETAILS_H
#define DDRDETAILS_H

#define MAX_IDX 8
#define MAX_NUM_CLOCK_PLAN 14

#define DDR_DETAILS_STRUCT_VERSION 0x0000000000040000
#define MAX_CHANNELS 2
#define MAX_RANKS 2

struct ddr_freq_table {
   UINT32 freq_khz;
   UINT8  enable;
};

typedef struct ddr_freq_plan_entry_info {
  struct ddr_freq_table ddr_freq[MAX_NUM_CLOCK_PLAN];
  UINT8  num_ddr_freqs;
  UINT32* clk_period_address;
} ddr_freq_plan_entry;

struct ddr_part_details {

  UINT8 revision_id1[2];
  UINT8 revision_id2[2];
  UINT8 width[2];
  UINT8 density[2];
};

typedef struct ddr_details_entry_info {
  UINT8 manufacturer_id;
  UINT8 device_type;
  struct ddr_part_details ddr_params[MAX_IDX];
  ddr_freq_plan_entry     ddr_freq_tbl;
  UINT8 num_channels;
  UINT8 num_ranks[MAX_CHANNELS];/* Number of ranks per channel */
  UINT8 hbb[MAX_CHANNELS][MAX_RANKS];/* Highest Bank Bit per rank per channel */
} ddr_details_entry;

#endif /* DDRDETAILS_H */
