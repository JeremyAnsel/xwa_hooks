namespace XwaJoystickConfig;

internal interface IJoystickDevices
{
    void Release();

    int GetNumDevs();

    bool GetCaps(int deviceIndex, out JoyCaps caps);

    string GetDeviceName(int deviceIndex);

    bool GetDevicePosition(int deviceIndex, out JoyInfoEx info);

    bool IsButtonPressed(int deviceIndex, int buttonIndex);
}
