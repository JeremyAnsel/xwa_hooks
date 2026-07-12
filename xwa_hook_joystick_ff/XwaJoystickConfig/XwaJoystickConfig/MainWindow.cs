#if DEBUG
#define DESIGN
#endif

using Aprillz.MewUI;
using Aprillz.MewUI.Controls;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Text;
using XwaJoystickConfig.CustomConverters;

namespace XwaJoystickConfig;

internal class MainWindow : Window
{
    private static readonly Encoding _encoding = Encoding.GetEncoding("iso-8859-1");

    private System.Timers.Timer? _timer;

    private IJoystickDevices? _joystickDevices;

    public ObservableValue<int> JoystickSettingFFDeviceIndex { get; } = new();

    public ObservableValue<bool> JoystickSettingEnableSmallMovement { get; } = new();

    public ObservableValue<int> JoystickSettingSmallMovement_8C1CC2 { get; } = new();

    public ObservableValue<int> JoystickSettingSmallMovement_8C1CC0 { get; } = new();

    public ObservableValue<int> JoystickSettingSmallMovement_8C1CC4 { get; } = new();

    public ObservableValue<int> JoystickSettingMainControllerIndex { get; } = new();

    public ObservableValue<int> JoystickSettingYawControllerIndex { get; } = new();

    public ObservableValue<int> JoystickSettingYawControllerAxisIndex { get; } = new();

    public ObservableValue<bool> JoystickSettingInvertYaw { get; } = new();

    public ObservableValue<float> JoystickSettingYawMultiplicator { get; } = new();

    public ObservableValue<int> JoystickSettingPitchControllerIndex { get; } = new();

    public ObservableValue<int> JoystickSettingPitchControllerAxisIndex { get; } = new();

    public ObservableValue<bool> JoystickSettingInvertPitch { get; } = new();

    public ObservableValue<float> JoystickSettingPitchMultiplicator { get; } = new();

    public ObservableValue<int> JoystickSettingThrottleControllerIndex { get; } = new();

    public ObservableValue<int> JoystickSettingThrottleControllerAxisIndex { get; } = new();

    public ObservableValue<bool> JoystickSettingInvertThrottle { get; } = new();

    public ObservableValue<float> JoystickSettingThrottleMultiplicator { get; } = new();

    public ObservableValue<int> JoystickSettingRudderControllerIndex { get; } = new();

    public ObservableValue<int> JoystickSettingRudderControllerAxisIndex { get; } = new();

    public ObservableValue<bool> JoystickSettingInvertRudder { get; } = new();

    public ObservableValue<float> JoystickSettingRudderMultiplicator { get; } = new();

    public ObservableValue<bool> JoystickSettingUsePovControllerAsButtons { get; } = new();

    public ObservableValue<int> JoystickSettingVirtualCockpitLookSensitivity { get; } = new();

    public ObservableCollection<JoystickConfigButton> JoystickConfigButtons { get; } = [];

    public ObservableCollection<JoystickConfigAxis> JoystickConfigAxes { get; } = [];

    public ObservableCollection<JoystickController> JoystickControllers { get; } = [];

    public MainWindow()
    {
        if (File.Exists("XwaJoystickConfig.ico"))
        {
            this.Icon(IconSource.FromFile("XwaJoystickConfig.ico"));
        }

        this.Title("Xwa Joystick Config");
        this.Resizable(850, 700);
        this.BorderThickness(0);
        this.StartCenterScreen();

        this.OnLoaded(() => MainWindow_Loaded());
        this.OnClosed(() => MainWindow_Unloaded());

        JoystickControllers.CollectionChanged += (s, e) =>
        {
            JoystickSettingMainControllerIndex.NotifyChanged();
            JoystickSettingYawControllerIndex.NotifyChanged();
            JoystickSettingPitchControllerIndex.NotifyChanged();
            JoystickSettingThrottleControllerIndex.NotifyChanged();
            JoystickSettingRudderControllerIndex.NotifyChanged();
        };

        this.Content(
            new DockPanel()
            .Children(
                CreateToolBar().DockTop(),
                CreateTabControl()
                )
            );
    }

    private StackPanel CreateToolBar()
    {
        var toolbar = new StackPanel()
            .Horizontal()
            .Spacing(8)
            .Margin(new Thickness(8))
            .Children(
                new Button().Content("Open...").OnClick(() => OpenConfigButton_Click()),
                new Button().Content("Save...").OnClick(() => SaveConfigButton_Click()),
                new Button().Content("Set default").OnClick(() => SetDefaultConfigButton_Click()),
                new Button().Content("Init buttons (Index)").OnClick(() => InitButtonsIndexButton_Click()),
                new Button().Content("Init buttons (Id)").OnClick(() => InitButtonsIdButton_Click())
            );

        return toolbar;
    }

    private TabControl CreateTabControl()
    {
        var tabControl = new TabControl()
            .TabItems(
                CreateSettingsTabItem(),
                CreateSettingsAxesTabItem(),
                CreateButtonsTabItem(),
                CreateAxesTabItem(),
                CreateSystemControllersTabItem()
            );

        return tabControl;
    }

    private TabItem CreateSettingsTabItem()
    {
        var tabItem = new TabItem()
            .Header("Settings");

        var grid = new ScrollViewer()
            .Content(
                new StackPanel()
                .Children(
                    new TextBlock(),
                    new GroupBox().Content(new StackPanel().Children(
                        new TextBlock().Text("; Select a joystick with force feedback"),
                        new TextBlock().Text("; 0 to select the first device ; -1 to select the last device"),
                        new TextBlock().Text("; integer value to select a device by its index"),
                        new TextBox().BindText(JoystickSettingFFDeviceIndex, IntConverter.Convert, IntConverter.ConvertBack)
                        )),
                    new TextBlock(),
                    new GroupBox().Content(new StackPanel().Children(
                        new CheckBox().Content("Enable small joystick movement").BindIsChecked(JoystickSettingEnableSmallMovement),
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("8C1CC2:"),
                            new TextBox().Width(120).BindText(JoystickSettingSmallMovement_8C1CC2, IntConverter.Convert, IntConverter.ConvertBack)
                            ),
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("8C1CC0:"),
                            new TextBox().Width(120).BindText(JoystickSettingSmallMovement_8C1CC0, IntConverter.Convert, IntConverter.ConvertBack)
                            ),
                            new StackPanel().Horizontal().Children(
                                new TextBlock().Width(120).Text("8C1CC4:"),
                                new TextBox().Width(120).BindText(JoystickSettingSmallMovement_8C1CC4, IntConverter.Convert, IntConverter.ConvertBack)
                            )
                        )),
                    new TextBlock(),
                    new GroupBox().Content(new StackPanel().Children(
                        new TextBlock().Text("; Select the controller used for the main axis and buttons"),
                        new TextBox().BindText(JoystickSettingMainControllerIndex, IntConverter.Convert, IntConverter.ConvertBack),
                        new TextBlock().BindText(JoystickSettingMainControllerIndex, t => ControllerNameConverter.Convert(JoystickControllers, t))
                        )),
                    new TextBlock(),
                    new GroupBox().Content(new StackPanel().Children(
                        new CheckBox().Content("Use the POV as regular buttons").BindIsChecked(JoystickSettingUsePovControllerAsButtons)
                        )),
                    new TextBlock(),
                    new GroupBox().Content(new StackPanel().Children(
                        new TextBlock().Text("; Virtual cockpit look sensitivity"),
                        new TextBox().BindText(JoystickSettingVirtualCockpitLookSensitivity, IntConverter.Convert, IntConverter.ConvertBack)
                        ))
                )
            );

        tabItem.Content(grid);
        return tabItem;
    }

    private TabItem CreateSettingsAxesTabItem()
    {
        var tabItem = new TabItem()
            .Header("Settings axes");

        var grid = new ScrollViewer()
            .Content(
                new StackPanel()
                .Children(
                    new TextBlock(),
                    new GroupBox().Header("Yaw axis").Content(new StackPanel().Children(
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Controller index or Id:"),
                            new TextBox().Width(120).BindText(JoystickSettingYawControllerIndex, IntConverter.Convert, IntConverter.ConvertBack),
                            new TextBlock().Width(300).Margin(10, 0).BindText(JoystickSettingYawControllerIndex, t => ControllerNameConverter.Convert(JoystickControllers, t))
                            ),
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Axis index:"),
                            new TextBox().Width(120).BindText(JoystickSettingYawControllerAxisIndex, IntConverter.Convert, IntConverter.ConvertBack)
                            ),
                        new StackPanel().Horizontal().Children(
                            new CheckBox().Width(120).Content("Invert axis").BindIsChecked(JoystickSettingInvertYaw),
                            new TextBlock().Width(120).Text("Axis multiplicator:"),
                            new TextBox().Width(120).BindText(JoystickSettingYawMultiplicator, FloatConverter.Convert, FloatConverter.ConvertBack)
                            )
                        )),
                    new TextBlock(),
                    new GroupBox().Header("Pitch axis").Content(new StackPanel().Children(
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Controller index or Id:"),
                            new TextBox().Width(120).BindText(JoystickSettingPitchControllerIndex, IntConverter.Convert, IntConverter.ConvertBack),
                            new TextBlock().Width(300).Margin(10, 0).BindText(JoystickSettingPitchControllerIndex, t => ControllerNameConverter.Convert(JoystickControllers, t))
                            ),
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Axis index:"),
                            new TextBox().Width(120).BindText(JoystickSettingPitchControllerAxisIndex, IntConverter.Convert, IntConverter.ConvertBack)
                            ),
                        new StackPanel().Horizontal().Children(
                            new CheckBox().Width(120).Content("Invert axis").BindIsChecked(JoystickSettingInvertPitch),
                            new TextBlock().Width(120).Text("Axis multiplicator:"),
                            new TextBox().Width(120).BindText(JoystickSettingPitchMultiplicator, FloatConverter.Convert, FloatConverter.ConvertBack)
                            )
                        )),
                    new TextBlock(),
                    new GroupBox().Header("Throttle axis").Content(new StackPanel().Children(
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Controller index or Id:"),
                            new TextBox().Width(120).BindText(JoystickSettingThrottleControllerIndex, IntConverter.Convert, IntConverter.ConvertBack),
                            new TextBlock().Width(300).Margin(10, 0).BindText(JoystickSettingThrottleControllerIndex, t => ControllerNameConverter.Convert(JoystickControllers, t))
                            ),
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Axis index:"),
                            new TextBox().Width(120).BindText(JoystickSettingThrottleControllerAxisIndex, IntConverter.Convert, IntConverter.ConvertBack)
                            ),
                        new StackPanel().Horizontal().Children(
                            new CheckBox().Width(120).Content("Invert axis").BindIsChecked(JoystickSettingInvertThrottle),
                            new TextBlock().Width(120).Text("Axis multiplicator:"),
                            new TextBox().Width(120).BindText(JoystickSettingThrottleMultiplicator, FloatConverter.Convert, FloatConverter.ConvertBack)
                            )
                        )),
                    new TextBlock(),
                    new GroupBox().Header("Roll axis").Content(new StackPanel().Children(
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Controller index or Id:"),
                            new TextBox().Width(120).BindText(JoystickSettingRudderControllerIndex, IntConverter.Convert, IntConverter.ConvertBack),
                            new TextBlock().Width(300).Margin(10, 0).BindText(JoystickSettingRudderControllerIndex, t => ControllerNameConverter.Convert(JoystickControllers, t))
                            ),
                        new StackPanel().Horizontal().Children(
                            new TextBlock().Width(120).Text("Axis index:"),
                            new TextBox().Width(120).BindText(JoystickSettingRudderControllerAxisIndex, IntConverter.Convert, IntConverter.ConvertBack)
                            ),
                        new StackPanel().Horizontal().Children(
                            new CheckBox().Width(120).Content("Invert axis").BindIsChecked(JoystickSettingInvertRudder),
                            new TextBlock().Width(120).Text("Axis multiplicator:"),
                            new TextBox().Width(120).BindText(JoystickSettingRudderMultiplicator, FloatConverter.Convert, FloatConverter.ConvertBack)
                            )
                        ))
                )
            );

        tabItem.Content(grid);
        return tabItem;
    }

    private TabItem CreateButtonsTabItem()
    {
        var tabItem = new TabItem()
            .Header("Buttons");

        var grid = new GridView()
            .ItemsSource(JoystickConfigButtons)
            .Columns(
                new GridViewColumn<JoystickConfigButton>()
                    .Header("Controller")
                    .Width(220)
                    .Text(item => item.Controller),
                new GridViewColumn<JoystickConfigButton>()
                    .Header("Key")
                    .Width(170)
                    .Template(
                        build: _ => new TextBox().IsReadOnly(),
                        bind: (s, item) => s.Text = item.Key
                        ),
                new GridViewColumn<JoystickConfigButton>()
                    .Header("Pressed")
                    .Width(60)
                    .Template(
                        build: _ => new RadioButton().IsEnabled(false),
                        bind: (s, r) => s.BindIsChecked(r.IsPressed)
                        ),
                new GridViewColumn<JoystickConfigButton>()
                    .Header("Value")
                    .Width(360)
                    .Template(
                        build: _ => new ComboBox().Items([.. JoystickKeys.Keys.Values]),
                        bind: (s, r) => s.BindSelectedIndex(
                            r.Value,
                            r => DictionaryKeyConverter.Convert(r, JoystickKeys.Keys),
                            r => DictionaryKeyConverter.ConvertBack(r, JoystickKeys.Keys)
                            )
                        )
                    );

        tabItem.Content(grid);
        return tabItem;
    }

    private TabItem CreateAxesTabItem()
    {
        var tabItem = new TabItem()
            .Header("Axes");

        var grid = new GridView()
            .ItemsSource(JoystickConfigAxes)
            .Columns(
                new GridViewColumn<JoystickConfigAxis>()
                    .Header("Controller Name")
                    .Width(250)
                    .Text(item => item.ControllerName),
                new GridViewColumn<JoystickConfigAxis>()
                    .Header("Controller")
                    .Width(250)
                    .Template(
                        build: _ => new TextBox().IsReadOnly(),
                        bind: (s, item) => s.Text = item.Controller
                        ),
                new GridViewColumn<JoystickConfigAxis>()
                    .Header("Axis")
                    .Width(150)
                    .Text(item => item.Axis),
                new GridViewColumn<JoystickConfigAxis>()
                    .Header("Position")
                    .Width(100)
                    .Bind(
                        build: ctx => new TextBlock(),
                        bind: (s, item) => s.BindText(item.Position, IntConverter.Convert)
                        )
            );

        tabItem.Content(grid);
        return tabItem;
    }

    private TabItem CreateSystemControllersTabItem()
    {
        var tabItem = new TabItem()
            .Header("System Controllers");

        var grid = new GridView()
            .ItemsSource(JoystickControllers)
            .Columns(
                new GridViewColumn<JoystickController>()
                    .Header("Index")
                    .Width(100)
                    .Text(item => item.ControllerIndex.ToString(CultureInfo.InvariantCulture)),
                new GridViewColumn<JoystickController>()
                    .Header("ID")
                    .Width(100)
                    .Text(item => item.Id.ToString(CultureInfo.InvariantCulture)),
                new GridViewColumn<JoystickController>()
                    .Header("Name")
                    .Width(300)
                    .Text(item => item.Name),
                new GridViewColumn<JoystickController>()
                    .Header("Axes Count")
                    .Width(100)
                    .Text(item => item.AxesCount.ToString(CultureInfo.InvariantCulture)),
                new GridViewColumn<JoystickController>()
                    .Header("Buttons Count")
                    .Width(100)
                    .Text(item => item.ButtonsCount.ToString(CultureInfo.InvariantCulture))
            );

        tabItem.Content(grid);
        return tabItem;
    }

    private void MainWindow_Loaded()
    {
        IList<string> lines = XwaHooksConfig.GetFileLines("hook_joystick_ff.cfg");

        if (lines.Count == 0)
        {
            lines = XwaHooksConfig.GetFileLines("hooks.ini", "hook_joystick_ff");
        }

        bool joystickUseDirectInput = XwaHooksConfig.GetFileKeyValueInt(lines, "JoystickUseDirectInput", 0) != 0;

        if (joystickUseDirectInput)
        {
            _joystickDevices = new JoystickDevicesDirectInput(this);
        }
        else
        {
            _joystickDevices = new JoystickDevicesWin32();
        }

        this.JoystickControllers.Clear();

        foreach (JoystickController controller in this.ReadJoystickControllers())
        {
            this.JoystickControllers.Add(controller);
        }

#if DESIGN
        if (this.JoystickControllers.Count == 0)
        {
            this.JoystickControllers.Add(new JoystickController("Controller 1"));
            this.JoystickControllers.Add(new JoystickController("Controller 2"));
            this.JoystickControllers.Add(new JoystickController("Controller 3"));
        }
#endif

        if (File.Exists("JoystickConfig.txt"))
        {
            this.ReadJoystickConfig("JoystickConfig.txt");
        }
        else
        {
            this.SetDefaultSettings();
            this.SetDefaultJoystickConfigButtons(0);
        }

        this.Update();
        StartTimer();
    }

    private void MainWindow_Unloaded()
    {
        StopTimer();
        _joystickDevices?.Release();
        _joystickDevices = null;
        SaveConfig();
    }

    public void Update()
    {
        // todo
        //this.Invalidate();
    }

    private void SaveConfig()
    {
        if (System.IO.File.Exists("XWingAlliance.exe"))
        {
            try
            {
                this.SaveJoystickConfig("JoystickConfig.txt");

                NativeMessageBox.Show(this.Handle, "JoystickConfig.txt Saved", this.Title);
            }
            catch (Exception ex)
            {
                NativeMessageBox.Show(this.Handle, ex.Message, this.Title, NativeMessageBoxButtons.Ok, NativeMessageBoxIcon.Error);
            }
        }
    }

    private void StartTimer()
    {
        _timer = new System.Timers.Timer(100);
        _timer.Elapsed += (s, e) => TimerFunction(s, e);
        _timer.Start();
    }

    private void StopTimer()
    {
        _timer?.Stop();
        _timer?.Dispose();
        _timer = null;
    }

    private void InitButtons(bool useIds)
    {
        this.JoystickConfigButtons.Clear();

        if (_joystickDevices is null)
        {
            this.Update();
            return;
        }

        int count = _joystickDevices.GetNumDevs();
        int controllerIndex = -1;

        int buttonIndex = 0;
        int povIndex = 0;

        for (int deviceIndex = 0; deviceIndex < count; deviceIndex++)
        {
            if (!_joystickDevices.GetCaps(deviceIndex, out JoyCaps caps))
            {
                continue;
            }

            controllerIndex++;

            if (useIds)
            {
                int id = caps.ManufacturerID << 16 | caps.ProductID;
                controllerIndex = id;
            }

            for (int i = 0; i < caps.wNumButtons; i++)
            {
                string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", controllerIndex, i + 1);
                int value = GetDefaultConfigButton(buttonIndex + 1);
                buttonIndex++;

                string controller = ControllerNameConverter.Convert(this.JoystickControllers, controllerIndex);
                this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
            }

            if (caps.wCaps.HasFlag(JoyDriverCaps.HasPov))
            {
                for (int i = 0; i < 4; i++)
                {
                    string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", controllerIndex, i + 1);
                    int value = GetDefaultConfigPov(povIndex + 1);
                    povIndex++;

                    string controller = ControllerNameConverter.Convert(this.JoystickControllers, controllerIndex);
                    this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
                }
            }
        }

        this.Update();
    }

    private void SetDefaultSettings()
    {
        this.JoystickSettingFFDeviceIndex.Value = 0;
        this.JoystickSettingEnableSmallMovement.Value = true;
        this.JoystickSettingSmallMovement_8C1CC2.Value = 0;
        this.JoystickSettingSmallMovement_8C1CC0.Value = 0;
        this.JoystickSettingSmallMovement_8C1CC4.Value = 0;
        this.JoystickSettingMainControllerIndex.Value = 0;
        this.JoystickSettingYawControllerIndex.Value = 0;
        this.JoystickSettingYawControllerAxisIndex.Value = 0;
        this.JoystickSettingInvertYaw.Value = false;
        this.JoystickSettingYawMultiplicator.Value = 1;
        this.JoystickSettingPitchControllerIndex.Value = 0;
        this.JoystickSettingPitchControllerAxisIndex.Value = 1;
        this.JoystickSettingInvertPitch.Value = false;
        this.JoystickSettingPitchMultiplicator.Value = 1;
        this.JoystickSettingThrottleControllerIndex.Value = 0;
        this.JoystickSettingThrottleControllerAxisIndex.Value = 2;
        this.JoystickSettingInvertThrottle.Value = false;
        this.JoystickSettingThrottleMultiplicator.Value = 1;
        this.JoystickSettingRudderControllerIndex.Value = 0;
        this.JoystickSettingRudderControllerAxisIndex.Value = 3;
        this.JoystickSettingInvertRudder.Value = false;
        this.JoystickSettingRudderMultiplicator.Value = 1;
        this.JoystickSettingUsePovControllerAsButtons.Value = false;
        this.JoystickSettingVirtualCockpitLookSensitivity.Value = 1200;
    }

    private void SetDefaultJoystickConfigButtons(int controllerIndex)
    {
        this.JoystickConfigButtons.Clear();

        for (int i = 0; i < 16; i++)
        {
            string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", controllerIndex, i + 1);
            int value = GetDefaultConfigButton(i + 1);

            string controller = ControllerNameConverter.Convert(this.JoystickControllers, controllerIndex);
            this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
        }

        for (int i = 0; i < 4; i++)
        {
            string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", controllerIndex, i + 1);
            int value = GetDefaultConfigPov(i + 1);

            string controller = ControllerNameConverter.Convert(this.JoystickControllers, controllerIndex);
            this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
        }
    }

    private static int GetDefaultConfigButton(int index)
    {
        int value = index switch
        {
            1 => 156,
            2 => 157,
            3 => 114,
            4 => 108,
            5 => 101,
            6 => 105,
            7 => 91,
            8 => 8,
            9 => 13,
            10 => 93,
            _ => 0,
        };
        return value;
    }

    private static int GetDefaultConfigPov(int index)
    {
        int value = index switch
        {
            1 => 186,
            2 => 184,
            3 => 180,
            4 => 182,
            _ => 0,
        };
        return value;
    }

    private void ReadJoystickConfig(string path)
    {
        IList<string> lines = XwaHooksConfig.GetFileLines(path);

        this.JoystickSettingFFDeviceIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "JoystickFFDeviceIndex", 0);
        this.JoystickSettingEnableSmallMovement.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "EnableSmallMovement", 1) != 0;
        this.JoystickSettingSmallMovement_8C1CC2.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "SmallMovement_8C1CC2", 64);
        this.JoystickSettingSmallMovement_8C1CC0.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "SmallMovement_8C1CC0", 24);
        this.JoystickSettingSmallMovement_8C1CC4.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "SmallMovement_8C1CC4", 64);
        this.JoystickSettingMainControllerIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "MainControllerIndex", 0);
        this.JoystickSettingYawControllerIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "YawControllerIndex", 0);
        this.JoystickSettingYawControllerAxisIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "YawControllerAxisIndex", 0);
        this.JoystickSettingInvertYaw.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertYaw", 0) != 0;
        this.JoystickSettingYawMultiplicator.Value = XwaHooksConfigEx.GetFileKeyValueFloat(lines, "YawMultiplicator", 1);
        this.JoystickSettingPitchControllerIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "PitchControllerIndex", 0);
        this.JoystickSettingPitchControllerAxisIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "PitchControllerAxisIndex", 1);
        this.JoystickSettingInvertPitch.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertPitch", 0) != 0;
        this.JoystickSettingPitchMultiplicator.Value = XwaHooksConfigEx.GetFileKeyValueFloat(lines, "PitchMultiplicator", 1);
        this.JoystickSettingThrottleControllerIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "ThrottleControllerIndex", 0);
        this.JoystickSettingThrottleControllerAxisIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "ThrottleControllerAxisIndex", 2);
        this.JoystickSettingInvertThrottle.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertThrottle", 0) != 0;
        this.JoystickSettingThrottleMultiplicator.Value = XwaHooksConfigEx.GetFileKeyValueFloat(lines, "ThrottleMultiplicator", 1);
        this.JoystickSettingRudderControllerIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "RudderControllerIndex", 0);
        this.JoystickSettingRudderControllerAxisIndex.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "RudderControllerAxisIndex", 3);
        this.JoystickSettingInvertRudder.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertRudder", 0) != 0;
        this.JoystickSettingRudderMultiplicator.Value = XwaHooksConfigEx.GetFileKeyValueFloat(lines, "RudderMultiplicator", 1);
        this.JoystickSettingUsePovControllerAsButtons.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "UsePovControllerAsButtons", 0) != 0;
        this.JoystickSettingVirtualCockpitLookSensitivity.Value = XwaHooksConfig.GetFileKeyValueInt(lines, "VirtualCockpitLookSensitivity", 1200);

        this.JoystickConfigButtons.Clear();

        foreach (string line in lines)
        {
            int pos = line.IndexOf('=');

            if (pos == -1)
            {
                continue;
            }

            string key = line[..pos].Trim();

            string start = "joybutton_";
            if (key.StartsWith(start, StringComparison.OrdinalIgnoreCase))
            {
                string indexString = key[start.Length..key.IndexOf('_', start.Length)];
                int index;

                try
                {
                    index = int.Parse(indexString, CultureInfo.InvariantCulture);
                }
                catch
                {
                    continue;
                }

                string valueString = line[(pos + 1)..].Trim();
                int value = int.Parse(valueString, CultureInfo.InvariantCulture);

                string controller = ControllerNameConverter.Convert(this.JoystickControllers, index);
                this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
            }
        }
    }

    private void SaveJoystickConfig(string path)
    {
        using var writer = new StreamWriter(path, false, _encoding);

        writer.WriteLine();
        writer.WriteLine("; Select a joystick with force feedback");
        writer.WriteLine("; 0 to select the first device");
        writer.WriteLine("; -1 to select the last device");
        writer.WriteLine("; integer value to select a device by its index");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "JoystickFFDeviceIndex = {0}", this.JoystickSettingFFDeviceIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Enable small joystick movement");
        writer.WriteLine("; 0 to disable small movement");
        writer.WriteLine("; 1 to enable small movement");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "EnableSmallMovement = {0}", this.JoystickSettingEnableSmallMovement.Value ? 1 : 0));
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "SmallMovement_8C1CC2 = {0}", this.JoystickSettingSmallMovement_8C1CC2.Value));
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "SmallMovement_8C1CC0 = {0}", this.JoystickSettingSmallMovement_8C1CC0.Value));
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "SmallMovement_8C1CC4 = {0}", this.JoystickSettingSmallMovement_8C1CC4.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller used for the main axis and buttons");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "MainControllerIndex = {0}", this.JoystickSettingMainControllerIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller used for yaw");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "YawControllerIndex = {0}", this.JoystickSettingYawControllerIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller axis used for yaw");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "YawControllerAxisIndex = {0}", this.JoystickSettingYawControllerAxisIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Invert yaw axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertYaw = {0}", this.JoystickSettingInvertYaw.Value ? 1 : 0));
        writer.WriteLine();
        writer.WriteLine("; Multiply yaw axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "YawMultiplicator = {0}", this.JoystickSettingYawMultiplicator.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller used for pitch");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "PitchControllerIndex = {0}", this.JoystickSettingPitchControllerIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller axis used for pitch");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "PitchControllerAxisIndex = {0}", this.JoystickSettingPitchControllerAxisIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Invert pitch axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertPitch = {0}", this.JoystickSettingInvertPitch.Value ? 1 : 0));
        writer.WriteLine();
        writer.WriteLine("; Multiply pitch axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "PitchMultiplicator = {0}", this.JoystickSettingPitchMultiplicator.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller used for throttle");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleControllerIndex = {0}", this.JoystickSettingThrottleControllerIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller axis used for throttle");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleControllerAxisIndex = {0}", this.JoystickSettingThrottleControllerAxisIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Invert throttle axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertThrottle = {0}", this.JoystickSettingInvertThrottle.Value ? 1 : 0));
        writer.WriteLine();
        writer.WriteLine("; Multiply throttle axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleMultiplicator = {0}", this.JoystickSettingThrottleMultiplicator.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller used for rudder");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderControllerIndex = {0}", this.JoystickSettingRudderControllerIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Select the controller axis used for rudder");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderControllerAxisIndex = {0}", this.JoystickSettingRudderControllerAxisIndex.Value));
        writer.WriteLine();
        writer.WriteLine("; Invert rudder axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertRudder = {0}", this.JoystickSettingInvertRudder.Value ? 1 : 0));
        writer.WriteLine();
        writer.WriteLine("; Multiply rudder axis");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderMultiplicator = {0}", this.JoystickSettingRudderMultiplicator.Value));
        writer.WriteLine();
        writer.WriteLine("; Use the POV as regular buttons");
        writer.WriteLine("; 0 to continious movement");
        writer.WriteLine("; 1 to enable regular buttons");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "UsePovControllerAsButtons = {0}", this.JoystickSettingUsePovControllerAsButtons.Value ? 1 : 0));
        writer.WriteLine();
        writer.WriteLine("; Virtual cockpit look sensitivity");
        writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "VirtualCockpitLookSensitivity = {0}", this.JoystickSettingVirtualCockpitLookSensitivity.Value));
        writer.WriteLine();

        foreach (var item in this.JoystickConfigButtons)
        {
            if (JoystickKeys.Keys.TryGetValue(item.Value.Value, out string? value))
            {
                writer.WriteLine("; " + value);
            }

            writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "{0} = {1}", item.Key, item.Value.Value));
        }

        writer.WriteLine();
    }

    private List<JoystickController> ReadJoystickControllers()
    {
        var controllers = new List<JoystickController>();

        if (_joystickDevices is null)
        {
            return controllers;
        }

        int count = _joystickDevices.GetNumDevs();
        int controllerIndex = -1;

        for (int deviceIndex = 0; deviceIndex < count; deviceIndex++)
        {
            if (!_joystickDevices.GetCaps(deviceIndex, out JoyCaps caps))
            {
                continue;
            }

            controllerIndex++;

            var controller = new JoystickController(deviceIndex, controllerIndex, caps, _joystickDevices);
            controllers.Add(controller);
        }

        return controllers;
    }

    private void TimerFunction(object? sender, System.Timers.ElapsedEventArgs e)
    {
        //StopTimer();

        if (_joystickDevices is null)
        {
            return;
        }

        int count = _joystickDevices.GetNumDevs();
        int controllerIndex = -1;

        var pressedKeys = new List<string>();

        for (int deviceIndex = 0; deviceIndex < count; deviceIndex++)
        {
            if (!_joystickDevices.GetCaps(deviceIndex, out JoyCaps caps))
            {
                continue;
            }

            if (!_joystickDevices.GetDevicePosition(deviceIndex, out JoyInfoEx info))
            {
                continue;
            }

            controllerIndex++;

            int index1 = controllerIndex;
            int index2 = caps.ManufacturerID << 16 | caps.ProductID;

            for (int i = 0; i < 6; i++)
            {
                string controllerName = ControllerNameConverter.Convert(this.JoystickControllers, controllerIndex);
                string controller = string.Format(CultureInfo.InvariantCulture, "{0} : {1}", index1, index2);
                string axis = string.Format(CultureInfo.InvariantCulture, "{0}", i);

                uint position = i switch
                {
                    0 => info.dwXpos,
                    1 => info.dwYpos,
                    2 => info.dwZpos,
                    3 => info.dwRpos,
                    4 => info.dwUpos,
                    5 => info.dwVpos,
                    _ => 0,
                };

                int index = -1;

                for (int j = 0; j < this.JoystickConfigAxes.Count; j++)
                {
                    if (string.Equals(this.JoystickConfigAxes[j].Controller, controller, StringComparison.Ordinal)
                        && string.Equals(this.JoystickConfigAxes[j].Axis, axis, StringComparison.Ordinal))
                    {
                        index = j;
                        break;
                    }
                }

                if (index != -1)
                {
                    this.JoystickConfigAxes[index].Position.Value = (int)position;
                }
                else
                {
                    this.JoystickConfigAxes.Add(new JoystickConfigAxis(controller, controllerName, axis, (int)position));
                }
            }

            for (int i = 0; i < caps.wNumButtons; i++)
            {
                if (_joystickDevices.IsButtonPressed(deviceIndex, i))
                {
                    string key1 = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", index1, i + 1);
                    string key2 = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", index2, i + 1);

                    pressedKeys.Add(key1);
                    pressedKeys.Add(key2);
                }
            }

            if (caps.wCaps.HasFlag(JoyDriverCaps.HasPov))
            {
                if (info.dwPOV != 0xffff)
                {
                    int povIndex = (int)(info.dwPOV / 9000U);

                    string key1 = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", index1, povIndex + 1);
                    string key2 = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", index2, povIndex + 1);

                    pressedKeys.Add(key1);
                    pressedKeys.Add(key2);
                }
            }
        }

        foreach (JoystickConfigButton button in this.JoystickConfigButtons)
        {
            button.IsPressed.Value = false;

            foreach (string pressedKey in pressedKeys)
            {
                if (string.Equals(button.Key, pressedKey, StringComparison.Ordinal))
                {
                    button.IsPressed.Value = true;
                    break;
                }
            }
        }

#if DESIGN
        if (this.JoystickConfigAxes.Count == 0)
        {
            this.JoystickConfigAxes.Add(new JoystickConfigAxis("Controller", "Name", "Axis 1"));
            this.JoystickConfigAxes.Add(new JoystickConfigAxis("Controller", "Name", "Axis 2"));
            this.JoystickConfigAxes.Add(new JoystickConfigAxis("Controller", "Name", "Axis 3"));
        }
#endif
    }

    private void OpenConfigButton_Click()
    {
        string? fileName = FileDialog.OpenFile(new OpenFileDialogOptions
        {
            Owner = this.Handle,
            Title = "Open JoystickConfig.txt",
            Filter = "JoystickConfig.txt|*.txt",
            InitialDirectory = Directory.GetCurrentDirectory()
        });

        if (fileName is null)
        {
            return;
        }

        try
        {
            this.ReadJoystickConfig(fileName);
        }
        catch (Exception ex)
        {
            NativeMessageBox.Show(this.Handle, fileName + "\n" + ex.Message, this.Title, NativeMessageBoxButtons.Ok, NativeMessageBoxIcon.Error);
        }

        this.Update();
    }

    private void SaveConfigButton_Click()
    {
        string? fileName = FileDialog.SaveFile(new SaveFileDialogOptions
        {
            Owner = this.Handle,
            Title = "Save JoystickConfig.txt",
            DefaultExtension = ".txt",
            Filter = "JoystickConfig.txt|*.txt",
            FileName = "JoystickConfig.txt",
            InitialDirectory = Directory.GetCurrentDirectory()
        });

        if (fileName is null)
        {
            return;
        }

        try
        {
            this.SaveJoystickConfig(fileName);

            NativeMessageBox.Show(this.Handle, "Saved", this.Title);
        }
        catch (Exception ex)
        {
            NativeMessageBox.Show(this.Handle, ex.Message, this.Title, NativeMessageBoxButtons.Ok, NativeMessageBoxIcon.Error);
        }
    }

    private void SetDefaultConfigButton_Click()
    {
        this.SetDefaultSettings();
        this.SetDefaultJoystickConfigButtons(0);

        this.Update();
    }

    private void InitButtonsIndexButton_Click()
    {
        InitButtons(false);
    }

    private void InitButtonsIdButton_Click()
    {
        InitButtons(true);
    }
}
