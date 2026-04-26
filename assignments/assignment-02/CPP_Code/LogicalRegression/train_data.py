import pandas as pd
import numpy as np

# Load original dataset
data = pd.read_csv('CPP_Code/LogicalRegression/Data/mnist_micro.csv', header=None)

# Make 10 copies, one for each digit
datasets = [data.copy() for _ in range(10)]

# Convert final column into one-vs-rest binary labels
for i in range(10):
    label_col = datasets[i].columns[-1]   # last column
    datasets[i][label_col] = (datasets[i][label_col] == i).astype(int)

# Example:
# datasets[4] now has label 1 where original label was 4, else 0
train_test_datadict = {}

for i in range(10):
    X = datasets[i].iloc[:, :-1].to_numpy()   # all columns except last
    y = datasets[i].iloc[:, -1].to_numpy()    # last column only
    train_test_datadict[f'D{i}'] = [X, y]

print(train_test_datadict['D0'][1])  # Should show (num_samples, num_features)

for name, (X, y) in train_test_datadict.items():

    df = pd.DataFrame(X)

    df["label"] = y

    df.to_csv(f"{name}.csv", index=False)