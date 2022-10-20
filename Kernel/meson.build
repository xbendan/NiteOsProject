add_languages('c', 'cpp')

bintoelf = find_program('bintoelf.sh', './bintoelf.sh')
bing = generator(bintoelf,
output : '@BASENAME@.bin.o',
arguments : ['@INPUT@','@OUTPUT@', meson.current_source_dir() + '/src/Arch/x86_64/', '@BASENAME@'])

kernel_c_args = [
    '-Wno-write-strings', '-Wno-unused-parameter', '-Wno-sign-compare',
    '-DLemon64',
    '-ffreestanding', '-nostdlib',
    '-mcmodel=large', '-mno-red-zone', '-fno-pic',
    '-mno-mmx', '-mno-sse', '-mno-sse2',
    '-fno-stack-protector', '-fno-builtin',
    '-finline-functions',
]

kernel_cpp_args = [
    '-fno-exceptions', '-fno-rtti', '-Wno-deprecated-volatile', '-Wno-non-c-typedef-for-linkage',
]

kernel_cpp_files_x86_64 = [
    'src/Arch/x86_64/ACPI.cpp',
    'src/Arch/x86_64/APIC.cpp',
    'src/Arch/x86_64/Arch.cpp',
    'src/Arch/x86_64/BOOTX64.cpp',
    'src/Arch/x86_64/CPU.cpp',
    'src/Arch/x86_64/GDT.cpp',
    'src/Arch/x86_64/IDT.cpp',
    'src/Arch/x86_64/Paging.cpp',
    'src/Arch/x86_64/PIC.cpp',
    'src/Arch/x86_64/PIT.cpp',
    'src/Arch/x86_64/SMBios.cpp',
    'src/Arch/x86_64/SMP.cpp'
]

kernel_asm_files_x86_64 = [
    'src/Arch/x86_64/Header.asm',
    'src/Arch/x86_64/IRQ.asm',
    'src/Arch/x86_64/SMPTrampoline.asm',
    'src/Arch/x86_64/Stack.asm',
    'src/Arch/x86_64/Wrapper.asm'
]

kernel_link_args = [
    '-m64',
    '-T', meson.current_source_dir() + '/build/x86_64/LinkerScript-x86_64.ld',
    '-z', 'max-page-size=0x1000'
]

kernel_cpp_files = [
    'src/Memory/Memory.cpp',
    'src/Memory/PhysicalMemoryAllocator.cpp',
    'src/Memory/PhysicalPageAllocator.cpp',
    'src/Panic.cpp',
    'src/String.cpp'
]

kernel_includes = [
    'include/',
    'include/Arch/x86_64/'
]

kernel_link_args += kernel_c_args

nasm = find_program('nasm')
asmg = generator(nasm,
    output : '@BASENAME@.asm.o',
    arguments : [
        '-f', 'elf64',
        '-g', '-F', 'dwarf', '-w+gnu-elf-extensions',
        '@INPUT@',
        '-o', '@OUTPUT@'])

kernel = executable('kernel.sys',
    [asmg.process(kernel_asm_files_x86_64), bing.process(kernel_asm_bin_files_x86_64), kernel_cpp_files, kernel_cpp_files_x86_64],
    include_directories : [kernel_includes], 
    c_args : kernel_c_args, cpp_args : [kernel_c_args, kernel_cpp_args], link_args: kernel_link_args, link_depends: '/build/x86_64/LinkerScript-x86_64.ld')