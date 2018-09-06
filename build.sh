make clean
sudo emconfigure cmake .
sudo emmake make

# post process;
echo "[Sharpen] Post compiling process ..."
sudo node scripts/post-compile.js