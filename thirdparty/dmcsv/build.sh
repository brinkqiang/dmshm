#!/bin/bash

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#

# pushd thirdparty/dmprotobuf
# libtoolize && aclocal && autoheader && autoconf && automake --add-missing
# sh configure
# popd

rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=relwithdebinfo ..
cmake --build . --config relwithdebinfo -- -j$(nproc)
cd ..

# popd

# echo continue && read -n 1
