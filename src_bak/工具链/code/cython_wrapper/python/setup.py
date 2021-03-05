from distutils.core import setup
from pathlib import Path
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

self_path = Path(__file__).resolve()
self_dir = self_path.parent
project_dir = self_dir.parent
lib_dir = project_dir.joinpath("build/lib")
inc_dir = project_dir.joinpath("build/inc")
source_dir = project_dir.joinpath("python")

extension = Extension(
    "pybinaryvector",
    sources=[str(source_dir.joinpath("pybinaryvector.pyx"))],
    include_dirs=[str(inc_dir)],
    libraries = ["vector"],
    library_dirs = [str(lib_dir)]
)

setup(
    cmdclass={'build_ext': build_ext},
    ext_modules=cythonize(extension),
)