from point import Point
from statistics import median
from math import sin, cos, sqrt, atan2, radians

import csv
import datetime
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

"""
Author: Patrick Concannon

Problem: Develop an algorithm that, given a series of data
points (latitude,longitude,timestamp) for a car journey from
A->B,# which will disregard potentially erroneous points
(present in #the dataset).

Date Modified: 31 Jan, 2019


All Formulas referenced from: 
-  http://www.ltcconline.net/greenl/courses/201/descstat/mean.htm
 
-  Median Absolute Derivation(MAD) used as it is
  as it is more robust to outliers

-  Chebyshev's Theorem uses a constant k to tell us that at 
  least k%  of data lies within k%  of the Median. By increasing
  and decreasing it it will allow control over the optimal distance
  between points
  
"""

CHEBYSHEV_CONST = 20
RAD_EARTH = 6373.0 # approximate radius of earth in km
ZERO_DIST = 0.0


class PointSet:
  def __init__(self):
    self.pointSet = []
    self.distances = []


  def loadData(self, fileName):
    if fileName:
      # Load points data from file
      with open(fileName, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
          self.pointSet.append(Point(float(row[0]),float(row[1]),float(row[2])))


  def getLength(self):
        return len(self.pointSet)


  # Returns distance between two points
  def calcDist(self, p1,p2):
    # Convert to radians
    lat1 = radians(p1.lt)
    lon1 = radians(p1.ln)
    lat2 = radians(p2.lt)
    lon2 = radians(p2.ln)

    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))

    return RAD_EARTH * c


  # Gets Median Absolute Deviation(MAD)
  def getMAD(self):
    med_dist = []
    med = median(self.distances)
    for d in self.distances:
      med_dist.append(abs(d - med))

    return median(med_dist)


  # Returns a list of the distances between points
  def checkDistances(self):  
    if self.getLength() == 0:
          return None
    temp = []
    temp.append(self.pointSet[0])

    # skip duplicates e.g. points where no distance is covered 
    for i in range(self.getLength()-1):
      d = self.calcDist(self.pointSet[i],self.pointSet[i+1])
      
      if d == ZERO_DIST:
        continue

      # store distance to next point within Point obj
      self.pointSet[i].distance = d
      # store all distances between points within PointSet
      self.distances.append(d)
      # keep track of valid points
      temp.append(self.pointSet[i])

    self.pointSet = temp[:]

  # Check for timestamps which aren't consecutive
  def checkTimes(self):
    cur_low = 0
    temp = []
    for pt in self.pointSet:
      current = pt.timestamp
      # skip points with bad timestamp
      if (current < cur_low): 
        continue
      cur_low = current
      temp.append(pt)

    self.pointSet = temp[:]

  # Remove outlying points
  def removeOutliers(self):
    self.checkTimes()
    self.checkDistances()

    med = median(self.distances)
    ab_med_dist = self.getMAD() * CHEBYSHEV_CONST
    
    # Add starting point
    temp = []
    temp.append(self.pointSet[0])

    for i in range(0,self.getLength()-1):
       # if so, add that point
      d = self.pointSet[i].distance
      if med-ab_med_dist <= d <= med+ab_med_dist:
          temp.append(self.pointSet[i+1])

    self.pointSet = temp[:]


  def plotPoints(self):
    x = []
    y = []
    count = 0
    for pt in self.pointSet:
      y.append(pt.lt)
      x.append(pt.ln)
      print("Count: "+str(count) + "  Y:"+ str(pt.lt) + " X:" + str(pt.ln) + "D: " + str(pt.distance))
      count+=1
    # set options and plot
    mpl.use('Agg')
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111)

    #ax.plot(x,y) # comment to remove line from plot 
    plt.scatter(x,y) # comment to remove points from plot
    
    # Save copy of graph
    timestamp = datetime.datetime.now().strftime("%y-%m-%d-%H-%M-%S")
    fig.savefig('graphs/Graph-' + timestamp + '.png')
  

  # Print details of algorithm run
  def printPoints(self): # change to printDetails() and add more info
    print("Current count: " + str(self.getLength()))
    #for pt in self.pointSet:
    #      print(str(pt.lt)+" "+str(pt.ln))
    x = ((self.getLength()/227) * 100)
    print(str(self.getLength()) + "/227 points printed") 
    print("Percentage: {:.2f}%".format(x))