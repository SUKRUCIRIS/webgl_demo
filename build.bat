mkdir build 2>NUL
cd build
call emcmake cmake ..
call emmake make
cd ..