cd bind_lib
./configure CFLAGS=-fPIC
make
cd ..
cmake .
make
