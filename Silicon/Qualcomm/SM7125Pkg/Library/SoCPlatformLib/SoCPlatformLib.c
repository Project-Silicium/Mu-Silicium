#include <Library/ArmLib.h>
#include <Library/SoCPlatformLib.h>

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  {
    // Cluster 0, Core 0
    0x000,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 0, Core 1
    0x100,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 0, Core 2
    0x200,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 0, Core 3
    0x300,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 0, Core 4
    0x400,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 0, Core 5
    0x500,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 1, Core 0
    0x600,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
  {
    // Cluster 1, Core 1
    0x700,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  }
};

ARM_CORE_INFO*
GetCoreTable ()
{
  return mArmPlatformMpCoreInfoTable;
}