using SelectLineWAWIApiCore.Server.Amazon;
using SelectLineWAWIApiCore.Server.Database;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Model;
using Syncfusion.Blazor.Lists.Internal;

namespace SelectLineWAWIApiCore.Server.Services.Amazon
{
    public class OrderService
    {
        private readonly WAWIDbContext _dbContext;
        private readonly AmazonAuthContext _amazonContext;
        private readonly OrdersApi _apiInstance;

        public OrderService(WAWIDbContext dbContext, AmazonAuthContext amazonContext)
        {
            _dbContext = dbContext;
            _amazonContext = amazonContext;

            _apiInstance = new OrdersApi.Builder()
                .SetLWAAuthorizationCredentials(_amazonContext.LWAAuthorizationCredentials)
                .Build();
        }

        public async List<Order> GetOrdesFromToAsync(DateTime from, DateTime to)
        {
            var marketplaceIds = new List<string>() { AmazonUtil.MARKETPLACE_IDs["DE"] };

            var result = await _apiInstance.GetOrdersAsync(marketplaceIds, from.ToString(AmazonUtil.DATE_FORMAT), to.ToString(AmazonUtil.DATE_FORMAT));
            var orders = result.Payload.Orders.ToList();

            foreach (var order in orders)
            {
                var item = await _apiInstance.GetOrderItemsAsync(order.AmazonOrderId);
            }




            Console.WriteLine(result.ToJson());
        }
    }
}
