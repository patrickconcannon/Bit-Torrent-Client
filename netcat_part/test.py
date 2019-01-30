import csv
import numpy as np
import matplotlib.pyplot as plt

x = []
y = []
cur_low=0
counter=0
with open('data_points.csv', 'r') as f:
	reader = csv.reader(f)
	for row in reader:
		counter+=1
		current = (int(row[2]))
		if (current < cur_low):
			print("ERROR: " + row[2] + " " + str(cur_low))
			continue
		cur_low = current
		x.append(float(row[0]))
		y.append(float(row[1]))
plt.scatter(x[:200],y[:200]) # 60 will show how it loops back on itself
plt.plot(x[:200],y[:200]) # 60
plt.show()

# In order to remove the outliers, you can look at the standard dev and the mean distance between points
# as you are looking for erreous points