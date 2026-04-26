import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
from sklearn.utils import shuffle

#===================
# 1. DATA GENERATION (using variance to create non-separable classes)
#===================
def generate_overlapping_data(n_per_class=5):
    """Generates 5 classes with 5 points each. Increased variance creates overlap."""
    data = []
    centers = [(2, 2), (2, 7), (5, 4.5), (8, 2), (8, 7)]
    variance = 0.5  # Increased variance to create overlap
    np.random.seed(42)  # for reproducibility
    for class_id, (cx, cy) in enumerate(centers):
        for _ in range(n_per_class):
            # Increased scale (1.2) ensures points cross into other class territories
            x1 = np.random.normal(cx, variance)
            x2 = np.random.normal(cy, variance)
            data.append([x1, x2, class_id])
            
    df = pd.DataFrame(data, columns=['feat1', 'feat2', 'label'])
    # Shuffle so the model doesn't see classes in perfect blocks during training
    return shuffle(df, random_state=42).reset_index(drop=True)

#==============
# 2. MODEL MATH
#==============
def softmax(z):
    # keepdims=True ensures that the output has the same shape as z, 
    # which is important for broadcasting during the division
    exp_z = np.exp(z - np.max(z, axis=1, keepdims=True))
    return exp_z / np.sum(exp_z, axis=1, keepdims=True)

def forward_prop(X, W, b):
    z = np.dot(X, W) + b
    return softmax(z)

#=================
# 3. TRAINING LOOP
#=================
def train_model(X, y, lr=0.05, epochs=2000):
    num_samples, num_features = X.shape
    num_classes = len(np.unique(y))
    
    # Initialization
    W = np.random.randn(num_features, num_classes) * 0.01
    b = np.zeros((1, num_classes))
    y_oh = pd.get_dummies(y).values
    
    print(f"Training on {num_samples} points...")
    for epoch in range(epochs):
        # Forward pass: compute predicted probabilities
        probs = forward_prop(X, W, b)
        
        # Gradient of loss function w.r.t. weights and biases
        error = probs - y_oh
        dW = (1 / num_samples) * np.dot(X.T, error)
        db = (1 / num_samples) * np.sum(error, axis=0, keepdims=True)
        
        # Update
        W -= lr * dW
        b -= lr * db
        
        if epoch % 500 == 0:
            loss = -np.mean(np.sum(y_oh * np.log(probs + 1e-9), axis=1))
            print(f"Epoch {epoch}: Loss {loss:.4f}")
            
    return W, b

#=========================================
# 4. VISUALIZATION (Decision Boundaries)
#=========================================
def plot_boundaries(X, y, W, b, filename):
    plt.figure(figsize=(10, 8))
    
    # Create a grid of points covering the feature space
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.05),
                         np.arange(y_min, y_max, 0.05))
    
    # Predict for every point in the grid
    grid_points = np.c_[xx.ravel(), yy.ravel()]
    probs = forward_prop(grid_points, W, b)
    predictions = np.argmax(probs, axis=1)
    predictions = predictions.reshape(xx.shape)
    
    # Plot background colors
    plt.contourf(xx, yy, predictions, alpha=0.3, cmap='viridis')
    
    # Plot the original data points
    scatter = plt.scatter(X[:, 0], X[:, 1], c=y, cmap='viridis', edgecolors='k', s=40)
    
    plt.title("Multinomial Logistic Regression: Decision Boundaries (Non-Separable)")
    plt.xlabel("Feature 1")
    plt.ylabel("Feature 2")
    plt.savefig(filename)
    plt.close()
    print(f"Boundary plot saved to {filename}")

#==========
# EXECUTION
#==========
df = generate_overlapping_data(50)
print(df)

Path("./data").mkdir(exist_ok=True)

df = generate_overlapping_data(50)
X_train = df[['feat1', 'feat2']].values
y_train = df['label'].values

print(X_train)

'''
# Run Training
W_final, b_final = train_model(X_train, y_train)

# Generate Visual
plot_boundaries(X_train, y_train, W_final, b_final, "./data/decision_boundaries.png")

df.to_csv("output.csv", index=False, sep=";")
'''