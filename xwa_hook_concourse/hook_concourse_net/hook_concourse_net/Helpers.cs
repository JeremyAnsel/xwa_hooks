using System.Text;

namespace hook_concourse_net
{
    internal static class Helpers
    {
        public static string StrFormatByteSize(long filesize)
        {
            var sb = new StringBuilder(11);
            NativeMethods.StrFormatByteSize(filesize, sb, sb.Capacity);
            return sb.ToString();
        }
    }
}
