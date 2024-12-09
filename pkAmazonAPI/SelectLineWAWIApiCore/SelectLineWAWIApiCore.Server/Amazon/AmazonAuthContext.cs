using Amazon.SellingPartnerAPIAA;
using Microsoft.Identity.Client;
using SelectLineWAWIApiCore.Server.Amazon.Util;
using SelectLineWAWIApiCore.Server.Models.Token;

namespace SelectLineWAWIApiCore.Server.Amazon
{
    public class AmazonAuthContext
    {
        public static readonly string SECTION = "AmazonAuth";

        public EnviromentManager.Environments Environment => EnviromentManager.Environemnt;
        public LWAAuthorizationCredentials LWAAuthorizationCredentials { get; private set; }
        public Region Region { get; private set; }

        private TokenDataCache tokenCache { get; set; }

        public AmazonAuthContext(IConfiguration configuration)
        {
            tokenCache = new TokenDataCache();

            var config = configuration.GetSection(SECTION);

            LWAAuthorizationCredentials = new LWAAuthorizationCredentials
            {
                ClientId = config.GetValue<string>("ClientId"),
                ClientSecret = config.GetValue<string>("ClientSecret"),
                RefreshToken = config.GetValue<string>("RefreshToken"),
                Endpoint = new Uri(config.GetValue<string>("AuthEndpoint"))
            };
            tokenCache.SetAWSAuthenticationTokenData(LWAAuthorizationCredentials);
            Region = Constants.Europe;
        }

        public TokenResponse GetToken(TokenDataType tokenDataType)
        {
            return tokenCache.GetToken(tokenDataType);
        }

        public void SetToken(TokenDataType tokenDataType, TokenResponse token)
        {
            tokenCache.SetToken(tokenDataType, token);
        }

        public LWAAuthorizationCredentials GetAWSAuthenticationTokenData()
        {
            return tokenCache.GetLWAAuthorizationCredentials();
        }

        public void SetAWSAuthenticationTokenData(LWAAuthorizationCredentials tokenData)
        {
            tokenCache.SetAWSAuthenticationTokenData(tokenData);
        }
    }
}
