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
    ///  Class for testing SellersApi
    /// </summary>
    /// <remarks>
    /// This file is automatically generated by Swagger Codegen.
    /// Please update the test case below to test the API endpoint.
    /// </remarks>
    [TestFixture]
    public class SellersApiTests
    {
        private SellersApi instance;

        public static void Main(string[] args)
        {
            try
            {
                LWAAuthorizationCredentials lwaAuthorizationCredentials = new LWAAuthorizationCredentials
                {
                    ClientId = "amzn1.application-oa2-client.******************",
                    ClientSecret = "***********************************",
                    RefreshToken = "Atzr|***********************************",
                    Endpoint = new Uri("https://api.amazon.com/auth/o2/token")
                };
                SellersApi sellersApi = new SellersApi.Builder()
                    .SetLWAAuthorizationCredentials(lwaAuthorizationCredentials)
                    .Build();

                GetMarketplaceParticipationsResponse result = sellersApi.GetMarketplaceParticipations();
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
            //instance = new SellersApi();
        }

        /// <summary>
        /// Clean up after each unit test
        /// </summary>
        [TearDown]
        public void Cleanup()
        {

        }

        /// <summary>
        /// Test an instance of SellersApi
        /// </summary>
        [Test]
        public void InstanceTest()
        {
            // TODO uncomment below to test 'IsInstanceOfType' SellersApi
            //Assert.IsInstanceOfType(typeof(SellersApi), instance, "instance is a SellersApi");
        }

        
        /// <summary>
        /// Test GetMarketplaceParticipations
        /// </summary>
        [Test]
        public void GetMarketplaceParticipationsTest()
        {
            // TODO uncomment below to test the method and replace null with proper value
            //var response = instance.GetMarketplaceParticipations();
            //Assert.IsInstanceOf<GetMarketplaceParticipationsResponse> (response, "response is GetMarketplaceParticipationsResponse");
        }
        
    }

}