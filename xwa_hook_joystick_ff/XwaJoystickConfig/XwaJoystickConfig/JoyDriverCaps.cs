using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    [Flags]
    internal enum JoyDriverCaps
    {
        HasZ = 0x0001,

        HasR = 0x0002,

        HasU = 0x0004,

        HasV = 0x0008,

        HasPov = 0x0010,

        Pov4Dir = 0x0020,

        PovCts = 0x0040
    }
}
