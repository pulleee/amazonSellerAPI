using Amazon.SellingPartnerAPIAA;
using SelectLineWAWIApiCore.Server.Amazon;
using SelectLineWAWIApiCore.Server.Database;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;

namespace SelectLineWAWIApiCore.Server.Services.Amazon
{
    public class ReportService : RequestService
    {
        private readonly WAWIDbContext _dbContext;
        private readonly AmazonAuthContext _amazonContext;
        private readonly ReportsApi _apiInstance;
        private readonly TokensApi _tokensApi;

        public ReportService(WAWIDbContext dbContext, AmazonAuthContext amazonContext) : base(amazonContext)
        {
            _dbContext = dbContext;
            _amazonContext = amazonContext;

            _apiInstance = new ReportsApi.Builder()
                .SetLWAAuthorizationCredentials(_amazonContext.LWAAuthorizationCredentials)
                .Build();

            _tokensApi = new TokensApi.Builder()
                .SetLWAAuthorizationCredentials(_amazonContext.LWAAuthorizationCredentials)
                .Build();
        }

        public async Task<List<Report>> GetReportsAsync()
        {
            var reports = new List<Report>();
            var reportTypes = new List<string>() { "GET_FLAT_FILE_ALL_ORDERS_DATA_BY_LAST_UPDATE_GENERAL" };

            try
            {
                //var test = await _apiInstance.GetReportsAsync(reportTypes: reportTypes, marketplaceIds: new List<string> { AmazonUtil.MARKETPLACE_IDs["DE"] }, createdSince: DateTime.Now.AddDays(-30), createdUntil: DateTime.Now);

                var test = await _apiInstance.GetReportAsync("815376020053");

                List<RestrictedResource> restrictedResourceList = new List<RestrictedResource>();
                //List<string> dataElements = new List<string>
                //{
                //    "GET_ORDER_REPORT_DATA_INVOICING",
                //};
                restrictedResourceList.Add(new RestrictedResource(RestrictedResource.MethodEnum.GET, "/reports/2021-06-30/reports/815376020053"));
                CreateRestrictedDataTokenRequest createRDT = new CreateRestrictedDataTokenRequest(null, restrictedResourceList);
                CreateRestrictedDataTokenResponse rdtResponse = _tokensApi.CreateRestrictedDataToken(createRDT);
                _apiInstance.Configuration.AddDefaultHeader("x-amz-access-token", rdtResponse.RestrictedDataToken);

                var result = await _apiInstance.GetReportsAsync(reportTypes);

                Console.WriteLine(result.ToJson());

                //reports = result.Reports; 
            }
            catch (LWAException e)
            {
                Console.WriteLine("LWA Exception when calling SellersApi#GetReports");
                Console.WriteLine(e.getErrorCode());
                Console.WriteLine(e.getErrorMessage());
                Console.WriteLine(e.Message);
            }
            catch (ApiException e)
            {
                Console.WriteLine("Exception when calling SellersApi#GetReports");
                Console.WriteLine(e.Message);
            }

            return reports;
        }
    }
}
