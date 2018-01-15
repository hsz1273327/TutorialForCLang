from distutils.core import setup
from pathlib import Path
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

source_dir = Path("../build")
lib_dir = source_dir.joinpath("lib")
inc_dir = source_dir.joinpath("include")
extension = Extension(
    "binaryvector",
    sources=["binaryvector.pyx"],
    include_dirs=[str(inc_dir)],
    libraries = ["vector"],
    library_dirs = [str(lib_dir)]
)

setup(
    cmdclass={'build_ext': build_ext},
    ext_modules=cythonize(extension),
)