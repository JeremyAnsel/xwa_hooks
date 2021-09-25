using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    [Flags]
    internal enum JoyInfoExOptions
    {
        ReturnX = 0x00000001,

        ReturnY = 0x00000002,

        ReturnZ = 0x00000004,

        ReturnR = 0x00000008,

        ReturnU = 0x00000010,

        ReturnV = 0x00000020,

        ReturnPov = 0x00000040,

        ReturnButtons = 0x00000080,

        ReturnRawData = 0x00000100,

        ReturnPovCts = 0x00000200,

        ReturnCentered = 0x00000400,

        UseDeadZone = 0x00000800,

        ReturnAll = ReturnX | ReturnY | ReturnZ | ReturnR | ReturnU | ReturnV | ReturnPov | ReturnButtons
    }
}
