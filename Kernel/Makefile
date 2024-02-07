# Define project
PROJ_NAME = siberix
KERN_NAME = ""
PLATFORM := ""
BUILD_VERSION := 1.0.0
BUILD_MODE := snapshot
BUILD_NAME := $(PROJ_NAME)-v$(BUILD_VERSION)-$(PLATFORM)-$(BUILD_MODE)

# Define compiling and linking flags
CXXFLAGS = \
	-Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-write-strings \
	-Waddress-of-packed-member \
	-ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -fno-pic \
	-mno-sse -mno-sse2 -mno-sse3 -mno-sse4 -mno-sse4.1 -mno-sse4.2 \
	-mno-avx -mno-avx2 -mno-mmx \
	-std=c++23 \
	-mcmodel=kernel \
	-mno-red-zone \
	-O0 \
	-g
LDFLAGS = \
	-nostdlib -z max-page-size=0x1000

# Source files
SOURCE_GENERIC := $(wildcard src/siberix/*.cpp) \
	$(wildcard src/siberix/dvc/*.cpp) \
    $(wildcard src/siberix/audio/*.cpp) \
	$(wildcard src/siberix/gfx/*.cpp) \
	$(wildcard src/siberix/mem/*.cpp) \
	$(wildcard src/siberix/init/*.cpp) \
	$(wildcard src/siberix/fs/*.cpp) \
	$(wildcard src/siberix/net/*.cpp) \
	$(wildcard src/siberix/proc/*.cpp) \
	$(wildcard src/siberix/hal/*.cpp) \
	$(wildcard src/impls/*.cpp)  
# OBJECT_GENERIC := $(patsubst build/%.cpp, src/%.o, $(SOURCE_GENERIC))

SOURCE_PLATFORM_X86_64 := $(wildcard src/platforms/x86_64/*.cpp)  
SOURCE_ASSEMBLY_X86_64 := $(wildcard src/platforms/x86_64/*.asm)  
OBJECT_PLATFORM_X86_64 := $(patsubst src/%.cpp, build/%.o, $(SOURCE_PLATFORM_X86_64))  
OBJECT_ASSEMBLY_X86_64 := $(patsubst src/%.asm, build/%.o, $(SOURCE_ASSEMBLY_X86_64))
OBJECT_GENERIC_X86_64 := $(patsubst src/%.cpp, build/%.o, $(SOURCE_GENERIC))  

# Rule to build the executable
$(OBJECT_PLATFORM_X86_64): build/%.o: src/%.cpp  
	mkdir -p $(dir $@)  
	x86_64-elf-g++ -I inc $(CXXFLAGS) -c $< -o $@  
$(OBJECT_ASSEMBLY_X86_64): build/%.o: src/%.asm  
	mkdir -p $(dir $@)  
	nasm -f elf64 $< -o $@
$(OBJECT_GENERIC_X86_64): build/%.o: src/%.cpp  
	mkdir -p $(dir $@)  
	x86_64-elf-g++ -I inc $(CXXFLAGS) -c $< -o $@

# Default rule
all: $(OBJECT_GENERIC) $(OBJECT_PLATFORM_X86_64)

x64-all: $(OBJECT_GENERIC_X86_64) $(OBJECT_PLATFORM_X86_64) $(OBJECT_ASSEMBLY_X86_64)
	$(eval PLATFORM := x86_64)
	mkdir -p build/${PLATFORM}
	mkdir -p dist/${PLATFORM}
	$(LD) $(LDFLAGS) -o dist/${PLATFORM}/$(PROJ_NAME).elf \
		-T target/LinkerScript-${PLATFORM}.ld \
		$(OBJECT_GENERIC_X86_64) \
		$(OBJECT_PLATFORM_X86_64) \
		$(OBJECT_ASSEMBLY_X86_64)  
	cp dist/${PLATFORM}/$(PROJ_NAME).elf target/${PLATFORM}/iso_limine/$(PROJ_NAME).elf
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		target/${PLATFORM}/iso-limine -o  dist/${PLATFORM}/$(PROJ_NAME).iso
	../Thirdparty/limine/limine-deploy dist/${PLATFORM}/$(BUILD_NAME).iso
	cp dist/${PLATFORM}/$(BUILD_NAME).iso /mnt/c/Users/MejiroArdan/Codes/Cxxlang/$(BUILD_NAME).iso


x64-test: x64-all
	qemu-system-x86_64.exe -smp 4 -m 512m \
		-cdrom C:/Users/MejiroArdan/Codes/Cxxlang/$(BUILD_NAME).iso  

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
