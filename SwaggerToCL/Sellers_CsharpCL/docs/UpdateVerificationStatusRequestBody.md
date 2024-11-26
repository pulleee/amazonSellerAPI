# SellingPartnerAPI.SellerAPI.Model.UpdateVerificationStatusRequestBody
## Properties

Name | Type | Description | Notes
------------ | ------------- | ------------- | -------------
**Status** | **VerificationStatus** | The new verification status of the order. | [optional] 
**ExternalReviewerId** | **string** | The identifier of the order&#39;s regulated information reviewer. | 
**RejectionReasonId** | **string** | The unique identifier of the rejection reason used for rejecting the order&#39;s regulated information. Only required if the new status is rejected. | [optional] 
**VerificationDetails** | [**VerificationDetails**](VerificationDetails.md) | Additional information regarding the verification of the order. | [optional] 

[[Back to Model list]](../README.md#documentation-for-models) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to README]](../README.md)

