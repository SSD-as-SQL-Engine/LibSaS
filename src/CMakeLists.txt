
# Make an explicit list of all source files in `LibSaS_SRC`. This is important
# because CMake is not a build system: it is a build system generator. Suppose
# you add a file foo.cpp to src/ after running cmake .. . If you set
# `LibSaS_SRC` with `file(GLOB ... )`, this is not passed to the makefile;
# the makefile doesn't know that foo.cpp exists and will not re-run cmake. Your
# collaborator's builds will fail and it will be unclear why. Whether you use
# file(GLOB ...) or not, you will need to re-run cmake, but with an explicit
# file list, you know beforehand why your code isn't compiling. 


set(LibSaS_SRC
    nvme_ioctl.c  
    nvme_main.c
    sqlite_api.c
    sas.cc
    )

# Form the full path to the source files...
PREPEND(LibSaS_SRC)
# ... and pass the variable to the parent scope.
set(LibSaS_SRC ${LibSaS_SRC}  PARENT_SCOPE)
