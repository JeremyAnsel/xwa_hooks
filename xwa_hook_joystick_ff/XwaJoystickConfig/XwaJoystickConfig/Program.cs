using Aprillz.MewUI;
using System.Text;
using XwaJoystickConfig;

AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
Application.DispatcherUnhandledException += Application_DispatcherUnhandledException;

Win32Platform.Register();
Direct2DBackend.Register();

ThemeManager.Default = ThemeVariant.Dark;

Window window = new MainWindow();

Application.Run(window);

static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
{
    var sb = new StringBuilder();

    sb.AppendLine("CurrentDomain_UnhandledException");
    sb.AppendLine(e.ToString());
    sb.AppendLine((e.ExceptionObject as Exception)?.ToString());

    NativeMessageBox.Show(sb.ToString());
}

void Application_DispatcherUnhandledException(DispatcherUnhandledExceptionEventArgs obj)
{
    var sb = new StringBuilder();

    sb.AppendLine("Application_DispatcherUnhandledException");
    sb.AppendLine(obj.Exception.ToString());

    NativeMessageBox.Show(sb.ToString());
}
