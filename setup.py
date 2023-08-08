import os
import distutils.ccompiler
import distutils.sysconfig
import setuptools
import shlex
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.errors import CompileError, LinkError
import sys

__version__ = '0.0.1'

# list of all sources files
sources = [
    "src/Timer/TimeMeter.cpp",
    "src/Utils/BenchmarkUtils.cpp",
    "src/Utils/UtilityFunctions.cpp",
    "src/Evaluation/Euclidean.cpp",
    "src/Evaluation/NMI.cpp",
    "src/Evaluation/CMM.cpp",
    "src/Evaluation/Purity.cpp",
    "src/Evaluation/Evaluation.cpp",
    "src/Sinks/DataSink.cpp",
    "src/Sources/DataSource.cpp",
    "src/Engine/Engine.cpp",
    "src/Engine/SimpleEngine.cpp",
    "src/Engine/SingleThread.cpp",
    "src/APIs/APIs.cpp",
    "src/Algorithm/DesignAspect/V9.cpp",
    "src/Algorithm/DesignAspect/V10.cpp",
    "src/Algorithm/DesignAspect/V16.cpp",
    "src/Algorithm/Benne.cpp",
    "src/Algorithm/WindowModel/LandmarkWindow.cpp",
    "src/Algorithm/WindowModel/DampedWindow.cpp",
    "src/Algorithm/WindowModel/WindowModel.cpp",
    "src/Algorithm/WindowModel/WindowFactory.cpp",
    "src/Algorithm/OfflineRefinement/OfflineRefinement.cpp",
    "src/Algorithm/OfflineRefinement/KMeans.cpp",
    "src/Algorithm/OfflineRefinement/DBSCAN.cpp",
    "src/Algorithm/OfflineRefinement/ConnectedRegions.cpp",
    "src/Algorithm/DataStructure/Point.cpp",
    "src/Algorithm/DataStructure/TreeNode.cpp",
    "src/Algorithm/DataStructure/CoresetTree.cpp",
    "src/Algorithm/DataStructure/MicroCluster.cpp",
    "src/Algorithm/DataStructure/Snapshot.cpp",
    "src/Algorithm/DataStructure/WeightedAdjacencyList.cpp",
    "src/Algorithm/DataStructure/DataStructureFactory.cpp",
    "src/Algorithm/DataStructure/CFTree.cpp",
    "src/Algorithm/DataStructure/DPTree.cpp",
    "src/Algorithm/DataStructure/DPNode.cpp",
    "src/Algorithm/DataStructure/Cache.cpp",
    "src/Algorithm/DataStructure/OutlierResevoir.cpp",
    "src/Algorithm/DataStructure/FeatureVector.cpp",
    "src/Algorithm/DataStructure/CharacteristicsVector.cpp",
    "src/Algorithm/DataStructure/DensityGrid.cpp",
    "src/Algorithm/DataStructure/GridCluster.cpp",
    "src/Algorithm/DataStructure/MeyersonSketch.cpp",
    "src/Algorithm/StreamKM.cpp",
    "src/Algorithm/CluStream.cpp",
    "src/Algorithm/Birch.cpp",
    "src/Algorithm/DenStream.cpp",
    "src/Algorithm/EDMStream.cpp",
    "src/Algorithm/DBStream.cpp",
    "src/Algorithm/DStream.cpp",
    "src/Algorithm/SlidingWindowClustering.cpp",
    "src/Algorithm/Algorithm.cpp",
    "src/Algorithm/AlgorithmFactory.cpp",
    "benchmark/src/Benchmark.cpp",
    "python/src/bindings.cpp"
]

# list of all include directories
include_dirs = [
    "src",
    "include"
]

os.environ["CC"] = "g++"
os.environ["CXX"] = "g++"

extension = Extension(
    "benne",
    sources=sources,
    include_dirs=include_dirs,
    library_dirs=['/usr/lib/x86_64-linux-gnu/'],
    language="c++",
    extra_compile_args=[
        "-std=c++20",
        "-Wall",
        "-Werror=return-type",
        "-fconcepts-diagnostics-depth=2",
        "-fopenmp",
        "-march=native",
        "-Wno-ignored-qualifiers",
        "-Wno-sign-compare",
        "-O3",
        "-DNDEBUG",
        "-fPIC",
        "-rdynamic"
    ],
    extra_link_args=['-fopenmp'],  # explicitly link libomp
    libraries=['gflags', 'gomp', '/usr/lib/x86_64-linux-gnu/libgomp.so.1'],  # Use 'gomp' and 'omp'
)

class CustomBuildExtCommand(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7', '-lomp']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append('-fopenmp')
            opts.append('-std=c++20')
            opts.append('-march=native')
            # get output of python3 -m pybind11 --includes
            # and split it into a list of include directories
            opts.extend(shlex.split(
                subprocess.getoutput('python3 -m pybind11 --includes')))
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)

setup(
    name='benne',
    version=__version__,
    author='Shuhao',
    url='https://github.com/intellistream/Sesame',
    description='Benne using pybind11',
    long_description='This project aims at building a scalable stream mining library on modern hardware. The repo contains currently several representative real-world stream clustering algorithms and several synthetic algorithms.',
    ext_modules=[extension],
    cmdclass={'build_ext': CustomBuildExtCommand},
    zip_safe=False,
)