/* 
 * Orders v0
 *
 * Use the Orders Selling Partner API to programmatically retrieve order information. With this API, you can develop fast, flexible, and custom applications to manage order synchronization, perform order research, and create demand-based decision support tools.   _Note:_ For the JP, AU, and SG marketplaces, the Orders API supports orders from 2016 onward. For all other marketplaces, the Orders API supports orders for the last two years (orders older than this don't show up in the response).
 *
 * OpenAPI spec version: v0
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
    ///  Class for testing OrdersList
    /// </summary>
    /// <remarks>
    /// This file is automatically generated by Swagger Codegen.
    /// Please update the test case below to test the model.
    /// </remarks>
    [TestFixture]
    public class OrdersListTests
    {
        // TODO uncomment below to declare an instance variable for OrdersList
        //private OrdersList instance;

        /// <summary>
        /// Setup before each test
        /// </summary>
        [SetUp]
        public void Init()
        {
            // TODO uncomment below to create an instance of OrdersList
            //instance = new OrdersList();
        }

        /// <summary>
        /// Clean up after each test
        /// </summary>
        [TearDown]
        public void Cleanup()
        {

        }

        /// <summary>
        /// Test an instance of OrdersList
        /// </summary>
        [Test]
        public void OrdersListInstanceTest()
        {
            // TODO uncomment below to test "IsInstanceOfType" OrdersList
            //Assert.IsInstanceOfType<OrdersList> (instance, "variable 'instance' is a OrdersList");
        }


        /// <summary>
        /// Test the property 'Orders'
        /// </summary>
        [Test]
        public void OrdersTest()
        {
            // TODO unit test for the property 'Orders'
        }
        /// <summary>
        /// Test the property 'NextToken'
        /// </summary>
        [Test]
        public void NextTokenTest()
        {
            // TODO unit test for the property 'NextToken'
        }
        /// <summary>
        /// Test the property 'LastUpdatedBefore'
        /// </summary>
        [Test]
        public void LastUpdatedBeforeTest()
        {
            // TODO unit test for the property 'LastUpdatedBefore'
        }
        /// <summary>
        /// Test the property 'CreatedBefore'
        /// </summary>
        [Test]
        public void CreatedBeforeTest()
        {
            // TODO unit test for the property 'CreatedBefore'
        }

    }

}