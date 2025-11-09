/**
  Copyright (C) 2014 Samsung Electronics

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
**/

#ifndef _PMU_H_
#define _PMU_H_

//
// PMU Registers
//
#define PS_HOLD_CONTROL 0x030C
#define RST_STAT        0x0404
#define MAGIC_INFORM    0x0808
#define PANIC_INFORM    0x080C
#define SYSIP_DAT0      0x0810
#define SWRESET         0x3A00

//
// PMU Values
//
#define SWRESET_TRIGGER (1 << 1)

//
// Power Flags
//
#define SEC_REBOOT_NORMAL 0x4E
#define SEC_REBOOT_LPM    0x70

//
// Reset Prefixes
//
#define RESET_SET_PREFIX 0xABC00000

//
// Reset Reasons
//
#define RESET_REASON_UNKNOWN (RESET_SET_PREFIX | 0)

#endif /* _PMU_H_ */
