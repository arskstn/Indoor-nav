import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import os
import csv

image_path = "confidential/kartasmall.png"
A = int(input("number of columns (A): "))
B = int(input("number of rows (B): "))

dead_zones = []
num_dead = int(input("number of dead zones: "))
print("dead zone: x1 y1 x2 y2")
for i in range(num_dead):
    x1, y1, x2, y2 = map(int, input(f"dead zone #{i+1}: ").split())
    dead_zones.append((x1, y1, x2, y2))

img = mpimg.imread(image_path)
height, width = img.shape[:2]
dx = width / A
dy = height / B

grid = [['X' for _ in range(A)] for _ in range(B)]  # [row][col]
for (x1, y1, x2, y2) in dead_zones:
    for x in range(x1, x2):
        for y in range(y1, y2):
            if 0 <= x < A and 0 <= y < B:
                grid[B - y - 1][x] = 'B'

fig, ax = plt.subplots()
ax.imshow(img)
for i in range(A + 1):
    x = i * dx
    ax.plot([x, x], [0, height], color='red', linewidth=1)
for j in range(B + 1):
    y = j * dy
    ax.plot([0, width], [y, y], color='red', linewidth=1)

for (x1, y1, x2, y2) in dead_zones:
    rect_x = x1 * dx
    rect_y = height - y2 * dy
    rect_width = (x2 - x1) * dx
    rect_height = (y2 - y1) * dy
    rect = plt.Rectangle((rect_x, rect_y), rect_width, rect_height,
                         linewidth=0, facecolor='red', alpha=0.4)
    ax.add_patch(rect)
ax.set_ylim([height, 0])
ax.set_xlim([0, width])
ax.axis('off')

plt.tight_layout()
plt.show()

output_img = "confidential/kartasmall_with_grid.png"
fig.savefig(output_img, bbox_inches='tight', pad_inches=0)
print(f"Image saved to: {output_img}")

output_csv = "confidential/kartasmall_grid.csv"
with open(output_csv, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    for row in grid:
        writer.writerow(row)

print(f"csv map saved to: {output_csv}")
