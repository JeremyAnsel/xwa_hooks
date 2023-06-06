using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace XwaJoystickConfig
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static float GetFileKeyValueFloat(IList<string> lines, string key, float defaultValue = 0)
        {
            string value = XwaHooksConfig.GetFileKeyValue(lines, key);

            if (value.Length == 0)
            {
                return defaultValue;
            }

            if (!float.TryParse(value, out float result))
            {
                return defaultValue;
            }

            return result;
        }

        private static readonly Encoding _encoding = Encoding.GetEncoding("iso-8859-1");

        private System.Timers.Timer _timer;

        public MainWindow()
        {
            InitializeComponent();

#if !DEBUG
            Application.Current.DispatcherUnhandledException += Current_DispatcherUnhandledException;
#endif

            this.JoystickControllers = this.ReadJoystickControllers();

            if (System.IO.File.Exists("JoystickConfig.txt"))
            {
                this.ReadJoystickConfig("JoystickConfig.txt");
            }
            else
            {
                this.SetDefaultSettings();
                this.SetDefaultJoystickConfigButtons(0);
            }

            this.Update();
        }

        public int JoystickSettingFFDeviceIndex { get; set; }

        public bool JoystickSettingEnableSmallMovement { get; set; }

        public int JoystickSettingSmallMovement_8C1CC2 { get; set; }

        public int JoystickSettingSmallMovement_8C1CC0 { get; set; }

        public int JoystickSettingSmallMovement_8C1CC4 { get; set; }

        public int JoystickSettingMainControllerIndex { get; set; }

        public int JoystickSettingYawControllerIndex { get; set; }

        public int JoystickSettingYawControllerAxisIndex { get; set; }

        public bool JoystickSettingInvertYaw { get; set; }

        public float JoystickSettingYawMultiplicator { get; set; }

        public int JoystickSettingPitchControllerIndex { get; set; }

        public int JoystickSettingPitchControllerAxisIndex { get; set; }

        public bool JoystickSettingInvertPitch { get; set; }

        public float JoystickSettingPitchMultiplicator { get; set; }

        public int JoystickSettingThrottleControllerIndex { get; set; }

        public int JoystickSettingThrottleControllerAxisIndex { get; set; }

        public bool JoystickSettingInvertThrottle { get; set; }

        public float JoystickSettingThrottleMultiplicator { get; set; }

        public int JoystickSettingRudderControllerIndex { get; set; }

        public int JoystickSettingRudderControllerAxisIndex { get; set; }

        public bool JoystickSettingInvertRudder { get; set; }

        public float JoystickSettingRudderMultiplicator { get; set; }

        public bool JoystickSettingUsePovControllerAsButtons { get; set; }

        public int JoystickSettingVirtualCockpitLookSensitivity { get; set; }

        public ObservableCollection<JoystickConfigButton> JoystickConfigButtons { get; } = new ObservableCollection<JoystickConfigButton>();

        public ObservableCollection<JoystickConfigAxis> JoystickConfigAxes { get; } = new ObservableCollection<JoystickConfigAxis>();

        public List<JoystickController> JoystickControllers { get; }

        private void Current_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            e.Handled = true;

            if (e.Exception is System.Runtime.InteropServices.COMException comException && comException.ErrorCode == -2147221040) // CLIPBRD_E_CANT_OPEN
            {
                return;
            }

            MessageBox.Show(this, e.Exception.ToString(), "Press Ctrl+C to copy the text", MessageBoxButton.OK, MessageBoxImage.Error);
            Application.Current.Shutdown();
        }

        public void Update()
        {
            this.DataContext = null;
            this.DataContext = this;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            if (System.IO.File.Exists("XWingAlliance.exe"))
            {
                try
                {
                    this.SaveJoystickConfig("JoystickConfig.txt");

                    MessageBox.Show(this, "JoystickConfig.txt Saved", this.Title);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(this, ex.Message, this.Title, MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            _timer = new System.Timers.Timer(100);
            _timer.Elapsed += TimerFunction;
            _timer.Start();
        }

        private void MainWindow_Unloaded(object sender, RoutedEventArgs e)
        {
            _timer?.Stop();
            _timer?.Dispose();
            _timer = null;
        }

        private void OpenConfigButton_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new OpenFileDialog
            {
                DefaultExt = ".txt",
                CheckFileExists = true,
                Filter = "JoystickConfig.txt|*.txt",
                FileName = "JoystickConfig.txt",
                InitialDirectory = System.IO.Directory.GetCurrentDirectory()
            };

            string fileName;

            if (dialog.ShowDialog(this) == true)
            {
                fileName = dialog.FileName;
            }
            else
            {
                return;
            }

            try
            {
                this.ReadJoystickConfig(fileName);
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, fileName + "\n" + ex.Message, this.Title, MessageBoxButton.OK, MessageBoxImage.Error);
            }

            this.Update();
        }

        private void SaveConfigButton_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new SaveFileDialog
            {
                AddExtension = true,
                DefaultExt = ".txt",
                Filter = "JoystickConfig.txt|*.txt",
                FileName = "JoystickConfig.txt",
                InitialDirectory = System.IO.Directory.GetCurrentDirectory()
            };

            string fileName;

            if (dialog.ShowDialog(this) == true)
            {
                fileName = dialog.FileName;
            }
            else
            {
                return;
            }

            try
            {
                this.SaveJoystickConfig(fileName);

                MessageBox.Show(this, "Saved", this.Title);
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, ex.Message, this.Title, MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void SetDefaultConfigButton_Click(object sender, RoutedEventArgs e)
        {
            this.SetDefaultSettings();
            this.SetDefaultJoystickConfigButtons(0);

            this.Update();
        }

        private void InitButtonsIndexButton_Click(object sender, RoutedEventArgs e)
        {
            InitButtons(false);
        }

        private void InitButtonsIdButton_Click(object sender, RoutedEventArgs e)
        {
            InitButtons(true);
        }

        private void InitButtons(bool useIds)
        {
            this.JoystickConfigButtons.Clear();

            int count = NativeMethods.JoyGetNumDevs();
            int size = Marshal.SizeOf(typeof(JoyCaps));
            int controllerIndex = -1;

            int buttonIndex = 0;
            int povIndex = 0;

            for (int deviceIndex = 0; deviceIndex < count; deviceIndex++)
            {
                if (NativeMethods.JoyGetDevCaps(new IntPtr(deviceIndex), out JoyCaps caps, size) != 0)
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

                    string controller = (string)ControllerNameConverter.Default.Convert(new object[] { this.JoystickControllers, controllerIndex }, null, null, null);
                    this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
                }

                if (caps.wCaps.HasFlag(JoyDriverCaps.HasPov))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", controllerIndex, i + 1);
                        int value = GetDefaultConfigPov(povIndex + 1);
                        povIndex++;

                        string controller = (string)ControllerNameConverter.Default.Convert(new object[] { this.JoystickControllers, controllerIndex }, null, null, null);
                        this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
                    }
                }
            }

            this.Update();
        }

        private void SetDefaultSettings()
        {
            this.JoystickSettingFFDeviceIndex = 0;
            this.JoystickSettingEnableSmallMovement = true;
            this.JoystickSettingSmallMovement_8C1CC2 = 0;
            this.JoystickSettingSmallMovement_8C1CC0 = 0;
            this.JoystickSettingSmallMovement_8C1CC4 = 0;
            this.JoystickSettingMainControllerIndex = 0;
            this.JoystickSettingYawControllerIndex = 0;
            this.JoystickSettingYawControllerAxisIndex = 0;
            this.JoystickSettingInvertYaw = false;
            this.JoystickSettingYawMultiplicator = 1;
            this.JoystickSettingPitchControllerIndex = 0;
            this.JoystickSettingPitchControllerAxisIndex = 1;
            this.JoystickSettingInvertPitch = false;
            this.JoystickSettingPitchMultiplicator = 1;
            this.JoystickSettingThrottleControllerIndex = 0;
            this.JoystickSettingThrottleControllerAxisIndex = 2;
            this.JoystickSettingInvertThrottle = false;
            this.JoystickSettingThrottleMultiplicator = 1;
            this.JoystickSettingRudderControllerIndex = 0;
            this.JoystickSettingRudderControllerAxisIndex = 3;
            this.JoystickSettingInvertRudder = false;
            this.JoystickSettingRudderMultiplicator = 1;
            this.JoystickSettingUsePovControllerAsButtons = false;
            this.JoystickSettingVirtualCockpitLookSensitivity = 1200;
        }

        private void SetDefaultJoystickConfigButtons(int controllerIndex)
        {
            this.JoystickConfigButtons.Clear();

            for (int i = 0; i < 16; i++)
            {
                string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", controllerIndex, i + 1);
                int value = GetDefaultConfigButton(i + 1);

                string controller = (string)ControllerNameConverter.Default.Convert(new object[] { this.JoystickControllers, controllerIndex }, null, null, null);
                this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
            }

            for (int i = 0; i < 4; i++)
            {
                string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", controllerIndex, i + 1);
                int value = GetDefaultConfigPov(i + 1);

                string controller = (string)ControllerNameConverter.Default.Convert(new object[] { this.JoystickControllers, controllerIndex }, null, null, null);
                this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
            }
        }

        private static int GetDefaultConfigButton(int index)
        {
            int value;

            switch (index)
            {
                case 1:
                    value = 156;
                    break;

                case 2:
                    value = 157;
                    break;

                case 3:
                    value = 114;
                    break;

                case 4:
                    value = 108;
                    break;

                case 5:
                    value = 101;
                    break;

                case 6:
                    value = 105;
                    break;

                case 7:
                    value = 91;
                    break;

                case 8:
                    value = 8;
                    break;

                case 9:
                    value = 13;
                    break;

                case 10:
                    value = 93;
                    break;

                default:
                    value = 0;
                    break;
            }

            return value;
        }

        private static int GetDefaultConfigPov(int index)
        {
            int value;

            switch (index)
            {
                case 1:
                    value = 186;
                    break;

                case 2:
                    value = 184;
                    break;

                case 3:
                    value = 180;
                    break;

                case 4:
                    value = 182;
                    break;

                default:
                    value = 0;
                    break;
            }

            return value;
        }

        private void ReadJoystickConfig(string path)
        {
            IList<string> lines = XwaHooksConfig.GetFileLines(path);

            this.JoystickSettingFFDeviceIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "JoystickFFDeviceIndex", 0);
            this.JoystickSettingEnableSmallMovement = XwaHooksConfig.GetFileKeyValueInt(lines, "EnableSmallMovement", 1) != 0;
            this.JoystickSettingSmallMovement_8C1CC2 = XwaHooksConfig.GetFileKeyValueInt(lines, "SmallMovement_8C1CC2", 64);
            this.JoystickSettingSmallMovement_8C1CC0 = XwaHooksConfig.GetFileKeyValueInt(lines, "SmallMovement_8C1CC0", 24);
            this.JoystickSettingSmallMovement_8C1CC4 = XwaHooksConfig.GetFileKeyValueInt(lines, "SmallMovement_8C1CC4", 64);
            this.JoystickSettingMainControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "MainControllerIndex", 0);
            this.JoystickSettingYawControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "YawControllerIndex", 0);
            this.JoystickSettingYawControllerAxisIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "YawControllerAxisIndex", 0);
            this.JoystickSettingInvertYaw = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertYaw", 0) != 0;
            this.JoystickSettingYawMultiplicator = GetFileKeyValueFloat(lines, "YawMultiplicator", 1);
            this.JoystickSettingPitchControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "PitchControllerIndex", 0);
            this.JoystickSettingPitchControllerAxisIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "PitchControllerAxisIndex", 1);
            this.JoystickSettingInvertPitch = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertPitch", 0) != 0;
            this.JoystickSettingPitchMultiplicator = GetFileKeyValueFloat(lines, "PitchMultiplicator", 1);
            this.JoystickSettingThrottleControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "ThrottleControllerIndex", 0);
            this.JoystickSettingThrottleControllerAxisIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "ThrottleControllerAxisIndex", 2);
            this.JoystickSettingInvertThrottle = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertThrottle", 0) != 0;
            this.JoystickSettingThrottleMultiplicator = GetFileKeyValueFloat(lines, "ThrottleMultiplicator", 1);
            this.JoystickSettingRudderControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "RudderControllerIndex", 0);
            this.JoystickSettingRudderControllerAxisIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "RudderControllerAxisIndex", 3);
            this.JoystickSettingInvertRudder = XwaHooksConfig.GetFileKeyValueInt(lines, "InvertRudder", 0) != 0;
            this.JoystickSettingRudderMultiplicator = GetFileKeyValueFloat(lines, "RudderMultiplicator", 1);
            this.JoystickSettingUsePovControllerAsButtons = XwaHooksConfig.GetFileKeyValueInt(lines, "UsePovControllerAsButtons", 0) != 0;
            this.JoystickSettingVirtualCockpitLookSensitivity = XwaHooksConfig.GetFileKeyValueInt(lines, "VirtualCockpitLookSensitivity", 1200);

            this.JoystickConfigButtons.Clear();

            foreach (string line in lines)
            {
                int pos = line.IndexOf('=');

                if (pos == -1)
                {
                    continue;
                }

                string key = line.Substring(0, pos).Trim();
                string valueString = line.Substring(pos + 1).Trim();
                int value = int.Parse(valueString, CultureInfo.InvariantCulture);

                string start = "joybutton_";
                if (key.StartsWith(start, StringComparison.OrdinalIgnoreCase))
                {
                    string indexString = key.Substring(start.Length, key.IndexOf('_', start.Length) - start.Length);
                    int index;

                    try
                    {
                        index = int.Parse(indexString, CultureInfo.InvariantCulture);
                    }
                    catch
                    {
                        continue;
                    }

                    string controller = (string)ControllerNameConverter.Default.Convert(new object[] { this.JoystickControllers, index }, null, null, null);
                    this.JoystickConfigButtons.Add(new JoystickConfigButton(controller, key, value));
                }
            }
        }

        private void SaveJoystickConfig(string path)
        {
            using (var writer = new System.IO.StreamWriter(path, false, _encoding))
            {
                writer.WriteLine();
                writer.WriteLine("; Select a joystick with force feedback");
                writer.WriteLine("; 0 to select the first device");
                writer.WriteLine("; -1 to select the last device");
                writer.WriteLine("; integer value to select a device by its index");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "JoystickFFDeviceIndex = {0}", this.JoystickSettingFFDeviceIndex));
                writer.WriteLine();
                writer.WriteLine("; Enable small joystick movement");
                writer.WriteLine("; 0 to disable small movement");
                writer.WriteLine("; 1 to enable small movement");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "EnableSmallMovement = {0}", this.JoystickSettingEnableSmallMovement ? 1 : 0));
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "SmallMovement_8C1CC2 = {0}", this.JoystickSettingSmallMovement_8C1CC2));
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "SmallMovement_8C1CC0 = {0}", this.JoystickSettingSmallMovement_8C1CC0));
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "SmallMovement_8C1CC4 = {0}", this.JoystickSettingSmallMovement_8C1CC4));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for the main axis and buttons");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "MainControllerIndex = {0}", this.JoystickSettingMainControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for yaw");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "YawControllerIndex = {0}", this.JoystickSettingYawControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller axis used for yaw");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "YawControllerAxisIndex = {0}", this.JoystickSettingYawControllerAxisIndex));
                writer.WriteLine();
                writer.WriteLine("; Invert yaw axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertYaw = {0}", this.JoystickSettingInvertYaw ? 1 : 0));
                writer.WriteLine();
                writer.WriteLine("; Multiply yaw axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "YawMultiplicator = {0}", this.JoystickSettingYawMultiplicator));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for pitch");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "PitchControllerIndex = {0}", this.JoystickSettingPitchControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller axis used for pitch");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "PitchControllerAxisIndex = {0}", this.JoystickSettingPitchControllerAxisIndex));
                writer.WriteLine();
                writer.WriteLine("; Invert pitch axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertPitch = {0}", this.JoystickSettingInvertPitch ? 1 : 0));
                writer.WriteLine();
                writer.WriteLine("; Multiply pitch axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "PitchMultiplicator = {0}", this.JoystickSettingPitchMultiplicator));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for throttle");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleControllerIndex = {0}", this.JoystickSettingThrottleControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller axis used for throttle");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleControllerAxisIndex = {0}", this.JoystickSettingThrottleControllerAxisIndex));
                writer.WriteLine();
                writer.WriteLine("; Invert throttle axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertThrottle = {0}", this.JoystickSettingInvertThrottle ? 1 : 0));
                writer.WriteLine();
                writer.WriteLine("; Multiply throttle axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleMultiplicator = {0}", this.JoystickSettingThrottleMultiplicator));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for rudder");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderControllerIndex = {0}", this.JoystickSettingRudderControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller axis used for rudder");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderControllerAxisIndex = {0}", this.JoystickSettingRudderControllerAxisIndex));
                writer.WriteLine();
                writer.WriteLine("; Invert rudder axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "InvertRudder = {0}", this.JoystickSettingInvertRudder ? 1 : 0));
                writer.WriteLine();
                writer.WriteLine("; Multiply rudder axis");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderMultiplicator = {0}", this.JoystickSettingRudderMultiplicator));
                writer.WriteLine();
                writer.WriteLine("; Use the POV as regular buttons");
                writer.WriteLine("; 0 to continious movement");
                writer.WriteLine("; 1 to enable regular buttons");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "UsePovControllerAsButtons = {0}", this.JoystickSettingUsePovControllerAsButtons ? 1 : 0));
                writer.WriteLine();
                writer.WriteLine("; Virtual cockpit look sensitivity");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "VirtualCockpitLookSensitivity = {0}", this.JoystickSettingVirtualCockpitLookSensitivity));
                writer.WriteLine();

                foreach (var item in this.JoystickConfigButtons)
                {
                    if (JoystickKeys.Keys.TryGetValue(item.Value, out string value))
                    {
                        writer.WriteLine("; " + value);
                    }

                    writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "{0} = {1}", item.Key, item.Value));
                }

                writer.WriteLine();
            }
        }

        private List<JoystickController> ReadJoystickControllers()
        {
            var controllers = new List<JoystickController>();

            int count = NativeMethods.JoyGetNumDevs();
            int size = Marshal.SizeOf(typeof(JoyCaps));
            int controllerIndex = -1;

            for (int deviceIndex = 0; deviceIndex < count; deviceIndex++)
            {
                if (NativeMethods.JoyGetDevCaps(new IntPtr(deviceIndex), out JoyCaps caps, size) != 0)
                {
                    continue;
                }

                controllerIndex++;

                var controller = new JoystickController(deviceIndex, controllerIndex, caps);
                controllers.Add(controller);
            }

            return controllers;
        }

        private void TimerFunction(object sender, System.Timers.ElapsedEventArgs e)
        {
            int count = NativeMethods.JoyGetNumDevs();
            int sizeCaps = Marshal.SizeOf(typeof(JoyCaps));
            int sizeInfo = Marshal.SizeOf(typeof(JoyInfoEx));
            int controllerIndex = -1;

            var pressedKeys = new List<string>();

            for (int deviceIndex = 0; deviceIndex < count; deviceIndex++)
            {
                if (NativeMethods.JoyGetDevCaps(new IntPtr(deviceIndex), out JoyCaps caps, sizeCaps) != 0)
                {
                    continue;
                }

                JoyInfoEx info = new JoyInfoEx
                {
                    dwSize = (uint)sizeInfo,
                    dwOptions = JoyInfoExOptions.ReturnAll | JoyInfoExOptions.ReturnCentered | JoyInfoExOptions.UseDeadZone
                };

                if (NativeMethods.JoyGetPosEx(deviceIndex, ref info) != 0)
                {
                    continue;
                }

                controllerIndex++;

                int index1 = controllerIndex;
                int index2 = caps.ManufacturerID << 16 | caps.ProductID;

                for (int i = 0; i < 6; i++)
                {
                    string controllerName = (string)ControllerNameConverter.Default.Convert(new object[] { this.JoystickControllers, controllerIndex }, null, null, null);
                    string controller = string.Format(CultureInfo.InvariantCulture, "{0} : {1}", index1, index2);
                    string axis = string.Format(CultureInfo.InvariantCulture, "{0}", i);
                    uint position = 0;

                    switch (i)
                    {
                        case 0:
                            position = info.dwXpos;
                            break;

                        case 1:
                            position = info.dwYpos;
                            break;

                        case 2:
                            position = info.dwZpos;
                            break;

                        case 3:
                            position = info.dwRpos;
                            break;

                        case 4:
                            position = info.dwUpos;
                            break;

                        case 5:
                            position = info.dwVpos;
                            break;
                    }

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
                        this.JoystickConfigAxes[index].Position = (int)position;
                    }
                    else
                    {
                        this.Dispatcher.Invoke(() => this.JoystickConfigAxes.Add(new JoystickConfigAxis(controller, controllerName, axis) { Position = (int)position }));
                    }
                }

                for (int i = 0; i < caps.wNumButtons; i++)
                {
                    if ((info.dwButtons & 1U << i) != 0)
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
                button.IsPressed = false;

                foreach (string pressedKey in pressedKeys)
                {
                    if (string.Equals(button.Key, pressedKey, StringComparison.Ordinal))
                    {
                        button.IsPressed = true;
                        break;
                    }
                }
            }
        }
    }
}
