/* 
 * Orders v0
 *
 * Use the Orders Selling Partner API to programmatically retrieve order information. With this API, you can develop fast, flexible, and custom applications to manage order synchronization, perform order research, and create demand-based decision support tools.   _Note:_ For the JP, AU, and SG marketplaces, the Orders API supports orders from 2016 onward. For all other marketplaces, the Orders API supports orders for the last two years (orders older than this don't show up in the response).
 *
 * OpenAPI spec version: v0
 * 
 * Generated by: https://github.com/swagger-api/swagger-codegen.git
 */

using System;
using System.IO;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using RestSharp;
using NUnit.Framework;

using SellingPartnerAPI.SellerAPI.Client;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Model;
using Amazon.SellingPartnerAPIAA;

namespace SellingPartnerAPI.SellerAPI.Test
{
    /// <summary>
    ///  Class for testing OrdersApi
    /// </summary>
    /// <remarks>
    /// This file is automatically generated by Swagger Codegen.
    /// Please update the test case below to test the API endpoint.
    /// </remarks>
    [TestFixture]
    public class OrdersApiTests
    {
        private OrdersApi instance;

        public static void Main(string[] args)
        {
            try
            {
                LWAAuthorizationCredentials lwaAuthorizationCredentials = new LWAAuthorizationCredentials
                {
                    ClientId = "amzn1.application-oa2-client.02086c935a8a44b2bf35d2cda496dcdc",
                    ClientSecret = "amzn1.oa2-cs.v1.44fa7cd478b212c7c697b1c3ae00a5baaaea47a01e86dc79c57e643171f9abb7",
                    RefreshToken = "Atzr|IwEBIAjORrxOQhmoOUakZx8BbDJvNJIEcz8KgVwH1bJwPLfzltEGSHnO0b54SmzDvO4OEvH2CyO-BREvj5M9QPa8bgqUJjVst87NiHutOiRCss2pizLWTTGQgeYpikgqamDu-qk_6IGNoweKd9LnRZdfAjPPJKtMRXJ2F9eQjy4TgKE8xjXccsTGYvxdb8uyNNQa1vQgBGz_5R_oFmvqOjJNVM2gJqtFMAiKXqZg36Xy673CpgCbHWeKkHNUsYhDag54leXnaNQYJCC_KcT_J1rhTJklwet-6dUBt9DTESTjkiB-YPQ-kSUz2oUJITxRhl_nYLU",
                    Endpoint = new Uri("https://api.amazon.com/auth/o2/token")
                };
                OrdersApi ordersApi = new OrdersApi.Builder()
                    .SetLWAAuthorizationCredentials(lwaAuthorizationCredentials)
                    .Build();

                var marketplaceIds = new List<string>() { "A1PA6795UKMFR9" };

                var result = ordersApi.GetOrders(marketplaceIds, "2024-01-01T00:00:00.00", "2024-11-25T00:00:00.00");
                Console.WriteLine(result.ToJson());
            }
            catch (LWAException e)
            {
                Console.WriteLine("LWA Exception when calling SellersApi#getMarketplaceParticipations");
                Console.WriteLine(e.getErrorCode());
                Console.WriteLine(e.getErrorMessage());
                Console.WriteLine(e.Message);
            }
            catch (ApiException e)
            {
                Console.WriteLine("Exception when calling SellersApi#getMarketplaceParticipations");
                Console.WriteLine(e.Message);
            }
        }

        /// <summary>
        /// Setup before each unit test
        /// </summary>
        [SetUp]
        public void Init()
        {
            // TODO uncomment below to initialize instance for testing
            //instance = new OrdersApi();
        }

        /// <summary>
        /// Clean up after each unit test
        /// </summary>
        [TearDown]
        public void Cleanup()
        {

        }

        /// <summary>
        /// Test an instance of OrdersApi
        /// </summary>
        [Test]
        public void InstanceTest()
        {
            // TODO uncomment below to test 'IsInstanceOfType' OrdersApi
            //Assert.IsInstanceOfType(typeof(OrdersApi), instance, "instance is a OrdersApi");
        }

        
        /// <summary>
        /// Test ConfirmShipment
        /// </summary>
        [Test]
        public void ConfirmShipmentTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //ConfirmShipmentRequest payload = null;
            //instance.ConfirmShipment(orderId, payload);
            
        }
        
        /// <summary>
        /// Test GetOrder
        /// </summary>
        [Test]
        public void GetOrderTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //var response = instance.GetOrder(orderId);
            //Assert.IsInstanceOf<GetOrderResponse> (response, "response is GetOrderResponse");
        }
        
        /// <summary>
        /// Test GetOrderAddress
        /// </summary>
        [Test]
        public void GetOrderAddressTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //var response = instance.GetOrderAddress(orderId);
            //Assert.IsInstanceOf<GetOrderAddressResponse> (response, "response is GetOrderAddressResponse");
        }
        
        /// <summary>
        /// Test GetOrderBuyerInfo
        /// </summary>
        [Test]
        public void GetOrderBuyerInfoTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //var response = instance.GetOrderBuyerInfo(orderId);
            //Assert.IsInstanceOf<GetOrderBuyerInfoResponse> (response, "response is GetOrderBuyerInfoResponse");
        }
        
        /// <summary>
        /// Test GetOrderItems
        /// </summary>
        [Test]
        public void GetOrderItemsTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //string nextToken = null;
            //var response = instance.GetOrderItems(orderId, nextToken);
            //Assert.IsInstanceOf<GetOrderItemsResponse> (response, "response is GetOrderItemsResponse");
        }
        
        /// <summary>
        /// Test GetOrderItemsBuyerInfo
        /// </summary>
        [Test]
        public void GetOrderItemsBuyerInfoTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //string nextToken = null;
            //var response = instance.GetOrderItemsBuyerInfo(orderId, nextToken);
            //Assert.IsInstanceOf<GetOrderItemsBuyerInfoResponse> (response, "response is GetOrderItemsBuyerInfoResponse");
        }
        
        /// <summary>
        /// Test GetOrderRegulatedInfo
        /// </summary>
        [Test]
        public void GetOrderRegulatedInfoTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //var response = instance.GetOrderRegulatedInfo(orderId);
            //Assert.IsInstanceOf<GetOrderRegulatedInfoResponse> (response, "response is GetOrderRegulatedInfoResponse");
        }
        
        /// <summary>
        /// Test GetOrders
        /// </summary>
        [Test]
        public void GetOrdersTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //List<string> marketplaceIds = null;
            //string createdAfter = null;
            //string createdBefore = null;
            //string lastUpdatedAfter = null;
            //string lastUpdatedBefore = null;
            //List<string> orderStatuses = null;
            //List<string> fulfillmentChannels = null;
            //List<string> paymentMethods = null;
            //string buyerEmail = null;
            //string sellerOrderId = null;
            //int? maxResultsPerPage = null;
            //List<string> easyShipShipmentStatuses = null;
            //List<string> electronicInvoiceStatuses = null;
            //string nextToken = null;
            //List<string> amazonOrderIds = null;
            //string actualFulfillmentSupplySourceId = null;
            //bool? isISPU = null;
            //string storeChainStoreId = null;
            //string earliestDeliveryDateBefore = null;
            //string earliestDeliveryDateAfter = null;
            //string latestDeliveryDateBefore = null;
            //string latestDeliveryDateAfter = null;
            //var response = instance.GetOrders(marketplaceIds, createdAfter, createdBefore, lastUpdatedAfter, lastUpdatedBefore, orderStatuses, fulfillmentChannels, paymentMethods, buyerEmail, sellerOrderId, maxResultsPerPage, easyShipShipmentStatuses, electronicInvoiceStatuses, nextToken, amazonOrderIds, actualFulfillmentSupplySourceId, isISPU, storeChainStoreId, earliestDeliveryDateBefore, earliestDeliveryDateAfter, latestDeliveryDateBefore, latestDeliveryDateAfter);
            //Assert.IsInstanceOf<GetOrdersResponse> (response, "response is GetOrdersResponse");
        }
        
        /// <summary>
        /// Test UpdateShipmentStatus
        /// </summary>
        [Test]
        public void UpdateShipmentStatusTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //UpdateShipmentStatusRequest payload = null;
            //instance.UpdateShipmentStatus(orderId, payload);
            
        }
        
        /// <summary>
        /// Test UpdateVerificationStatus
        /// </summary>
        [Test]
        public void UpdateVerificationStatusTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //string orderId = null;
            //UpdateVerificationStatusRequest payload = null;
            //instance.UpdateVerificationStatus(orderId, payload);
            
        }
        
    }

}