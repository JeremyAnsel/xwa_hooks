using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    public class JoystickController
    {
        internal JoystickController(int deviceIndex, int controllerIndex, JoyCaps caps)
        {
            this.Id = caps.ManufacturerID << 16 | caps.ProductID;

            this.DeviceIndex = deviceIndex;
            this.ControllerIndex = controllerIndex;
            this.AxesCount = caps.wNumAxes;
            this.ButtonsCount = caps.wNumButtons;

            string name = this.ReadName(caps);
            this.Name = string.IsNullOrEmpty(name) ? caps.ProductName : name;
        }

        public int Id { get; }

        public int DeviceIndex { get; }

        public int ControllerIndex { get; }

        public string Name { get; }

        public int AxesCount { get; }

        public int ButtonsCount { get; }

        private string ReadName(JoyCaps caps)
        {
            string currentJoystickSettingsKey = $"HKEY_CURRENT_USER\\System\\CurrentControlSet\\Control\\MediaResources\\Joystick\\{caps.RegistryKey}\\CurrentJoystickSettings";
            string currentJoystickSettingsValue = $"Joystick{(this.DeviceIndex + 1).ToString(CultureInfo.InvariantCulture)}OEMName";
            string oemName = (string)Registry.GetValue(currentJoystickSettingsKey, currentJoystickSettingsValue, string.Empty);

            if (string.IsNullOrEmpty(oemName))
            {
                return null;
            }

            string joystickPropertiesKey = "HKEY_CURRENT_USER\\System\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM\\" + oemName;
            string joystickPropertiesValue = "OEMName";
            string name = (string)Registry.GetValue(joystickPropertiesKey, joystickPropertiesValue, string.Empty);

            if (string.IsNullOrEmpty(name))
            {
                return null;
            }

            return name;
        }
    }
}
