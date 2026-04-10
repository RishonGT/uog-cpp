import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score

df = pd.read_csv('assignments/assignment-02/Python LR/data/mnist_micro.csv', header=None)

# Shuffle the data so that we can visualize various digits
df_shuffled = df.sample(frac=1, random_state=42).reset_index(drop=True)

# Split features and labels -- in numpy arrays
X = df_shuffled.iloc[:,:-1].values
y = df_shuffled.iloc[:, -1].values

print(f"type(df) = {type(df)}")
print(f"type(X) = {type(X)}")
print(f"type(y) = {type(y)}")

print(f"X.shape = {X.shape}")
print(f"y.shape = {y.shape}")

fig, axes = plt.subplots(3, 4, figsize=(8, 6))
fig.suptitle("Shuffled Sample Images from Micro MNIST dataset", fontsize=14)
for i, ax in enumerate(axes.flatten()):
    image = X[i].reshape(28, 28)

    # Display the image with magma colormap. 
    # A few colormap to try out: 
    #   viridis (default), plasma, inferno, magma, cividis, gray
    ax.imshow(image, 'magma')
    # avoid floating point presentation by casting to int: int(y[i])
    ax.set_title(f"Label: {int(y[i])}", fontsize=16)
    ax.axis('off')

plt.tight_layout()
plt.show()

# We don't need to split the dataset into training set and test set. We just use all the given data here. We use max_iter=1000 to ensure
# convergence on the given dataset
model = LogisticRegression(max_iter=1000) 
model.fit(X, y)

# Make prediction on the entire set of images
y_predict = model.predict(X)

fig, axes = plt.subplots(3, 4, figsize=(8, 6))
fig.suptitle("Images and Predictions", fontsize=14)

accuracy = accuracy_score(y_true=y, y_pred=y_predict)
for i, ax in enumerate(axes.flatten()):
    image = X[i].reshape(28, 28)
    ax.imshow(image, cmap="magma")
    
    actual, predicted = y[i], y_predict[i]
    color = 'green' if predicted == actual else 'red'
    ax.set_title(f"Pred: {int(predicted)}\nActual: {int(actual)}", color=color)

    ax.axis(False)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.show()