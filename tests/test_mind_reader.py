import unittest

from mind_reader import next_guess, update_range


class MindReaderLogicTests(unittest.TestCase):
    def test_next_guess_returns_midpoint(self):
        self.assertEqual(next_guess(1, 1000), 500)
        self.assertEqual(next_guess(501, 1000), 750)

    def test_update_range_for_higher_hint(self):
        low, high = update_range(1, 1000, 500, "y")
        self.assertEqual((low, high), (501, 1000))

    def test_update_range_for_lower_hint(self):
        low, high = update_range(1, 1000, 500, "a")
        self.assertEqual((low, high), (1, 499))

    def test_update_range_for_exact_hint(self):
        low, high = update_range(1, 1000, 500, "e")
        self.assertEqual((low, high), (1, 1000))

    def test_inconsistent_hints_raise_error(self):
        with self.assertRaises(ValueError):
            update_range(500, 500, 500, "y")


if __name__ == "__main__":
    unittest.main()
