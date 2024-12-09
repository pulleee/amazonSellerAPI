namespace SelectLineWAWIApiCore.Server.Amazon
{
    public class Region
    {
        public string RegionName { get; set; }
        public string HostUrl { get; set; }
        public string SandboxHostUrl { get; set; }

        public Region(string regionName, string hostUrl, string sandboxHostUrl)
        {
            RegionName = regionName;
            HostUrl = hostUrl;
            SandboxHostUrl = sandboxHostUrl;
        }
    }
}
