import matplotlib.pyplot as plt
import numpy as np

# -----------------------------
# Data
# -----------------------------
kruskal_data = [
    (100, 848.6, 33100731),
    (200, 1766.6, 33123666),
    (400, 3517.4, 33162874),
    (800, 7016.4, 33240740),
    (1600, 14227.8, 33414077),
    (3200, 28434.4, 33821182),
    (6400, 56963.6, 34602138),
    (12800, 113780.8, 36392134),
    (25600, 222554.4, 40386060),
]

decremental_data = [
    (100, 1.4),
    (200, 4.4),
    (400, 35.0),
    (800, 82.8),
    (1600, 131.0),
    (3200, 231.2),
    (6400, 370.8),
    (12800, 615.0),
    (25600, 1067.2),
]

# -----------------------------
# Extract plotting data
# -----------------------------
kr_removals = np.array([x[0] for x in kruskal_data])
kr_time = np.array([x[1] for x in kruskal_data])

dec_removals = np.array([x[0] for x in decremental_data])
dec_time = np.array([x[1] for x in decremental_data])

# -----------------------------
# Power transformation (compress large values)
# -----------------------------
power = 0.45
kr_time_trans = kr_time**power
dec_time_trans = dec_time**power

kr_removals_trans = kr_removals**power
dec_removals_trans = dec_removals**power

# -----------------------------
# Create plot
# -----------------------------
plt.figure(figsize=(10, 6))
plt.plot(kr_removals_trans, kr_time_trans, 'o-r', label="Algoritmo de Kruskal")
plt.plot(dec_removals_trans, dec_time_trans, 'o-b', label="MSF Decremental")

# X-axis: show original values as ticks
x_ticks = [100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600]
x_ticks_trans = np.array(x_ticks)**power
plt.xticks(x_ticks_trans, x_ticks, rotation=90)
plt.xlabel("Número de arestas removidas")

# Y-axis: custom ticks showing actual milliseconds
yticks_original = [10, 250, 1000,  2500, 5000, 10000, 25000, 50000, 100000, 200000, 250000]
yticks_trans = [y**power for y in yticks_original]
plt.yticks(yticks_trans, [str(y) for y in yticks_original])
plt.ylabel("Tempo da execução em milissegundos")

# Grid, title, legend
plt.grid(True, linestyle='--', alpha=0.5)
plt.title("Comparação de desempenho: Algoritmo de Kruskal vs MSF Decremental")
plt.legend()
plt.tight_layout()

# Save figure
plt.savefig("5_graphs_average.png", dpi=300)
print("Saved graph.png")
