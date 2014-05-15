#
#  Copyright 2014 ARM Limited
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of ARM Limited nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

'''change the copy right year
there are 4 cases
 1 2014 => 2014
 2 20xx => 20xx-14
 3 20xx-bb => 20xx-14
 4 20xx-14 => 20xx-14
'''
import re, os, sys
import fileinput
import argparse

def extend_copyright(line, year):
    '''year is the year which you want to extend to'''

    #match the format like 'Copyright 2014 ARM Limited' or 'Copyright 2011-14 ARM Limited'
    p2014 = re.compile(r'.*{}.*'.format(year))
    if p2014.match(line):
        return line

    #match the format like 'Copyright 2011-12 ARM Limited'
    p20xx_bb = re.compile(r'(.*)(20\d\d)(-)(\d\d)(.*)')
    m = p20xx_bb.match(line)
    if m:
        return p20xx_bb.sub(r'\g<1>\g<2>\g<3>{}\g<5>'.format(year), line)

    #match the format like 'Copyright 2012 ARM Limited'
    p20xx = re.compile(r'(.*)(20\d\d)(.*)')
    m = p20xx.match(line)
    if m:
        return p20xx.sub(r'\g<1>\g<2>-{}\g<3>'.format(year), line)

def replace_line(file,search_exp,replace_exp):
    for line in fileinput.input(file, inplace=1):
        if search_exp in line:
            line = line.replace(search_exp, replace_exp)
        sys.stdout.write(line)

def test():
    year = '14'
    if extend_copyright('Copyright 2011-12 ARM Limited', year) != 'Copyright 2011-14 ARM Limited':
        print "test failed"
        return

    if extend_copyright('Copyright 2013-14 ARM Limited', year) != 'Copyright 2013-14 ARM Limited':
        print "test failed"
        return

    if extend_copyright('Copyright 2012 ARM Limited', year) != 'Copyright 2012-14 ARM Limited':
        print "test failed"
        return

    if extend_copyright('Copyright 2014 ARM Limited', year) != 'Copyright 2014 ARM Limited':
        print "test failed"
        return

    print "test success."


def extend_copyright_all(extend_to_year):
    all_files = []
    for root, dirs, files in os.walk(os.getcwd()):
        for f in files:
            #exclude this script file
            if f != os.path.basename(sys.argv[0]):
                all_files.append(os.path.join(root, f))

    pcopy_right = re.compile(r'.*Copyright [0-9-]* ARM Limited.*')
    for f in all_files:
        fd = open(f, 'r')
        for line in fd.readlines():
            m = pcopy_right.match(line)
            if m:
                old_line = m.group(0)
                new_line = extend_copyright(old_line, extend_to_year)
                fd.close()
                replace_line(f, old_line, new_line)

def main():
    parser = argparse.ArgumentParser(description='Extend copyright year to the year you specified.')
    parser.add_argument('year', nargs='?', help='year you want to extend, only 2 digitals, e.g.\'14\'')
    parser.add_argument('-t', '--test', action='store_true', help='run the test')
    args = parser.parse_args()
    if args.test:
        test()
        return
    else:
        #check input year includes 2 digitals
        pdigital2 = re.compile(r'^\d\d$')
        if args.year and pdigital2.search(args.year):
            extend_copyright_all(args.year)
        else:
            parser.print_help()

if __name__ == '__main__':
    main()
