#ifndef _EFI_MAZE_DET_H_
#define _EFI_MAZE_DET_H_

//
// Global GUID for the MazeDet Protocol
//
#define EFI_MAZE_DET_PROTOCOL_GUID { 0xB5902DCC, 0x5B30, 0x4678, { 0xB9, 0x09, 0x99, 0x52, 0xF1, 0xF3, 0xA5, 0x67 } }

//
// Declare forward reference to the MazeDet Protocol
//
typedef struct _EFI_MAZE_DET_PROTOCOL EFI_MAZE_DET_PROTOCOL;

/**
  This Function Terminates Maze.

  @return Status        The EFI_STATUS returned by the Function.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_TERMINATE_MAZE) (VOID);

//
// Define Protocol Functions
//
struct _EFI_MAZE_DET_PROTOCOL {
  UINT64              Revision;
  EFI_TERMINATE_MAZE  TerminateMaze;

  // NOTE: There are maybe more Functions
};

extern EFI_GUID gMazeDetProtocolGuid;

#endif /* _EFI_MAZE_DET_H_ */
