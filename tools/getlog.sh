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
# NE10 Library : getlog.sh
#
#!/bin/bash

echo "NE10 NIGHTLY BUILD SCRIPT"
echo "(C) 2011, ARM Ltd."
date

echo
echo
echo -e "\033[4mSYSTEM:\033[0m"
uname -a
cat /proc/cpuinfo

echo
echo
echo -e "\033[4mINSTALLED TOOLS:\033[0m"
echo "git:"
if [ "`which git`" = "" ]; then
 echo "fatal: 'git' is not installed on this system" 1>&2
 exit 1
fi
git --version | paste -s -d ';' -
echo
echo "gcc:"
if [ "`which gcc`" = "" ]; then
 echo "fatal: 'gcc' is not installed on this system" 1>&2
 exit 1
fi
gcc --version | paste -s -d ';' -
echo
echo "as:"
if [ "`which as`" = "" ]; then
 echo "fatal: 'as' is not installed on this system" 1>&2
 exit 1
fi
as --version | paste -s -d ';' -
echo
echo "ar:"
if [ "`which ar`" = "" ]; then
 echo "fatal: 'ar' is not installed on this system" 1>&2
 exit 1
fi
ar --version | paste -s -d ';' -
echo
echo
echo "perl:"
if [ "`which perl`" = "" ]; then
 echo "fatal: 'perl' is not installed on this system" 1>&2
 exit 1
fi
perl --version | paste -s -d ';' -

echo
if [ -e .git ]; then
 echo
 echo -e "\033[4mCURRENT 'git' CONFIGURATION:\033[0m"
 git config -l
fi

echo
echo
echo -e "\033[4mCURRENT USER AND PATH:\033[0m"
echo `whoami` "@" `pwd`

echo
echo
echo -e "\033[4mENVIRONMENT VARIABLES:\033[0m"
echo
echo "PATH = " $PATH
echo
echo "LD_LIBRARY_PATH = " $LD_LIBRARY_PATH


echo
if [ -e .git ]; then
echo
echo -e "\033[4mCURRENT GIT/SOURCE STATUS:\033[0m"
  git show
fi


