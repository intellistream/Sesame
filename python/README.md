# Python

Python API binding for sesame

## How to build pypi wheel

```bash
docker run -it --rm -v $(pwd):/Sesame quay.io/pypa/manylinux_2_28_x86_64 /bin/bash

# run in docker bash

cd Sesame
for v in 3.7 3.8 3.9 3.10 3.11 3.12 3.13
do
python$v -m pip install build
python$v -m build
done

auditwheel repair dist/pysame-*-linux_x86_64.whl
python3.9 -m pip install twine
python3.9 -m twine upload --repository testpypi wheelhouse/*
```