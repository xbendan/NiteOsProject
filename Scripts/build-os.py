import os

folders = [
    'src/common',
    'src/utils',
    'src/core',
    'src/device',
    'src/display',
    'src/drivers',
    'src/fs',
    'src/init',
    'src/mm',
    'src/proc',
    'src/sched'
]

proj_name = 'siberix'
proj_langs = ['c', 'cpp', 'asm']
build_options = [
    '-DARCH_X86_64',
    '-DDEBUG',
    '-Wall -Wno-write-strings -Wno-unused-parameter -Wno-sign-compare',
    '-ffreestanding -finline-functions -fno-exceptions -fno-rtti -fno-pic',
    '-nostdlib',
    '-std=gnu++17',
    '-mno-red-zone -mno-mmx -mno-sse -mno-sse2',
    '-mcmodel=kernel',
    '-O0',
    '-g'
]
link_options = [
    '-nostdlib',
    '-z max-page-size=0x1000',
    '-Map=build/map.txt'
]

# Scan all files in folders and return a list of files
def scan_files(dirs, array):
    for dir in dirs:
        for root, dirs, files in os.walk(dir):
            for file in files:
                if file.endswith('.c'):
                    array.append(os.path.join(root, file))
                elif file.endswith('.cpp'):
                    array.append(os.path.join(root, file))
                elif file.endswith('.asm'):
                    array.append(os.path.join(root, file))
                else:
                    pass


if __name__ == '__main__':
    target = 'x86_64'
    cc = f'{target}-elf-gcc'
    cxx = f'{target}-elf-g++'
    ld = f'{target}-elf-ld'
    nasm = 'nasm'

    source_general = []
    source_arch = []
    object_files = []

    scan_files(folders, source_general)
    scan_files([f'src/arch/{target}'], source_arch)

    print(f"Building {proj_name} for {target}...")
    print("Source files:")

    for file in source_arch:
        print(f"- {file}")
    for file in source_general:
        print(f"- {file}")

    # Create directory for build
    if not os.path.exists('build'):
        os.mkdir('build')

    # Compile general source files
    for file in (source_general + source_arch):
        filepath = file.replace('src/', 'compile/').replace('.cpp', '.o').replace('.asm', '.o')
        if not os.path.exists(f"build/{os.path.dirname(filepath)}"):
            os.makedirs(f"build/{os.path.dirname(filepath)}")

        print(f"Compiling [%-40s] ---> [%-40s]" % (file, filepath))
        if (file.endswith('.cpp')):
            os.system(f"{cxx} -c {file} -Iinclude -o build/{filepath} {' '.join(build_options)}")
            object_files.append(f"build/{filepath}")
        elif (file.endswith('.asm')):
            os.system(f"{nasm} -f elf64 {file} -o build/{filepath}")
            object_files.append(f"build/{filepath}")

        
    # Link object files
    print("Linking...")
    if not os.path.exists(f"build/{target}"):
        os.mkdir(f"build/{target}")
    os.system(f"{ld} -T target/LinkerScript-{target}.ld -o build/{target}/{proj_name}.elf {' '.join(object_files)} {' '.join(link_options)}")

    # Create ISO
    print("Creating ISO...")
    os.system(f"cp build/{target}/{proj_name}.elf target/{target}/limine-st/{proj_name}.elf")

    os.system(f"xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        target/{target}/limine-st -o dist/{target}/nite_sbrxkrnl.iso")
    os.system(f"../Thirdparty/limine/bin/limine-deploy dist/{target}/nite_sbrxkrnl.iso")
    # os.system(f"xorriso -as mkisofs -b limine-bios-cd.bin \
    #     -no-emul-boot -boot-load-size 4 -boot-info-table \
    #     --efi-boot limine-uefi-cd.bin \
    #     -efi-boot-part --efi-boot-image --protective-msdos-label \
    #     target/{target}/limine -o dist/{target}/nite_sbrxkrnl.iso")
    # os.system(f"limine bios-install ./dist/{target}/nite_sbrxkrnl.iso")
    os.system(f"cp ./dist/{target}/nite_sbrxkrnl.iso /mnt/c/Users/MejiroArdan/Codes/Cxxlang/NiteProjectSbrxkrnl.iso")