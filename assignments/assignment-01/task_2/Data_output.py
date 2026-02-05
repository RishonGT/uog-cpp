import matplotlib.pyplot as plt
import numpy as np

# Load data from the file
data = np.loadtxt('data.csv', delimiter=',', skiprows=1)
# Separate the data into x and y
x = data[:, 0]
y = data[:, 1]
# Create a scatter plot of the data
plt.scatter(x, y, color='blue', label='Data Points')
# Add labels and title
plt.xlabel('x')
plt.ylabel('y')
plt.title('Scatter Plot of Data Points')
plt.legend()
# Show the plot
plt.savefig('scatter_plot.png')

