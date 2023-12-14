# gcc for my x86 mac
g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld -std=c++20 main.cpp 

g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld_isolated -std=c++20 main_isolated.cpp 

# clang for my arm mac
clang++ -I /Users/huangsizhe/micromamba/envs/py3.10/include/python3.10 -L/Users/huangsizhe/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld -std=c++20 main.cpp 

clang++ -I /Users/huangsizhe/micromamba/envs/py3.10/include/python3.10 -L/Users/huangsizhe/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld_isolated -std=c++20 main_isolated.cpp 
