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
output_file = 'data_plots.png'
fig.savefig(output_file, dpi=150)
print(f'Plots saved to {output_file}')

