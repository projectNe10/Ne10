#!/usr/bin/python
'''
  Copyright 2013 ARM Limited
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.'''

'''convert GAS arm assembly to Clang's integrated-as'''
import re,sys,os

''' parse .qn .dn .req directive for arm assembly
    input is string list
'''
def parse_alias(inlines):
    #the value name doesn't include .F32 suffix, but include [0], such as d0[0]
    alias_exp = re.compile(r" *"
                        r"(?P<alias>[_a-zA-Z0-9]+)"
                        " +\.(req|qn|dn) +"
                        r"(?P<value>[a-zA-Z0-9\[\]]+)"
                           )
    global_exp = re.compile(r"\.global")
    #store aliases in different function in different dictionary
    #use .global to label different function
    alias_dics = []
    alias_dics.append({})
    func_count = 0
    for line in inlines:
        if global_exp.search(line):
            alias_dics.append({})
            func_count += 1
        result = alias_exp.search(line)
        if result != None:
            alias = result.group('alias')
            value = result.group('value')
            alias_dics[func_count][alias] = value
    #print alias_dics

    #replace alias
    func_count = 0
    line_result1 = []
    for line in inlines:
        if global_exp.search(line):
            func_count += 1
        for alias in alias_dics[func_count]:
            alias_str = "\\b"+alias+"\\b"
            alias_exp = re.compile(alias_str)
            line = alias_exp.sub(alias_dics[func_count][alias], line)
        line_result1.append(line)

    #remove .qn .dn .req line
    line_result2 = []
    alias_exp = re.compile(r"\.(qn|dn|req|unreq)")
    for line in line_result1:
        if not alias_exp.search(line):
            line_result2.append(line)

    return line_result2

''' parse .qn .dn .req directive for arm assembly
    input is string list.
    this is for fft module's new format: in the new format,
    all aliases are defined at the begin of files and all functions
    share one set of defines.
'''
def parse_alias_fft(inlines):
    #the value name doesn't include .F32 suffix, but include [0], such as d0[0]
    alias_exp = re.compile(r" *"
                        r"(?P<alias>[_a-zA-Z0-9]+)"
                        " +\.(req|qn|dn) +"
                        r"(?P<value>[a-zA-Z0-9\[\]]+)"
                           )
    aliases = {}
    for line in inlines:
        result = alias_exp.search(line)
        if result != None:
            alias = result.group('alias')
            value = result.group('value')
            aliases[alias] = value
    #print aliases

    #replace alias
    line_result1 = []
    for line in inlines:
        for alias in aliases:
            alias_str = "\\b"+alias+"\\b"
            alias_exp = re.compile(alias_str)
            line = alias_exp.sub(aliases[alias], line)
        line_result1.append(line)

    #remove .qn .dn .req line
    line_result2 = []
    alias_exp = re.compile(r"\.(qn|dn|req|unreq)")
    for line in line_result1:
        if not alias_exp.search(line):
            line_result2.append(line)

    return line_result2

'''add .F32 to some instructions for Clang's as doesn't support register with
 datatype, such as VADD Q0.F32, Q1.F32, Q2.F32'''
def add_f32(inlines):
    instructions = ['VADD', 'VSUB', 'VLD1', 'VLD2', 'VMUL', 'VMLA', 'VMLS',
                    'VZIP', 'VST1', 'VST2', 'VPADD', 'VEXT', 'VREV64',
                    'VDUP', 'VMOV']
    line_result = []
    for line in inlines:
        for instruction in instructions:
            instruction_exp = re.compile(instruction)
            instruction_f32_exp = re.compile(instruction + "\\.[Ff]32")
            #only add .F32 to instruction without .F32(or .f32)
            if instruction_exp.search(line) and \
            not instruction_f32_exp.search(line):
                line = instruction_exp.sub(instruction + ".F32", line)
        line_result.append(line)
    return line_result

'''remove .end instruction'''
def remove_end(inlines):
    end_exp = re.compile(r"\.end\b")
    line_result = []
    for line in inlines:
        if not end_exp.search(line):
            line_result.append(line)
    return line_result

'''expan ldr rx, =label for Clang, this method has *not* been used in current
assembly convertion, Clang doesn't support this pseudo-instruction'''
def expan_ldr1(inlines):
    #search ldr rx, =label
    labels = []
    ldr_exp = re.compile(r"\s*LDR.*="
                         r"(?P<label>[_a-zA-Z0-9]+)")
    for line in inlines:
        result = ldr_exp.search(line)
        if result != None:
            label = result.group('label')
            if label not in labels:
                labels.append(label)
    #print labels

    if labels:
        #change ldr rx, =label to ldr rx, Llabel
        lines_result1 = []
        for line in inlines:
            result_line = line
            for label in labels:
                ldr_exp = re.compile("=" + label)
                if ldr_exp.search(line):
                    result_line = ldr_exp.sub("L" + label, line)
                    break
            lines_result1.append(result_line)

        #append a .text section to file and also add local label
        lines_result1.append("        .text\n")
        lines_result1.append("        .align 2\n")
        for label in labels:
            lines_result1.append("L" + label + ":\n")
            lines_result1.append("       .long " + label + "\n")

        #remove .extern directive which is only used in GNU as
        lines_result2 = []
        for line in lines_result1:
            del_line = False
            for label in labels:
                extern_exp = re.compile(r"\s*\.extern\s*" + label)
                if extern_exp.search(line):
                    del_line = True
            if not del_line:
                lines_result2.append(line)
        return lines_result2
    return inlines

'''expan ldr rx, =label for Clang, Clang doesn't support this
pseudo-instruction'''
def expan_ldr2(inlines):
    #search "ldr rx, =label" line, save the label
    labels = []
    ldr_exp = re.compile(r"\s*(LDR|ldr)\s*(?P<reg>[a-zA-Z0-9]+),\s*="
                         r"(?P<label>[_a-zA-Z0-9]+)")
    for line in inlines:
        result = ldr_exp.search(line)
        if result != None:
            label = result.group('label')
            if label not in labels:
                labels.append(label)
    #print labels

    if labels:
        #change ldr rx, =label to ldr rx, Llabel
        lines_result1 = []
        count = 0
        for line in inlines:
            result_line = line
            for label in labels:
                result = ldr_exp.search(line)
                if result:
                    reg = result.group('reg')
                    result_line = "                            MOVW         "\
                        + reg + ",:lower16:(L" + label +\
                        "$non_lazy_ptr - (LPC0_" + str(count) + " + 4))\n" +\
                        "                            MOVT         " + reg +\
                        ",:upper16:(L" + label + "$non_lazy_ptr - (LPC0_" +\
                        str(count) + " + 4))\n" + "LPC0_" + str(count) + ":\n"\
                        + "                            ADD         " + reg + \
                        ",PC\n" + "                            LDR         " +\
                        reg + ",[" + reg + "]\n"
                    count += 1
                    break
            lines_result1.append(result_line)

        #append a .text section to file and also add local label
        lines_result1.append("        .section        __DATA,__nl_symbol_ptr,\
        non_lazy_symbol_pointers\n")
        lines_result1.append("        .align 2\n")
        for label in labels:
            lines_result1.append("L" + label + "$non_lazy_ptr:\n")
            lines_result1.append("        .indirect_symbol " + label + "\n")
            lines_result1.append("        .long 0\n")
        lines_result1.append("\n        .subsections_via_symbols\n")

        #remove .extern directive which is only used in GNU as
        lines_result2 = []
        for line in lines_result1:
            del_line = False
            for label in labels:
                extern_exp = re.compile(r"\s*\.extern\s*" + label)
                if extern_exp.search(line):
                    del_line = True
            if not del_line:
                lines_result2.append(line)
        return lines_result2
    return inlines

'''add prefix 'L' to local label '''
def add_Llabel(inlines):
    #get global label
    glabels = []
    for line in inlines:
        glabel_exp = re.compile(r"\s*\.globa?l\s*"
                                r"(?P<glabel>\b[_a-zA-Z0-9]+\b)")
        result = glabel_exp.search(line)
        if result:
            glabels.append(result.group('glabel'))
    #print glabels

    #get non-leading L local label
    llabels = []
    for line in inlines:
        llabel_exp = re.compile(r"(?P<llabel>^[._a-zA-Z0-9]+):")
        result = llabel_exp.search(line)
        if result:
            llabel = result.group('llabel')
            leading_L_exp = re.compile(r"^L.*")
            if llabel not in glabels and not leading_L_exp.search(llabel):
                llabels.append(llabel)

    #print llabels
    #add leading L to local label
    result_lines = []
    for line in inlines:
        result_line = line
        for llabel in llabels:
            new_llabel = llabel.replace('.L','')
            #apply different change to label with .L or not
            if new_llabel != llabel:
                llabel_exp = re.compile(llabel.replace('.','\\.'))
            else:
                llabel_exp = re.compile("\\b" + llabel + "\\b")
            #print "\\b" + re.escape(llabel) + "\\b"
            if llabel_exp.search(line):
                result_line = llabel_exp.sub("L" + new_llabel, line)
                break
        result_lines.append(result_line)

    return result_lines

def main ():
    fft_file_names = ['NE10_fft_float32.neon.s',
                      'NE10_fft_int16.neon.s',
                      'NE10_fft_int32.neon.s']
    if len(sys.argv) < 3:
        print "Usage: convert.py <input file> <output file>"
        return
    else:
        infilename = sys.argv[1]
        outfilename = sys.argv[2]
        infile = open(infilename, 'r')
        lines = []
        for line in infile.readlines():
            lines.append(line)
        infile.close()

        #the fft file processing is hard coded.
        if os.path.basename(infilename) in fft_file_names:
            result = parse_alias_fft(lines)
        else:
            result = parse_alias(lines)
        result = add_f32(result)
        result = remove_end(result)
        result = add_Llabel(result)
        result = expan_ldr2(result)

        outfile = open(outfilename, 'w')
        for line in result:
            outfile.write(line)
        outfile.close()

main()
