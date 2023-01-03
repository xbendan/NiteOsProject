enum DeviceType
{
    DeviceTypeAudio,
    DeviceTypeBattery,
    DeviceTypeBluetooth,
    DeviceTypeFsDriveDevice,
    DeviceTypeFsDriveController,
    DeviceTypeDisplayAdapter,
    DeviceTypeDisplayMonitor,
    DeviceTypeUSBDevice,
    DeviceTypeUSBController,
    DeviceTypePCIDevice,
    DeviceTypePCIController,
    DeviceTypeNetworkAdapter,
    DeviceTypeSecurity,
    DeviceTypeProcessor,
    DeviceTypePrinter,
    DeviceTypeSystemDevices
};

struct Device
{
    char *m_Name;                   /* Name of the device */
    enum DeviceType m_Type;         /* Device Type*/
    struct Device *m_Dependence;    /* Device that this device depends on */
};

struct Device *GetDevice(const char *str);
