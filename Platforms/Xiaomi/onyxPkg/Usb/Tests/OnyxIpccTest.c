/** IPCC IRQ ownership / mailbox semantics, using production implementation. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef NULL
#include "../OnyxIpccDxe.c"
EFI_BOOT_SERVICES *gBS;
EFI_GUID gHardwareInterrupt2ProtocolGuid;
EFI_GUID gEfiEventExitBootServicesGuid;
#define CHECK(X) do { if (!(X)) { fprintf(stderr,"line %d: %s\n",__LINE__,#X); exit(1); } } while(0)
STATIC EFI_BOOT_SERVICES Bs;
STATIC EFI_HARDWARE_INTERRUPT2_PROTOCOL Gic;
STATIC EFI_TPL Tpl=TPL_APPLICATION;
STATIC BOOLEAN Enabled;
STATIC BOOLEAN FailRegister;
STATIC UINT32 Queue[100], ReadIndex, WriteIndex, Clears, Callbacks, Eois, LastIndex, LastSignal;
STATIC UINTN LastWriteAddress;
STATIC UINT32 LastWrite;
STATIC HARDWARE_INTERRUPT_HANDLER Handler;
VOID *EFIAPI ZeroMem(VOID *Buffer,UINTN Length) { return memset(Buffer,0,Length); }
UINT32 EFIAPI MmioRead32(UINTN Address) {
  CHECK(Address==0x406010);
  return ReadIndex<WriteIndex ? Queue[ReadIndex] : MAX_UINT32;
}
UINT32 EFIAPI MmioWrite32(UINTN Address,UINT32 Value) {
  LastWriteAddress=Address; LastWrite=Value;
  if(Address==0x40601c) { CHECK(ReadIndex<WriteIndex && Queue[ReadIndex]==Value); ReadIndex++; Clears++; }
  return Value;
}
UINT32 EFIAPI MmioAnd32(UINTN Address,UINT32 Mask) { CHECK(Address==0x406008 && Mask==(UINT32)~BIT0); return 0; }
STATIC EFI_TPL EFIAPI Raise(EFI_TPL NewTpl) { EFI_TPL Old=Tpl; CHECK(NewTpl>=Old); Tpl=NewTpl; return Old; }
STATIC VOID EFIAPI Restore(EFI_TPL Old) { CHECK(Old<=Tpl); Tpl=Old; }
STATIC EFI_STATUS EFIAPI Enable(EFI_HARDWARE_INTERRUPT2_PROTOCOL *This,HARDWARE_INTERRUPT_SOURCE Source) { CHECK(Source==261); Enabled=TRUE; return EFI_SUCCESS; }
STATIC EFI_STATUS EFIAPI Disable(EFI_HARDWARE_INTERRUPT2_PROTOCOL *This,HARDWARE_INTERRUPT_SOURCE Source) { CHECK(Source==261); Enabled=FALSE; return EFI_SUCCESS; }
STATIC EFI_STATUS EFIAPI Eoi(EFI_HARDWARE_INTERRUPT2_PROTOCOL *This,HARDWARE_INTERRUPT_SOURCE Source) { CHECK(!Enabled && Source==261); Eois++; return EFI_SUCCESS; }
STATIC EFI_STATUS EFIAPI Register(EFI_HARDWARE_INTERRUPT2_PROTOCOL *This,HARDWARE_INTERRUPT_SOURCE Source,HARDWARE_INTERRUPT_HANDLER Fn) {
 CHECK(Source==261); if(FailRegister) { return EFI_DEVICE_ERROR; } Handler=Fn; Enabled=TRUE; return EFI_SUCCESS;
}
STATIC EFI_STATUS EFIAPI Trigger(EFI_HARDWARE_INTERRUPT2_PROTOCOL *This,HARDWARE_INTERRUPT_SOURCE Source,EFI_HARDWARE_INTERRUPT2_TRIGGER_TYPE Type) {
 CHECK(Source==261 && Type==EFI_HARDWARE_INTERRUPT2_TRIGGER_LEVEL_HIGH); return EFI_SUCCESS;
}
STATIC VOID EFIAPI Receive(VOID *Context,UINT32 Index,UINT32 Signal) {
 CHECK(Context==(VOID *)0x1234 && !Enabled && Tpl==TPL_CALLBACK);
 CHECK(Clears==Callbacks+1); Callbacks++; LastIndex=Index; LastSignal=Signal;
}
STATIC VOID Pump(VOID) { EFI_TPL Old=Raise(TPL_CALLBACK); IpccPump(NULL,NULL); Restore(Old); }
int main(void) {
 VOID *Handle=NULL,*Other=NULL; UINT32 I; EFI_SYSTEM_CONTEXT Context={0};
 Bs.RaiseTPL=Raise; Bs.RestoreTPL=Restore; gBS=&Bs;
 Gic.RegisterInterruptSource=Register; Gic.DisableInterruptSource=Disable;
 Gic.EnableInterruptSource=Enable; Gic.EndOfInterrupt=Eoi; Gic.SetTriggerType=Trigger; mIpccGic=&Gic;
 FailRegister=TRUE;
 CHECK(IpccOpen(&Handle,0)==-1 && Handle==NULL && !mIpccBanks[0].Started);
 FailRegister=FALSE;
 CHECK(IpccOpen(&Handle,0)==0 && Handler!=NULL && !Enabled);
 CHECK(IpccRegister(Handle,33,2,2,Receive,(VOID *)0x1234)==0);
 CHECK(LastWriteAddress==0x406014 && LastWrite==0x210002);
 CHECK(IpccOpen(&Other,0)==0);
 CHECK(IpccRegister(Other,33,1,3,Receive,(VOID *)0x1234)==-4);
 CHECK(IpccRegister(Handle,33,8,8,Receive,(VOID *)0x1234)==-3);
 Pump(); CHECK(Enabled);
 Queue[WriteIndex++]=0x210002;
 Handler(261,Context); CHECK(!Enabled && Eois==1 && Callbacks==0 && Clears==0);
 Pump(); CHECK(Enabled && Callbacks==1 && LastIndex==7 && LastSignal==2);
 // A continuously pending source is bounded and cannot starve the menu.
 ReadIndex=WriteIndex=0;
 for(I=0;I<70;I++) { Queue[WriteIndex++]=0x210002; }
 Handler(261,Context); Pump(); CHECK(!Enabled && ReadIndex==64);
 Pump(); CHECK(Enabled && ReadIndex==70 && Callbacks==71);
 CHECK(IpccSend(Handle,33,0,1)==0 && LastWriteAddress==0x40600c && LastWrite==0x210001);
 CHECK(IpccUnregister(Other,33,2,2)==-3);
 CHECK(IpccClose(&Handle)==0 && Handle==NULL && LastWriteAddress==0x406018 && LastWrite==0x210002);
 ReadIndex=WriteIndex=0; Queue[WriteIndex++]=0x210002;
 Handler(261,Context); Pump(); CHECK(Enabled && Callbacks==71 && ReadIndex==WriteIndex);
 IpccExit(NULL,NULL); CHECK(!Enabled && IpccSend(Other,33,0,0)==-3);
 puts("PASS: IPCC IRQ mask/EOI, deferred explicit ACK, client ID translation, bounded drain, ownership and exit");
 return 0;
}
