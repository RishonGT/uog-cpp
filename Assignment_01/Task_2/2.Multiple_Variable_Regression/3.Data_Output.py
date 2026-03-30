import matplotlib
matplotlib.use('Agg')  # Use non-interactive backend for saving plot
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Load data from the file
data = np.loadtxt('data.csv', delimiter=',', skiprows=1)
# Separate the data into x1, x2 and y
x1 = data[:, 0]
x2 = data[:, 1]
y = data[:, 2]

# Create a figure with a 3D scatter and two 2D projections
fig = plt.figure(figsize=(12, 5))

# 3D scatter: x1, x2, y
ax3d = fig.add_subplot(1, 3, 1, projection='3d')
ax3d.scatter(x1, x2, y, c='blue', s=10)
ax3d.set_xlabel('x1')
ax3d.set_ylabel('x2')
ax3d.set_zlabel('y')
ax3d.set_title('3D: x1 vs x2 vs y')

# 2D scatter: x1 vs y
ax2 = fig.add_subplot(1, 3, 2)
ax2.scatter(x1, y, color='green', s=10)
ax2.set_xlabel('x1')
ax2.set_ylabel('y')
ax2.set_title('x1 vs y')

# 2D scatter: x2 vs y
ax3 = fig.add_subplot(1, 3, 3)
ax3.scatter(x2, y, color='red', s=10)
ax3.set_xlabel('x2')
ax3.set_ylabel('y')
ax3.set_title('x2 vs y')

plt.tight_layout()
# Define plane parameters (set `m1`, `m2`, `b` here)
# Example: m1 = 1.0, m2 = 0.5, b = 0.0
m1 = 2.40402
m2 = 3.58494
b = 606.071

# Create a grid over the x1/x2 ranges and evaluate the plane y = m1*x1 + m2*x2 + b
x1_s = np.linspace(x1.min(), x1.max(), 20)
x2_s = np.linspace(x2.min(), x2.max(), 20)
X1, X2 = np.meshgrid(x1_s, x2_s)
Y_plane = m1 * X1 + m2 * X2 + b

# Plot the plane on the 3D axes
ax3d.plot_surface(X1, X2, Y_plane, color='orange', alpha=0.5, linewidth=0, antialiased=True)

# --- Projections of the plane onto the 2D plots ---
# For x1 vs y (ax2): show shaded envelope of plane across x2 range
x1_line = np.linspace(x1.min(), x1.max(), 200)
y_proj_x1_min = m1 * x1_line + m2 * x2.min() + b
y_proj_x1_max = m1 * x1_line + m2 * x2.max() + b
ax2.fill_between(x1_line, y_proj_x1_min, y_proj_x1_max, color='orange', alpha=0.15)

# For x2 vs y (ax3): show shaded envelope of plane across x1 range
x2_line = np.linspace(x2.min(), x2.max(), 200)
y_proj_x2_min = m1 * x1.min() + m2 * x2_line + b
y_proj_x2_max = m1 * x1.max() + m2 * x2_line + b
ax3.fill_between(x2_line, y_proj_x2_min, y_proj_x2_max, color='orange', alpha=0.15)

output_file = 'Data_Plots.png'
fig.savefig(output_file, dpi=150)
print(f'Plots saved to {output_file}')
print(f'Plane: y = {m1}*x1 + {m2}*x2 + {b}')

