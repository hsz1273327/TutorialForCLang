g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o load_module -std=c++17 main.cpp


g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o load_module_withconf -std=c++17 -Wwrite-strings mainwithconf.cpp


g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o mainwithdataaccess -std=c++17 -Wwrite-strings mainwithdataaccess.cpp


g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o mainwithmore -std=c++20 -Wwrite-strings -Wunused-result mainwithmore.cpp
