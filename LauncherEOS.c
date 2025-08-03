#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eos_sdk.h"
#include "eos_platform.h"
#include "eos_auth.h"

void LaunchGameExecutable(const char* path) {
    printf("Launching game: %s\n", path);
    system(path); // Replace with ShellExecute or CreateProcess on Windows if needed
}

void OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data) {
    if (Data->ResultCode == EOS_Success) {
        printf("✅ Login successful!\n");
        LaunchGameExecutable("YourGameExecutable.exe");
    } else {
        printf("❌ Login failed: %d\n", Data->ResultCode);
    }
}

int main() {
    EOS_InitializeOptions initOptions = { 0 };
    initOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
    initOptions.ProductName = "YourGameName";
    initOptions.ProductVersion = "1.0";

    EOS_EResult initResult = EOS_Initialize(&initOptions);
    if (initResult != EOS_Success) {
        printf("EOS Initialization failed: %d\n", initResult);
        return 1;
    }

    EOS_Platform_Options platformOptions = { 0 };
    platformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
    platformOptions.ProductId = "your_product_id";
    platformOptions.SandboxId = "your_sandbox_id";
    platformOptions.DeploymentId = "your_deployment_id";
    platformOptions.ClientCredentials.ClientId = "your_client_id";
    platformOptions.ClientCredentials.ClientSecret = "your_client_secret";

    EOS_HPlatform platformHandle = EOS_Platform_Create(&platformOptions);
    if (!platformHandle) {
        printf("Failed to create EOS platform.\n");
        return 1;
    }

    EOS_Auth_Credentials credentials = { 0 };
    credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
    credentials.Type = EOS_LCT_DeviceCode;

    EOS_Auth_LoginOptions loginOptions = { 0 };
    loginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
    loginOptions.Credentials = &credentials;

    EOS_Auth_Login(platformHandle, &loginOptions, NULL, OnLoginComplete);

    // Keep the app alive to wait for callback
    printf("🔄 Waiting for login...\n");
    while (1) {
        EOS_Platform_Tick(platformHandle);
        Sleep(100); // Windows only; use usleep on POSIX
    }

    EOS_Shutdown();
    return 0;
}