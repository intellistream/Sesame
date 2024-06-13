#!python3

from pysame import Benne, Birch, BenneObj
# from sklearn.cluster import Birch

X = [[0, 1], [0.3, 1], [-0.3, 1], [0, -1], [0.3, -1], [-0.3, -1]]

brc = Birch(
    n_clusters=2,
    dim=2,
    distance_threshold=0.5,
)

print(brc.partial_fit(X).predict(X))

bne = Benne(
    n_clusters=2,
    dim=2,
    distance_threshold=0.5,
    obj=BenneObj.accuracy,
)
print(bne.partial_fit(X).predict(X))
