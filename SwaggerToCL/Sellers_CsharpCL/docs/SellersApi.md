# SellingPartnerAPI.SellerAPI.Api.SellersApi

All URIs are relative to *https://sellingpartnerapi-na.amazon.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**GetMarketplaceParticipations**](SellersApi.md#getmarketplaceparticipations) | **GET** /sellers/v1/marketplaceParticipations | 


<a name="getmarketplaceparticipations"></a>
# **GetMarketplaceParticipations**
> GetMarketplaceParticipationsResponse GetMarketplaceParticipations ()



Returns a list of marketplaces that the seller submitting the request can sell in and information about the seller's participation in those marketplaces.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.016 | 15 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, see [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class GetMarketplaceParticipationsExample
    {
        public void main()
        {
            var apiInstance = new SellersApi();

            try
            {
                GetMarketplaceParticipationsResponse result = apiInstance.GetMarketplaceParticipations();
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling SellersApi.GetMarketplaceParticipations: " + e.Message );
            }
        }
    }
}
```

### Parameters
This endpoint does not need any parameter.

### Return type

[**GetMarketplaceParticipationsResponse**](GetMarketplaceParticipationsResponse.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

