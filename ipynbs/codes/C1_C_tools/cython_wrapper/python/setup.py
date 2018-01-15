from distutils.core import setup
from Cython.Build import cythonize


setup(
    name="binaryvector",
    ext_modules=cythonize('*.pyx')
)

# source_dir = Path("../source")
# src_dir = source_dir.joinpath("src/binary_vector.c")
# inc_dir = source_dir.joinpath("include")
# extension = Extension(
#     "binaryvector",
#     sources=["binaryvector.pyx",str(src_dir)],
#     include_dirs=[str(inc_dir)]
# )

# setup(
#     cmdclass={'build_ext': build_ext},
#     ext_modules=cythonize(extension),
# )
