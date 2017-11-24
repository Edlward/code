import matplotlib.pyplot as plt
import numpy as np
import math

def plotsqrt():
    x = np.arange(0.0001, 1.1, 0.005)
    y = x
    yx2 = [math.sqrt(i) for i in x]
    plot1 = plt.plot(x,y,'-r',label='y=x')
    plot2 = plt.plot(x,yx2, '-g', label='y=sqrt(x)')
    plt.legend(loc='lower right')
    plt.show()

x = np.arange(0.5,1.5,0.005)
y = x-1
ylog = [math.log(i) for i in x]
plot1 = plt.plot(x,y,'-r',label='y=x-1')
plot2 = plt.plot(x,ylog, '-g', label='y=log(x)')
plt.legend(loc='lower right')
plt.show()
