#include <Library/DebugLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/AcpiTableUpdateLib.h>
#include <Library/IoLib.h>

VOID
UpdateAcpiTables ()
{
  EFI_STATUS                   Status;
  EFI_ACPI_DESCRIPTION_HEADER *DsdtTable;
  UINTN                        DsdtHandle;

  // Locate DSDT Table
  Status = LocateTableBySignature (EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE, &DsdtTable, &DsdtHandle);
  if (EFI_ERROR (Status) && Status != EFI_NOT_FOUND) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Locate DSDT Table! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  // Verify Table Presense
  if (Status == EFI_NOT_FOUND) {
    return;
  }

  // Set Default Values
  UINT32 STOR = 0x1;

  // Get Values from Memory
  UINT32 EMUL = MmioRead32 (0x01FC8004) & 0x3;

  // Update DSDT Names
  AslUpdateName (DsdtTable, SIGNATURE_32 ('E', 'M', 'U', 'L'), &EMUL, 4);
  AslUpdateName (DsdtTable, SIGNATURE_32 ('S', 'T', 'O', 'R'), &STOR, 4);

  // Reinstall DSDT Table
  Status = ReinstallTable (DsdtTable, &DsdtHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Reinstall DSDT Table! Status = %r\n", __FUNCTION__, Status));
  }
}
