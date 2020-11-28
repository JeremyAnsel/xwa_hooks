using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace XwaJoystickConfig
{
    public class DictionaryKeyConverter : IValueConverter
    {
        public static readonly DictionaryKeyConverter Default = new DictionaryKeyConverter();

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (!(parameter is Dictionary<int, string> keys))
            {
                return -1;
            }

            int key = (int)value;

            int keyIndex = 0;

            foreach (int k in keys.Keys)
            {
                if (k == key)
                {
                    return keyIndex;
                }

                keyIndex++;
            }

            return -1;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (!(parameter is Dictionary<int, string> keys))
            {
                return 0;
            }

            int index = (int)value;

            KeyValuePair<int, string> item = keys.ElementAtOrDefault(index);

            return item.Key;
        }
    }
}
