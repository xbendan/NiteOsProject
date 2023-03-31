CC = clang
CXX = clang++
LLD = lld
NASM = nasm

TARGET=x86_64  

KERNEL_C_FLAGS = -DARCH_X86_64 \
	-Wall \
	-Wno-write-strings \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-ffreestanding \
	-finline-functions \
	-fno-exceptions \
	-fno-rtti \
	-nostdlib \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mcpu=x86-64 \
	-mcmodel=large \
	-mno-red-zone \
	-fno-pic \
	-O0 \
	-g \
	--target=x86_64-unknown-none-elf

KERNEL_LD_FLAGS = \
	-nostdlib \
	-z max-page-size=0x1000 \
	-Map=ElfLinkStruct

KERNEL_FILES_IN_CPP 			:= $(shell find ./src -name *.cpp)
KERNEL_FILES_IN_ASM				:= $(shell find ./src -name *.asm)
KERNEL_FILES_OUT_CPP			:= $(patsubst ./src/%.cpp, ./build/%.o, $(KERNEL_FILES_IN_CPP))
KERNEL_FILES_OUT_ASM 			:= $(patsubst ./src/%.asm, ./build/%.o, $(KERNEL_FILES_IN_ASM))

$(KERNEL_FILES_OUT_CPP): build/%.o : src/%.cpp  
	mkdir -p $(dir $@) && \
	$(CXX) -Iinclude $(KERNEL_C_FLAGS) -c $(patsubst build/%.o, src/%.cpp, $@) -o $@
$(KERNEL_FILES_OUT_ASM): build/%.o : src/%.asm  
	mkdir -p $(dir $@) && \
	$(NASM) -f elf64 $(patsubst build/%.o, src/%.asm, $@) -o $@

.PHONY: build
build: $(KERNEL_FILES_OUT_CPP) $(KERNEL_FILES_OUT_ASM)
	mkdir -p ./dist/${TARGET}
	$(LLD) $(KERNEL_LD_FLAGS) -n -o ./dist/${TARGET}/atrikrnl.elf -O2 -T ./target/LinkerScript-${TARGET}.ld $(KERNEL_FILES_OUT_CPP) $(KERNEL_FILES_OUT_ASM)
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
	rm -rf build/*