dynamic-graph-corba
===================

This software provide a corba server to remotely run python commands with an
interpreter embedded in a dynamic-graph entity.

Setup
-----

To compile this package, it is recommended to create a separate build
directory:

    mkdir _build
    cd _build
    cmake [OPTIONS] ..
    make install

Please note that CMake produces a `CMakeCache.txt` file which should
be deleted to reconfigure a package from scratch.
