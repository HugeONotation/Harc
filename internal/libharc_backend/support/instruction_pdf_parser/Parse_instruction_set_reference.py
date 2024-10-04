#!/usr/bin/env python

"""
This script is meant to extract information from Intel's 64 and IA-32
Architectures Software Developer's Manual, Volume 2.

Since the PDF file format is not meant to facilitate machine extraction of
data, this script contains a number of hacky workarounds. This script was
designed to work with the developer manual created on 9/25/2023. This script is
likely to fail for other versions and will hence require adjustment of the
hacks.
"""

# Install py-pdf-parser package before running

import py_pdf_parser.tables
from py_pdf_parser.loaders import load_file
from py_pdf_parser.components import ElementOrdering
from py_pdf_parser.filtering import ElementList

from math import isclose
from functools import reduce

counter = 0


class Instruction_variant_info:
    opcode = ''
    instruction = ''
    valid_in_64_bit_mode = ''
    compat_leg_mode = ''
    op_en = ''
    support_64_32_bit = ''
    compat_leg_mode = ''
    cpuid_feature_flag = ''
    description = ''


class Instruction_info:
    name = str
    variants: list
    encoding_table = []


table_fonts = []

instruction_page_header_font: str = ''

def extract_table(elems: py_pdf_parser.filtering.ElementList):
    # Hacky edge case handling
    nan_elements = [e for e in elems if e.text().startswith('NaN,')]
    filtered_elements = elems.remove_elements(*nan_elements)

    # Organize elements into columns using vertical overlap
    # It's assumed that elements from different columns have no vertical overlap
    placement_queue = set(filtered_elements)
    columns = []
    while placement_queue:
        e0 = placement_queue.pop()
        column = {e0}

        exploration_queue = set(filtered_elements.vertically_in_line_with(e0, all_pages=True))
        while exploration_queue:
            e1 = exploration_queue.pop()
            placement_queue.discard(e1)
            column.add(e1)
            exploration_queue |= set(filtered_elements.vertically_in_line_with(e1, all_pages=True)) - column

        columns.append(sorted(column, key=lambda e: e.bounding_box.x0))

    columns = sorted(columns, key=lambda r: r[0].bounding_box.x0)


    # Organize elements into rows using horizontal overlap
    # It's assumed that elements from different rows have no horizontal overlap
    placement_queue = set(filtered_elements)
    rows = []
    while placement_queue:
        e0 = placement_queue.pop()
        row = {e0}

        exploration_queue = set(filtered_elements.horizontally_in_line_with(e0))
        while exploration_queue:
            e1 = exploration_queue.pop()
            placement_queue.discard(e1)
            row.add(e1)
            exploration_queue |= set(filtered_elements.horizontally_in_line_with(e1)) - row

        rows.append(sorted(row, key=lambda e: e.bounding_box.x0))

    rows = sorted(rows, key=lambda r: -r[0].bounding_box.y0 + 1000 * r[0].page_number)

    # Organize elements into table as strings
    table_elements = []
    for row in rows:
        table_row = []
        for column in columns:
            cell_entries = set(row) & set(column)
            table_row.append([e for e in cell_entries])
        table_elements.append(table_row)

    # Merge incomplete rows upwards
    num_columns = max(0, *[len(r) for r in table_elements])

    table = []
    for i in range(0, len(rows)):
        populated_entry_count = reduce(lambda x, y: x+y, [bool(l) for l in table_elements[i]])

        if populated_entry_count == num_columns:
            table.append(table_elements[i])
            continue

        for j in range(0, num_columns):
            table[-1][j] += table_elements[i][j]

    # Convert table to table of strings
    table_as_strings = []
    for i in range(0, len(table)):
        string_row = [''] * num_columns
        for j in range(0, num_columns):
            cell_elems = sorted(table[i][j], key=lambda e: -e.bounding_box.y0)
            string_row[j] = '\n'.join([e.text() for e in cell_elems])

        table_as_strings.append(string_row)

    # Remove duplicates of the heading row
    heading_row = table_as_strings[0]
    table_as_strings = [row for row in table_as_strings if 'opcode' not in row[0].lower()]
    table_as_strings.insert(0, heading_row)

    # Hacky edge case handling
    for row in table_as_strings:
        for i in range(0, len(row)):
            if row[i].startswith('Tests the input for the following'):
                row[i] = 'Tests the input for the following categories: NaN, +0, -0, +Infinity, -Infinity, denormal, finite negative. The immediate field provides a mask bit for each of these category tests. The masked test results are OR-ed together to form a mask result.'

    return table_as_strings


def extract_instruction_info(
    elems: py_pdf_parser.filtering.ElementList,
    section_heading_fonts
):
    global instruction_page_header_font
    page_headings = list(elems.filter_by_font(instruction_page_header_font))

    instruction_heading = next((page_heading for page_heading in reversed(page_headings) if page_heading.page_number == page_headings[0].page_number), None)

    section_heading_candidates = (elems.remove_elements(*page_headings)).filter_by_fonts(*section_heading_fonts)

    # Figure headings used the same font as section headings
    figure_headings = elems.filter_by_text_contains('Figure')

    # Table headings used the same font as section headings
    table_headings = elems.filter_by_text_contains('Table')

    # Table headings may be erroneously recognized as section headings
    # They can be filtered out based on the fact that section headings don't have neighbors
    table_column_headings = [e for e in section_heading_candidates if len(elems.horizontally_in_line_with(e)) != 0]

    # If column headers don't have horizontal neighbors, then they may pass the previious filter
    # They are unlikely to have the proper alignment however
    removal_candidates = [e for e in section_heading_candidates if not isclose(e.bounding_box.x0, 45.0, abs_tol=10) and not isclose((e.bounding_box.x1 + e.bounding_box.x0) / 2.0, 302.0, abs_tol=10.0)]

    # List of section headings
    section_headings = (section_heading_candidates - figure_headings - table_headings).remove_elements(*table_column_headings).remove_elements(*removal_candidates)

    # Page headings and footings
    page_headings_footings = [e for e in elems if (e.bounding_box.y1 <= 55.00) or (745.0 <= e.bounding_box.y0)]

    # Labeled notes that may appear after table
    note_anchor = next((e for e in elems if 'notes:' in e.text().lower()), None)
    if note_anchor is not None:
        tmp = elems.after(note_anchor, inclusive=True)
        later_section_headings = (elems.after(note_anchor, inclusive=True) & section_headings)

        if later_section_headings:
            note_elements = tmp.before(later_section_headings[0])
        else:
            note_elements = tmp
    else:
        note_elements = []

    # Unlabeled notes that may come after table
    unlabeled_notes = [e for e in elems if (e.bounding_box.x1 - e.bounding_box.x0) > 287.8]

    # Random backslash on the MOVS page above the upper-left corner of the first table
    backslash_elements = [e for e in elems if e.text() == '\\']

    # List of elements which may be part of a table
    table_candidates = (elems
        .remove_elements(*page_headings)
        .remove_elements(*section_headings)
        .remove_elements(*page_headings_footings)
        .remove_elements(*note_elements)
        .remove_elements(*unlabeled_notes)
        .remove_elements(*backslash_elements)
    )

    # Extract instruction variant table
    instruction_table_candidates = table_candidates.between(instruction_heading, section_headings[0])
    instruction_table = extract_table(instruction_table_candidates)

    # Extract instruction encoding table
    encoding_table_heading_idx = None
    encoding_table_heading_matches = [i for i, e in enumerate(section_headings) if 'Instruction Operand Encoding' in e.text()]
    if len(encoding_table_heading_matches) > 0:
        encoding_table_heading_idx = encoding_table_heading_matches[0]

    encoding_table = None
    if encoding_table_heading_idx is not None:
        encoding_table_heading = section_headings[encoding_table_heading_idx]
        if section_headings[-1] == encoding_table_heading:
            encoding_table = extract_table(table_candidates.after(encoding_table_heading))
        else:
            e0 = section_headings[encoding_table_heading_idx]
            e1 = section_headings[encoding_table_heading_idx + 1]

            encoding_table = extract_table(table_candidates.between(e0, e1))

    # Construct return value
    ret = Instruction_info()
    ret.name = elems[0].text()
    ret.variants = []

    for values in instruction_table[1:]:
        tmp = dict(zip(instruction_table[0], values))
        ret.variants.append(tmp)

    ret.encoding_table = encoding_table

    return ret


def parse_instruction_set_reference(path):
    layout_dict = {
        'line_overlap': 0.5,
        'char_margin': 0.81438,  # This has been lowered from 2.0
        'line_margin': 0.11,  # This has been lowered from 0.5
        'word_margin': 0.1,
        'boxes_flow': 0.5,
        'detect_vertical': False,
        'all_texts': False
    }

    doc = load_file(
        path,
        la_params=layout_dict,
        element_ordering=ElementOrdering.LEFT_TO_RIGHT_TOP_TO_BOTTOM
    )

    # Identify first relevant page
    first_relevant_page = None
    for i in range(0, len(doc.pages)):
        # Check for page heading corresponding to relevant chapter
        if 'INSTRUCTION SET REFERENCE, ' not in doc.pages[i].elements[0].text():
            continue

        if 'NeoSansIntel,9.0' not in doc.pages[i].elements[0].font:
            continue

        if not isclose(749.076, doc.pages[i].elements[0].bounding_box.y0, abs_tol=0.25):
            continue

        # Check for instruction table heading
        if '—' not in doc.pages[i].elements[1].text():
            continue

        if 'NeoSansIntelMedium,12.0' not in doc.pages[i].elements[1].font:
            continue

        if not isclose(711.41, doc.pages[i].elements[1].bounding_box.y0, abs_tol=0.25):
            continue

        first_relevant_page = i + 1
        break

    if first_relevant_page is None:
        first_relevant_page = 1

    # Identify last relevant page
    last_relevant_page = None
    for i in range(len(doc.pages) - 1, -1, -1):
        if 'CHAPTER ' not in doc.pages[i].elements[0].text():
            continue

        if '\nSAFER MODE EXTENSIONS REFERENCE' not in doc.pages[i].elements[0].text():
            continue

        last_relevant_page = i
        break

    if last_relevant_page is None:
        last_relevant_page = len(doc.pages)

    # Filter for pages in relevant range
    elems = doc.elements.filter_by_pages(*range(first_relevant_page, last_relevant_page + 1))

    # Filter for instruction headers
    instruction_headers = elems.filter_by_font(elems[1].font).filter_by_regex('.*—.*')

    # Search for section heading font
    section_heading_fonts = [f for f in doc.fonts if 'NeoSansIntelMedium,10.0' in f or 'NNLNGJ+NeoSansIntelMedium,9.0' in f]
    table_headers_font = [f for f in doc.fonts if 'NeoSansIntelMedium,9.0' in f][0]
    table_body_font0 = [f for f in doc.fonts if 'NeoSansIntel,9.0' in f][0]
    table_body_font1 = [f for f in doc.fonts if 'NeoSansIntel,Italic,9.0'][0]

    global table_fonts
    table_fonts = [table_headers_font, table_body_font0, table_body_font1]

    global instruction_page_header_font
    instruction_page_header_font = [f for f in doc.fonts if 'NeoSansIntelMedium,12.0' in f][0]

    # Extract instruction information
    instruction_data = []
    for e0, e1 in zip(instruction_headers, instruction_headers[1:]):
        relevant_elements = elems.between(e0, e1).add_element(e0)

        results = extract_instruction_info(
            relevant_elements,
            section_heading_fonts
        )

        if results:
            instruction_data.append(results)

    results = extract_instruction_info(
        elems.after(instruction_headers[-1]).add_element(instruction_headers[-1]),
        section_heading_fonts
    )

    if results:
        instruction_data.append(results)

    return instruction_data


def clean_keys(results: list):
    corrections = {
        'Opcode/\nInstruction': 'Opcode/Instruction',
        'Opcode /\nInstruction': 'Opcode/Instruction',
        'Opcode\nInstruction': 'Opcode/Instruction',
        'Opcode*/\nInstruction': 'Opcode/Instruction',
        'Opcode / \nInstruction': 'Opcode/Instruction',
        'Opcode Instruction': 'Opcode/Instruction',
        'Opcode*/Instruction': 'Opcode/Instruction',
        'Opcode*': 'Opcode',
        'Opcode1': 'Opcode',
        'Op/ \nEn': 'Op/En',
        'Op / \nEn': 'Op/En',
        'Op/\nEn': 'Op/En',
        'Op/E\nn': 'Op/En',
        'Op \nEn': 'Op/En',
        'Op\n/ \nEn': 'Op/En',
        'Op \n/ En': 'Op/En',
        'Op\n/\nEn': 'Op/En',
        'Op / En': 'Op/En',
        'Op/ En': 'Op/En',
        'Op En': 'Op/En',
        'Op/E n': 'Op/En',
        'Compat/\nLeg Mode': 'Compat/Leg Mode',
        'Compat/\nLeg Mode1': 'Compat/Leg Mode',
        'Compat/Leg Mode1': 'Compat/Leg Mode',
        'Compat/\nLeg \nMode': 'Compat/Leg Mode',
        '64-bit \nMode': '64-bit Mode',
        '64-Bit \nMode': '64-bit Mode',
        '64-Bit\nMode': '64-bit Mode',
        '64-Bit Mode': '64-bit Mode',
        '64/32bit \nMode \nSupport': '64/32-bit Mode Support',
        '64/32 bit \nMode \nSupport': '64/32-bit Mode Support',
        '32/64 \nbit Mode \nSupport': '64/32-bit Mode Support',
        '32/64\nbit Mode\nSupport': '64/32-bit Mode Support',
        '32/64 bit Mode Support': '64/32-bit Mode Support',
        '64/32bit Mode Support': '64/32-bit Mode Support',
        '64/32 bit Mode Support': '64/32-bit Mode Support',
        '64/32-bit \nMode': '64/32-bit Mode',
        '64/32-\nbit \nMode': '64/32-bit Mode',
        '64/32-\nbit Mode': '64/32-bit Mode',
        '64/32 \nbit Mode \nSupport': '64/32-bit Mode',
        '64/32 \nbit \nMode \nSupport': '64/32-bit Mode',
        '64/3\n2-bit \nMode': '64/32-bit Mode',
        '64/32\n-bit \nMode': '64/32-bit Mode',
        '64/32- bit Mode': '64/32-bit Mode',
        '64/32 -bit Mode': '64/32-bit Mode',
        '64/3 2-bit Mode': '64/32-bit Mode',
        'CPUID \nFeature \nFlag': 'CPUID Feature Flag',
        'CPUID Feature \nFlag': 'CPUID Feature Flag',
        'CPUID\n Feature Flag': 'CPUID Feature Flag',
        'CPUID \nFeature Flag': 'CPUID Feature Flag',
        'CPUID Fea-\nture Flag': 'CPUID Feature Flag',
        'CPUID\nFeature Flag': 'CPUID Feature Flag',
        'CPUID Fea- ture Flag': 'CPUID Feature Flag'
    }

    for result in results:
        ret = []
        for variant in result.variants:
            keys = [key.replace('\n', ' ').replace('  ', ' ').replace('/ ', '/').replace(' /', '/') for key in variant.keys()]
            keys = list(map(lambda x: corrections.get(x, x), keys))
            vals = variant.values()
            ret.append(dict(zip(keys, vals)))

        result.variants = ret

    problem_keys = [
        '',
        'Description\nCPUID \nFeature \nFlag',
        'INSTRUCTION SET REFERENCE, A-L',
        'Opcode/',
        'INSTRUCTION SET REFERENCE, M-U',
        'Description\nOp/ En\nOpcode/\nInstruction\n64/32 bit \nMode \nSupport\nCPUID \nFeature \nFlag',
        '32/64',
        'Description\nOpcode/\nInstruction\nOp/ \nEn\nCPUID \nFeature Flag\n64/32 bit \nMode \nSupport',
        'Description\nCPUID \nFeature \nFlag',
        'Op/ \nEn\nOpcode/\nInstruction\n64-Bit \nMode',
        'Support'
    ]

    dummy0 = set()
    dummy1 = set()
    dummy2 = set()
    problem_results = set()
    for result in results:
        if result.encoding_table:
            for variant in result.variants:
                dummy0 |= set(variant.keys())
            dummy1 |= set(result.encoding_table[0])
            dummy2.add(';'.join(result.encoding_table[0]))

    return results


class Instruction_data:
    name: str = None
    opcode: str = None
    cpuid: str = None
    operand_encoding: str = None
    support_64_bit: str = None


def create_instruction_data_structs(infos: [Instruction_info]):
    ret = []
    for info in infos:
        for variant in info.variants:
            instruction_data = Instruction_data()
            for key in variant.keys():
                match key:
                    case 'Instruction':
                        instruction_data.name = variant[key].replace('\n', ' ')
                    case 'Opcode':
                        instruction_data.opcode = '"' + variant[key].replace('\n', ' ') + '"'
                    case 'Description':
                        pass
                    case 'Opcode/Instruction':
                        if variant[key].count('\n') == 1:
                            a, b = variant[key].split('\n')
                            instruction_data.opcode = a
                            instruction_data.name = b
                        else:
                            lines = variant[key].split('\n')
                            instruction_data.opcode = lines[0]
                            instruction_data.name = ' '.join(lines[1:])
                    case 'Compat/Leg Mode':
                        pass
                    case '64/32-bit Mode':
                        #TODO: Consider conversion from single character codes to something else
                        a, b = variant[key].split('/')
                        instruction_data.support_64_bit = a
                    case 'Op/En':
                        table = info.encoding_table
                        for row in table[1:]:
                            if row[0] != variant[key]:
                                continue

                            str_representation = '\"'
                            for value, header in zip(row, table[0]):
                                str_representation += header + ': ' + value + ';'
                            str_representation += '\"'

                            str_representation.replace('\n', ' ')

                            instruction_data.operand_encoding = str_representation
                    case '64-bit Mode':
                        instruction_data.support_64_bit = variant[key]
                    case '64/32-bit Mode Support':
                        #TODO: Consider conversion from single character codes to something else
                        if '/' in variant[key]:
                            a, b = variant[key].split('/')
                            instruction_data.support_64_bit = a
                        else:
                            instruction_data.support_64_bit = variant[key]
                    case 'CPUID Feature Flag':
                        instruction_data.cpuid = '"' + variant[key].replace('\n', ';') + '"'
                    case 'CPUID':
                        instruction_data.cpuid = '"' + variant[key].replace('\n', ';') + '"'
                    case _:
                        print('Unrecognized key: ' + repr(key))
            ret.append(instruction_data)
    return ret


def export_csv(data: [Instruction_data]):
    # Initialize output with column headers
    output = 'Name, Opcode, CPU Flags, 64-bit Support, Operand Encoding\n'

    # Print instruction data
    for instruction_datum in data:
        output += '\"' + str(instruction_datum.name) + '\", '
        output += str(instruction_datum.opcode) + ', '
        output += str(instruction_datum.cpuid) + ', '
        output += str(instruction_datum.support_64_bit) + ', '
        output += str(instruction_datum.operand_encoding) + '\n'

    with open('instructions.csv', 'w') as f:
        f.write(output)


def main():
    #path = '325383-sdm-vol-2abcd-1.pdf'
    path = 'relevant_pages.pdf'
    #path = './test.pdf'
    results = parse_instruction_set_reference(path)
    cleaned_results = clean_keys(results)

    dummy0 = set()
    dummy1 = set()
    dummy2 = set()
    problem_results = set()
    for cleaned_result in cleaned_results:
        if cleaned_result.encoding_table:
            for variant in cleaned_result.variants:
                dummy0 |= set(variant.keys())
            dummy1 |= set(cleaned_result.encoding_table[0])
            dummy2.add(';'.join(cleaned_result.encoding_table[0]))

    structs = create_instruction_data_structs(cleaned_results)
    export_csv(structs)

    global counter
    dummy = 345

    pass


if __name__ == "__main__":
    main()
