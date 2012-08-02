#!/bin/sh
#
#  Copyright 2011-12 ARM Limited
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

#
# NE10 Library : astyle.sh
#
#!/bin/bash

# This script is used to format the source code
#################################################

if [ $# -ge 1 ]; then
    CMD_OPS=$*
else
    echo "too less param..."
    echo " eg: ${0} a.c"
    echo "-r: recursive process subdirectories."
    echo " eg: ${0} -r \"\${DIR}/\\*.cpp \${DIR}/\\*.h\""
    exit
fi

FORMAT_OPS="--style=ansi --options=none  -p -d -U -c"

astyle ${FORMAT_OPS} ${CMD_OPS}
