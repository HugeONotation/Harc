#!/usr/bin/env python

import csv


class Encoding_properties:
    np: bool = False
    nfx: bool = False
    rex_changes_operand_size_or_semantics: bool = False
    mod_rm_opcode_extension: str = None
    mod_rm_contains_reg_and_rm_operand: bool = False
    code_offset_type: str = None
    immediate_size: str = None
    opcode_encoded_register_operand_size: str = None

    #TODO: Consider +0, ..., +7 fields

    has_vex_prefix: str
    has_vex3_prefix: str

    vex_vector_length: str
    vex_implied_prefix: str
    vex_implied_escape_byte: str
    vex_implied_rex_prefix: str



    has_evex_prefix: str
    evex_vector_length: str
    evex_implied_prefix: str
    evex_implied_escape_byte: str
    evex_implied_rex_prefix: str

    opcode: str = ''


def parse_opcode_encoding(text):
    ret = Encoding_properties()
    words = ret.split(' ')

    for word in words:
        if word == 'NP':
            ret.np = True
            continue

        if word == 'NFx':
            ret.nfx = True
            continue

        if word == 'REX.W':
            ret.rex_changes_operand_size_or_semantics = True
            continue

        if word[0] == '/' and word[1].isdigit():
            ret. mod_rm_opcode_extension = word[1]
            continue

        if word == '/r' or word == '/r1':
            ret.mod_rm_contains_reg_and_rm_operand = True
            continue

        if word in ['cb', 'cw', 'cd', 'cp', 'co', 'ct']:
            ret.code_offset_type = word
            continue

        if word in ['ib', 'iw', 'id', 'io']:
            ret.immediate_size = word
            continue

        if word in ['+rb', '+rw', '+rd', '+ro']:
            ret.opcode_encoded_register_operand_size = word
            continue

        if word.startswith('VEX.'):
            parts = word.split('.')
            vex, size, implied_prefix, implied_escape_byte, w = parts

            ret.has_vex_prefix = True
            ret.vex_vector_length = size
            ret.vex_implied_prefix = implied_prefix
            ret.vex_implied_escape_byte = implied_escape_byte
            ret.vex_implied_rex_prefix = w

            continue #TODO: Touch up implementation

        if word.startswith('EVEX.'):
            parts = word.split('.')
            evex, size, implied_prefix, implied_escape_byte, w = parts

            ret.has_evex_prefix = True
            ret.evex_vector_length = size
            ret.evex_implied_prefix = implied_prefix
            ret.evex_implied_escape_byte = implied_escape_byte
            ret.evex_implied_rex_prefix = w

            continue #TODO: Touch up implementation

        if word[0] in ['0123456789abcdef']:
            ret.opcode += word
            continue # TODO: Touch up implementation to handle prefixes more gracefully

        print('Unhandled input: ', word)

    return ret


def main():
    with open('instructions.csv', 'r') as f:
        table = list(csv.reader(f, delimiter=',', ))

    for row in table[1:]:
        name = row[0]
        opcode = row[1]
        cpu_flags = row[2]
        support_64_bit = row[3]
        operand_encoding = row[4]

        output = name + ': '
        parse_opcode_encoding(opcode)


    pass


if __name__ == "__main__":
    main()
