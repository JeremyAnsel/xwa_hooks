using Aprillz.MewUI;
using System.ComponentModel;

namespace XwaJoystickConfig;

public class JoystickConfigButton : INotifyPropertyChanged
{
    private string controller = string.Empty;

    private string key = string.Empty;

    public event PropertyChangedEventHandler? PropertyChanged;

    public JoystickConfigButton()
    {
    }

    public JoystickConfigButton(string controller, string key, int value)
    {
        this.controller = controller;
        this.key = key;
        this.Value.Value = value;
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

    public string Key
    {
        get
        {
            return this.key;
        }

        set
        {
            if (value != this.key)
            {
                this.key = value;
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Key)));
            }
        }
    }

    public ObservableValue<int> Value { get; } = new ObservableValue<int>(-1);

    public ObservableValue<bool> IsPressed { get; } = new(false);
}
