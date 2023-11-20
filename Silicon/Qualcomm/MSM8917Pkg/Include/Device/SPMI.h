/* Copyright (c) 2012,2014-2015 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
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
#ifndef _SPMI_H_
#define _SPMI_H_

// PMIC ARB Shift States
#define PMIC_ARB_CMD_OPCODE_SHIFT          27
#define PMIC_ARB_CMD_PRIORITY_SHIFT        26
#define PMIC_ARB_CMD_SLAVE_ID_SHIFT        20
#define PMIC_ARB_CMD_ADDR_SHIFT            12
#define PMIC_ARB_CMD_ADDR_OFFSET_SHIFT     4
#define PMIC_ARB_CMD_BYTE_CNT_SHIFT        0

// SPMI Commands
#define SPMI_CMD_EXT_REG_WRITE_LONG        0x0
#define SPMI_CMD_EXT_REG_READ_LONG         0x1
#define SPMI_CMD_EXT_REG_READ_LONG_DELAYED 0x2
#define SPMI_CMD_TRANSFER_BUS_OWNERSHIP    0x3

enum spmi_geni_cmd_return_value {
  SPMI_CMD_DONE,
  SMPI_CMD_DENIED,
  SPMI_CMD_FAILURE,
  SPMI_ILLEGAL_CMD,
  SPMI_CMD_OVERRUN = 6,
  SPMI_TX_FIFO_RD_ERR,
  SPMI_TX_FIFO_WR_ERR,
  SPMI_RX_FIFO_RD_ERR,
  SPMI_RX_FIFO_WR_ERR
};

enum pmic_arb_chnl_return_values {
  PMIC_ARB_CMD_DONE,
  PMIC_ARB_CMD_FAILURE,
  PMIC_ARB_CMD_DENIED,
  PMIC_ARB_CMD_DROPPED,
};

struct pmic_arb_cmd{
  UINT8 opcode;
  UINT8 priority;
  UINT8 slave_id;
  UINT8 address;
  UINT8 offset;
  UINT8 byte_cnt;
};

struct pmic_arb_param{
  UINT8 *buffer;
  UINT8  size;
};

#endif /* _SPMI_H_ */