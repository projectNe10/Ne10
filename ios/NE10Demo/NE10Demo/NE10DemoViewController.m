/*
 *  Copyright 2013-14 ARM Limited and Contributors.
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
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "NE10DemoViewController.h"
#import "test_funcs.h"

@interface NE10DemoViewController ()

- (void)runNe10TestThreadMain;
@end

@implementation NE10DemoViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self->lock = [[NSLock alloc] init];
    self->ne10Finished = TRUE;
    self->ne10Result = NULL;

    self.webView.delegate = self;
    NSString *htmlFile = [[NSBundle mainBundle] pathForResource:@"demo" ofType:@"html" inDirectory:nil];
    NSURL *url = [NSURL fileURLWithPath:htmlFile];
    NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];

    [self.webView loadRequest:requestObj];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

// This function is called on all location change,
// here we use this to communicate with javascript
- (BOOL)webView:(UIWebView *)webView
        shouldStartLoadWithRequest:(NSURLRequest *)request
        navigationType:(UIWebViewNavigationType)navigationType {
    NSURL *url = request.URL;
    if ([[url scheme] isEqualToString:@"ne10"]) {
        if (self->ne10Finished) {
            //our ne10 test return "(null)" when all tests finished
            if (self->ne10Result != NULL && ![self->ne10Result isEqual: @"\"(null)\""]) {
                NSString *jsFunction = [NSString stringWithFormat:@"returnFromObjectivec(%@)", self->ne10Result];
                [self.webView stringByEvaluatingJavaScriptFromString:jsFunction];
            }
            self->ne10Finished = FALSE;
            NSThread* ne10TestThread = [[NSThread alloc] initWithTarget:self
                                                               selector:@selector(runNe10TestThreadMain)
                                                                 object:nil];
            [ne10TestThread start];  // Actually create the thread
        }
        return NO;
    }
    // Accept this location change, it makes no sense here
    return YES;
}

- (void)runNe10TestThreadMain {
    //call ne10 performance test function
    self->ne10Result = [NSString stringWithFormat:@"%s", NE10RunTest()];
    //escape quote in string
    self->ne10Result = [self->ne10Result stringByReplacingOccurrencesOfString:@"\"" withString:@"\\\""];
    self->ne10Result = [NSString stringWithFormat:@"\"%@\"",self->ne10Result];
    self->ne10Finished = TRUE;
}

@end
