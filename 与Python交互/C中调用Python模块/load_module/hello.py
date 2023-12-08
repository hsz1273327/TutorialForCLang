import math
from typing import List, Tuple


def apply():
    print("hello world!")


def callnumpy():
    import numpy as np
    print(f"numpy version: {np.version.__version__}")


def call_with_params_and_return(x: int, y: float, z: List[str] = []) -> Tuple[str, str]:
    _z: str = ", ".join(z)
    return f"get x {x},y {y}, and z: {_z}", "something else"


class PyVector:
    x: float
    y: float
    _mod: float = None

    def __init__(self, x: float, y: float) -> None:
        self.x = x
        self.y = y

    def calculate_mod(self) -> float:
        return math.sqrt(self.x**2+self.y**2)

    @property
    def mod(self) -> float:
        if self._mod is None:
            self._mod = self.calculate_mod()
        return self._mod
