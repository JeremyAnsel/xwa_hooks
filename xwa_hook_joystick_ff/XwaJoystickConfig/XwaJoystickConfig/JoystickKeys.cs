using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    static class JoystickKeys
    {
        static JoystickKeys()
        {
            using (var reader = new StringReader(JoystickKeysResources.JoystickTxt))
            {
                string line;

                while ((line = reader.ReadLine()) != null)
                {
                    if (string.IsNullOrWhiteSpace(line))
                    {
                        continue;
                    }

                    int s1 = line.Length <= 1 ? -1 : line.IndexOf('!', 1);

                    if (s1 == -1)
                    {
                        continue;
                    }

                    int s2 = line.Length <= s1 + 1 ? -1 : line.IndexOf(' ', s1 + 1);

                    if (s2 == -1)
                    {
                        continue;
                    }

                    string keyString = line.Substring(s1 + 1, s2 - s1 - 1);
                    int key = int.Parse(keyString, CultureInfo.InvariantCulture);
                    string description = line.Substring(s2 + 1);

                    Keys.Add(key, description);
                }
            }
        }

        public static readonly Dictionary<int, string> Keys = new Dictionary<int, string>();
    }
}
