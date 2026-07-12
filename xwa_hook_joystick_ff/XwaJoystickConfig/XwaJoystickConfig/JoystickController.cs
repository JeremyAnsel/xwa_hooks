namespace XwaJoystickConfig;

public class JoystickController
{
    internal JoystickController(string name)
    {
        this.Name = name;
    }

    internal JoystickController(int deviceIndex, int controllerIndex, JoyCaps caps, IJoystickDevices joystickDevices)
    {
        this.Id = caps.ManufacturerID << 16 | caps.ProductID;

        this.DeviceIndex = deviceIndex;
        this.ControllerIndex = controllerIndex;
        this.AxesCount = caps.wNumAxes;
        this.ButtonsCount = caps.wNumButtons;

        string name = joystickDevices.GetDeviceName(deviceIndex);
        this.Name = string.IsNullOrEmpty(name) ? caps.ProductName : name;
    }

    public int Id { get; }

    public int DeviceIndex { get; }

    public int ControllerIndex { get; }

    public string Name { get; }

    public int AxesCount { get; }

    public int ButtonsCount { get; }
}
