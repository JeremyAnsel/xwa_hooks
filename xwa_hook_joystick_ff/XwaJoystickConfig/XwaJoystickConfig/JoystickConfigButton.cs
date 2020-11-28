using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    public class JoystickConfigButton
    {
        public JoystickConfigButton()
        {
        }

        public JoystickConfigButton(string key, int value)
        {
            this.Key = key;
            this.Value = value;
        }

        public string Key { get; set; }

        public int Value { get; set; }
    }
}
