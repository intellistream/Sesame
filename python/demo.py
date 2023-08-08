import benne

# Create an instance of Parameters
params = benne.Parameters()

# Get and set the algorithm
algorithm = params.algo

print("Current algorithm:", algorithm)
params.algo = benne.AlgoType.BIRCH
params.input_file = "/home/shaun/Sesame/benchmark/datasets/CoverType.txt"
print("Updated algorithm:", params.algo)

# ... Continue getting and setting other parameters
# Accessing docstring
print(benne.Parameters.algo.__doc__)

print("Input file:", params.input_file)

# Run the SESAME algorithm
benne.run()
