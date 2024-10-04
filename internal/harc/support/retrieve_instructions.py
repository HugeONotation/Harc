#!/usr/bin/env python3

import urllib.request
import os

def main():
    uica_instructions_url = 'https://www.uops.info/instructions.xml'
    file_name = 'instructions_x86.xml'

    if os.path.exists('assets/' + file_name):
        return

    urllib.request.urlretrieve(uica_instructions_url, file_name)

    agner_instructions_url = 'https://www.agner.org/optimize/instruction_tables.ods'


    #TODO: Read the instructions xml file and repurpose the

if __name__=="__main__":
    main()
