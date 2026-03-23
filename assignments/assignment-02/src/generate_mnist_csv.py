# %%
import pandas as pd
from sklearn.datasets import fetch_openml

print("Fetching full MNIST dataset (this step takes some time)...")
mnist = fetch_openml('mnist_784', version=1, as_frame=True) # mnist is a Bunch object
df = mnist.frame    # attribute frame is a dataframe
print(f"type(mnist) = {type(mnist)}")
print(f"type(df) = {type(df)}")
# %%
pixel_columns = df.columns.drop('class')
df[pixel_columns] = df[pixel_columns].astype('float64') / 255.0
# %%
print("Saving full dataset to 'mnist_full.csv'...")
df.to_csv('./data/mnist_full.csv', index=False, header=False)
# %%
def generate_distribute(n_value):
    distribute = lambda x: x.sample(n=n_value, random_state=42) 
    return distribute   # distribute is a function object

n_values = [1000, 300]
filenames = ["./data/mnist_mini.csv", "./data/mnist_micro.csv"]
for i in range(len(n_values)):
    print(f"Creating balanced {filenames[i]} with {10 * n_values[i]} images...")
    distribute = generate_distribute(n_value=n_values[i])
    df_subset = df.groupby('class').apply(distribute).reset_index(drop=True)
    print(f"df_subset.shape = {df_subset.shape}")
    df_subset.to_csv(filenames[i], index=False, header=False)