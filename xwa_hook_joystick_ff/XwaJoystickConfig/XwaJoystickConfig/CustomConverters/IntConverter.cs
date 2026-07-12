using System.Globalization;

namespace XwaJoystickConfig.CustomConverters;

internal static class IntConverter
{
    public static string Convert(int value)
    {
        return value.ToString(CultureInfo.InvariantCulture);
    }

    public static int ConvertBack(string text)
    {
        if (int.TryParse(text, CultureInfo.InvariantCulture, out int result))
        {
            return result;
        }

        return 0;
    }
}
