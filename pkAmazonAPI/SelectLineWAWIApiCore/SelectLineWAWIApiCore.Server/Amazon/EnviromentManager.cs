namespace SelectLineWAWIApiCore.Server.Amazon.Util
{
    public static class EnvironentManager
    {
        public static Environments Environemnt { get; set; } = Environments.Production;
        public enum Environments
        {
            Sandbox, Production
        }
    }
}
