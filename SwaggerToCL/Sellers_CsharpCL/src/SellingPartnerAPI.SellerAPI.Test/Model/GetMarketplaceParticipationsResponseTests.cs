/* 
 * Selling Partner API for Sellers
 *
 * The Selling Partner API for Sellers lets you retrieve information on behalf of sellers about their seller account, such as the marketplaces they participate in. Along with listing the marketplaces that a seller can sell in, the API also provides additional information about the marketplace such as the default language and the default currency. The API also provides seller-specific information such as whether the seller has suspended listings in that marketplace.
 *
 * OpenAPI spec version: v1
 * 
 * Generated by: https://github.com/swagger-api/swagger-codegen.git
 */


using NUnit.Framework;

using System;
using System.Linq;
using System.IO;
using System.Collections.Generic;
using SellingPartnerAPI.SellerAPI.Api;
using SellingPartnerAPI.SellerAPI.Model;
using SellingPartnerAPI.SellerAPI.Client;
using System.Reflection;
using Newtonsoft.Json;

namespace SellingPartnerAPI.SellerAPI.Test
{
    /// <summary>
    ///  Class for testing GetMarketplaceParticipationsResponse
    /// </summary>
    /// <remarks>
    /// This file is automatically generated by Swagger Codegen.
    /// Please update the test case below to test the model.
    /// </remarks>
    [TestFixture]
    public class GetMarketplaceParticipationsResponseTests
    {
        // TODO uncomment below to declare an instance variable for GetMarketplaceParticipationsResponse
        //private GetMarketplaceParticipationsResponse instance;

        /// <summary>
        /// Setup before each test
        /// </summary>
        [SetUp]
        public void Init()
        {
            // TODO uncomment below to create an instance of GetMarketplaceParticipationsResponse
            //instance = new GetMarketplaceParticipationsResponse();
        }

        /// <summary>
        /// Clean up after each test
        /// </summary>
        [TearDown]
        public void Cleanup()
        {

        }

        /// <summary>
        /// Test an instance of GetMarketplaceParticipationsResponse
        /// </summary>
        [Test]
        public void GetMarketplaceParticipationsResponseInstanceTest()
        {
            // TODO uncomment below to test "IsInstanceOfType" GetMarketplaceParticipationsResponse
            //Assert.IsInstanceOfType<GetMarketplaceParticipationsResponse> (instance, "variable 'instance' is a GetMarketplaceParticipationsResponse");
        }


        /// <summary>
        /// Test the property 'Payload'
        /// </summary>
        [Test]
        public void PayloadTest()
        {
            // TODO unit test for the property 'Payload'
        }
        /// <summary>
        /// Test the property 'Errors'
        /// </summary>
        [Test]
        public void ErrorsTest()
        {
            // TODO unit test for the property 'Errors'
        }

    }

}