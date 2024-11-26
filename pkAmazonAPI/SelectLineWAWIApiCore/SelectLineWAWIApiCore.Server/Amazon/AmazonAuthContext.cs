using Amazon.SellingPartnerAPIAA;

namespace SelectLineWAWIApiCore.Server.Amazon
{
    public class AmazonAuthContext
    {
        public static readonly string SECTION = "AmazonAuth";        

        public LWAAuthorizationCredentials LWAAuthorizationCredentials { get; set; }

        public AmazonAuthContext(IConfiguration configuration)
        {
            var config = configuration.GetSection(SECTION).GetChildren();

            LWAAuthorizationCredentials = new LWAAuthorizationCredentials
            {
                ClientId = config.FirstOrDefault(x => x.Key == "ClientId")?.Value,
                ClientSecret = config.FirstOrDefault(x => x.Key == "ClientSecret")?.Value,
                RefreshToken = config.FirstOrDefault(x => x.Key == "RefreshToken")?.Value,
                Endpoint = new Uri(config.FirstOrDefault(x => x.Key == "AuthEndpoint")?.Value)
            };
        }
    }
}
