import numpy as np
import matplotlib.pyplot as plt

def gaussian(x, mean, sigma):
    return 1/(sigma*np.sqrt(2*np.pi))*np.exp(-(pow(x, 1/12)-mean)**2/(2*sigma**2))

x = np.linspace(0, 10, 10)
y = gaussian(x, 6, 2)

plt.plot(x, y)
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Fonction Gaussienne centrée en 6 et d\'écart-type 2')
plt.show()