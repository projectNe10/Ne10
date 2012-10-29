#
#  Copyright 2011-12 ARM Limited
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

#
# NE10 Library : review.sh
#
#!/bin/sh

BRANCH=$1

BASE=${2-"master"}

if [ "$BRANCH" = "" ]; then
  echo "Usage: review.sh <branch to review> [parent branch]"
  exit
else

  LABEL=`echo $1 | perl -pe '$_ =~ /dev\/([a-zA-Z0-9]+)\/(.+)/;$_=$2'`
  GLUSER=`echo $1 | perl -pe '$_ =~ /dev\/([a-zA-Z0-9]+)\/(.+)/;$_=$1'`

  NEWBRANCH="staging/$GLUSER/$LABEL"

  echo "Pushing $BRANCH from $BASE for review as $NEWBRANCH"

  git branch $NEWBRANCH $BASE
  git push origin $NEWBRANCH
  git checkout $NEWBRANCH
  git rebase $BRANCH
  git push origin $NEWBRANCH

fi

