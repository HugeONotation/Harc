#!/usr/bin/env python

import requests
import zipfile
import argparse
import os
from urllib.request import urlretrieve

zip_path = './UCD.zip'


def download_ucd_tables(requested_version: str):
    """
    Retrieve UCD tables directly from unicode.org

    :param requested_version: Unicode version string. Must be of the form X.Y.Z or 'latest'
    """

    url = 'https://www.unicode.org/Public/'
    if requested_version == 'latest':
        url += 'UCD/latest/ucd/UCD.zip'
    else:
        url += requested_version + '/ucd/UCD.zip'

    try:
        urlretrieve(url, zip_path)
    except Exception as e:
        print('Could not complete download of UCD.zip from ', url)

    with zipfile.ZipFile(zip_path) as zip_ref:
        zip_ref.extractall('./ucd/')


def trim_comments(line: str):
    idx = line.find('#')
    if idx == -1:
        return line
    return line[0:idx]


def preprocess_ucd_file(text: str):
    """

    :param text: Text from a UCD file
    :return: List corresponding to lines containing strings for each column of
    source file
    """
    lines = text.split('\n')

    # Trim comments off of lines
    lines = [trim_comments(line) for line in lines]

    # Filter empty lines
    lines = [line for line in lines if line]

    # Split lines into columns and trim whitespace
    ret = [[col.strip() for col in line.split(';')] for line in lines]

    return ret


def generate_unicode_identifier_tables():

    with open('./ucd/DerivedCoreProperties.txt', 'r') as f:
        properties_file = f.read()
        lines = preprocess_ucd_file(properties_file)

    # String mappings
    mappings = {
        'ID_Continue' : 0,
        'ID_Start' : 1,
    }

    # Filter out irrelevant lines
    lines = [line for line in lines if line[1] in mappings.keys()]

    # Extract range boundaries
    identifier_ranges = {}
    for columns in lines:
        range_boundary_strings = columns[0].split('..')

        if len(range_boundary_strings) == 1:
            range_boundary_strings *= 2

        key = (
            int(range_boundary_strings[0], 16),
            int(range_boundary_strings[1], 16) + 1
        )

        identifier_ranges[key] = max(mappings[columns[1]], identifier_ranges.get(key, 0))

    # Manually set _ as being able to start identifiers
    underscore_key = (95, 96)
    identifier_ranges[underscore_key] = 1

    # Join adjacent ranges
    identifier_ranges = sorted(identifier_ranges.items(), key=lambda t: t[0][0])

    refined_range_bounds = [identifier_ranges[0]]
    for range_bounds in identifier_ranges[1:]:
        if refined_range_bounds[-1][1] == range_bounds[0] and refined_range_bounds[-1][2] == range_bounds[2]:
            refined_range_bounds[-1][1] = range_bounds[1]
        else:
            refined_range_bounds.append(range_bounds)

    return refined_range_bounds


def generate_identifier_range_cpp_arrays(range_bounds, lower_bound: int, upper_bound: int):
    # Filter out ranges that contain elements outside the range of [lower_bound, upper_bound)
    filtered_bounds = []
    for r in range_bounds:
        c0 = r[0][0] < lower_bound and r[0][1] < lower_bound
        c1 = upper_bound <= r[0][0] and upper_bound <= r[0][1]

        if c0 or c1:
            # Range does not fall into specified bounds
            continue

        if r[0][0] < lower_bound:
            r[0][0] = lower_bound

        if upper_bound <= r[0][1]:
            r[0][1] = lower_bound

        filtered_bounds.append(r)

    range_bounds = filtered_bounds

    # Generate C++ code
    target_width = 80
    indentation = 8
    max_col_width = len(str(range_bounds[-1][0][1]))
    n_columns = (target_width - indentation) // (max_col_width + 2)

    range_begins = '        '
    range_lengths = '        '

    for i in range(0, len(range_bounds)):
        range_begin_str = str(range_bounds[i][0][0])
        range_begin_str = ' ' * (max_col_width - len(range_begin_str)) + range_begin_str
        range_begins += range_begin_str
        range_begins += ', '

        range_end_str = str(range_bounds[i][0][1] - range_bounds[i][0][0])
        range_end_str = ' ' * (max_col_width - len(range_end_str)) + range_end_str
        range_lengths += range_end_str
        range_lengths += ', '

        if i % n_columns == (n_columns - 1):
            range_begins += '\n        '
            range_lengths += '\n        '

    # Generate C++ bit arrays
    mask_as_number = int(''.join([str(r[1]) for r in range_bounds]), 2)
    mask_hex_string = '{:02x}'.format(mask_as_number)
    mask_hex_string = '0' * (len(mask_hex_string) % 8) + mask_hex_string

    masks = [mask_hex_string[i:i + 8] for i in range(0, len(mask_hex_string), 8)]

    masks = ['0x' + mask for mask in masks]

    range_masks = '        '
    for i in range(0, len(masks)):
        mask_str = str(masks[i])
        range_masks += mask_str
        range_masks += ', '

        if i % 4 == 3:
            range_masks += '\n        '

    return range_begins, range_lengths, range_masks


def generate_whitespace_table():
    with open('./ucd/PropList.txt', 'r') as f:
        properties_file = f.read()
        lines = preprocess_ucd_file(properties_file)

    ret = []
    for line in lines:
        if line[1] != 'Pattern_White_Space':
            continue

        range_bounds = line[0].split('..')
        if len(range_bounds) == 1:
            ret.append(int(range_bounds[0], 16))
        elif len(range_bounds) == 2:
            a = int(range_bounds[0], 16)
            b = int(range_bounds[1], 16)
            ret += list(range(a, b))

    ret = [r for r in ret if r >= 128]

    return ret


def generate_white_space_cpp_arrays(codepoints: list[int]):
    ret = '        '
    for i in range(0, len(codepoints)):
        mask_str = str(codepoints[i])
        ret += mask_str
        ret += ', '

        if i % 4 == 3:
            ret += '\n        '

    ret = ret.rstrip()

    return ret


def main():
    # Configure command line arguments
    parser = argparse.ArgumentParser(description='Unicode table generator')
    parser.add_argument('-v', "--version", help="Unicode version", default='latest')
    args = parser.parse_args()

    # Check if UCD files have already been downloaded
    if os.path.isdir('./ucd/'):
        print('UCD directory found. Skipping download. Manually delete UCD directory to avoid this')
    else:
        download_ucd_tables(args.version)

    id_subranges = generate_unicode_identifier_tables()
    range_begins16, range_lengths16, range_masks16 = generate_identifier_range_cpp_arrays(id_subranges, 128, 65536)
    range_begins21, range_lengths21, range_masks21 = generate_identifier_range_cpp_arrays(id_subranges, 128, 2097152)

    white_space_codepoints = generate_whitespace_table()
    white_space_table = generate_white_space_cpp_arrays(white_space_codepoints)

    with open('./Unicode_tables_template.hpp', 'r') as f:
        file_template = f.read()

    file_template = file_template.replace('$CODEPOINT16_BEGINS', range_begins16)
    file_template = file_template.replace('$CODEPOINT16_ENDS', range_lengths16)
    file_template = file_template.replace('$CODEPOINT16_CATEGORIES', range_masks16)

    file_template = file_template.replace('$CODEPOINT21_BEGINS', range_begins21)
    file_template = file_template.replace('$CODEPOINT21_ENDS', range_lengths21)
    file_template = file_template.replace('$CODEPOINT21_CATEGORIES', range_masks21)

    file_template = file_template.replace('$NON_ASCII_WHITESPACE_ARRAY', white_space_table)

    with open('../../include/harc/unicode/Unicode_tables.hpp', 'w+') as f:
        f.write(file_template)


if __name__ == "__main__":
    main()
