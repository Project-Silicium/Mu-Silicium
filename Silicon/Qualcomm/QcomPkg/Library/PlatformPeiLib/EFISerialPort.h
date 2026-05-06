#ifndef _EFI_SERIAL_PORT_H_
#define _EFI_SERIAL_PORT_H_

/**
  This Function Reads Data from the Serial Device.

  @param[out] Buffer                       - The Data from the Serial Device.
  @param[in]  NumberOfBytes                - The Amount of Bytes for the Data.

  @return BytesRead                        - The Amount of Bytes Read.
**/
typedef
UINTN
(*EFI_SIO_READ) (
  OUT UINT8 *Buffer,
  IN  UINTN  NumberOfBytes
  );

/**
  This Function Writes Data to the Serial Device.

  @param[in] Buffer                        - The Data for the Serial Device.
  @param[in] NumberOfBytes                 - The Amount of Bytes of the Data.

  @return BytesWritten                     - The Amount of Bytes Written.
**/
typedef
UINTN
(*EFI_SIO_WRITE) (
  IN UINT8 *Buffer,
  IN UINTN  NumberOfBytes
  );

/**
  This Function Polls the Serial Device to see if any Data is waiting to be Read.

  @return TRUE                             - There is Data waiting to be Read.
  @return FALSE                            - There is no Data waiting to be Read.
**/
typedef
BOOLEAN
(*EFI_SIO_POLL) ();

/**
  This Function ...

  @return ...                              - ...
**/
typedef
UINTN
(*EFI_SIO_DRAIN) ();

/**
  This Function ...

  @return ...                              - ...
**/
typedef
UINTN
(*EFI_SIO_FLUSH) ();

/**
  This Function ...

  @param[in] Arg                           - ...
  @param[in] Param                         - ...

  @return ...                              - ...
**/
typedef
UINTN
(*EFI_SIO_CONTROL) (
  IN UINTN Arg,
  IN UINTN Param
  );

/**
  This Function sets the Attributes of the Serial Device.

  @param[in, out] BaudRate                 - The current/new Baud Rate of/for the Serial Device.
  @param[in, out] ReceiveFifoDepth         - The current/new FIFO Depth of/for the Serial Device.
  @param[in, out] Timeout                  - The current/new Timeout of/for the Serial Device.
  @param[in, out] Parity                   - The current/new Parity Type of/for the Serial Device.
  @param[in, out] DataBits                 - The current/new Data Bits of/for the Serial Device.
  @param[in, out] StopBits                 - The current/new Stop Bits of/for the Serial Device.

  @return EFI_SUCCESS                      - The new Attributes were set Successfully.
  ...
**/
typedef
EFI_STATUS
(*EFI_SIO_SETATTRIBUTES) (
  IN OUT UINT64             *BaudRate,
  IN OUT UINT32             *ReceiveFifoDepth,
  IN OUT UINT32             *Timeout,
  IN OUT EFI_PARITY_TYPE    *Parity,
  IN OUT UINT8              *DataBits,
  IN OUT EFI_STOP_BITS_TYPE *StopBits
  );

//
// Serial Port Library
//
typedef struct {
  UINT32                LibVersion;
  EFI_SIO_READ          Read;
  EFI_SIO_WRITE         Write;
  EFI_SIO_POLL          Poll;
  EFI_SIO_DRAIN         Drain;
  EFI_SIO_FLUSH         Flush;
  EFI_SIO_CONTROL       Control;
  EFI_SIO_SETATTRIBUTES SetAttributes;
} EFI_SERIAL_PORT_LIBRARY;

#endif /* _EFI_SERIAL_PORT_H_ */
