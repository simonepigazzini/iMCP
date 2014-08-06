#!/usr/bin/python

import sys
import os
import commands
from commands import getstatusoutput
import datetime
import argparse
import string

if __name__ == '__main__':

    parser = argparse.ArgumentParser (description = 'do iMCP TB analysis')
    parser.add_argument ('-s', '--scanLabel' , default = 'Scan1' , help='Scan to be analized')
    parser.add_argument ('-t', '--thCut' , default = 'SB', help='threshold type: SB->S/B=1000, 3s->3sigma_base, 5s->5sigma_base')
    parser.add_argument ('-o', '--hodoCut' , default = 'no', help='hodoscope selection: no->no hodo cut, all->hit everywhere, tight->hit in the central 4 fibres')
    parser.add_argument ('-m', '--testMCP' , default = 'MiB2', help='test MCP')
    parser.add_argument ('-a', '--analysis' , default = "eff", help='select analysis: eff->efficiency, Q->charge')

    args = parser.parse_args ()

    print './doAnalysis '+args.thCut+' '+args.hodoCut+' '+args.testMCP+' '+args.analysis+' '+args.scanLabel
    os.system('./doAnalysis '+args.thCut+' '+args.hodoCut+' '+args.testMCP+' '+args.analysis+' '+args.scanLabel)


