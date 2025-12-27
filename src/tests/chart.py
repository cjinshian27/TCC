import matplotlib.pyplot as plt

# Full data for table
kruskal_data = [
    (100, 882, 33100731),
    (200, 1779, 33123666),
    (400, 3701, 33162874),
    (800, 8030, 33240740),
    (1600, 16763, 33414077),
    (3200, 34159, 33821182),
    (6400, 68632, 34602138),
    (12800, 135427, 36392134),
    (25600, 255063, 40386060),
]

decremental_data = [
    (100, 1),
    (200, 3),
    (400, 7),
    (800, 57),
    (1600, 135),
    (3200, 196),
    (6400, 386),
    (12800, 650),
    (25600, 1079),
]

# Prepare table data
table_data = []
for i in range(len(kruskal_data)):
    removals = kruskal_data[i][0]
    mst_weight = kruskal_data[i][2]
    kr_time_val = kruskal_data[i][1]
    dec_time_val = decremental_data[i][1]
    table_data.append([removals, mst_weight, kr_time_val, dec_time_val])

column_labels = ["Arestas removidas", "peso da MST", "Algoritmo de Kruskal (ms)", "MSF Decremental (ms)"]

# Create figure for table only
fig, ax_table = plt.subplots(figsize=(8, 6))
ax_table.axis('off')  # Hide axes

# Create table
table = ax_table.table(cellText=table_data, colLabels=column_labels, loc='center', cellLoc='center')

# Style table
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1, 1.5)

column_widths = [0.3, 0.15, 0.3, 0.3]

for key, cell in table.get_celld().items():
    row, col = key
    if row == 0:
        cell.set_text_props(weight='bold', color='black')
        cell.set_facecolor('#d3d3d3')
    else:
        cell.set_facecolor('white')
    cell.set_linewidth(1)
    # Set column width
    if col < len(column_widths):
        cell.set_width(column_widths[col])

plt.tight_layout()
plt.savefig("performance_table_5_graphs_average.png", dpi=300)
print("Saved performance_table.png")
