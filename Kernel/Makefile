CC = ${TARGET}-elf-gcc
CXX = ${TARGET}-elf-g++
AS = ${TARGET}-elf-as
OC = ${TARGET}-elf-objcopy
LD = ${TARGET}-elf-ld
NASM = nasm -f elf64

KERNEL_CFLAGS = -ffreestanding \
	-DARCH_X86_64 \
	-D__DEBUG__ \
	-O2 \
	-std=c++11 \
	-static \
	-mcmodel=large \
	-Wno-unused-function \
	-Wno-unused-parameter \
	-Wwrite-strings \
	-mno-red-zone \
	-fno-omit-frame-pointer \
	-fno-exceptions \
	-fno-rtti \
	-mfsgsbase \
	-mgeneral-regs-only \
	-z max-page-size=0x1000 \
	-nostdlib
# -g

KERNEL_LDFLAGS = -nostdlib \
	-O2 \
	-z max-page-size=0x1000

#ARCH_ASM_FILES_X64_IN 			:=  $(shell find ./Arch/x86_64 -name *.asm)
#ARCH_C_FILES_X64_IN 			:=  $(shell find ./Arch/x86_64 -name *.cc)

#ARCH_ASM_FILES_X64_OUT			:=  $(patsubst ./Arch/x86_64/%.asm, ./build/x86_64/%.o, $(ARCH_ASM_FILES_X64_IN))
#ARCH_C_FILES_X64_OUT			:=  $(patsubst ./Arch/x86_64/%.cc, ./build/x86_64/%.o, $(ARCH_C_FILES_X64_IN))

#ARCH_x86_64_OUTPUT 				= 	$(ARCH_ASM_FILES_X64_OUT) $(ARCH_C_FILES_X64_OUT)

KERNEL_INCLUDES = -Iinclude

KERNEL_FILES_IN_CPP 			:= $(shell find ./src -name *.cc)
KERNEL_FILES_IN_ASM				:= $(shell find ./src -name *.asm)
KERNEL_FILES_OUT_CPP			:= $(patsubst ./src/%.cc, ./build/kernel/%.o, $(KERNEL_FILES_IN_CPP))
KERNEL_FILES_OUT_ASM 			:= $(patsubst ./src/%.asm, ./build/kernel/%.o, $(KERNEL_FILES_IN_ASM))

#$(ARCH_ASM_FILES_X64_OUT): build/x86_64/%.o : Arch/x86_64/%.asm
#	mkdir -p $(dir $@) && \
#	$(NASM) $(patsubst build/x86_64/%.o, Arch/x86_64/%.asm, $@) -o $@
#$(ARCH_C_FILES_X64_OUT): build/x86_64/%.o : Arch/x86_64/%.cc
#	mkdir -p $(dir $@) && \
#	$(CXX) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) -c $(patsubst build/x86_64/%.o, Arch/x86_64/%.cc, $@) -o $@



$(KERNEL_FILES_OUT_CPP): build/kernel/%.o : src/%.cc  
	mkdir -p $(dir $@) && \
	$(CXX) $(KERNEL_INCLUDES) $(KERNEL_CFLAGS) -c $(patsubst build/kernel/%.o, src/%.cc, $@) -o $@
$(KERNEL_FILES_OUT_ASM): build/kernel/%.o : src/%.asm  
	mkdir -p $(dir $@) && \
	$(NASM) $(patsubst build/kernel/%.o, src/%.asm, $@) -o $@


.PHONY: build
build: $(KERNEL_FILES_OUT_CPP) $(KERNEL_FILES_OUT_ASM)
	mkdir -p ./dist/${TARGET}
	mkdir -p ./build/${TARGET}
	mkdir -p ./build/kernel
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_LDFLAGS) -n -o ./dist/${TARGET}/atrikrnl.elf -O2 -T ./build/LinkerScript-${TARGET}.ld $(KERNEL_FILES_OUT_CPP) $(KERNEL_FILES_OUT_ASM)
	cp ./dist/${TARGET}/atrikrnl.elf ./target/${TARGET}/iso_limine/atrikrnl.elf
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        ./target/${TARGET}/iso_limine -o ./dist/${TARGET}/nite.iso
	../../limine/bin/limine-deploy ./dist/${TARGET}/nite.iso
	cp ./dist/${TARGET}/nite.iso "/mnt/f/niteOS/${TARGET}/nite.iso"

.PHONY: run
run:
	qemu-system-x86_64.exe -smp 4 -cdrom F:/niteOS/${TARGET}/nite.iso -m 512m

.PHONY: clean
clean:
	rm -rf ./build/kernel
	rm -rf ./build/${TARGET}
	rm -rf ./dist/${TARGET}