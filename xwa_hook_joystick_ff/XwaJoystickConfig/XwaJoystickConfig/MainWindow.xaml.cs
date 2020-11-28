using Microsoft.Win32;
using System;
using System.Collections.Generic;
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
        private static readonly Encoding _encoding = Encoding.GetEncoding("iso-8859-1");

        public MainWindow()
        {
            InitializeComponent();

#if !DEBUG
            Application.Current.DispatcherUnhandledException += Current_DispatcherUnhandledException;
#endif

            if (System.IO.File.Exists("JoystickConfig.txt"))
            {
                this.ReadJoystickConfig("JoystickConfig.txt");
            }
            else
            {
                this.SetDefaultSettings();
                this.SetDefaultJoystickConfigButtons(0);
            }

            this.JoystickControllers = this.ReadJoystickControllers();

            this.Update();
        }

        public int JoystickSettingFFDeviceIndex { get; set; }

        public bool JoystickSettingEnableSmallMovement { get; set; }

        public int JoystickSettingMainControllerIndex { get; set; }

        public int JoystickSettingThrottleControllerIndex { get; set; }

        public int JoystickSettingThrottleControllerAxisIndex { get; set; }

        public int JoystickSettingRudderControllerIndex { get; set; }

        public int JoystickSettingRudderControllerAxisIndex { get; set; }

        public bool JoystickSettingUsePovControllerAsButtons { get; set; }

        public List<JoystickConfigButton> JoystickConfigButtons { get; } = new List<JoystickConfigButton>();

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

        private void InitButtonsButton_Click(object sender, RoutedEventArgs e)
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

                for (int i = 0; i < caps.wNumButtons; i++)
                {
                    string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", controllerIndex, i + 1);
                    int value = GetDefaultConfigButton(buttonIndex + 1);
                    buttonIndex++;

                    this.JoystickConfigButtons.Add(new JoystickConfigButton(key, value));
                }

                if (caps.wCaps.HasFlag(JoyDriverCaps.HasPov))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", controllerIndex, i + 1);
                        int value = GetDefaultConfigPov(povIndex + 1);
                        povIndex++;

                        this.JoystickConfigButtons.Add(new JoystickConfigButton(key, value));
                    }
                }
            }

            this.Update();
        }

        private void SetDefaultSettings()
        {
            this.JoystickSettingFFDeviceIndex = 0;
            this.JoystickSettingEnableSmallMovement = true;
            this.JoystickSettingMainControllerIndex = 0;
            this.JoystickSettingThrottleControllerIndex = 0;
            this.JoystickSettingThrottleControllerAxisIndex = 2;
            this.JoystickSettingRudderControllerIndex = 0;
            this.JoystickSettingRudderControllerAxisIndex = 3;
            this.JoystickSettingUsePovControllerAsButtons = false;
        }

        private void SetDefaultJoystickConfigButtons(int controllerIndex)
        {
            this.JoystickConfigButtons.Clear();

            for (int i = 0; i < 16; i++)
            {
                string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_{1}", controllerIndex, i + 1);
                int value = GetDefaultConfigButton(i + 1);

                this.JoystickConfigButtons.Add(new JoystickConfigButton(key, value));
            }

            for (int i = 0; i < 4; i++)
            {
                string key = string.Format(CultureInfo.InvariantCulture, "joybutton_{0}_pov{1}", controllerIndex, i + 1);
                int value = GetDefaultConfigPov(i + 1);

                this.JoystickConfigButtons.Add(new JoystickConfigButton(key, value));
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
            this.JoystickSettingMainControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "MainControllerIndex", 0);
            this.JoystickSettingThrottleControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "ThrottleControllerIndex", 0);
            this.JoystickSettingThrottleControllerAxisIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "ThrottleControllerAxisIndex", 2);
            this.JoystickSettingRudderControllerIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "RudderControllerIndex", 0);
            this.JoystickSettingRudderControllerAxisIndex = XwaHooksConfig.GetFileKeyValueInt(lines, "RudderControllerAxisIndex", 3);
            this.JoystickSettingUsePovControllerAsButtons = XwaHooksConfig.GetFileKeyValueInt(lines, "UsePovControllerAsButtons", 0) != 0;

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

                if (key.StartsWith("joybutton_", StringComparison.OrdinalIgnoreCase))
                {
                    this.JoystickConfigButtons.Add(new JoystickConfigButton(key, value));
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
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for the main axis and buttons");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "MainControllerIndex = {0}", this.JoystickSettingMainControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for throttle");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleControllerIndex = {0}", this.JoystickSettingThrottleControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller axis used for throttle");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "ThrottleControllerAxisIndex = {0}", this.JoystickSettingThrottleControllerAxisIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller used for rudder");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderControllerIndex = {0}", this.JoystickSettingRudderControllerIndex));
                writer.WriteLine();
                writer.WriteLine("; Select the controller axis used for rudder");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "RudderControllerAxisIndex = {0}", this.JoystickSettingRudderControllerAxisIndex));
                writer.WriteLine();
                writer.WriteLine("; Use the POV as regular buttons");
                writer.WriteLine("; 0 to continious movement");
                writer.WriteLine("; 1 to enable regular buttons");
                writer.WriteLine(string.Format(CultureInfo.InvariantCulture, "UsePovControllerAsButtons = {0}", this.JoystickSettingUsePovControllerAsButtons ? 1 : 0));
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
    }
}
