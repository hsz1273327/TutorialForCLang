gcc -c -I./inc ./src/struct_operator.c -o src/struct_operator.o
gcc -c -I./inc ./src/binary_operator.c -o src/binary_operator.o
gcc -c -I./inc ./src/unary_operator.c -o src/unary_operator.o
ar crv ./lib/libvector.a src/*.o
gcc -I./inc -L./lib -o ./build/bin/static_source ./test/test.c -lvector
