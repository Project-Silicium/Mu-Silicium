# OpenSSL Native Instructions

The OpenSSL assembly files are traditionally generated at build time using a perl script.
To avoid that burden on EDK2 users, these end-result assembly files are generated during
the configuration steps through process_files.pl.

## Generating the assembly files

This only needs to be done when updating to a new OpenSSL version.

Due to the script wrapping required to process the OpenSSL configuration data,
each native architecture must be processed individually (in addition to the standard version).
Furthermore the standard version must be processed first before processing any other ARCH.

Current supported ARCHs: [X64, X64Gcc, A32].

[From the OpenSSL library directory]

  1. ./process_files.pl
  2. ./process_files.pl X64
  3. ./process_files.pl [Arch] etc.

## How include on your platform

To include the precompiled OpenSSL native instructions you need to reference the specific
architecture .inf file within you dsc file as well as include the appropriate align size
within the build options.

Note: In the build options the correct alignments are:

  1. 256 for 64-bit
  2. 64 for 32-bit

Note: Specificity of where to put the buildoptions and library class references will vary.

Example with X64:

  ``` DSC
  [LibraryClasses]
    OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLibX64.inf
  ```

  ``` BuildOptions
  [BuildOptions.X64]
    MSFT:*_*_*_DLINK_FLAGS = /ALIGN:256
  ```

Example with IA32:

  ``` DSC
  [LibraryClasses]
    OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLibIA32.inf
  ```

  ``` BuildOptions
  [BuildOptions.X64]
    MSFT:*_*_*_DLINK_FLAGS = /ALIGN:64
  ```
