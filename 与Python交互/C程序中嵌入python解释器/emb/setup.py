from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            name="emb",  # as it would be imported
            # may include packages/namespaces separated by `.`
            # all sources are compiled into a single binary file
            sources=["emb/emb.cpp"],
            # include_dirs=[
            #     "/Users/mac/micromamba/envs/py3.10/include/python3.10"],
            # library_dirs=["/Users/mac/micromamba/envs/py3.10/lib"],
            # libraries=["python3.10"],
            extra_compile_args=["-std=c++20"],
            language="c++"
        ),
    ]
)
