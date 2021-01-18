using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace XwaJoystickConfig
{
    class ControllerNameConverter : IMultiValueConverter
    {
        public static readonly ControllerNameConverter Default = new ControllerNameConverter();

        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if (values.Length < 2)
            {
                return null;
            }

            if (!(values[0] is IList<JoystickController> controllers))
            {
                return null;
            }


            int value;

            if (values[1] is int valueInt)
            {
                value = valueInt;
            }
            else if (values[1] is string valueString)
            {
                if (!int.TryParse(valueString, out value))
                {
                    return null;
                }
            }
            else
            {
                return null;
            }

            if (value == -1)
            {
                return "Undefined";
            }

            JoystickController controller;

            if (value >= 0 && value < 100)
            {
                controller = controllers.ElementAtOrDefault(value);
            }
            else
            {
                controller = controllers.FirstOrDefault(t => t.Id == value);
            }

            if (controller == null)
            {
                return "Unknown";
            }

            string name = controller.Name;

            return name;
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
