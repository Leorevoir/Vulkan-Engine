#!/bin/bash

for file in *.frag *.vert *.comp; do
    [ -f "$file" ] || continue
    slangc "$file" -o "$file.spv" &
    echo "Compiling $file to $file.spv"
done

wait
echo "All shaders compiled."
