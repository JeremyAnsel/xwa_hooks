using System.Globalization;

namespace XwaJoystickConfig.CustomConverters;

internal static class FloatConverter
{
    public static string Convert(float value)
    {
        return value.ToString(CultureInfo.InvariantCulture);
    }

    public static float ConvertBack(string text)
    {
        if (float.TryParse(text, CultureInfo.InvariantCulture, out float result))
        {
            return result;
        }

        return 0;
    }
}
