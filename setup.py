from setuptools import setup, Extension
import json

def parse_compile_commands():
    compile_commands_file = "compile_commands.json"  # Update with the actual path to your compile_commands.json file

    with open(compile_commands_file, "r") as f:
        compile_commands = json.load(f)

    sources = []
    include_dirs = []

    for command in compile_commands:
        sources.append(command["file"])
        include_dirs.append(command["directory"])

    return sources, include_dirs

sources, include_dirs = parse_compile_commands()

module = Extension(
    "benne_module",
    sources=sources,
    include_dirs=include_dirs,
    # Add any additional compilation flags or libraries as needed
    extra_compile_args=["-std=c++20", "-Wall", "-Werror=return-type", "-fPIC"],
    extra_link_args=["-lpthread", "-fopenmp"],
)

setup(
    name="benne_package",
    version="0.1",
    ext_modules=[module],
)