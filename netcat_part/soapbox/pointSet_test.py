import unittest
import pointSet

class TestPointSet(unittest.TestCase):


  def test_calcDist(self):
    TEST_FILE = 'test_data/test_data.csv'
    testPointSet = pointSet.PointSet()
    testPointSet.loadData(TEST_FILE)
    self.assertEqual(157.27478863265398, \
    testPointSet.calcDist(testPointSet.pointSet[0].lt,testPointSet.pointSet[0].ln,testPointSet.pointSet[1].lt,testPointSet.pointSet[1].ln))


  def test_checkDist(self):
      TEST_FILE = 'test_data/test_data.csv'
      testPointSet = pointSet.PointSet()
      testPointSet.loadData(TEST_FILE)
      self.assertEqual(5, testPointSet.getLength())
      testPointSet.checkDistances()
      self.assertEqual(3, testPointSet.getLength())


  def test_getMAD(self):
    TEST_FILE = 'test_data/test_data.csv'
    testPointSet = pointSet.PointSet()
    testPointSet.loadData(TEST_FILE)
    testPointSet.checkDistances()  ## change the name of checkDistances()
    self.assertEqual(1242.324292013906, testPointSet.getMAD())


  def test_checkTimes(self):
    TEST_FILE = 'test_data/test_data.csv'
    testPointSet = pointSet.PointSet()
    testPointSet.loadData(TEST_FILE)
    # with bad timestamps
    testPointSet.checkTimes()
    self.assertEqual(5, testPointSet.getLength())
    
    TEST_TIME_FILE = 'test_data/test_timedata.csv'
    testTimeSet = pointSet.PointSet()
    testTimeSet.loadData(TEST_TIME_FILE)
    # without timestamps
    testTimeSet.checkTimes()
    self.assertEqual(2, testTimeSet.getLength())


  def test_removeOutliers(self):
    TEST_FILE = 'test_data/test_data.csv'
    testPointSet = pointSet.PointSet()
    testPointSet.loadData(TEST_FILE)
    testPointSet.removeOutliers()
    testPointSet.printPoints()
    self.assertEqual(3, testPointSet.getLength())

    # Check various degrees of accuracy, 
    # percentage results, and correct points found 


if __name__ == '__main__':
    unittest.main()