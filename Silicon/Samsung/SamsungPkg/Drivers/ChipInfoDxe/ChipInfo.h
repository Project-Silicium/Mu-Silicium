/**
  Copyright@ Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _CHIP_INFO_H_
#define _CHIP_INFO_H_

//
// Chip Revision
//
#define REVISION_MASK        0xF
#define MAJOR_REVISION_SHIFT 20
#define MINOR_REVISION_SHIFT 16

//
// Chip Info Structure
//
typedef struct {
  UINT32 Reserved1;
  UINT32 Id[2];
  UINT32 Reserved2;
  UINT32 Revision;
} EFI_CHIP_INFO;

#endif /* _CHIP_INFO_H_ */
