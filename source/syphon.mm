#include "syphon.h"
#include<GL/glew.h>

#ifdef SYPHON_SERVER
#import <Syphon/Syphon.h>
#import<AGL/agl.h>
#include<iostream>

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

#endif


