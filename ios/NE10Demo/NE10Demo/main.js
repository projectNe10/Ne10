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
var barColors = ["green", "blue", "brown", "red", "cyan", "balck"];

var case_count = 0;
var acceleration_rate = 0;

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

//add result of benchmark
function add_result(acceleration_rate, case_count) {
    var sum_acc_rate = acceleration_rate / case_count;
    var sum_acc_rate_display = Math.floor(sum_acc_rate * 100);

    //result bar for Ne10
    $("#result_bar_ne10").html("Ne10 version  " + sum_acc_rate_display + "%");
    $("#result_bar_ne10").css("visibility", "visible");

    //result bar for C
    $("#result_bar_c").css("height", 100/sum_acc_rate + "%");
    $("#result_bar_c").html("C version  " + "100%");
}

//this function is called by objective-c code
//see more details at:
//http://tetontech.wordpress.com/2008/08/14/
//calling-objective-c-from-javascript-in-an-iphone-uiwebview/
function returnFromObjectivec(ne10TestResultStr)
{
    var progress = 0;
    var progress_display = 0;

    if (ne10TestResultStr) {
        //parse json data from string
        var testResult = eval( '(' + ne10TestResultStr + ')' );
        for (i = 0; i < testResult.length; ++i) {
            ++case_count;
            testResult[i].name = case_count.toString() + " "
            + testResult[i].name;
            addShowBarBlock(testResult[i].name,
                            testResult[i].time_c,
                            testResult[i].time_neon);
            acceleration_rate = acceleration_rate +
            testResult[i].time_c / testResult[i].time_neon;
        }

        progress = case_count/117*100;
        progress_display = Math.floor(progress);
        var font_height = $("#progress_bar_in").height()*0.7;

        $("#progress_bar_in").css("width", progress + "%");
        $("#progress_bar_in").css("font-size",font_height + "px");
        $("#progress_bar_in").html(progress_display + "%");

        //TODO
        //Currently, 117 is number of total test cases.
        //due to the limit of current test framework, we can't
        //get the number of all test cases at compiling time.
        if( case_count == 117 )
            add_result(acceleration_rate, case_count);
    }
}

function run_cases()
{
    //trig a object-c call, in which would call ne10's performance test function
    window.location = "ne10://";
    setTimeout("run_cases()", 10);
}

function start() {
    setTimeout("run_cases()", 10);
}
