import unittest
import pointSet

class TestPointSet(unittest.TestCase):

  def setup(self):
    TEST_FILE = 'testdata/test_data.csv'
    pointSet = PointSet()
    pointSet.loadData()
  
  def test_calcDist(self):
    self.assertEqual("", pointSet.calcDist())

  def test_getMAD(self):
    self.assertEqual("", getMAD())

  def test_getTimes(self):
    # with bad timestamps
    pointSet.checkTimes()
    self.assertEqual("", pointSet.getLength())

    TEST_TIME_FILE = 'testdata/test_timedata.csv'
    testTimeSet = PointSet()
    testTimeSet.loadData()
    # without timestamps
    testTimeSet.checkTimes()
    self.assertEqual("", pointSet.getLength())

  def test_checkDist(self):
    pointSet.checkDistances()
    self.assertEqual("", pointSet.getLength())

  def test_removeOutliers(self):
    pointSet.removeOutliers()
    # Check various degrees of accuracy, 
    # percentage results, and correct points found 
