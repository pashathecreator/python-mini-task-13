import pytest
from numpy import array_equal
from matrix_power import foreign_matrix_power as matrix_power


def test_matrix_power_default():
    matrix = [[1, 2], [3, 4]]
    power = 2
    expected = [[7, 10], [15, 22]]
    assert array_equal(matrix_power(matrix, power), expected)

def test_matrix_power_another_default():
    matrix = [[1, 2], [3, 4]]
    power = 0
    expected = [[1, 0], [0, 1]]
    assert array_equal(matrix_power(matrix, power), expected)


def test_not_squared_matrix():
    matrix = [[1, 2, 3], [4, 5, 6]]
    power = 2
    with pytest.raises(ValueError, match="Matrix must be square"):
        matrix_power(matrix, power)

def test_empty_matrix():
    matrix = []
    power = 0
    with pytest.raises(ValueError, match="Matrix cannot be empty"):
        matrix_power(matrix, power)


def test_negative_power():
    matrix = [[1, 2], [5, 6]]
    power = -1
    with pytest.raises(ValueError, match="Power must be a positive integer"):
        matrix_power(matrix, power)


def test_nonumeric_values_in_matrix():
    matrix = [[1, 2], [4, "foobar"]]
    power = 2
    with pytest.raises(ValueError, match="Matrix contains non-numeric values"):
        matrix_power(matrix, power)
