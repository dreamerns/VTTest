//
//  main.cpp
//  VTTest - Test VideoToolbox Compression Session creation with various params
//
//  Created by Nikola Novakovic on 5/27/21.
//
#include <VideoToolbox/VideoToolbox.h>
#include <iostream>

void compression_clb(void* encoder_opaque,
                     void* request_opaque,
                     OSStatus status,
                     VTEncodeInfoFlags info,
                     CMSampleBufferRef sbuf) {}

bool initialize_vt(int32_t width, int32_t height, bool require_hw_enc) {
    // set encoding params
    CFStringRef keys[] = {kVTVideoEncoderSpecification_RequireHardwareAcceleratedVideoEncoder};
    CFTypeRef values[] = {kCFBooleanTrue};
    CFDictionaryRef encoder_spec = CFDictionaryCreate(NULL, (const void **)keys, (const void **)values, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    std::cout << "Creating session width: " << width << " height: " << height << " require hw encoding: " << std::boolalpha << require_hw_enc << "\n";
    VTCompressionSessionRef session;
    OSStatus status = VTCompressionSessionCreate(NULL, width, height, kCMVideoCodecType_H264,
                                                 require_hw_enc ? encoder_spec : NULL, NULL, NULL, &compression_clb, NULL, &session);
    if (status != noErr) {
        std::cout << " result: FAILED, error: " << status << "\n";
        return false;
    }
    std::cout << " result: SUCCESS\n";
    VTCompressionSessionInvalidate(session);
    return true;
}

int main(int argc, const char * argv[]) {
    std::cout << "Test VideoToolbox Compression Session creation\n";
    for (bool hw_enc : {false, true}) {
        initialize_vt(1280, 720, hw_enc);
        initialize_vt(640, 480, hw_enc);
        initialize_vt(640, 360, hw_enc);
        initialize_vt(320, 240, hw_enc);
        initialize_vt(320, 180, hw_enc);
    }
    std::cout << "The end\n";
    return 0;
}
