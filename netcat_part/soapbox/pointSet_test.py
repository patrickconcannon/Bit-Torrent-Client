import unittest
import pointSet

class TestPointSet(unittest.TestCase):

  def setup(self):
    TEST_FILE = 'testdata/test_data.csv'
    testPointSet = PointSet()
    testPointSet.loadData()
  
  def test_calcDist(self):
    self.assertEqual("", testPointSet.calcDist())

  def test_getMAD(self):
    self.assertEqual("", getMAD())

  def test_getTimes(self):
    # with bad timestamps
    testPointSet.checkTimes()
    self.assertEqual("", testPointSet.getLength())

    TEST_TIME_FILE = 'testdata/test_timedata.csv'
    testTimeSet = PointSet()
    testTimeSet.loadData()
    # without timestamps
    testTimeSet.checkTimes()
    self.assertEqual("", testPointSet.getLength())

  def test_checkDist(self):
    testPointSet.checkDistances()
    self.assertEqual("", testPointSet.getLength())

  def test_removeOutliers(self):
    testPointSet.removeOutliers()
    # Check various degrees of accuracy, 
    # percentage results, and correct points found 


if __name__ == '__main__':
    unittest.main()