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
    /// The status of the Amazon Easy Ship order. This property is only included for Amazon Easy Ship orders.
    /// </summary>
    /// <value>The status of the Amazon Easy Ship order. This property is only included for Amazon Easy Ship orders.</value>
    
    [JsonConverter(typeof(StringEnumConverter))]
    
    public enum EasyShipShipmentStatus
    {
        
        /// <summary>
        /// Enum PendingSchedule for value: PendingSchedule
        /// </summary>
        [EnumMember(Value = "PendingSchedule")]
        PendingSchedule = 1,
        
        /// <summary>
        /// Enum PendingPickUp for value: PendingPickUp
        /// </summary>
        [EnumMember(Value = "PendingPickUp")]
        PendingPickUp = 2,
        
        /// <summary>
        /// Enum PendingDropOff for value: PendingDropOff
        /// </summary>
        [EnumMember(Value = "PendingDropOff")]
        PendingDropOff = 3,
        
        /// <summary>
        /// Enum LabelCanceled for value: LabelCanceled
        /// </summary>
        [EnumMember(Value = "LabelCanceled")]
        LabelCanceled = 4,
        
        /// <summary>
        /// Enum PickedUp for value: PickedUp
        /// </summary>
        [EnumMember(Value = "PickedUp")]
        PickedUp = 5,
        
        /// <summary>
        /// Enum DroppedOff for value: DroppedOff
        /// </summary>
        [EnumMember(Value = "DroppedOff")]
        DroppedOff = 6,
        
        /// <summary>
        /// Enum AtOriginFC for value: AtOriginFC
        /// </summary>
        [EnumMember(Value = "AtOriginFC")]
        AtOriginFC = 7,
        
        /// <summary>
        /// Enum AtDestinationFC for value: AtDestinationFC
        /// </summary>
        [EnumMember(Value = "AtDestinationFC")]
        AtDestinationFC = 8,
        
        /// <summary>
        /// Enum Delivered for value: Delivered
        /// </summary>
        [EnumMember(Value = "Delivered")]
        Delivered = 9,
        
        /// <summary>
        /// Enum RejectedByBuyer for value: RejectedByBuyer
        /// </summary>
        [EnumMember(Value = "RejectedByBuyer")]
        RejectedByBuyer = 10,
        
        /// <summary>
        /// Enum Undeliverable for value: Undeliverable
        /// </summary>
        [EnumMember(Value = "Undeliverable")]
        Undeliverable = 11,
        
        /// <summary>
        /// Enum ReturningToSeller for value: ReturningToSeller
        /// </summary>
        [EnumMember(Value = "ReturningToSeller")]
        ReturningToSeller = 12,
        
        /// <summary>
        /// Enum ReturnedToSeller for value: ReturnedToSeller
        /// </summary>
        [EnumMember(Value = "ReturnedToSeller")]
        ReturnedToSeller = 13,
        
        /// <summary>
        /// Enum Lost for value: Lost
        /// </summary>
        [EnumMember(Value = "Lost")]
        Lost = 14,
        
        /// <summary>
        /// Enum OutForDelivery for value: OutForDelivery
        /// </summary>
        [EnumMember(Value = "OutForDelivery")]
        OutForDelivery = 15,
        
        /// <summary>
        /// Enum Damaged for value: Damaged
        /// </summary>
        [EnumMember(Value = "Damaged")]
        Damaged = 16
    }

}