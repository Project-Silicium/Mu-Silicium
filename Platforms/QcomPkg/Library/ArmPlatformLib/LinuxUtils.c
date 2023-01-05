#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#include <Library/MemoryMapHelperLib.h>
#include <Library/PlatformPrePiLib.h>

#include "LinuxUtils.h"

BOOLEAN IsLinuxAvailable(IN VOID *KernelLoadAddress)
{
  VOID *LinuxKernelAddr = KernelLoadAddress + FixedPcdGet32(PcdFdSize);
  UINT32 *LinuxKernelMagic = (UINT32*)(LinuxKernelAddr + LINUX_KERNEL_ARCH_MAGIC_OFFSET);
  return *LinuxKernelMagic == LINUX_KERNEL_AARCH64_MAGIC;
}

VOID BootLinux(IN VOID *DeviceTreeLoadAddress, IN VOID *KernelLoadAddress)
{
  VOID *LinuxKernelAddr = KernelLoadAddress + FixedPcdGet32(PcdFdSize);
  LINUX_KERNEL LinuxKernel = (LINUX_KERNEL) LinuxKernelAddr;

  DEBUG(
      (EFI_D_INFO | EFI_D_LOAD,
       "Kernel Load Address = 0x%llx, Device Tree Load Address = 0x%llx\n",
       LinuxKernelAddr, DeviceTreeLoadAddress));

  // Jump to linux
  LinuxKernel ((UINT64)DeviceTreeLoadAddress, 0, 0, 0);

  // We should never reach here
  CpuDeadLoop();
}

STATIC BOOLEAN RanOnceFlag = FALSE;

VOID ContinueToLinuxIfAllowed(IN VOID *DeviceTreeLoadAddress, IN VOID *KernelLoadAddress)
{
  if (RanOnceFlag) {
    return;
  }

  RanOnceFlag = TRUE;

  if (IsLinuxAvailable(KernelLoadAddress)) {
    DEBUG(
        (EFI_D_INFO | EFI_D_LOAD,
        "Kernel Load Address = 0x%llx, Device Tree Load Address = 0x%llx\n",
        KernelLoadAddress, DeviceTreeLoadAddress));

    if (IsLinuxBootRequested()) {
      BootLinux(DeviceTreeLoadAddress, KernelLoadAddress);
    }
  }
}

VOID ArmPlatformPeiBootAction(IN VOID *DeviceTreeLoadAddress, IN VOID *KernelLoadAddress)
{
  ContinueToLinuxIfAllowed(DeviceTreeLoadAddress, KernelLoadAddress);
}