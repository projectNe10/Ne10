/*
 *  Copyright 2013 ARM Limited
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// <settings
// for color names, reference to
// http://www.w3schools.com/html/html_colornames.asp
var barColors = ["green", "blue", "brown", "red", "cyan"];

var caseCount = 0;

function debug_log(message){
    document.getElementById('debug_message').innerHTML += message + "<br>";
}

// Add and show for a feature block
// argv[1:] be the bars
function addShowBarBlock(title) {
    var i;
    var max = 0;
    var eBlock;
    var eBar;

    for (i = 1; i < arguments.length; ++i) {
        if (max < arguments[i]) {
            max = arguments[i];
        }
    }
    max *= 1.2; // this is for right padding

    // block is the container
    eBlock = $("<div class='block'/>");

    $("<h3/>").html(title).appendTo(eBlock);
    for (i = 1; i < arguments.length; ++ i) {
        eBar = $("<div class='bar'/>");
        eBar.css("width", 100 * arguments[i] / max + "%" );
        eBar.css("background-color", barColors[i - 1]);
        eBar.css("box-shadow", "5px 5px 15px " + barColors[i - 1]);
        eBar.html(arguments[i]);

        eBar.appendTo(eBlock);
    }

    eBlock.appendTo($("#content"));
}

//this function is called by objective-c code
//see more details at:
//http://tetontech.wordpress.com/2008/08/14/
//calling-objective-c-from-javascript-in-an-iphone-uiwebview/
function returnFromObjectivec(ne10TestResultStr)
{
    if (ne10TestResultStr) {
        //parse json data from string
        var testResult = eval( '(' + ne10TestResultStr + ')' );
        for (i = 0; i < testResult.length; ++i) {
            ++caseCount;
            testResult[i].name = caseCount.toString() + " "
                                + testResult[i].name;
            addShowBarBlock(testResult[i].name,
                            testResult[i].time_c,
                            testResult[i].time_neon);
        }
        $("#progress").html(caseCount);
    }
}

function run_cases()
{
    //trig a object-c call, in which would call ne10's performance test function
    window.location = "ne10://";
    setTimeout("run_cases()", 10);
}

function start() {
    var el;

    el = $("<div/>");
    el.css("width", "150px");
    el.css("height", "24px");
    el.css("background-color", barColors[0]);
    el.html("C version(us)");
    el.appendTo("#head");

    el = $("<div/>");
    el.css("width", "150px");
    el.css("height", "24px");
    el.css("background-color", barColors[1]);
    el.html("NEON version(us)");
    el.appendTo("#head");

    setTimeout("run_cases()", 10);
}
