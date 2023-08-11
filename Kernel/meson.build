project('siberix', 'cpp')

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

target = 'x86_64'

source_files = []

src = files('src/core/*.cpp')

executable('sbrxkrnl',
    sources: src,
    include_directories: ['include']
)