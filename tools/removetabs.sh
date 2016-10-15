#
#  Copyright 2011-15 ARM Limited and Contributors.
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
#  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
# NE10 Library : removetabs.sh
#
#!/bin/bash

# This script removes tab characters in files and replaces them with
#  the right number of spaces. It also removes trailing whitespaces.

# remove trailing whitespaces
LSw=`grep -lsri --exclude="Makefile" --exclude-dir=".git" '\s$' .`;
for flw in $LSw
do
    echo "HAS SPACES: " $flw; # just to see a list of the files that include unwanted tabs
    perms=`stat -c '%a' $flw`;
    sed 's/[ \t]*$//gi' $flw > .exp.tmp;
    sync;
    # rename the file to the original file
    mv .exp.tmp $flw;
    chmod $perms $flw;
    sync;
done

# remove tabs
chtab=$'\t'; # only works in bash but not in sh
LSt=`grep -lrsi --exclude="Makefile" --exclude-dir=".git" "$chtab" .`;
for flt in $LSt
do
    echo "HAS TABS: " $flt; # just to see a list of the files that include unwanted tabs
    perms=`stat -c '%a' $flt`;
    # remove tabs
    expand $flt > .exp.tmp;
    sync;
    # rename the file to the original file
    mv .exp.tmp $flt;
    chmod $perms $flt;
    sync;
done

