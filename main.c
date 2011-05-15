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
    CFUserNotificationRef notification = CFUserNotificationCreate
        (NULL, 0, flags, &error, dictionary);
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
