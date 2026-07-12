using System.Globalization;

namespace XwaJoystickConfig.CustomConverters;

internal static class BoolConverter
{
    public static string Convert(bool value)
    {
        return value.ToString(CultureInfo.InvariantCulture);
    }

    public static bool ConvertBack(string text)
    {
        if (bool.TryParse(text, out bool result))
        {
            return result;
        }

        return false;
    }
}
