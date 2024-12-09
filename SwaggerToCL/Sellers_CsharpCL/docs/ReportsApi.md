# SellingPartnerAPI.SellerAPI.Api.ReportsApi

All URIs are relative to *https://sellingpartnerapi-eu.amazon.com*

Method | HTTP request | Description
------------- | ------------- | -------------
[**CancelReport**](ReportsApi.md#cancelreport) | **DELETE** /reports/2021-06-30/reports/{reportId} | cancelReport
[**CancelReportSchedule**](ReportsApi.md#cancelreportschedule) | **DELETE** /reports/2021-06-30/schedules/{reportScheduleId} | cancelReportSchedule
[**CreateReport**](ReportsApi.md#createreport) | **POST** /reports/2021-06-30/reports | createReport
[**CreateReportSchedule**](ReportsApi.md#createreportschedule) | **POST** /reports/2021-06-30/schedules | createReportSchedule
[**GetReport**](ReportsApi.md#getreport) | **GET** /reports/2021-06-30/reports/{reportId} | getReport
[**GetReportDocument**](ReportsApi.md#getreportdocument) | **GET** /reports/2021-06-30/documents/{reportDocumentId} | getReportDocument
[**GetReportSchedule**](ReportsApi.md#getreportschedule) | **GET** /reports/2021-06-30/schedules/{reportScheduleId} | getReportSchedule
[**GetReportSchedules**](ReportsApi.md#getreportschedules) | **GET** /reports/2021-06-30/schedules | getReportSchedules
[**GetReports**](ReportsApi.md#getreports) | **GET** /reports/2021-06-30/reports | getReports


<a name="cancelreport"></a>
# **CancelReport**
> void CancelReport (string reportId)

cancelReport

Cancels the report that you specify. Only reports with `processingStatus=IN_QUEUE` can be cancelled. Cancelled reports are returned in subsequent calls to the `getReport` and `getReports` operations.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0222 | 10 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class CancelReportExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportId = reportId_example;  // string | The identifier for the report. This identifier is unique only in combination with a seller ID.

            try
            {
                // cancelReport
                apiInstance.CancelReport(reportId);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.CancelReport: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportId** | **string**| The identifier for the report. This identifier is unique only in combination with a seller ID. | 

### Return type

void (empty response body)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="cancelreportschedule"></a>
# **CancelReportSchedule**
> void CancelReportSchedule (string reportScheduleId)

cancelReportSchedule

Cancels the report schedule that you specify.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0222 | 10 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class CancelReportScheduleExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportScheduleId = reportScheduleId_example;  // string | The identifier for the report schedule. This identifier is unique only in combination with a seller ID.

            try
            {
                // cancelReportSchedule
                apiInstance.CancelReportSchedule(reportScheduleId);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.CancelReportSchedule: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportScheduleId** | **string**| The identifier for the report schedule. This identifier is unique only in combination with a seller ID. | 

### Return type

void (empty response body)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="createreport"></a>
# **CreateReport**
> CreateReportResponse CreateReport (CreateReportSpecification body)

createReport

Creates a report.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0167 | 15 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class CreateReportExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var body = new CreateReportSpecification(); // CreateReportSpecification | Information required to create the report.

            try
            {
                // createReport
                CreateReportResponse result = apiInstance.CreateReport(body);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.CreateReport: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **body** | [**CreateReportSpecification**](CreateReportSpecification.md)| Information required to create the report. | 

### Return type

[**CreateReportResponse**](CreateReportResponse.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="createreportschedule"></a>
# **CreateReportSchedule**
> CreateReportScheduleResponse CreateReportSchedule (CreateReportScheduleSpecification body)

createReportSchedule

Creates a report schedule. If a report schedule with the same report type and marketplace IDs already exists, it will be cancelled and replaced with this one.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0222 | 10 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class CreateReportScheduleExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var body = new CreateReportScheduleSpecification(); // CreateReportScheduleSpecification | Information required to create the report schedule.

            try
            {
                // createReportSchedule
                CreateReportScheduleResponse result = apiInstance.CreateReportSchedule(body);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.CreateReportSchedule: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **body** | [**CreateReportScheduleSpecification**](CreateReportScheduleSpecification.md)| Information required to create the report schedule. | 

### Return type

[**CreateReportScheduleResponse**](CreateReportScheduleResponse.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="getreport"></a>
# **GetReport**
> Report GetReport (string reportId)

getReport

Returns report details (including the `reportDocumentId`, if available) for the report that you specify.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 2 | 15 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class GetReportExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportId = reportId_example;  // string | The identifier for the report. This identifier is unique only in combination with a seller ID.

            try
            {
                // getReport
                Report result = apiInstance.GetReport(reportId);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.GetReport: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportId** | **string**| The identifier for the report. This identifier is unique only in combination with a seller ID. | 

### Return type

[**Report**](Report.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="getreportdocument"></a>
# **GetReportDocument**
> ReportDocument GetReportDocument (string reportDocumentId)

getReportDocument

Returns the information required for retrieving a report document's contents.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0167 | 15 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class GetReportDocumentExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportDocumentId = reportDocumentId_example;  // string | The identifier for the report document.

            try
            {
                // getReportDocument
                ReportDocument result = apiInstance.GetReportDocument(reportDocumentId);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.GetReportDocument: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportDocumentId** | **string**| The identifier for the report document. | 

### Return type

[**ReportDocument**](ReportDocument.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="getreportschedule"></a>
# **GetReportSchedule**
> ReportSchedule GetReportSchedule (string reportScheduleId)

getReportSchedule

Returns report schedule details for the report schedule that you specify.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0222 | 10 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class GetReportScheduleExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportScheduleId = reportScheduleId_example;  // string | The identifier for the report schedule. This identifier is unique only in combination with a seller ID.

            try
            {
                // getReportSchedule
                ReportSchedule result = apiInstance.GetReportSchedule(reportScheduleId);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.GetReportSchedule: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportScheduleId** | **string**| The identifier for the report schedule. This identifier is unique only in combination with a seller ID. | 

### Return type

[**ReportSchedule**](ReportSchedule.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="getreportschedules"></a>
# **GetReportSchedules**
> ReportScheduleList GetReportSchedules (List<string> reportTypes)

getReportSchedules

Returns report schedule details that match the filters that you specify.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0222 | 10 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class GetReportSchedulesExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportTypes = new List<string>(); // List<string> | A list of report types used to filter report schedules. Refer to [Report Type Values](https://developer-docs.amazon.com/sp-api/docs/report-type-values) for more information.

            try
            {
                // getReportSchedules
                ReportScheduleList result = apiInstance.GetReportSchedules(reportTypes);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.GetReportSchedules: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportTypes** | [**List&lt;string&gt;**](string.md)| A list of report types used to filter report schedules. Refer to [Report Type Values](https://developer-docs.amazon.com/sp-api/docs/report-type-values) for more information. | 

### Return type

[**ReportScheduleList**](ReportScheduleList.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

<a name="getreports"></a>
# **GetReports**
> GetReportsResponse GetReports (List<string> reportTypes = null, List<string> processingStatuses = null, List<string> marketplaceIds = null, int? pageSize = null, DateTime? createdSince = null, DateTime? createdUntil = null, string nextToken = null)

getReports

Returns report details for the reports that match the filters that you specify.  **Usage Plan:**  | Rate (requests per second) | Burst | | - -- - | - -- - | | 0.0222 | 10 |  The `x-amzn-RateLimit-Limit` response header returns the usage plan rate limits that were applied to the requested operation, when available. The table above indicates the default rate and burst values for this operation. Selling partners whose business demands require higher throughput may see higher rate and burst values than those shown here. For more information, refer to [Usage Plans and Rate Limits in the Selling Partner API](https://developer-docs.amazon.com/sp-api/docs/usage-plans-and-rate-limits-in-the-sp-api).

### Example
```csharp
using System;
using System.Diagnostics;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace Example
{
    public class GetReportsExample
    {
        public void main()
        {
            var apiInstance = new ReportsApi();
            var reportTypes = new List<string>(); // List<string> | A list of report types used to filter reports. Refer to [Report Type Values](https://developer-docs.amazon.com/sp-api/docs/report-type-values) for more information. When reportTypes is provided, the other filter parameters (processingStatuses, marketplaceIds, createdSince, createdUntil) and pageSize may also be provided. Either reportTypes or nextToken is required. (optional) 
            var processingStatuses = processingStatuses_example;  // List<string> | A list of processing statuses used to filter reports. (optional) 
            var marketplaceIds = new List<string>(); // List<string> | A list of marketplace identifiers used to filter reports. The reports returned will match at least one of the marketplaces that you specify. (optional) 
            var pageSize = 56;  // int? | The maximum number of reports to return in a single call. (optional)  (default to 10)
            var createdSince = 2013-10-20T19:20:30+01:00;  // DateTime? | The earliest report creation date and time for reports to include in the response, in <a href='https://developer-docs.amazon.com/sp-api/docs/iso-8601'>ISO 8601</a> date time format. The default is 90 days ago. Reports are retained for a maximum of 90 days. (optional) 
            var createdUntil = 2013-10-20T19:20:30+01:00;  // DateTime? | The latest report creation date and time for reports to include in the response, in <a href='https://developer-docs.amazon.com/sp-api/docs/iso-8601'>ISO 8601</a> date time format. The default is now. (optional) 
            var nextToken = nextToken_example;  // string | A string token returned in the response to your previous request. `nextToken` is returned when the number of results exceeds the specified `pageSize` value. To get the next page of results, call the `getReports` operation and include this token as the only parameter. Specifying `nextToken` with any other parameters will cause the request to fail. (optional) 

            try
            {
                // getReports
                GetReportsResponse result = apiInstance.GetReports(reportTypes, processingStatuses, marketplaceIds, pageSize, createdSince, createdUntil, nextToken);
                Debug.WriteLine(result);
            }
            catch (Exception e)
            {
                Debug.Print("Exception when calling ReportsApi.GetReports: " + e.Message );
            }
        }
    }
}
```

### Parameters

Name | Type | Description  | Notes
------------- | ------------- | ------------- | -------------
 **reportTypes** | [**List&lt;string&gt;**](string.md)| A list of report types used to filter reports. Refer to [Report Type Values](https://developer-docs.amazon.com/sp-api/docs/report-type-values) for more information. When reportTypes is provided, the other filter parameters (processingStatuses, marketplaceIds, createdSince, createdUntil) and pageSize may also be provided. Either reportTypes or nextToken is required. | [optional] 
 **processingStatuses** | **List&lt;string&gt;**| A list of processing statuses used to filter reports. | [optional] 
 **marketplaceIds** | [**List&lt;string&gt;**](string.md)| A list of marketplace identifiers used to filter reports. The reports returned will match at least one of the marketplaces that you specify. | [optional] 
 **pageSize** | **int?**| The maximum number of reports to return in a single call. | [optional] [default to 10]
 **createdSince** | **DateTime?**| The earliest report creation date and time for reports to include in the response, in &lt;a href&#x3D;&#39;https://developer-docs.amazon.com/sp-api/docs/iso-8601&#39;&gt;ISO 8601&lt;/a&gt; date time format. The default is 90 days ago. Reports are retained for a maximum of 90 days. | [optional] 
 **createdUntil** | **DateTime?**| The latest report creation date and time for reports to include in the response, in &lt;a href&#x3D;&#39;https://developer-docs.amazon.com/sp-api/docs/iso-8601&#39;&gt;ISO 8601&lt;/a&gt; date time format. The default is now. | [optional] 
 **nextToken** | **string**| A string token returned in the response to your previous request. &#x60;nextToken&#x60; is returned when the number of results exceeds the specified &#x60;pageSize&#x60; value. To get the next page of results, call the &#x60;getReports&#x60; operation and include this token as the only parameter. Specifying &#x60;nextToken&#x60; with any other parameters will cause the request to fail. | [optional] 

### Return type

[**GetReportsResponse**](GetReportsResponse.md)

### Authorization

No authorization required

### HTTP request headers

 - **Content-Type**: application/json
 - **Accept**: application/json

[[Back to top]](#) [[Back to API list]](../README.md#documentation-for-api-endpoints) [[Back to Model list]](../README.md#documentation-for-models) [[Back to README]](../README.md)

