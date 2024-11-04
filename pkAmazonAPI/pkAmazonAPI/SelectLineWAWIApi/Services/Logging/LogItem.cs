namespace SelectLineWAWIApi.Services.Logging
{
    public class LogItem
    {
        public string Message { get; private set; }
        public DateTime Timestamp { get; private set; }
        public LogLevel LogLevel { get; private set; }        

        public LogItem(LogLevel logLevel, string message)
        {
            Message = message;
            LogLevel = logLevel;
            Timestamp = DateTime.Now;
        }

        public override string ToString()
        {          
            return $"[{Timestamp.ToString("HH:mm:ss")}] [{LogLevel}] {Message}";
        }
    }
}
