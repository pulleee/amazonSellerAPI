/* 
 * Selling Partner API for Sellers
 *
 * The Selling Partner API for Sellers lets you retrieve information on behalf of sellers about their seller account, such as the marketplaces they participate in. Along with listing the marketplaces that a seller can sell in, the API also provides additional information about the marketplace such as the default language and the default currency. The API also provides seller-specific information such as whether the seller has suspended listings in that marketplace.
 *
 * OpenAPI spec version: v1
 * 
 * Generated by: https://github.com/swagger-api/swagger-codegen.git
 */

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using RestSharp;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;
using Amazon.SellingPartnerAPIAA;

namespace SellingPartnerAPI.SellerAPI.Api
{
    /// <summary>
    /// Represents a collection of functions to interact with the API endpoints
    /// </summary>
    public interface ISellersApi : IApiAccessor
    {
        #region Synchronous Operations
        /// <summary>
        /// 
        /// </summary>
        /// <remarks>
        /// Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </remarks>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>GetMarketplaceParticipationsResponse</returns>
        GetMarketplaceParticipationsResponse GetMarketplaceParticipations ();

        /// <summary>
        /// 
        /// </summary>
        /// <remarks>
        /// Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </remarks>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>ApiResponse of GetMarketplaceParticipationsResponse</returns>
        ApiResponse<GetMarketplaceParticipationsResponse> GetMarketplaceParticipationsWithHttpInfo ();
        #endregion Synchronous Operations
        #region Asynchronous Operations
        /// <summary>
        /// 
        /// </summary>
        /// <remarks>
        /// Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </remarks>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>Task of GetMarketplaceParticipationsResponse</returns>
        System.Threading.Tasks.Task<GetMarketplaceParticipationsResponse> GetMarketplaceParticipationsAsync ();

        /// <summary>
        /// 
        /// </summary>
        /// <remarks>
        /// Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </remarks>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>Task of ApiResponse (GetMarketplaceParticipationsResponse)</returns>
        System.Threading.Tasks.Task<ApiResponse<GetMarketplaceParticipationsResponse>> GetMarketplaceParticipationsAsyncWithHttpInfo ();
        #endregion Asynchronous Operations
    }

    /// <summary>
    /// Represents a collection of functions to interact with the API endpoints
    /// </summary>
    public partial class SellersApi : ISellersApi
    {
        private SellingPartnerAPI.SellerAPI.Client.ExceptionFactory _exceptionFactory = (name, response) => null;

        /// <summary>
        /// Initializes a new instance of the <see cref="SellersApi"/> class
        /// using Configuration object
        /// </summary>
        /// <param name="configuration">An instance of Configuration</param>
        /// <returns></returns>
        public SellersApi(SellingPartnerAPI.SellerAPI.Client.Configuration configuration)
        {
            this.Configuration = configuration;
            ExceptionFactory = SellingPartnerAPI.SellerAPI.Client.Configuration.DefaultExceptionFactory;
        }

        /// <summary>
        /// Gets the base path of the API client.
        /// </summary>
        /// <value>The base path</value>
        public String GetBasePath()
        {
            return this.Configuration.ApiClient.RestClient.BaseUrl.ToString();
        }

        /// <summary>
        /// Sets the base path of the API client.
        /// </summary>
        /// <value>The base path</value>
        [Obsolete("SetBasePath is deprecated, please do 'Configuration.ApiClient = new ApiClient(\"http://new-path\")' instead.")]
        public void SetBasePath(String basePath)
        {
            // do nothing
        }

        /// <summary>
        /// Gets or sets the configuration object
        /// </summary>
        /// <value>An instance of the Configuration</value>
        public SellingPartnerAPI.SellerAPI.Client.Configuration Configuration {get; set;}

        /// <summary>
        /// Provides a factory method hook for the creation of exceptions.
        /// </summary>
        public SellingPartnerAPI.SellerAPI.Client.ExceptionFactory ExceptionFactory
        {
            get
            {
                if (_exceptionFactory != null && _exceptionFactory.GetInvocationList().Length > 1)
                {
                    throw new InvalidOperationException("Multicast delegate for ExceptionFactory is unsupported.");
                }
                return _exceptionFactory;
            }
            set { _exceptionFactory = value; }
        }

        /// <summary>
        /// Gets the default header.
        /// </summary>
        /// <returns>Dictionary of HTTP header</returns>
        [Obsolete("DefaultHeader is deprecated, please use Configuration.DefaultHeader instead.")]
        public IDictionary<String, String> DefaultHeader()
        {
            return new ReadOnlyDictionary<string, string>(this.Configuration.DefaultHeader);
        }

        /// <summary>
        /// Add default header.
        /// </summary>
        /// <param name="key">Header field name.</param>
        /// <param name="value">Header field value.</param>
        /// <returns></returns>
        [Obsolete("AddDefaultHeader is deprecated, please use Configuration.AddDefaultHeader instead.")]
        public void AddDefaultHeader(string key, string value)
        {
            this.Configuration.AddDefaultHeader(key, value);
        }

        /// <summary>
        ///  Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </summary>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>GetMarketplaceParticipationsResponse</returns>
        public GetMarketplaceParticipationsResponse GetMarketplaceParticipations ()
        {
             ApiResponse<GetMarketplaceParticipationsResponse> localVarResponse = GetMarketplaceParticipationsWithHttpInfo();
             return localVarResponse.Data;
        }

        /// <summary>
        ///  Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </summary>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>ApiResponse of GetMarketplaceParticipationsResponse</returns>
        public ApiResponse< GetMarketplaceParticipationsResponse > GetMarketplaceParticipationsWithHttpInfo ()
        {

            var localVarPath = "/sellers/v1/marketplaceParticipations";
            var localVarPathParams = new Dictionary<String, String>();
            var localVarQueryParams = new List<KeyValuePair<String, String>>();
            var localVarHeaderParams = new Dictionary<String, String>(this.Configuration.DefaultHeader);
            var localVarFormParams = new Dictionary<String, String>();
            var localVarFileParams = new Dictionary<String, FileParameter>();
            Object localVarPostBody = null;

            // to determine the Content-Type header
            String[] localVarHttpContentTypes = new String[] {
                "application/json"
            };
            String localVarHttpContentType = this.Configuration.ApiClient.SelectHeaderContentType(localVarHttpContentTypes);

            // to determine the Accept header
            String[] localVarHttpHeaderAccepts = new String[] {
                "application/json"
            };
            String localVarHttpHeaderAccept = this.Configuration.ApiClient.SelectHeaderAccept(localVarHttpHeaderAccepts);
            if (localVarHttpHeaderAccept != null)
                localVarHeaderParams.Add("Accept", localVarHttpHeaderAccept);



            // make the HTTP request
            IRestResponse localVarResponse = (IRestResponse) this.Configuration.ApiClient.CallApi(localVarPath,
                Method.GET, localVarQueryParams, localVarPostBody, localVarHeaderParams, localVarFormParams, localVarFileParams,
                localVarPathParams, localVarHttpContentType);

            int localVarStatusCode = (int) localVarResponse.StatusCode;

            if (ExceptionFactory != null)
            {
                Exception exception = ExceptionFactory("GetMarketplaceParticipations", localVarResponse);
                if (exception != null) throw exception;
            }

            return new ApiResponse<GetMarketplaceParticipationsResponse>(localVarStatusCode,
                localVarResponse.Headers.ToDictionary(x => x.Name, x => x.Value.ToString()),
                (GetMarketplaceParticipationsResponse) this.Configuration.ApiClient.Deserialize(localVarResponse, typeof(GetMarketplaceParticipationsResponse)));
        }

        /// <summary>
        ///  Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </summary>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>Task of GetMarketplaceParticipationsResponse</returns>
        public async System.Threading.Tasks.Task<GetMarketplaceParticipationsResponse> GetMarketplaceParticipationsAsync ()
        {
             ApiResponse<GetMarketplaceParticipationsResponse> localVarResponse = await GetMarketplaceParticipationsAsyncWithHttpInfo();
             return localVarResponse.Data;

        }

        /// <summary>
        ///  Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller&#39;s participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The &#x60;x-amzn-RateLimit-Limit&#x60; response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).
        /// </summary>
        /// <exception cref="SellingPartnerAPI.SellerAPI.Client.ApiException">Thrown when fails to make API call</exception>
        /// <returns>Task of ApiResponse (GetMarketplaceParticipationsResponse)</returns>
        public async System.Threading.Tasks.Task<ApiResponse<GetMarketplaceParticipationsResponse>> GetMarketplaceParticipationsAsyncWithHttpInfo ()
        {

            var localVarPath = "/sellers/v1/marketplaceParticipations";
            var localVarPathParams = new Dictionary<String, String>();
            var localVarQueryParams = new List<KeyValuePair<String, String>>();
            var localVarHeaderParams = new Dictionary<String, String>(this.Configuration.DefaultHeader);
            var localVarFormParams = new Dictionary<String, String>();
            var localVarFileParams = new Dictionary<String, FileParameter>();
            Object localVarPostBody = null;

            // to determine the Content-Type header
            String[] localVarHttpContentTypes = new String[] {
                "application/json"
            };
            String localVarHttpContentType = this.Configuration.ApiClient.SelectHeaderContentType(localVarHttpContentTypes);

            // to determine the Accept header
            String[] localVarHttpHeaderAccepts = new String[] {
                "application/json"
            };
            String localVarHttpHeaderAccept = this.Configuration.ApiClient.SelectHeaderAccept(localVarHttpHeaderAccepts);
            if (localVarHttpHeaderAccept != null)
                localVarHeaderParams.Add("Accept", localVarHttpHeaderAccept);



            // make the HTTP request
            IRestResponse localVarResponse = (IRestResponse) await this.Configuration.ApiClient.CallApiAsync(localVarPath,
                Method.GET, localVarQueryParams, localVarPostBody, localVarHeaderParams, localVarFormParams, localVarFileParams,
                localVarPathParams, localVarHttpContentType);

            int localVarStatusCode = (int) localVarResponse.StatusCode;

            if (ExceptionFactory != null)
            {
                Exception exception = ExceptionFactory("GetMarketplaceParticipations", localVarResponse);
                if (exception != null) throw exception;
            }

            return new ApiResponse<GetMarketplaceParticipationsResponse>(localVarStatusCode,
                localVarResponse.Headers.ToDictionary(x => x.Name, x => x.Value.ToString()),
                (GetMarketplaceParticipationsResponse) this.Configuration.ApiClient.Deserialize(localVarResponse, typeof(GetMarketplaceParticipationsResponse)));
        }


        public class Builder
        {
            private LWAAuthorizationCredentials lwaAuthorizationCredentials;
            private RateLimitConfiguration rateLimitConfiguration;

            public Builder SetLWAAuthorizationCredentials(LWAAuthorizationCredentials lwaAuthorizationCredentials)
            {
                this.lwaAuthorizationCredentials = lwaAuthorizationCredentials;
                return this;
            }
            
            
            public Builder SetRateLimitConfiguration(RateLimitConfiguration rateLimitConfiguration)
            {
                this.rateLimitConfiguration = rateLimitConfiguration;
                return this;
            }

            public SellersApi Build() 
            {
                if (lwaAuthorizationCredentials == null) 
                {
                    throw new NullReferenceException("LWAAuthoriztionCredentials not set");
                }

                
                SellingPartnerAPI.SellerAPI.Client.Configuration configuration = new SellingPartnerAPI.SellerAPI.Client.Configuration()
                {
                    AuthorizationCredentials = lwaAuthorizationCredentials,
                    RateLimitConfig = rateLimitConfiguration
                };

                // default HTTP connection timeout (in milliseconds)
                configuration.Timeout = 100000;

                return new SellersApi(configuration);
            }
        }
    }
}