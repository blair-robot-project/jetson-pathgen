#!/usr/bin/env bash
if [[ $(/usr/bin/id -u) -ne 0 ]]; then
    echo "Not run as root, exiting."
    exit
fi
mkdir installDependencies
cd installDependencies
apt -y install build-essential cmake wget
wget https://github.com/google/protobuf/releases/download/v2.6.1/protobuf-2.6.1.tar.gz
tar -xf protobuf-2.6.1.tar.gz
cd protobuf-2.6.1
./configure
make -j$(nproc)
make install
ldconfig
cd ..
wget https://github.com/zeromq/libzmq/releases/download/v4.2.2/zeromq-4.2.2.tar.gz
tar -xf zeromq-4.2.2.tar.gz
cd zeromq-4.2.2
./configure
make -j$(nproc)
make install
ldconfig
cd ..
wget https://github.com/zeromq/cppzmq/archive/v4.2.2.tar.gz
tar -xf v4.2.2.tar.gz
cd cppzmq-4.2.2/
mkdir build
cd build
cmake ..
make -j$(nproc) install
ldconfig
cd ../../../
rm -rf installDependencies
