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
    /// Contains information regarding the Shipping Settings Automation program, such as whether the order&#39;s shipping settings were generated automatically, and what those settings are.
    /// </summary>
    [DataContract]
    public partial class AutomatedShippingSettings :  IEquatable<AutomatedShippingSettings>, IValidatableObject
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="AutomatedShippingSettings" /> class.
        /// </summary>
        /// <param name="hasAutomatedShippingSettings">When true, this order has automated shipping settings generated by Amazon. This order could be identified as an SSA order..</param>
        /// <param name="automatedCarrier">Auto-generated carrier for SSA orders..</param>
        /// <param name="automatedShipMethod">Auto-generated ship method for SSA orders..</param>
        public AutomatedShippingSettings(bool? hasAutomatedShippingSettings = default(bool?), string automatedCarrier = default(string), string automatedShipMethod = default(string))
        {
            this.HasAutomatedShippingSettings = hasAutomatedShippingSettings;
            this.AutomatedCarrier = automatedCarrier;
            this.AutomatedShipMethod = automatedShipMethod;
        }
        
        /// <summary>
        /// When true, this order has automated shipping settings generated by Amazon. This order could be identified as an SSA order.
        /// </summary>
        /// <value>When true, this order has automated shipping settings generated by Amazon. This order could be identified as an SSA order.</value>
        [DataMember(Name="HasAutomatedShippingSettings", EmitDefaultValue=false)]
        public bool? HasAutomatedShippingSettings { get; set; }

        /// <summary>
        /// Auto-generated carrier for SSA orders.
        /// </summary>
        /// <value>Auto-generated carrier for SSA orders.</value>
        [DataMember(Name="AutomatedCarrier", EmitDefaultValue=false)]
        public string AutomatedCarrier { get; set; }

        /// <summary>
        /// Auto-generated ship method for SSA orders.
        /// </summary>
        /// <value>Auto-generated ship method for SSA orders.</value>
        [DataMember(Name="AutomatedShipMethod", EmitDefaultValue=false)]
        public string AutomatedShipMethod { get; set; }

        /// <summary>
        /// Returns the string presentation of the object
        /// </summary>
        /// <returns>String presentation of the object</returns>
        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.Append("class AutomatedShippingSettings {\n");
            sb.Append("  HasAutomatedShippingSettings: ").Append(HasAutomatedShippingSettings).Append("\n");
            sb.Append("  AutomatedCarrier: ").Append(AutomatedCarrier).Append("\n");
            sb.Append("  AutomatedShipMethod: ").Append(AutomatedShipMethod).Append("\n");
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
            return this.Equals(input as AutomatedShippingSettings);
        }

        /// <summary>
        /// Returns true if AutomatedShippingSettings instances are equal
        /// </summary>
        /// <param name="input">Instance of AutomatedShippingSettings to be compared</param>
        /// <returns>Boolean</returns>
        public bool Equals(AutomatedShippingSettings input)
        {
            if (input == null)
                return false;

            return 
                (
                    this.HasAutomatedShippingSettings == input.HasAutomatedShippingSettings ||
                    (this.HasAutomatedShippingSettings != null &&
                    this.HasAutomatedShippingSettings.Equals(input.HasAutomatedShippingSettings))
                ) && 
                (
                    this.AutomatedCarrier == input.AutomatedCarrier ||
                    (this.AutomatedCarrier != null &&
                    this.AutomatedCarrier.Equals(input.AutomatedCarrier))
                ) && 
                (
                    this.AutomatedShipMethod == input.AutomatedShipMethod ||
                    (this.AutomatedShipMethod != null &&
                    this.AutomatedShipMethod.Equals(input.AutomatedShipMethod))
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
                if (this.HasAutomatedShippingSettings != null)
                    hashCode = hashCode * 59 + this.HasAutomatedShippingSettings.GetHashCode();
                if (this.AutomatedCarrier != null)
                    hashCode = hashCode * 59 + this.AutomatedCarrier.GetHashCode();
                if (this.AutomatedShipMethod != null)
                    hashCode = hashCode * 59 + this.AutomatedShipMethod.GetHashCode();
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