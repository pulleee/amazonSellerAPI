using Amazon.SellingPartnerAPIAA;
using SelectLineWAWIApiCore.Server.Amazon;
using SelectLineWAWIApiCore.Server.Database;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Model;
using Syncfusion.Blazor.Lists.Internal;

namespace SelectLineWAWIApiCore.Server.Services.Amazon
{
    public class OrderService
    {
        private readonly WAWIDbContext _dbContext;
        private readonly AmazonAuthContext _amazonContext;
        private readonly OrdersApi _apiInstance;
        private readonly ReportsApi _apiInstanceReport;

        public OrderService(WAWIDbContext dbContext, AmazonAuthContext amazonContext)
        {
            _dbContext = dbContext;
            _amazonContext = amazonContext;

            _apiInstance = new OrdersApi.Builder()
                .SetLWAAuthorizationCredentials(_amazonContext.LWAAuthorizationCredentials)
                .Build();


            _apiInstanceReport = new ReportsApi.Builder()
                .SetLWAAuthorizationCredentials(_amazonContext.LWAAuthorizationCredentials)
                .Build();
        }

        public async Task<List<Order>> GetOrdesFromToAsync(DateTime from, DateTime to)
        {
            var orders = new List<Order>();
            try
            {
                var marketplaceIds = new List<string>() { AmazonUtil.MARKETPLACE_IDs["DE"] };

                var result = await _apiInstance.GetOrdersAsync(marketplaceIds, from.ToString(AmazonUtil.DATE_FORMAT), to.ToString(AmazonUtil.DATE_FORMAT));

                Console.WriteLine(result.ToJson());

                orders = result.Payload.Orders.ToList();

                foreach (var order in orders)
                {
                    var item = await _apiInstance.GetOrderItemsAsync(order.AmazonOrderId);

                    Console.WriteLine(item.ToJson());
                }

                Console.WriteLine(result.ToJson());
            }
            catch (LWAException e)
            {
                Console.WriteLine("LWA Exception when calling SellersApi#getOrders");
                Console.WriteLine(e.getErrorCode());
                Console.WriteLine(e.getErrorMessage());
                Console.WriteLine(e.Message);
            }
            catch (ApiException e)
            {
                Console.WriteLine("Exception when calling SellersApi#getOrders");
                Console.WriteLine(e.Message);
            }

            return orders;
        }
    }
}
