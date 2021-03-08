gcc -c -o src/struct_operator.o -I./inc ./src/struct_operator.c
gcc -c -o src/binary_operator.o -I./inc ./src/binary_operator.c
gcc -c -o src/unary_operator.o -I./inc ./src/unary_operator.c
ar crv ./lib/libvector.a src/*.o
gcc -I./inc -L./lib -o ./build/bin/static_source ./test/test.c -lvector
