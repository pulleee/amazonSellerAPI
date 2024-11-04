using System.Collections.ObjectModel;

namespace SelectLineWAWIApi.Services.Logging
{
    public enum LogLevel
    {
        Info,
        Warning,
        Error,
    }

    public class LogService
    {
        public ObservableCollection<LogItem> Logs { get; private set; } = new ObservableCollection<LogItem>();

        public void AddLog(LogLevel logLevel, string message)
        {
            Logs.Add(new LogItem(logLevel, message));
        }

        public void ClearLogs()
        {
            Logs.Clear();
        }
    }
}
