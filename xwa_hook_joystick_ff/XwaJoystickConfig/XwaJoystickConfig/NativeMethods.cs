using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    [SecurityCritical, SuppressUnmanagedCodeSecurity]
    internal static class NativeMethods
    {
        [DllImport("Winmm.dll", EntryPoint = "joyGetNumDevs")]
        public static extern int JoyGetNumDevs();

        [DllImport("Winmm.dll", EntryPoint = "joyGetDevCapsW")]
        public static extern int JoyGetDevCaps(IntPtr uJoyID, out JoyCaps pjc, int cbjc);

        [DllImport("Winmm.dll", EntryPoint = "joyGetPosEx")]
        public static extern int JoyGetPosEx(int uJoyID, ref JoyInfoEx pji);
    }
}
