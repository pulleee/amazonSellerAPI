# SellingPartnerAPI.SellerAPI.Model.Address
## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**Name** | **string** | The name. | 
**CompanyName** | **string** | The company name of the recipient.  **Note**: This attribute is only available for shipping address. | [optional] 
**AddressLine1** | **string** | The street address. | [optional] 
**AddressLine2** | **string** | Additional street address information, if required. | [optional] 
**AddressLine3** | **string** | Additional street address information, if required. | [optional] 
**City** | **string** | The city. | [optional] 
**County** | **string** | The county. | [optional] 
**District** | **string** | The district. | [optional] 
**StateOrRegion** | **string** | The state or region. | [optional] 
**Municipality** | **string** | The municipality. | [optional] 
**PostalCode** | **string** | The postal code. | [optional] 
**CountryCode** | **string** | The country code. A two-character country code, in ISO 3166-1 alpha-2 format. | [optional] 
**Phone** | **string** | The phone number of the buyer.  **Note**:  1. This attribute is only available for shipping address. 2. In some cases, the buyer phone number is suppressed:  a. Phone is suppressed for all &#x60;AFN&#x60; (fulfilled by Amazon) orders. b. Phone is suppressed for the shipped &#x60;MFN&#x60; (fulfilled by seller) order when the current date is past the Latest Delivery Date. | [optional] 
**ExtendedFields** | [**AddressExtendedFields**](AddressExtendedFields.md) | The container for address extended fields. For example, street name or street number.   **Note**: This attribute is currently only available with Brazil shipping addresses. | [optional] 
**AddressType** | **string** | The address type of the shipping address. | [optional] 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)

