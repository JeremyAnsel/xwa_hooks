using System.Globalization;

namespace XwaJoystickConfig;

public static class XwaHooksConfigEx
{
    public static float GetFileKeyValueFloat(IList<string> lines, string key, float defaultValue = 0)
    {
        string value = XwaHooksConfig.GetFileKeyValue(lines, key);

        if (value.Length == 0)
        {
            return defaultValue;
        }

        if (!float.TryParse(value, NumberStyles.Float | NumberStyles.AllowThousands, NumberFormatInfo.InvariantInfo, out float result))
        {
            return defaultValue;
        }

        return result;
    }
}
