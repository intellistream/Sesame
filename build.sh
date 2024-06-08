for v in 3.9 3.10 3.11 3.12 3.13
do
python$v -m pip install build
python$v -m build
done
