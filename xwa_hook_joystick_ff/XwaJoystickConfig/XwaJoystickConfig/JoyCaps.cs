using JeremyAnsel.DirectX.DXCommon;

namespace XwaJoystickConfig;

internal sealed class JoyCaps
{
    public const int Size = 728;

    public JoyCaps()
    {
    }

    public unsafe JoyCaps(nint buffer)
    {
        ManufacturerID = DXMarshal.ReadUnsignedInt16(ref buffer);
        ProductID = DXMarshal.ReadUnsignedInt16(ref buffer);
        ProductName = new string((char*)buffer);
        buffer += 32 * 2;
        wXmin = DXMarshal.ReadInt32(ref buffer);
        wXmax = DXMarshal.ReadInt32(ref buffer);
        wYmin = DXMarshal.ReadInt32(ref buffer);
        wYmax = DXMarshal.ReadInt32(ref buffer);
        wZmin = DXMarshal.ReadInt32(ref buffer);
        wZmax = DXMarshal.ReadInt32(ref buffer);
        wNumButtons = DXMarshal.ReadInt32(ref buffer);
        wPeriodMin = DXMarshal.ReadInt32(ref buffer);
        wPeriodMax = DXMarshal.ReadInt32(ref buffer);
        wRmin = DXMarshal.ReadInt32(ref buffer);
        wRmax = DXMarshal.ReadInt32(ref buffer);
        wUmin = DXMarshal.ReadInt32(ref buffer);
        wUmax = DXMarshal.ReadInt32(ref buffer);
        wVmin = DXMarshal.ReadInt32(ref buffer);
        wVmax = DXMarshal.ReadInt32(ref buffer);
        wCaps = (JoyDriverCaps)DXMarshal.ReadInt32(ref buffer);
        wMaxAxes = DXMarshal.ReadInt32(ref buffer);
        wNumAxes = DXMarshal.ReadInt32(ref buffer);
        wMaxButtons = DXMarshal.ReadInt32(ref buffer);
        RegistryKey = new string((char*)buffer);
        buffer += 32 * 2;
        OemVxdName = new string((char*)buffer);
        buffer += 260 * 2;
    }

    public ushort ManufacturerID { get; set; }

    public ushort ProductID { get; set; }

    //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
    public string ProductName { get; set; } = string.Empty;

    public int wXmin { get; set; }               /* minimum x position value */

    public int wXmax { get; set; }               /* maximum x position value */

    public int wYmin { get; set; }               /* minimum y position value */

    public int wYmax { get; set; }               /* maximum y position value */

    public int wZmin { get; set; }               /* minimum z position value */

    public int wZmax { get; set; }               /* maximum z position value */

    public int wNumButtons { get; set; }         /* number of buttons */

    public int wPeriodMin { get; set; }          /* minimum message period when captured */

    public int wPeriodMax { get; set; }          /* maximum message period when captured */

    public int wRmin { get; set; }               /* minimum r position value */

    public int wRmax { get; set; }               /* maximum r position value */

    public int wUmin { get; set; }               /* minimum u (5th axis) position value */

    public int wUmax { get; set; }               /* maximum u (5th axis) position value */

    public int wVmin { get; set; }               /* minimum v (6th axis) position value */

    public int wVmax { get; set; }               /* maximum v (6th axis) position value */

    public JoyDriverCaps wCaps { get; set; }               /* joystick capabilites */

    public int wMaxAxes { get; set; }            /* maximum number of axes supported */

    public int wNumAxes { get; set; }            /* number of axes in use */

    public int wMaxButtons { get; set; }         /* maximum number of buttons supported */

    //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
    public string RegistryKey { get; set; } = string.Empty;

    //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
    public string OemVxdName { get; set; } = string.Empty;
}
