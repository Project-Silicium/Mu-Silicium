import * as React from "react"
import * as TooltipPrimitive from "@radix-ui/react-tooltip"
import { cn } from "@/lib/utils"

const TooltipProvider = TooltipPrimitive.Provider
const Tooltip = TooltipPrimitive.Root
const TooltipTrigger = TooltipPrimitive.Trigger
const TooltipContent = React.forwardRef<
  React.ElementRef<typeof TooltipPrimitive.Content>,
  React.ComponentPropsWithoutRef<typeof TooltipPrimitive.Content>
>(({ className, sideOffset = 4, ...props }, ref) => (
  <TooltipPrimitive.Content
    ref={ref}
    sideOffset={sideOffset}
    className={cn(
      "z-50 overflow-hidden rounded-md border bg-popover px-3 py-1.5 text-sm text-popover-foreground shadow-md animate-in fade-in-0 zoom-in-95 data-[state=closed]:animate-out data-[state=closed]:fade-out-0 data-[state=closed]:zoom-out-95 data-[side=bottom]:slide-in-from-top-2 data-[side=left]:slide-in-from-right-2 data-[side=right]:slide-in-from-left-2 data-[side=top]:slide-in-from-bottom-2",
      className
    )}
    {...props}
  />
))
TooltipContent.displayName = TooltipPrimitive.Content.displayName

// Technical term definitions
const UEFI_TERMS = {
  "UEFI": "Unified Extensible Firmware Interface - A modern firmware standard that replaces traditional BIOS, providing faster boot times, security features, and support for larger drives.",
  "ARM": "Advanced RISC Machine - A family of reduced instruction set computing (RISC) processor architectures commonly used in mobile devices and tablets. ARM (stylised in lowercase as arm, formerly an acronym for Advanced RISC Machines and originally Acorn RISC Machine) is a family of RISC instruction set architectures (ISAs) for computer processors. Arm Holdings develops the ISAs and licenses them to other companies, who build the physical devices that use the instruction set. It also designs and licenses cores that implement these ISAs. Due to their low costs, low power consumption, and low heat generation, ARM processors are useful for light, portable, battery-powered devices, including smartphones, laptops, and tablet computers, as well as embedded systems. However, ARM processors are also used for desktops and servers, including Fugaku, the world's fastest supercomputer from 2020 to 2022. With over 230 billion ARM chips produced, since at least 2003, and with its dominance increasing every year, ARM is the most widely used family of instruction set architectures. There have been several generations of the ARM design. The original ARM1 used a 32-bit internal structure but had a 26-bit address space that limited it to 64 MB of main memory. This limitation was removed in the ARMv3 series, which has a 32-bit address space, and several additional generations up to ARMv7 remained 32-bit. Released in 2011, the ARMv8-A architecture added support for a 64-bit address space and 64-bit arithmetic with its new 32-bit fixed-length instruction set. Arm Holdings has also released a series of additional instruction sets for different roles: the Thumb extensions add both 32- and 16-bit instructions for improved code density, while Jazelle added instructions for directly handling Java bytecode. More recent changes include the addition of simultaneous multithreading (SMT) for improved performance or fault tolerance.",
  "Windows ARM64": "A version of Windows designed specifically for ARM-based processors, enabling full Windows compatibility on mobile devices.",
  "UEFI firmware": "Low-level software that initializes hardware components during boot and provides runtime services for the operating system.",
  "bootloader": "A small program that loads the operating system into memory when a computer starts up.",
  "secure boot": "A UEFI security feature that ensures only trusted software runs during the boot process, preventing malware attacks.",
  "ACPI": "Advanced Configuration and Power Interface - A standard that allows the operating system to control hardware power management and configuration.",
  "firmware": "Low-level software stored in non-volatile memory that provides basic operational instructions for hardware components.",
  "EFI": "Extensible Firmware Interface - The predecessor to UEFI, providing similar functionality but with fewer features."
} as const

type UEFITerm = keyof typeof UEFI_TERMS

interface TooltipTermProps {
  term: UEFITerm | string
  children: React.ReactNode
  customDefinition?: string
  className?: string
}

const TooltipTerm: React.FC<TooltipTermProps> = ({ 
  term, 
  children, 
  customDefinition,
  className 
}) => {
  const definition = customDefinition || UEFI_TERMS[term as UEFITerm]
  
  if (!definition) {
    return <>{children}</>
  }

  return (
    <TooltipProvider>
      <Tooltip>
        <TooltipTrigger asChild>
          <span className={cn(
            "underline decoration-dotted decoration-primary/50 cursor-help hover:decoration-primary transition-colors",
            className
          )}>
            {children}
          </span>
        </TooltipTrigger>
        <TooltipContent className="max-w-xs">
          <span>{definition}</span>
        </TooltipContent>
      </Tooltip>
    </TooltipProvider>
  )
}

export { TooltipTerm, TooltipProvider, Tooltip, TooltipTrigger, TooltipContent }
