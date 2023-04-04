# **ATRI Kernel**  
ATRI kernel is designed as a modern style system kernel. It's a monolithic kernel which contains process scheduling, memory management, graphics output, file system, network, device management and PnP services. One day in the future, we might migrate some parts of kernel components into user space for stability reasons, and it may eventually becomes a hybrid kernel.
# Architecture
- [x] x86 [64-bit] **(In progress)**
- [ ] arm [64-bit]
- [ ] risc-v
- [ ] loongarch
# Components  
### System  
- [ ] Power Management (ACPI)  
- [x] Multi-processor (ACPI) **(√)**  
### Memory  
- [x] Virtual Memory - Kernel Space **(√)**  
- [ ] Virtual Memory - User Space 
- [x] Physical Memory - Page Frame Allocate and Free **(√)**  
- [x] Physical Memory - Kernel Memory Allocate and Free **(√)**  
### Process & Thread  
- [ ] Process Create, Exit, and Terminate **(In progress)**  
- [ ] Context Switch **(In progress)**  
- [ ] Schedule (Round Robin)
- [ ] Schedule (Multilevel Feedback Queue Scheduling)
- [ ] Inter-process Communication
- [ ] Shared Memory  
### Graphics  
- [x] VGA Text Mode  
- [ ] Framebuffer **(In progress)**  
- [ ] Extended graphics  
### File System  
- [ ] FAT32  
- [ ] XFS  
- [ ] NTFS (Readonly)  
- [ ] exFAT  
- [ ] RamFS **(In progress)**  
### Security  
- [ ] User Account  
- [ ] Registry Policy  
- [ ] Permission Control  
### Network  
# Support hardware & device  
- [ ] USB (XHCI)
- [ ] USB (EHCI)
- [ ] Graphics (Framebuffer) **(In progress)**  
- [ ] Graphics (Intel integrated graphics)
- [ ] Graphics (Virtual machine graphics)
- [ ] Input (PS/2 Keyboard and Mouse) **(In progress)**  
- [ ] Input (USB Human Interface Devices)  
- [x] Clock (HPET)
- [x] Clock (APIC)
- [x] Clock (ACPI)
- [x] General (PCI/PCI Express)  
- [x] General (ACPI)  
- [ ] Storage (IDE)
- [x] Storage (AHCI/ATA)  
- [ ] Storage (NVMe)  
- [ ] Storage (USB Massive Storage)  
# Changelog
20230324: Finish kernel memory allocator (allocation only), add support to ACPI, PCI, SMP and AHCI.  
20230325: Add support to SSE, MMX
20230326: Add support to PS/2 controller and keyboard, update device.
20230327: Implement a simple console with no backspace.
20230403: Finally fixed issues on kernel memory allocation.