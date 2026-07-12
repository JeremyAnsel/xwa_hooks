using Aprillz.MewUI;
using JeremyAnsel.DirectX.DirectInput;

namespace XwaJoystickConfig;

internal class JoystickDevicesDirectInput : IJoystickDevices
{
    private readonly Window _mainWindow;
    private readonly DirectInputDeviceInfo[] _deviceInfos;
    private readonly int _devicesCount;
    private DirectInputFactory? _factory;
    private DirectInputDevice?[] _devices = [];
    private JoyCaps[] _deviceCaps = [];

    public JoystickDevicesDirectInput(Window window)
    {
        _mainWindow = window;
        _factory = new DirectInputFactory();
        _deviceInfos = _factory.EnumDevices(DirectInputDeviceEnumClasses.GameController, DirectInputDeviceEnumOptions.AttachedOnly);
        _devicesCount = _deviceInfos.Length;
        CreateDevices();
    }

    public void Release()
    {
        for (int i = 0; i < _devices.Length; i++)
        {
            _devices[i]?.Unacquire();
            _devices[i]?.Dispose();
            _devices[i] = null;
        }

        _factory?.Dispose();
        _factory = null;
    }

    private void CreateDevices()
    {
        if (_factory is null)
        {
            throw new InvalidOperationException("factory is null");
        }

        _devices = new DirectInputDevice[_devicesCount];
        _deviceCaps = new JoyCaps[_devicesCount];

        for (int deviceIndex = 0; deviceIndex < _devicesCount; deviceIndex++)
        {
            var device = _factory.CreateDevice(_deviceInfos[deviceIndex].Instance);
            _devices[deviceIndex] = device;
            device.SetDataFormatForJoystick2();
            device.SetCooperativeLevel(_mainWindow.Handle, DirectInputCooperativeLevels.Foreground | DirectInputCooperativeLevels.Exclusive);
            device.Acquire();
            InitializeCaps(deviceIndex, out _deviceCaps[deviceIndex]);
        }
    }

    public void InitializeCaps(int deviceIndex, out JoyCaps caps)
    {
        caps = new();

        var device = _devices[deviceIndex] ?? throw new InvalidOperationException("device is null");
        var deviceCaps = device.GetCapabilities();

        DirectInputDeviceObjectInfo[] objects = device.EnumObjects(DirectInputObjectDataTypes.All);

        foreach (DirectInputDeviceObjectInfo instance in objects)
        {
            if (instance.GuidType == DirectInputGuids.Button)
            {
                //Guid guid = instance.GuidType;
            }
            else if (instance.GuidType == DirectInputGuids.XAxis)
            {
                device.GetPropertyRange(DirectInputPropertyTypes.Range, DirectInputHowTypes.ById, (int)instance.DataType | (instance.DataInstance << 8), out int min, out int max);
                caps.wXmin = min;
                caps.wXmax = max;
            }
            else if (instance.GuidType == DirectInputGuids.YAxis)
            {
                device.GetPropertyRange(DirectInputPropertyTypes.Range, DirectInputHowTypes.ById, (int)instance.DataType | (instance.DataInstance << 8), out int min, out int max);
                caps.wYmin = min;
                caps.wYmax = max;
            }
            else if (instance.GuidType == DirectInputGuids.ZAxis)
            {
                device.GetPropertyRange(DirectInputPropertyTypes.Range, DirectInputHowTypes.ById, (int)instance.DataType | (instance.DataInstance << 8), out int min, out int max);
                caps.wZmin = min;
                caps.wZmax = max;
            }
            else if (instance.GuidType == DirectInputGuids.RyAxis)
            {
                device.GetPropertyRange(DirectInputPropertyTypes.Range, DirectInputHowTypes.ById, (int)instance.DataType | (instance.DataInstance << 8), out int min, out int max);
                caps.wRmin = min;
                caps.wRmax = max;
            }
            else if (instance.GuidType == DirectInputGuids.RxAxis)
            {
                device.GetPropertyRange(DirectInputPropertyTypes.Range, DirectInputHowTypes.ById, (int)instance.DataType | (instance.DataInstance << 8), out int min, out int max);
                caps.wUmin = min;
                caps.wUmax = max;
            }
            else if (instance.GuidType == DirectInputGuids.RzAxis)
            {
                device.GetPropertyRange(DirectInputPropertyTypes.Range, DirectInputHowTypes.ById, (int)instance.DataType | (instance.DataInstance << 8), out int min, out int max);
                caps.wVmin = min;
                caps.wVmax = max;
            }
            else if (instance.GuidType == DirectInputGuids.POV)
            {
                caps.wCaps |= JoyDriverCaps.HasPov;
            }
            else if (instance.GuidType == DirectInputGuids.Unknown)
            {
            }
            else
            {
                //Guid guid = instance.GuidType;
            }
        }

        device.GetPropertyUInt(DirectInputPropertyTypes.VendorIdProductId, DirectInputHowTypes.Device, 0, out uint id);
        caps.ManufacturerID = (ushort)id;
        caps.ProductID = (ushort)(id >> 16);
        caps.wNumButtons = deviceCaps.ButtonsCount;
        caps.wNumAxes = deviceCaps.AxesCount;

        if (caps.wZmin != 0 || caps.wZmax != 0)
        {
            caps.wCaps |= JoyDriverCaps.HasZ;
        }

        if (caps.wRmin != 0 || caps.wRmax != 0)
        {
            caps.wCaps |= JoyDriverCaps.HasR;
        }

        if (caps.wUmin != 0 || caps.wUmax != 0)
        {
            caps.wCaps |= JoyDriverCaps.HasU;
        }

        if (caps.wVmin != 0 || caps.wVmax != 0)
        {
            caps.wCaps |= JoyDriverCaps.HasV;
        }
    }

    public int GetNumDevs()
    {
        return _devicesCount;
    }

    public bool GetCaps(int deviceIndex, out JoyCaps caps)
    {
        if (deviceIndex < 0 || deviceIndex >= _devicesCount)
        {
            caps = new();
            return false;
        }

        caps = _deviceCaps[deviceIndex];
        return true;
    }

    public string GetDeviceName(int deviceIndex)
    {
        if (deviceIndex < 0 || deviceIndex >= _devicesCount)
        {
            return "Unknown";
        }

        return _deviceInfos[deviceIndex].InstanceName;
    }

    public bool GetDevicePosition(int deviceIndex, out JoyInfoEx info)
    {
        info = default;

        if (deviceIndex < 0 || deviceIndex >= _devicesCount)
        {
            return false;
        }

        var device = _devices[deviceIndex] ?? throw new InvalidOperationException("device is null");

        try
        {
            device.Poll();
        }
        catch
        {
            device.Acquire();
        }

        DirectInputJoystickState2 state = device.GetDeviceStateForJoystick2();

        info.dwXpos = (uint)state.X;
        info.dwYpos = (uint)state.Y;
        info.dwZpos = (uint)state.Z;
        info.dwRpos = (uint)state.Ry;
        info.dwUpos = (uint)state.Rx;
        info.dwVpos = (uint)state.Rz;
        info.dwPOV = (uint)state.POVs[0];

        return true;
    }

    public bool IsButtonPressed(int deviceIndex, int buttonIndex)
    {
        if (buttonIndex < 0 || buttonIndex >= 128)
        {
            return false;
        }

        if (deviceIndex < 0 || deviceIndex >= _devicesCount)
        {
            return false;
        }

        var device = _devices[deviceIndex] ?? throw new InvalidOperationException("device is null");

        try
        {
            device.Poll();
        }
        catch
        {
            device.Acquire();
        }

        DirectInputJoystickState2 state = device.GetDeviceStateForJoystick2();

        if (state.Buttons[buttonIndex] != 0)
        {
            return true;
        }

        return false;
    }
}
