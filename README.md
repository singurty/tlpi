This is what came to be when I was reading [The Linux Programming Interface](https://en.wikipedia.org/wiki/The_Linux_Programming_Interface).

## Notable programs
- [my implementation of malloc and free](memalloc/bmalloc.c)
- [psuser - shows PID and command name of all process owned by a particular user](sysinfo/psuser.c)
- [my implementation of pstree](sysinfo/pstree.c)
- [psfile - shows all process that have a particular file open](sysinfo/psfile.c)
- [benchmark - test how long your disk take to create n random 1-byte files in random order and delete them in ascending order](filesystem/benchmark.c)
- [ugperms - check what permissions a particular user or group has under ACL in a file](files/ugperms.c)
- [my implementation of ntfw - file tree walk](directories/nftw.c)
- [dirchangelog - watch for changes in a direcotry and recursive subdirectories and log to a file or stdout (if a subdirectory is added after watch has started, it is watched too)](inotify/dirchangelog.c)
