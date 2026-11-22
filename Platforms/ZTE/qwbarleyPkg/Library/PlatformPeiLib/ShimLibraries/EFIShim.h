#ifndef _EFI_SHIM_H_
#define _EFI_SHIM_H_

/**
  This Function installs the Specified Library Instance.

  @param[in] LibName                       - The Name of the Library Instance.
  @param[in] LibVersion                    - The Version of the Library Instance.
  @param[in] LibIntf                       - The Pointer to the Library Instance.

  @return EFI_SUCCESS                      - The Library Instance was Installed Successfully. (Forced)
**/
typedef
EFI_STATUS
(*EFI_INSTALL_LIB) (
  IN CHAR8 *LibName,
  IN UINT32 LibVersion,
  IN VOID  *LibIntf
  );

/**
  This Function gets the Specified Library Instance.

  @param[in]  LibName                      - The Name of the Library Instance.
  @param[in]  LibVersion                   - The Version of the Library Instance.
  @param[out] LibIntf                      - The Pointer to the Library Instance.

  @return EFI_SUCCESS                      - The Library Instance was Successfully Found.
  @return EFI_INVALID_PARAMETER            - One or more Arguments are NULL.
  @return EFI_NOT_FOUND                    - The Specified Library Instance was not Found.
**/
typedef
EFI_STATUS
(*EFI_LOAD_LIB) (
  IN  CHAR8 *LibName,
  IN  UINT32 LibVersion,
  OUT VOID **LibIntfPtr
  );

//
// Shim Library Loader
//
typedef struct {
  UINT32          LoaderVersion;
  EFI_INSTALL_LIB InstallLib;
  EFI_LOAD_LIB    LoadLib;
} EFI_SHIM_LIBRARY_LOADER;

//
// Shim Library Instance Data
//
typedef struct {
  CHAR8 *LibName;
  VOID  *LibIntf;
} EFI_SHIM_LIBRARY_INSTANCE_DATA;

#endif /* _EFI_SHIM_H_ */
