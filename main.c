// main.c
// Copyright 2011 Wincent Colaiuta. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <CoreFoundation/CoreFoundation.h>

#define PASSWORD_MAX_LEN 1024

int main (int argc, const char * argv[]) {
    SInt32 error;
    CFOptionFlags flags = CFUserNotificationSecureTextField(0) | kCFUserNotificationPlainAlertLevel;
    const void *keys[] = {
        kCFUserNotificationAlertHeaderKey,
        kCFUserNotificationTextFieldTitlesKey,
        kCFUserNotificationDefaultButtonTitleKey,
        kCFUserNotificationAlternateButtonTitleKey
    };
    const void *values[] = {
        CFSTR("Enter your password"),
        CFSTR("Password:"),
        CFSTR("Continue"),
        CFSTR("Cancel"),
    };
    CFDictionaryRef dictionary = CFDictionaryCreate(NULL, keys, values, sizeof(keys)/sizeof(*keys), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFUserNotificationRef notification = CFUserNotificationCreate(NULL, 0, flags, &error, dictionary);
    CFOptionFlags responseFlags;
    SInt32 response = CFUserNotificationReceiveResponse(notification, 0, &responseFlags);
    if (response == 0 && (responseFlags & 0x3) != kCFUserNotificationAlternateResponse)
    {
        CFStringRef password = CFUserNotificationGetResponseValue(notification, kCFUserNotificationTextFieldValuesKey, 0);
        char *buffer = malloc(PASSWORD_MAX_LEN);
        if (buffer) {
            if (CFStringGetCString(password, buffer, PASSWORD_MAX_LEN, kCFStringEncodingUTF8))
                fprintf(stdout, "%s\n", buffer);
            free(buffer);
        }
    }
    CFRelease(notification);
    CFRelease(dictionary);
    return EXIT_SUCCESS;
}
