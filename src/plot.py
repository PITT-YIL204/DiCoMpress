import numpy as np 
from matplotlib import pyplot as plt 

x = np.arange(1,125) 
y = (1+(x-1)/8)/x
plt.title("Compression Ratio") 
plt.xlabel("chain size") 
plt.ylabel("ratio") 
plt.plot(x,y) 
plt.show()