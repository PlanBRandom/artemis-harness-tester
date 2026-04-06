# tests/test_matrix.py

import itertools

def build_expected_from_net_ids(net_ids):
    n = len(net_ids)
    expected = [[0]*n for _ in range(n)]
    for i, j in itertools.product(range(n), range(n)):
        expected[i][j] = 1 if net_ids[i] == net_ids[j] else 0
    return expected

def test_simple_netlist():
    net_ids = [0, 0, 1, 2, 2]
    expected = build_expected_from_net_ids(net_ids)

    # same net → 1
    assert expected[0][1] == 1
    assert expected[3][4] == 1

    # different nets → 0
    assert expected[0][2] == 0
    assert expected[1][3] == 0
    assert expected[2][4] == 0
