import benne

# Create an instance of Parameters
params = benne.Param()

# Get and set the algorithm
algorithm = params.algo

print("Current algorithm:", algorithm)
params.algo = benne.AlgoType.BIRCH
params.input_file = "/home/shaun/Sesame/datasets/CoverType.txt"

# ... Continue getting and setting other parameters
# Accessing docstring
print(benne.Param.algo.__doc__)

# Run the SESAME algorithm
benne.run()
