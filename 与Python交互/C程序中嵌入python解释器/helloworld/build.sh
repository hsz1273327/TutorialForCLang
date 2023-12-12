g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld.o -std=c++20 -c helloworld.cpp
g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld_wrap.o -std=c++20 -c helloworld_wrap.cpp
g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o main.o -std=c++20 -c main.cpp 


g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -std=c++20 *.o -o helloworld




g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld -std=c++20 helloworld.cpp helloworld_wrap.cpp main.cpp 


gcc *.c -o