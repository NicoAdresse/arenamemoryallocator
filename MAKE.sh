# MAKE.sh

#!/bin/bash

set -e

BUILD="build"

echo "Running script..."

echo "Checking if old build cache is there..."
if [ -d "$BUILD" ]; then
    echo "$BUILD exists! Deleting..."
    rm -rf "$BUILD"
else
    echo "$BUILD doesn't exist!"
fi
echo "Done!"

echo "Creating new $BUILD!"
mkdir -p "$BUILD" 
echo "Done!"

echo "Initializing CMake in $BUILD!"
cmake -S . -B "$BUILD"
echo "Done!"

echo "Running..."
cmake --build "$BUILD" --target run
echo "Done!"

echo "Script finished!"
