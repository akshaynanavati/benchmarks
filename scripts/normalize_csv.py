#!/usr/bin/env python3

import sys
import csv
from collections import defaultdict


NAMES = [
    'Size',
    'Set - Insert',
    'Unordered Set - Insert',
    'Unordered Vector - Insert',
    'Set - Lookup',
    'Unordered Set - Lookup',
    'Unordered Vector - Lookup',
]

def normalize_name(name):
    normal = name.split('/')[0]
    if normal == 'BM_set_insert':
        return NAMES[1]
    if normal == 'BM_uset_insert':
        return NAMES[2]
    if normal == 'BM_vector_insert':
        return NAMES[3]
    if normal == 'BM_set_lookup':
        return NAMES[4]
    if normal == 'BM_uset_lookup':
        return NAMES[5]
    if normal == 'BM_vector_lookup':
        return NAMES[6]


def get_rows(results):
    for i in range(len(results['Size'])):
        yield [results[name][i] for name in NAMES]


if __name__ == '__main__':
    fname = sys.argv[1]
    result_d = defaultdict(list)
    with open(fname) as f:
        reader = csv.DictReader(f)
        for row in reader:
            result_d[normalize_name(row['name'])].append(row['cpu_time'])

    for i in range(5, 16):
        result_d['Size'].append(2 ** i)

    with open(fname, 'w') as f:
        writer = csv.writer(f)
        writer.writerow(NAMES)
        for row in get_rows(result_d):
            writer.writerow(row)
