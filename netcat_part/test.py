import csv
import matplotlib.pyplot as plt
import matplotlib as mpl

from random import randint
from math import sin, cos, sqrt, atan2, radians

ZERO_DIST = 0.0
TRIM = 10

## https://repl.it/repls/AnxiousPrivateHashfunction

# In order to highlight outliers, we compare it against the median absolute deviation(MAD) 
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

  def median(self, x):
    mid = int(abs(len(x)/2))
    if len(x) % 2 == 0:
      return int(avg(x[mid-1],x[mid+1]))
    else:  
      return x[mid+1]
  # Get absolute median deviation - more robust to outliers
  # might need to write Median so as not to depend on package
  def getMAD(self):
    med = self.median(self.distances)
    mad = []
    variance = 0
    for d in range(self.distances):
      mad.append(abs(d - med))
    return med(mad)

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

  def removeOutliers(self):
    for pt in (self.pointSet):
      mad = self.getMAD()
      median = median(self.distances)
      count = 0
      if pt.distance >= median-mad and pt.distance <= median+mad :
        #It's within the standard deviation
        count += 1
    print(count)

  # Plot points to show path
  def plotPoints(self):
    x=[]
    y=[]
    for pt in self.pointSet:
      y.append(pt.ln)	
      x.append(pt.lt)
    ax.plot(x,y)
    mpl.use('Agg')
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111)
    fig.savefig('graph.png')
    # plt.scatter(x,y) # :200 or :60 will show how it loops back on itself
    # plt.plot(x,y) # with guiding line
    # plt.show()

CSV_FILE = 'data_points.csv'
pointSet = PointSet(CSV_FILE)

pointSet.checkTime()
pointSet.checkDistances()
pointSet.removeOutliers() 
#pointSet.plotPoints() 

# Finally, you can use Chebyshev's Theorem to find the number of standard deviations (k) that values 
# are from the mean. So with this you can say that x% of points are within y distance of mean. If you 
# plot and look at the values you can check to see what values are the standard ones