#include <macros>

namespace Security
{
    enum Permission
    {
        /* Device */
        ListDevice,
        ReadDeviceProperty,
        GetDeviceInfo,
        
        /* Network */
        ListConnectedNetwork,
        ReadNetworkProperty,
        ChangeNetworkState,
        ConnectToNearbyDevice,

        /* Account */
        ListUserAccount,
        ModifyAccountProperty,
        CreateAccount,
        DeleteAccount,

        /* Bluetooth */
        CheckBluetoothState,
        ChangeBluetoothState,
        PairBluetoothDevice,

        /* Message */
        // Allow app receiving event about app message,
        // app can use this event to read message and make response.
        AccessAppMessageEvent,
        ReadSMS,
        SendSMS,
        MakePhoneCall,

        /* Storage */
        AccessAllProgramFolder,
        AccessMediaLibrary,
        AccessDocumentLibrary,
        AccessAllFileLibrary,
        AccessAllFiles,
        AccessAllFilesOnRemote,

        /* App */
        GetActivityList,
        GetInstalledAppList,
        GetInstalledComponentList,
        SendInstallRequest,

        // Settings
        ReadSystemSettings,
        WriteSystemSettings,
        ReadEnvironmentVariable,
        WriteEnvironmentVariable,

        // Multimedia
        UseCamara,
        UseMicrophone,
        ChangeVolume,
        ChangeVolumeMixer,

        // Misc
        UseBodySensors,
        UseClipboard,

        // Notification
        SendToastNotification,
        SendPernamentNotification,
        NotifyOnAppBadge,
        NotifyWhenScreenLocked,
        NotifyWhenFullscreen,
    }
}