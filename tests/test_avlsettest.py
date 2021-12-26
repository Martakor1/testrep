import unittest
from teamcity import is_running_under_teamcity
from teamcity.unittestpy import TeamcityTestRunner
import os

def uploadInput(amount, inputStr):
    f = open("avlset.in", "w")
    f.write(str(amount)+'\n'+inputStr)
    f.close()

def downloadOutput():
    f = open("avlset.out", "r")
    result = ' '.join(f.read().split())
    f.close()
    return result

class TestSum(unittest.TestCase):
    def test1(self):
        amount = 6
        inputStr = "A 3 A 4 A 5 C 4 C 6 D 5"
        expectedOut = "0 1 0 Y N -1"
        uploadInput(amount, inputStr)
        os.startfile("avlset.exe")
        result = downloadOutput()
        self.assertEqual(result, expectedOut)
    def test2(self):
        uploadInput(5, "A 3 A 2 A 7 A 8 A -5")
        os.startfile("avlset.exe")
        self.assertEqual("0 -1 0 1 0 Y N", downloadOutput())
    def test3(self):
        uploadInput(7, "A 3 A 2 D 3 A 6 A 8 C 6 C 3")
        os.startfile("avlset.exe")
        self.assertEqual("0 -1 0 1 0", downloadOutput())

if __name__ == '__main__':
    if is_running_under_teamcity():
        runner = TeamcityTestRunner()
    else:
        runner = unittest.TextTestRunner()
    unittest.main(testRunner=runner)