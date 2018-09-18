# cleanup;
make clean
rm -rf ./build
rm -rf CMakeCache.txt

echo $CMAKE_C_COMPILER_ID
if [ "$1" = "--native" ]
then
    export CMAKE_TARGET="NATIVE"
    sudo cmake .
    sudo make
elif [ "$1" = "--test" ]
then
    export CMAKE_TARGET="TEST"
    sudo cmake .
    sudo make
else
    export CMAKE_TARGET="WASM"
    sudo emconfigure cmake .
    sudo emmake make

    if [ $? -eq 0 ]
    then
        # post process;
        echo "[Sharpen] Post compiling process ..."
        sudo node scripts/compiler/post-compile.js
    fi
fi
