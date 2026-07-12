using Aprillz.MewUI;
using System.ComponentModel;

namespace XwaJoystickConfig;

public class JoystickConfigAxis : INotifyPropertyChanged
{
    private string controllerName = string.Empty;

    private string controller = string.Empty;

    private string axis = string.Empty;

    public event PropertyChangedEventHandler? PropertyChanged;

    public JoystickConfigAxis()
    {
    }

    public JoystickConfigAxis(string controller, string controllerName, string axis)
        : this(controller, controllerName, axis, 0)
    {
    }

    public JoystickConfigAxis(string controller, string controllerName, string axis, int position)
    {
        this.controllerName = controllerName;
        this.controller = controller;
        this.axis = axis;
        this.Position.Value = position;
    }

    public string ControllerName
    {
        get
        {
            return this.controllerName;
        }

        set
        {
            if (value != this.controllerName)
            {
                this.controllerName = value;
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ControllerName)));
            }
        }
    }

    public string Controller
    {
        get
        {
            return this.controller;
        }

        set
        {
            if (value != this.controller)
            {
                this.controller = value;
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Controller)));
            }
        }
    }

    public string Axis
    {
        get
        {
            return this.axis;
        }

        set
        {
            if (value != this.axis)
            {
                this.axis = value;
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Axis)));
            }
        }
    }

    public ObservableValue<int> Position { get; } = new();
}
