'''
DISCLAIMER: This is all AI generated as the task is not to make test and 
train data. This just simply split whatever file you put in
into a 80% train 20% test and shuffles it to prevent sequential learning.
'''



import pandas as pd
from sklearn.model_selection import train_test_split
import os

def split_mnist_data(filepath):
    print(f"Loading data from {filepath}...")
    # Load the full dataset
    df = pd.read_csv(filepath)
    
    total_rows = len(df)
    print(f"Successfully loaded {total_rows} rows.")

    # Split the data
    # test_size=0.2 means 20% goes to the test set, 80% to the train set
    # random_state=42 ensures the random shuffle is exactly the same every time you run it
    print("Splitting into 80/20 train and test sets...")
    train_df, test_df = train_test_split(df, test_size=0.2, random_state=42)

    # Figure out where to save the new files (in the same folder as the original)
    directory = os.path.dirname(filepath)
    train_path = os.path.join(directory, "mnist_train.csv")
    test_path = os.path.join(directory, "mnist_test.csv")

    # Save the new datasets to CSV files without the Pandas index column
    print(f"Saving training set ({len(train_df)} rows) to: {train_path}")
    train_df.to_csv(train_path, index=False)

    print(f"Saving testing set ({len(test_df)} rows) to: {test_path}")
    test_df.to_csv(test_path, index=False)

    print("Done!")

if __name__ == "__main__":
    # Your specific file path
    csv_path = "/home/jerem/Code/R/uog-cpp/assignments/assignment-02/Assignment_2/data/mnist_micro.csv"
    
    split_mnist_data(csv_path)