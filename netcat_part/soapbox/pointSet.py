from point import Point
from statistics import median
from math import sin, cos, sqrt, atan2, radians

import csv
import matplotlib.pyplot as plt
import matplotlib as mpl



# All Formulas referenced from: 
#   http://www.ltcconline.net/greenl/courses/201/descstat/mean.htm
#   
#   Median Absolute Derivation(MAD) used as it is
#   as it is more robust to outliers
#
#   Chebyshev's Theorem used to tell us that at 
#   least x points lies within x% of the Median Absolute
#   Derivation
#

CHEBYSHEV_CONST = 3
RAD_EARTH = 6373.0 # approximate radius of earth in km
ZERO_DIST = 0.0


class PointSet:
  def __init__(self):
    self.pointSet = []
    self.distances = []


  def loadData(self, fileName):
    if fileName:
      # Load points from file
      with open(fileName, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
          self.pointSet.append(Point(float(row[0]),float(row[1]),float(row[2])))


  def getLength(self):
        return len(self.pointSet)


  # Returns distance between two points
  def getDistances(self, lt1,ln1,lt2,ln2):
    # Convert to radians
    lat1 = radians(lt1)
    lon1 = radians(ln1)
    lat2 = radians(lt2)
    lon2 = radians(ln2)

    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))

    return RAD_EARTH * c


  # Get Median Absolute Deviation(MAD)
  def getMAD(self):
    med_dist = []
    med = median(self.distances)
    for d in self.distances:
      med_dist.append(abs(d - med))
    return median(med_dist)


  # Returns a list of the distances between points
  def checkDistances(self):  
    if len(self.pointSet) == 0:
          return None
    temp = []
    for i in range(len(self.pointSet)-1):
      d = self.calcDist(self.pointSet[i].lt,self.pointSet[i].ln,self.pointSet[i+1].lt,self.pointSet[i+1].ln)
      # skip duplicates e.g. points where no distance is covered 
      if d == ZERO_DIST:
        continue
      # remove duplicates from pointSet 
      temp.append(self.pointSet[i])
      # store distance to next point within Point obj
      self.pointSet[i].distance = d
      # store all distances betweeen points within PointSet
      self.distances.append(d)
    self.pointSet = temp


  # Assumes timestamps should always be increasing
  # Remove those which aren't
  def checkTimes(self):
    cur_low = 0
    temp = []
    for pt in self.pointSet:
      current = pt.timestamp
      if (current < cur_low): 
        continue
      cur_low = current
      temp.append(pt)
    self.pointSet = temp


  # Remove outlying points
  # Checks for correct time, distance and if within MAD range of median
  def removeOutliers(self):
    self.checkTimes()
    self.checkDistances()
    # Will allow us to check if 1 - 1 / k**2 are within MAD range # of median
    # e.g if k = 2, 1 - 0.25 = .75
    # This ensures (approx.) 75% are within range 
    mad = self.getMAD() * CHEBYSHEV_CONST 
    med_dist = median(self.distances)
    temp = []
    for pt in self.pointSet:
      if pt.distance > (med_dist-mad) and pt.distance < (med_dist+mad):
        temp.append(pt)
    # Remove all outlying points outside of MAD range    
    self.pointSet = temp


  # Plot points to show path
  def plotPoints(self):
    x=[]
    y=[]
    for pt in self.pointSet:
      y.append(pt.ln)	
      x.append(pt.lt)
    # Options for display of graph
    mpl.use('Agg')
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111)
    # Plot graph
    ax.plot(x,y)
    # Save graph as image
    timestamp = datetime.datetime.now().strftime("%y-%m-%d-%H-%M-%S")
    fig.savefig('graphs/Graph-' + timestamp + '.png')
  

  # Print details of algorithm run
  def printPoints(self):
    print("Count: " + str(len(self.pointSet)))
    print("Percentage: " + str((len(self.pointSet)/227) * 100 ) + "%")
