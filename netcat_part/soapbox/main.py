import pointSet


def main():
  CSV_FILE = 'data/data_points.csv'
  pointSet = PointSet()
  pointSet.loadData(CSV_FILE)
  #pointSet.removeOutliers() # take in constant here 
  #pointSet.plotPoints() 
  #pointSet.printPoints()

if __name__ == '__main__':
    main()
