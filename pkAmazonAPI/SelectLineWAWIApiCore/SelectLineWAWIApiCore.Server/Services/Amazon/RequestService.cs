using Azure.Core;
using Newtonsoft.Json;
using RestSharp;
using RestSharp.Serializers.Json;
using SelectLineWAWIApiCore.Server.Amazon;
using SelectLineWAWIApiCore.Server.Amazon.Util;
using SelectLineWAWIApiCore.Server.Models;
using SelectLineWAWIApiCore.Server.Models.Token;
using SellingPartnerAPI.SellerAPI.Model;

namespace SelectLineWAWIApiCore.Server.Services.Amazon
{
    public class RequestService
    {
        protected AmazonAuthContext AmazonContext { get; set; }
        protected RestRequest Request { get; set; }
        private ApiUrls ApiUrls { get; set; }

        protected string ApiBaseUrl
        {
            get
            {
                return AmazonContext.Environment == EnviromentManager.Environments.Sandbox ? AmazonContext.Region.SandboxHostUrl : AmazonContext.Region.SandboxHostUrl;
            }
        }

        public RequestService(AmazonAuthContext context)
        {
            AmazonContext = context;
            ApiUrls = new ApiUrls();
        }

        private void CreateRequest(string url, RestSharp.Method method)
        {
            var options = new RestClientOptions(ApiBaseUrl);
            var client = new RestClient(options,
                configureSerialization: s => s.UseSystemTextJson());

            Request = new RestRequest(url, method);
        }

        protected async Task CreateAuthorizedRequestAsync(string url, RestSharp.Method method,
            List<KeyValuePair<string, string>> queryParameters = null, object postJsonObj = null,
            TokenDataType tokenDataType = TokenDataType.Normal, object parameter = null,
            CancellationToken cancellationToken = default)
        {
            var piiObject = parameter as IParameterBasedPII;
            if (piiObject != null && piiObject.NeedRDT)
                await RefreshTokenAsync(TokenDataType.PII, piiObject.RDTRequest, cancellationToken);
            else
                await RefreshTokenAsync(tokenDataType, cancellationToken: cancellationToken);
            CreateRequest(url, method);
            if (postJsonObj != null)
                AddJsonBody(postJsonObj);
            if (queryParameters != null)
                AddQueryParameters(queryParameters);
        }

        protected async Task RefreshTokenAsync(TokenDataType tokenDataType = TokenDataType.Normal,
            CreateRestrictedDataTokenRequest requestPII = null, CancellationToken cancellationToken = default)
        {
                
            var token = AmazonContext.GetToken(tokenDataType);
            if (token == null)
            {
                if (tokenDataType == TokenDataType.PII)
                {
                    var pii = await CreateRestrictedDataTokenAsync(requestPII, cancellationToken);
                    if (pii != null)
                    {
                        token = new TokenResponse()
                        {
                            access_token = pii.RestrictedDataToken,
                            expires_in = pii.ExpiresIn
                        };
                    }
                    else
                    {
                        throw new ArgumentNullException(nameof(pii));
                    }
                }
                else
                {
                    token = await TokenGeneration.RefreshAccessTokenAsync(AmazonCredential, tokenDataType,
                        cancellationToken);
                }

                AmazonCredential.SetToken(tokenDataType, token);
            }


            AccessToken = token.access_token;
        }

        public async Task<CreateRestrictedDataTokenResponse> CreateRestrictedDataTokenAsync(
            CreateRestrictedDataTokenRequest createRestrictedDataTokenRequest,
            CancellationToken cancellationToken = default)
        {
            await CreateAuthorizedRequestAsync(TokenApiUrls.RestrictedDataToken, RestSharp.Method.Post,
                postJsonObj: createRestrictedDataTokenRequest, cancellationToken: cancellationToken);
            var response = await ExecuteRequestAsync<CreateRestrictedDataTokenResponse>(
                RateLimitType.Token_CreateRestrictedDataToken, cancellationToken: cancellationToken);
            return response;
        }

        public async Task<T> ExecuteRequestAsync<T>(RateLimitType rateLimitType = RateLimitType.UNSET,
            CancellationToken cancellationToken = default) where T : new()
        {
            var tryCount = 0;
            while (true)
            {
                try
                {
                    return await ExecuteRequestTry<T>(rateLimitType, cancellationToken);
                }
                catch (AmazonQuotaExceededException ex)
                {
                    if (tryCount >= AmazonCredential.MaxThrottledRetryCount)
                    {
                        if (AmazonCredential.IsDebugMode)
                            Console.WriteLine("Throttle max try count reached");

                        throw;
                    }

                    cancellationToken.ThrowIfCancellationRequested();

                    await AmazonCredential.UsagePlansTimings[rateLimitType].Delay();
                    tryCount++;
                }
            }
        }

        protected void AddQueryParameters(List<KeyValuePair<string, string>> queryParameters)
        {
            if (queryParameters != null)
                queryParameters.ForEach(qp => Request.AddQueryParameter(qp.Key, qp.Value));
        }

        protected void AddLimitHeader(int limit)
        {
            Request.AddQueryParameter("limit", limit.ToString());
        }

        protected void AddJsonBody(object jsonData)
        {
            var json = JsonConvert.SerializeObject(jsonData);
            Request.AddJsonBody(json);
        }

        protected void AddAccessToken()
        {
            lock (Request)
            {
                Request.AddOrUpdateHeader(AccessTokenHeaderName, AccessToken);
            }
        }
    }
}
