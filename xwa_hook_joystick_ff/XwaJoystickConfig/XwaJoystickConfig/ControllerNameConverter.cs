namespace XwaJoystickConfig;

public static class ControllerNameConverter
{
    public static string Convert(IList<JoystickController> controllers, object valueObject)
    {
        int value;

        if (valueObject is int valueInt)
        {
            value = valueInt;
        }
        else if (valueObject is string valueString)
        {
            if (!int.TryParse(valueString, out value))
            {
                return string.Empty;
            }
        }
        else
        {
            return string.Empty;
        }

        if (value == -1)
        {
            return "Undefined";
        }

        JoystickController? controller;

        if (value >= 0 && value < 100)
        {
            controller = controllers.ElementAtOrDefault(value);
        }
        else
        {
            controller = controllers.FirstOrDefault(t => t.Id == value);
        }

        if (controller == null)
        {
            return "Unknown";
        }

        string name = controller.Name;

        return name;
    }
}
