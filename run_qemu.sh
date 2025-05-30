#!/bin/bash
echo "Building firmware..."
cd embedded_app
make clean
make

echo "Starting QEMU..."
make qemu