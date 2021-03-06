#!/usr/bin/env python3

import sys
import csv
from collections import defaultdict


names = [
    'Size',
]

sizes = []

def normalize_name(name):
    normal, size = name.split('/')
    if normal not in names:
        names.append(normal)
    size = int(size)
    if size not in sizes:
        sizes.append(size)
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

    result_d['Size'] = sizes

    fstem = fname[:-4]
    with open(fstem + '_norm.csv', 'w') as f:
        writer = csv.writer(f)
        writer.writerow(names)
        for row in get_rows(result_d):
            writer.writerow(row)
