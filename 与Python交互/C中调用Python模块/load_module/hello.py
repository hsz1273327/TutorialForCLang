import math
from typing import List

def apply():
    print("hello world!")


def callnumpy():
    import numpy as np
    print(f"numpy version: {np.version.__version__}")


def call_with_params_and_return(x:int,y:float,z:List[str])->str:
    _z:str = ", ".join(z)
    return f"get x {x},y {y}, and z: {_z}"

class PyVector:
    x:float
    y:float
    
    def __init__(self,x:float,y:float)->None:
        self.x = x
        self.y = y
    
    def mod(self)->float:
        return math.sqrt(self.x**2+self.y**2)
        