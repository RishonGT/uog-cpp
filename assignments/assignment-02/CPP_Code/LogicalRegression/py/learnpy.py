import sympy as sp
import numpy as np

w, b, x = sp.symbols('w b x')
data = [(17, 0), (18, 0), (21, 1), (22, 1)]

# Logistic Model
sigmoid = 1 / (1 + sp.exp(-(w * x + b)))

# Loss Function (Binary Cross-Entropy)
loss_terms = []
for x_val, y_val in data:
    pred = sigmoid.subs(x, x_val)
    term = -(y_val * sp.log(pred) + (1 - y_val) * sp.log(1 - pred))
    loss_terms.append(term)

base_loss = sum(loss_terms) / len(data)
grad_w = sp.diff(base_loss, w)
grad_b = sp.diff(base_loss, b)

# Convert SymPy loss to a numerical function for plotting
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def plot_loss_surface(loss_expression, title, w_range=None, b_range=None, elev=30, azim=120):
    """
    Converts a SymPy expression to a numerical function and plots it in 3D.
    """
    # Convert to fast numerical function
    f_num = sp.lambdify((w, b), loss_expression, 'numpy')
    
    # Create grid
    if w_range is None:
        w_range = np.linspace(-5, 15, 100)
    if b_range is None:        
        b_range = np.linspace(-150, 50, 100)
    W, B = np.meshgrid(w_range, b_range)
    Z = f_num(W, B)
    
    # Plotting
    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(W, B, Z, cmap='viridis', edgecolor='none', alpha=0.8)
    
    ax.set_title(title)
    ax.set_xlabel('w (Weight)')
    ax.set_ylabel('b (Bias)')
    ax.set_zlabel('Loss')
    ax.view_init(elev=elev, azim=azim)
    plt.show()

# Case A: Without Regularization
plot_loss_surface(base_loss, "Loss Surface: No Regularization (Infinite Ramp)")