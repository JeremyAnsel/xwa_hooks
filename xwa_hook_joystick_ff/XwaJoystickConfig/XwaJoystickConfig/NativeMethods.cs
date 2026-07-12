using System.Runtime.InteropServices;
using System.Security;

namespace XwaJoystickConfig;

[SecurityCritical, SuppressUnmanagedCodeSecurity]
internal static unsafe partial class NativeMethods
{
    [LibraryImport("Winmm.dll", EntryPoint = "joyGetNumDevs")]
    public static partial int JoyGetNumDevs();

    [LibraryImport("Winmm.dll", EntryPoint = "joyGetDevCapsW")]
    private static partial int JoyGetDevCaps(IntPtr uJoyID, nint pjc, int cbjc);

    public static int JoyGetDevCaps(IntPtr uJoyID, out JoyCaps pjc)
    {
        byte* buffer = stackalloc byte[JoyCaps.Size];
        int result = JoyGetDevCaps(uJoyID, (nint)buffer, JoyCaps.Size);
        pjc = new JoyCaps((nint)buffer);
        return result;
    }

    [LibraryImport("Winmm.dll", EntryPoint = "joyGetPosEx")]
    public static partial int JoyGetPosEx(int uJoyID, ref JoyInfoEx pji);
}
