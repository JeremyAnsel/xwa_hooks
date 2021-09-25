using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    internal struct JoyInfoEx
    {
        public uint dwSize;

        public JoyInfoExOptions dwOptions;

        public uint dwXpos;

        public uint dwYpos;

        public uint dwZpos;

        public uint dwRpos;

        public uint dwUpos;

        public uint dwVpos;

        public uint dwButtons;

        public uint dwButtonNumber;

        public uint dwPOV;

        public uint dwReserved1;

        public uint dwReserved2;
    }
}
