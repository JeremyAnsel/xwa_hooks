using Microsoft.Win32;
using System.Globalization;

namespace XwaJoystickConfig;

internal class JoystickDevicesWin32 : IJoystickDevices
{
    public void Release()
    {
    }

    public int GetNumDevs()
    {
        return NativeMethods.JoyGetNumDevs();
    }

    public bool GetCaps(int deviceIndex, out JoyCaps caps)
    {
        return NativeMethods.JoyGetDevCaps(new IntPtr(deviceIndex), out caps) == 0;
    }

    public string GetDeviceName(int deviceIndex)
    {
        if (!GetCaps(deviceIndex, out JoyCaps caps))
        {
            return "Unknown";
        }

        string name = this.ReadName(deviceIndex, caps);
        return string.IsNullOrEmpty(name) ? caps.ProductName : name;
    }

    private string ReadName(int deviceIndex, in JoyCaps caps)
    {
        string currentJoystickSettingsKey = $"HKEY_CURRENT_USER\\System\\CurrentControlSet\\Control\\MediaResources\\Joystick\\{caps.RegistryKey}\\CurrentJoystickSettings";
        string currentJoystickSettingsValue = $"Joystick{(deviceIndex + 1).ToString(CultureInfo.InvariantCulture)}OEMName";
        string oemName = (string)Registry.GetValue(currentJoystickSettingsKey, currentJoystickSettingsValue, string.Empty)!;

        if (string.IsNullOrEmpty(oemName))
        {
            return string.Empty;
        }

        string joystickPropertiesKey = "HKEY_CURRENT_USER\\System\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\" + oemName;
        string joystickPropertiesValue = "OEMName";
        string name = (string)Registry.GetValue(joystickPropertiesKey, joystickPropertiesValue, string.Empty)!;

        if (string.IsNullOrEmpty(name))
        {
            return string.Empty;
        }

        return name;
    }

    public unsafe bool GetDevicePosition(int deviceIndex, out JoyInfoEx info)
    {
        info = new JoyInfoEx
        {
            dwSize = (uint)sizeof(JoyInfoEx),
            dwOptions = JoyInfoExOptions.ReturnAll | JoyInfoExOptions.ReturnCentered | JoyInfoExOptions.UseDeadZone
        };

        return NativeMethods.JoyGetPosEx(deviceIndex, ref info) == 0;
    }

    public bool IsButtonPressed(int deviceIndex, int buttonIndex)
    {
        if (buttonIndex < 0 || buttonIndex >= 32)
        {
            return false;
        }

        if (!GetDevicePosition(deviceIndex, out JoyInfoEx info))
        {
            return false;
        }

        if ((info.dwButtons & (1U << buttonIndex)) != 0)
        {
            return true;
        }

        return false;
    }
}
