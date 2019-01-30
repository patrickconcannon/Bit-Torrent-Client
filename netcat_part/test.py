import csv
# import numpy as np
import matplotlib.pyplot as plt

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from random import randint

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111)


from math import sin, cos, sqrt, atan2, radians


ZERO_DIST = 0.0
TRIM = 10

	# In order to remove the outliers, you can look at the standard dev and the mean distance between points
	# as you are looking for erreous points
class Point:
  def __init__(self, lt, ln, timestamp):
    self.lt = lt
    self.ln = ln
    self.timestamp = timestamp
    self.distance = 0 # distance to next point

class PointSet:
  def __init__(self, fileName):
    self.pointSet = []
    self.distances = []
    if fileName:
      # Load points from file
      with open(fileName, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
          self.pointSet.append(Point(float(row[0]),float(row[1]),float(row[2])))

  # Returns distance between two points
  def calcDist(self, lt1,ln1,lt2,ln2):
    
    # approximate radius of earth in km
    R = 6373.0
    # Using radians
    lat1 = radians(lt1)
    lon1 = radians(ln1)
    lat2 = radians(lt2)
    lon2 = radians(ln2)

    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))

    return R * c

  # Returns a list of the distances between points
  def checkDistances(self):  
    if len(self.pointSet) == 0:
          return None
    temp = []
    for i in range(len(self.pointSet)-1):
      d = self.calcDist(self.pointSet[i].lt,self.pointSet[i].ln,self.pointSet[i+1].lt,self.pointSet[i+1].ln)
      # skip duplicates - points where no distance is covered 
      if d == ZERO_DIST:
        continue
      # remove duplicates from pointSet 
      temp.append(self.pointSet[i])
      # store distance to next point in Point
      self.pointSet[i].distance = d
      # store all distances
      self.distances.append(d)
    self.pointSet = temp
  
  def avg(self, x):
    #x = [s for s in x if s.isdigit()]
    return float(sum(x)/(max(len(x),1)))

  def getMean(self):
    sd = sorted(self.distances) 			  # sorts list of distances low to high
    td = sd[TRIM:-TRIM] 	# trim bottom 10 values
    avg_d = float(sum(sd)/(max(len(sd),1))) 	      # mean
    avg_t = float(sum(td)/(max(len(td),1)))       # trimmed_mean distance
    mid = abs(len(td)/2)
    #self.pointSet = self.pointSet[27:147] #fix

  def getStandDev(self):
    mean = getMean()
    stand_dev = []
    variance = 0
    for d in range(self.distances):
      val = (d - mean)**2
      stand_dev.append(val)
      variancee += val
    return sqrt(variance/len(stand_dev)-1)

  # Check and remove points with erroneous timestamps
  # Assuming timestamps should always be increasing
  def checkTime(self):
    cur_low = 0
    temp = []
    for pt in self.pointSet:
      current = pt.timestamp
      if (current < cur_low): 
        continue
      cur_low = current
      temp.append(pt)
    self.pointSet = temp

  # Plot points to show path
  def plotPoints(self):
    x=[]
    y=[]
    for pt in self.pointSet:
      y.append(pt.ln)	
      x.append(pt.lt)
    ax.plot(x,y)
    fig.savefig('graph.png')
    # plt.scatter(x,y) # :200 or :60 will show how it loops back on itself
    # plt.plot(x,y) # with guiding line
    # plt.show()

  def removeOutliers(self):
    if len(self.pointSet) == 0:
      return None
    temp = []
    for i in range(len(self.pointSet)-1):
      d = self.calcDist(self.pointSet[i].lt,self.pointSet[i].ln,self.pointSet[i+1].lt,self.pointSet[i+1].ln)

      m = getMean()
      sd = getStandDev()
      r1 = m - sd
      r2 = m + sd
      
      per = ((1 / (m/sd)) * 100) # percentage of distances 
      # the lower per is the closer the distances are to the sd


CSV_FILE = 'data_points.csv'
pointSet = PointSet(CSV_FILE)

pointSet.checkTime()
pointSet.checkDistances()
pointSet.removeOutliers() # still dealing with distances
#print(len(pointSet.pointSet))
#pointSet.plotPoints() 

# So when checking distances that when I have to put in the checek


# Once you've settled on the Mean value to use, create a new list with values (x - mean)**2 
# Then add all of the values contained in this list
# Then divide by (list - 1) -- this will give the variance
# The square root of which is the +/- value by which most values are off by
# 
# Finally, you can use Chebyshev's Theorem to find the number of standard deviations (k) that values 
# are from the mean. So with this you can say that x% of points are within y distance of mean. If you 
# plot and look at the values you can check to see what values are the standard ones