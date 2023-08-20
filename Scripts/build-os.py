import os

folders = [
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
source_general = []
source_arch = []
build_options = [
    '-DARCH_X86_64',
    '-Wall -Wno-write-strings -Wno-unused-parameter -Wno-sign-compare',
    '-ffreestanding -finline-functions -fno-exceptions -fno-rtti -fno-pic',
    '-nostdlib',
    '-std=gnu++17',
    '-mno-red-zone -mno-mmx -mno-sse -mno-sse2',
    '-mcmodel=kernel',
    '-O0 -g'
]
link_options = [
    '-nostdlib',
    '-z max-page-size=0x1000'
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

    os.system('tree src')
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
        filepath = file.replace('src/', 'compiled/').replace('.c', '.o').replace('.cpp', '.o').replace('.asm', '.o')
        if not os.path.exists(f"build/{os.path.dirname(filepath)}"):
            os.makedirs(f"build/{os.path.dirname(filepath)}")

        print(f"Compiling [%-40s] ---> [%-40s]" % (file, filepath))
        if (file.endswith('.cpp')):
            os.system(f"{cxx} -c {file} -Iinclude -o build/{filepath} {' '.join(build_options)}")
        elif (file.endswith('.asm')):
            os.system(f"{nasm} -f elf64 {file} -o build/{filepath}")
        