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
    ///  Class for testing Marketplace
    /// </summary>
    /// <remarks>
    /// This file is automatically generated by Swagger Codegen.
    /// Please update the test case below to test the model.
    /// </remarks>
    [TestFixture]
    public class MarketplaceTests
    {
        // TODO uncomment below to declare an instance variable for Marketplace
        //private Marketplace instance;

        /// <summary>
        /// Setup before each test
        /// </summary>
        [SetUp]
        public void Init()
        {
            // TODO uncomment below to create an instance of Marketplace
            //instance = new Marketplace();
        }

        /// <summary>
        /// Clean up after each test
        /// </summary>
        [TearDown]
        public void Cleanup()
        {

        }

        /// <summary>
        /// Test an instance of Marketplace
        /// </summary>
        [Test]
        public void MarketplaceInstanceTest()
        {
            // TODO uncomment below to test "IsInstanceOfType" Marketplace
            //Assert.IsInstanceOfType<Marketplace> (instance, "variable 'instance' is a Marketplace");
        }


        /// <summary>
        /// Test the property 'Id'
        /// </summary>
        [Test]
        public void IdTest()
        {
            // TODO unit test for the property 'Id'
        }
        /// <summary>
        /// Test the property 'Name'
        /// </summary>
        [Test]
        public void NameTest()
        {
            // TODO unit test for the property 'Name'
        }
        /// <summary>
        /// Test the property 'CountryCode'
        /// </summary>
        [Test]
        public void CountryCodeTest()
        {
            // TODO unit test for the property 'CountryCode'
        }
        /// <summary>
        /// Test the property 'DefaultCurrencyCode'
        /// </summary>
        [Test]
        public void DefaultCurrencyCodeTest()
        {
            // TODO unit test for the property 'DefaultCurrencyCode'
        }
        /// <summary>
        /// Test the property 'DefaultLanguageCode'
        /// </summary>
        [Test]
        public void DefaultLanguageCodeTest()
        {
            // TODO unit test for the property 'DefaultLanguageCode'
        }
        /// <summary>
        /// Test the property 'DomainName'
        /// </summary>
        [Test]
        public void DomainNameTest()
        {
            // TODO unit test for the property 'DomainName'
        }

    }

}