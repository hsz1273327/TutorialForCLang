import unittest
from binaryvector import BinaryVector


class Test_BinaryVector(unittest.TestCase):

    def test_mod(self):
        a = BinaryVector(1, 2)
        self.assertEqual(round(a.mod(), 3), 2.236)

    def test_mul(self):
        a = BinaryVector(1, 2)
        b = BinaryVector(3, 4)
        self.assertEqual(a * b, 11.0)

    def test_add(self):
        a = BinaryVector(1, 2)
        b = BinaryVector(3, 4)
        c = a + b
        self.assertEqual(c.x, 4.0)
        self.assertEqual(c.y, 6.0)
