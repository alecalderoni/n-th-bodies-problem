import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.image as mpimg

#dati
data = pd.read_csv('dati.csv')

t = data["t"]
x1 = data["x_uno"]
y1 = data["y_uno"]
x2 = data["x_due"]
y2 = data["y_due"]
x3 = data["x_tre"]
y3 = data["y_tre"]
x4 = data["x_quattro"]
y4 = data["y_quattro"]


img = mpimg.imread('space.jpeg')

# Creare la figura e gli assi
fig, ax = plt.subplots()

# Impostare i limiti degli assi centrati su 0,0
ax.set_xlim(min(min(x1), min(x2), min(x3), min(x4)) - 1, max(max(x1), max(x2), max(x3), max(x4)) + 1)
ax.set_ylim(min(min(y1), min(y2), min(y3), min(y4)) - 1, max(max(y1), max(y2), max(y3), max(y4)) + 1)

ax.imshow(img, extent=[ax.get_xlim()[0], ax.get_xlim()[1], ax.get_ylim()[0], ax.get_ylim()[1]], aspect='auto')
# Creare le linee iniziali vuote per i due corpi e i punti
trace1, = ax.plot([], [], 'y-', lw=1, label='Traccia Corpo 1')  # Traccia Corpo 1
point1, = ax.plot([], [], 'yo', markersize=5, label='Corpo 1')  # Punto Corpo 1

trace2, = ax.plot([], [], 'b-', lw=1, label='Traccia Corpo 2')  # Traccia Corpo 2
point2, = ax.plot([], [], 'bo', markersize=5, label='Corpo 2')  # Punto Corpo 2

trace3, = ax.plot([], [], 'r-', lw=1, label='Traccia Corpo 3')  # Traccia Corpo 3
point3, = ax.plot([], [], 'ro', markersize=5, label='Corpo 3')  # Punto Corpo 3

trace4, = ax.plot([], [], 'g-', lw=1, label='Traccia Corpo 4')  # Traccia Corpo 4
point4, = ax.plot([], [], 'go', markersize=5, label='Corpo 4')  # Punto Corpo 4

# Aggiungere una leggenda
ax.legend()

# Funzione di inizializzazione
def init():
    trace1.set_data([], [])
    point1.set_data([], [])

    trace2.set_data([], [])
    point2.set_data([], [])

    trace3.set_data([], [])
    point3.set_data([], [])

    trace4.set_data([], [])
    point4.set_data([], [])

    return trace1, point1, trace2, point2, trace3, point3, trace4, point4

# Funzione di aggiornamento per ogni frame dell'animazione
def update(frame):
    trace1.set_data([x1[max(0, frame - 250):frame]], [y1[max(0, frame - 250):frame]])
    point1.set_data([x1[frame]], [y1[frame]])

    trace2.set_data([x2[max(0, frame - 250):frame]], [y2[max(0, frame - 250):frame]])
    point2.set_data([x2[frame]], [y2[frame]])

    trace3.set_data(x3[:frame], y3[:frame])
    point3.set_data(x3[frame], y3[frame])

    trace4.set_data(x4[:frame], y4[:frame])
    point4.set_data(x4[frame], y4[frame])

    return trace1, point1, trace2, point2, trace3, point3, trace4, point4

# Impostare un intervallo per aumentare la velocità dell'animazione
ani = FuncAnimation(fig, update, frames=len(t), init_func=init, blit=True, repeat=False, interval=2)

# Visualizzare l'animazione
plt.title("Moto di pianeti")
ax.set_aspect("equal")
plt.show()
