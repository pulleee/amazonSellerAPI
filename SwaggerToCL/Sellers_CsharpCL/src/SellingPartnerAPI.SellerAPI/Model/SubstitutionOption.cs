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
using System.Linq;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Runtime.Serialization;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System.ComponentModel.DataAnnotations;
using SwaggerDateConverter = SellingPartnerAPI.SellerAPI.Client.SwaggerDateConverter;

namespace SellingPartnerAPI.SellerAPI.Model
{
    /// <summary>
    /// Substitution options for an order item.
    /// </summary>
    [DataContract]
    public partial class SubstitutionOption :  IEquatable<SubstitutionOption>, IValidatableObject
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SubstitutionOption" /> class.
        /// </summary>
        /// <param name="aSIN">The item&#39;s Amazon Standard Identification Number (ASIN)..</param>
        /// <param name="quantityOrdered">The number of items to be picked for this substitution option. .</param>
        /// <param name="sellerSKU">The item&#39;s seller stock keeping unit (SKU)..</param>
        /// <param name="title">The item&#39;s title..</param>
        /// <param name="measurement">Measurement information for the substitution option..</param>
        public SubstitutionOption(string aSIN = default(string), int? quantityOrdered = default(int?), string sellerSKU = default(string), string title = default(string), Measurement measurement = default(Measurement))
        {
            this.ASIN = aSIN;
            this.QuantityOrdered = quantityOrdered;
            this.SellerSKU = sellerSKU;
            this.Title = title;
            this.Measurement = measurement;
        }
        
        /// <summary>
        /// The item&#39;s Amazon Standard Identification Number (ASIN).
        /// </summary>
        /// <value>The item&#39;s Amazon Standard Identification Number (ASIN).</value>
        [DataMember(Name="ASIN", EmitDefaultValue=false)]
        public string ASIN { get; set; }

        /// <summary>
        /// The number of items to be picked for this substitution option. 
        /// </summary>
        /// <value>The number of items to be picked for this substitution option. </value>
        [DataMember(Name="QuantityOrdered", EmitDefaultValue=false)]
        public int? QuantityOrdered { get; set; }

        /// <summary>
        /// The item&#39;s seller stock keeping unit (SKU).
        /// </summary>
        /// <value>The item&#39;s seller stock keeping unit (SKU).</value>
        [DataMember(Name="SellerSKU", EmitDefaultValue=false)]
        public string SellerSKU { get; set; }

        /// <summary>
        /// The item&#39;s title.
        /// </summary>
        /// <value>The item&#39;s title.</value>
        [DataMember(Name="Title", EmitDefaultValue=false)]
        public string Title { get; set; }

        /// <summary>
        /// Measurement information for the substitution option.
        /// </summary>
        /// <value>Measurement information for the substitution option.</value>
        [DataMember(Name="Measurement", EmitDefaultValue=false)]
        public Measurement Measurement { get; set; }

        /// <summary>
        /// Returns the string presentation of the object
        /// </summary>
        /// <returns>String presentation of the object</returns>
        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.Append("class SubstitutionOption {\n");
            sb.Append("  ASIN: ").Append(ASIN).Append("\n");
            sb.Append("  QuantityOrdered: ").Append(QuantityOrdered).Append("\n");
            sb.Append("  SellerSKU: ").Append(SellerSKU).Append("\n");
            sb.Append("  Title: ").Append(Title).Append("\n");
            sb.Append("  Measurement: ").Append(Measurement).Append("\n");
            sb.Append("}\n");
            return sb.ToString();
        }
  
        /// <summary>
        /// Returns the JSON string presentation of the object
        /// </summary>
        /// <returns>JSON string presentation of the object</returns>
        public virtual string ToJson()
        {
            return JsonConvert.SerializeObject(this, Formatting.Indented);
        }

        /// <summary>
        /// Returns true if objects are equal
        /// </summary>
        /// <param name="input">Object to be compared</param>
        /// <returns>Boolean</returns>
        public override bool Equals(object input)
        {
            return this.Equals(input as SubstitutionOption);
        }

        /// <summary>
        /// Returns true if SubstitutionOption instances are equal
        /// </summary>
        /// <param name="input">Instance of SubstitutionOption to be compared</param>
        /// <returns>Boolean</returns>
        public bool Equals(SubstitutionOption input)
        {
            if (input == null)
                return false;

            return 
                (
                    this.ASIN == input.ASIN ||
                    (this.ASIN != null &&
                    this.ASIN.Equals(input.ASIN))
                ) && 
                (
                    this.QuantityOrdered == input.QuantityOrdered ||
                    (this.QuantityOrdered != null &&
                    this.QuantityOrdered.Equals(input.QuantityOrdered))
                ) && 
                (
                    this.SellerSKU == input.SellerSKU ||
                    (this.SellerSKU != null &&
                    this.SellerSKU.Equals(input.SellerSKU))
                ) && 
                (
                    this.Title == input.Title ||
                    (this.Title != null &&
                    this.Title.Equals(input.Title))
                ) && 
                (
                    this.Measurement == input.Measurement ||
                    (this.Measurement != null &&
                    this.Measurement.Equals(input.Measurement))
                );
        }

        /// <summary>
        /// Gets the hash code
        /// </summary>
        /// <returns>Hash code</returns>
        public override int GetHashCode()
        {
            unchecked // Overflow is fine, just wrap
            {
                int hashCode = 41;
                if (this.ASIN != null)
                    hashCode = hashCode * 59 + this.ASIN.GetHashCode();
                if (this.QuantityOrdered != null)
                    hashCode = hashCode * 59 + this.QuantityOrdered.GetHashCode();
                if (this.SellerSKU != null)
                    hashCode = hashCode * 59 + this.SellerSKU.GetHashCode();
                if (this.Title != null)
                    hashCode = hashCode * 59 + this.Title.GetHashCode();
                if (this.Measurement != null)
                    hashCode = hashCode * 59 + this.Measurement.GetHashCode();
                return hashCode;
            }
        }

        /// <summary>
        /// To validate all properties of the instance
        /// </summary>
        /// <param name="validationContext">Validation context</param>
        /// <returns>Validation Result</returns>
        IEnumerable<System.ComponentModel.DataAnnotations.ValidationResult> IValidatableObject.Validate(ValidationContext validationContext)
        {
            yield break;
        }
    }

}