#!/usr/bin/env python3

import sys
import csv
from collections import defaultdict


names = [
    'Size',
]

def normalize_name(name):
    normal = name.split('/')[0]
    if normal not in names:
        names.append(normal)
    return normal


def get_rows(results):
    for i in range(len(results['Size'])):
        yield [results[name][i] for name in names]


if __name__ == '__main__':
    fname = sys.argv[1]
    result_d = defaultdict(list)
    with open(fname) as f:
        reader = csv.DictReader(f)
        for row in reader:
            result_d[normalize_name(row['name'])].append(row['cpu_time'])

    for i in range(5, 16):
        result_d['Size'].append(2 ** i)

    fstem = fname[:-4]
    with open(fstem + '_norm.csv', 'w') as f:
        writer = csv.writer(f)
        writer.writerow(names)
        for row in get_rows(result_d):
            writer.writerow(row)
