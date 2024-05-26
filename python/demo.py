#!python3

from pysame import Birch

X = [[0, 1], [0.3, 1], [-0.3, 1], [0, -1], [0.3, -1], [-0.3, -1]]

brc = Birch(
    n_clusters=2,
    dim=2,
    threshold=0.5,
)

print(brc.fit_predict(X))
