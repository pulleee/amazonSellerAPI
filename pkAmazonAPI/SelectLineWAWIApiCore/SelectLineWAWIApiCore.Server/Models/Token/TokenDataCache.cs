using Amazon.SellingPartnerAPIAA;

namespace SelectLineWAWIApiCore.Server.Models.Token
{
    public class TokenDataCache
    {
        protected TokenResponse NormalAccessToken { get; set; }
        protected TokenResponse PIIAccessToken { get; set; }
        protected TokenResponse GrantlessAccessToken { get; set; }
        protected LWAAuthorizationCredentials LWAAuthorizationCredentials { get; set; }

        public LWAAuthorizationCredentials GetLWAAuthorizationCredentials()
        {
            if (LWAAuthorizationCredentials == null) 
                return null;
            
            return LWAAuthorizationCredentials;
        }
        public void SetAWSAuthenticationTokenData(LWAAuthorizationCredentials data)
        {
            LWAAuthorizationCredentials = data;
        }
        public TokenResponse GetToken(TokenDataType tokenDataType)
        {
            TokenResponse token = null;
            if (tokenDataType == TokenDataType.Normal)
            {
                token = NormalAccessToken;
            }
            else if (tokenDataType == TokenDataType.PII)
            {
                token = PIIAccessToken;
            }
            else if (tokenDataType == TokenDataType.Grantless)
            {
                token = GrantlessAccessToken;
            }
            if (token == null)
            {
                return null;
            }
            else
            {
                var isExpired = IsTokenExpired(token.expires_in, token.date_Created);

                if (!isExpired)
                {
                    return token;
                }
                else
                {
                    return null;
                }
            }
        }

        public void SetToken(TokenDataType tokenDataType, TokenResponse token)
        {

            if (tokenDataType == TokenDataType.Normal)
            {
                NormalAccessToken = token;
            }
            else if (tokenDataType == TokenDataType.PII)
            {
                // Its not valid to save PII token and reuse it with other request
                //PIIAccessToken = token;
            }
            else if (tokenDataType == TokenDataType.Grantless)
            {
                GrantlessAccessToken = token;
            }
        }

        public static bool IsTokenExpired(int? expiresIn, DateTime? dateCreated)
        {
            if (dateCreated == null)
                return false;
            else
                return DateTime.UtcNow.Subtract((DateTime)dateCreated).TotalSeconds > expiresIn - 60; //Add Margent to a void expaired token
        }
    }
}
