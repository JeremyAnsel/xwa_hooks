namespace XwaJoystickConfig;

public static class DictionaryKeyConverter
{
    public static int Convert(int key, Dictionary<int, string> keys)
    {
        int keyIndex = 0;

        foreach (int k in keys.Keys)
        {
            if (k == key)
            {
                return keyIndex;
            }

            keyIndex++;
        }

        return -1;
    }

    public static int ConvertBack(int index, Dictionary<int, string> keys)
    {
        KeyValuePair<int, string> item = keys.ElementAtOrDefault(index);

        return item.Key;
    }
}
