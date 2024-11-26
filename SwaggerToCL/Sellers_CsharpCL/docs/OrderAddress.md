# SellingPartnerAPI.SellerAPI.Model.OrderAddress
## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**AmazonOrderId** | **string** | An Amazon-defined order identifier, in 3-7-7 format. | 
**BuyerCompanyName** | **string** | The company name of the contact buyer. For IBA orders, the buyer company must be Amazon entities. | [optional] 
**ShippingAddress** | [**Address**](Address.md) | The shipping address for the order.  **Note**: &#x60;ShippingAddress&#x60; is only available for orders with the following status values: &#x60;Unshipped&#x60;, &#x60;PartiallyShipped&#x60;, &#x60;Shipped&#x60;, and &#x60;InvoiceUnconfirmed&#x60;. | [optional] 
**DeliveryPreferences** | [**DeliveryPreferences**](DeliveryPreferences.md) |  | [optional] 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)

