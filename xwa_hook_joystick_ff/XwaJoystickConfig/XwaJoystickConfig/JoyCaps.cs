using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct JoyCaps
    {
        public short ManufacturerID;

        public short ProductID;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string ProductName;

        public int wXmin;               /* minimum x position value */

        public int wXmax;               /* maximum x position value */

        public int wYmin;               /* minimum y position value */

        public int wYmax;               /* maximum y position value */

        public int wZmin;               /* minimum z position value */

        public int wZmax;               /* maximum z position value */

        public int wNumButtons;         /* number of buttons */

        public int wPeriodMin;          /* minimum message period when captured */

        public int wPeriodMax;          /* maximum message period when captured */

        public int wRmin;               /* minimum r position value */

        public int wRmax;               /* maximum r position value */

        public int wUmin;               /* minimum u (5th axis) position value */

        public int wUmax;               /* maximum u (5th axis) position value */

        public int wVmin;               /* minimum v (6th axis) position value */

        public int wVmax;               /* maximum v (6th axis) position value */

        [MarshalAs(UnmanagedType.I4)]
        public JoyDriverCaps wCaps;               /* joystick capabilites */

        public int wMaxAxes;            /* maximum number of axes supported */

        public int wNumAxes;            /* number of axes in use */

        public int wMaxButtons;         /* maximum number of buttons supported */

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string RegistryKey;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string OemVxdName;
    }
}
