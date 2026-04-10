import pandas as pd
import numpy as np

df = pd.read_csv('assignments/assignment-02/Python LR/data/mnist_full.csv', header=None, delimiter=',')
print(len(df), "rows and", len(df.columns), "columns")

'''this was to understand the format of the data, 70,000 rows and 785 columns,
 where the first 784 columns are pixel values and the last column is the actual value of the number.
 repeated for 70,000 rows making 70,000 images of 28x28 pixels. of random numbers from 0 to 9.'''