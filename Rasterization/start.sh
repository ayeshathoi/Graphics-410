#!/bin/bash
echo "Removing old output files if there is any..."
for file in ./test_case/$1/1805062_*; do
    if [ -f "$file" ]; then
        echo "Removing $file"
        rm $file
    fi
done
echo "Done removing old output!"

echo "Compiling Code..."
g++ main.cpp -o test.out
echo "Running Code ..."
./test.out $1
echo "Check the ouput files!"
rm test.out



