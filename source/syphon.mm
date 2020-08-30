#include "syphon.h"
#include<GL/glew.h>
#ifdef SYPHON_SERVER
#import <Syphon/Syphon.h>
#import<AGL/agl.h>
#include<iostream>
#include<Foundation/Foundation.h>


@interface NSImage (NSImage_OpenCV) {
}
@property(nonatomic, readonly) char* CVMat;
@end

SyphonServer *_server = nil;
CGFloat ImageWidth = 1280, ImageHeight = 720;

void syphon_size(float w, float h) {
    ImageWidth = w;
    ImageHeight = h;
}

void syphon_start() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(_server != nil) {
        [_server stop];
        [_server release];
        _server = nil;
    }
    [pool drain];
}

void syphon_stop() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(_server != nil) {
        [_server stop];
        [_server release];
    }
    _server = nil;
    [pool drain];
}

void syphon_exit() {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if(_server != nil) {
        [_server stop];
        [_server release];
    }
    _server = nil;
    [pool drain];
}

void syphon_bind(float w, float h) {
     
    NSSize sz;
    sz.width = w;
    sz.height = h;
    
    if(_server)
        [_server bindToDrawFrameOfSize:sz];
}
int syphon_pushTexture(int tex) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    if (_server == nil)
    {
        CGLContextObj cgl_ctx = CGLGetCurrentContext();
        _server = [[SyphonServer alloc] initWithName:@"acidcamGL" context:cgl_ctx options:nil];
    }
    [_server unbindAndPublish];
    SyphonImage *image = [_server newFrameImage];
    int val = image.textureName;
    [pool drain];
    return val;
}

NSInteger _NSRunAlertPanel(NSString *msg1, NSString *msg2, NSString *button1, NSString *button2, NSString *button3) {
    NSAlert *alert = [[NSAlert alloc] init];
    if(button1 != nil) [alert addButtonWithTitle:button1];
    if(button2 != nil) [alert addButtonWithTitle:button2];
    if(msg1 != nil) [alert setMessageText:msg1];
    if(msg2 != nil) [alert setInformativeText:msg2];
    NSInteger rt_val = [alert runModal];
    [alert release];
    return rt_val;
}

void messageOutput(std::string title, std::string text) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString *s1 = [NSString stringWithUTF8String:title.c_str()];
    NSString *s2 = [NSString stringWithUTF8String:text.c_str()];
    _NSRunAlertPanel(s1, s2, @"Ok", nil, nil);
    [pool drain];
}


#endif


