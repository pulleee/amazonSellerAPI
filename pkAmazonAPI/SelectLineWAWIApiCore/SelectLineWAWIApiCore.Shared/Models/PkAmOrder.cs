namespace SelectLineWAWIApiCore.Shared.Models
{
    public class PkAmOrder()
    {
        public int? Id { get; set; }
        public DateTime? Processed { get; set; }
        public string Belegnummer { get; set; }
        public string Kennung { get; set; }
        public string OrderId { get; set; }
        public string OrderItemId { get; set; }
        public string PurchaseDate { get; set; }
        public string PaymentsDate { get; set; }
        public string BuyerEmail { get; set; }
        public string BuyerName { get; set; }
        public string PaymentMethodDetails { get; set; }
        public string BuyerPhoneNumber { get; set; }
        public string Sku { get; set; }
        public string Asin { get; set; }
        public string ProductName { get; set; }
        public int? QuantityPurchased { get; set; }
        public string Currency { get; set; }
        public double? ItemPrice { get; set; }
        public double? ItemTax { get; set; }
        public double? ShippingPrice { get; set; }
        public double? ShippingTax { get; set; }
        public string ShipServiceLevel { get; set; }
        public string RecipientName { get; set; }
        public string ShipAddress1 { get; set; }
        public string ShipAddress2 { get; set; }
        public string ShipAddress3 { get; set; }
        public string AddressType { get; set; }
        public string ShipCity { get; set; }
        public string ShipState { get; set; }
        public string ShipPostalCode { get; set; }
        public string ShipCountry { get; set; }
        public string ShipPhoneNumber { get; set; }
        public string BillName { get; set; }
        public string BillAddress1 { get; set; }
        public string BillAddress2 { get; set; }
        public string BillAddress3 { get; set; }
        public string BillCity { get; set; }
        public string BillState { get; set; }
        public string BillPostalCode { get; set; }
        public string BillCountry { get; set; }
        public string IsBusinessOrder { get; set; }
        public string PurchaseOrderNumber { get; set; }
        public string PriceDesignation { get; set; }
        public string IsPremiumOrder { get; set; }
        public string IsAmazonInvoiced { get; set; }
        public string IsPrime { get; set; }
        public double? VatExclusiveItemPrice { get; set; }
        public double? VatExclusiveShippingPrice { get; set; }
        public double? VatExclusiveGiftwrapPrice { get; set; }
        public double? ItemPromotionDiscount { get; set; }
        public string ItemPromotionId { get; set; }
        public double? ShipPromotionDiscount { get; set; }
        public string ShipPromotionId { get; set; }
        public string DeliveryStartDate { get; set; }
        public string DeliveryEndDate { get; set; }
        public string DeliveryTimeZone { get; set; }
        public string DeliveryInstructions { get; set; }
        public string SalesChannel { get; set; }
        public string ShipmentStatus { get; set; }
        public string IsSoldByAb { get; set; }
        public string FulfilledBy { get; set; }
        public string BuyerCompanyName { get; set; }
        public string BuyerCstNumber { get; set; }
        public string BuyerVatNumber { get; set; }
        public string BuyerTaxRegistrationId { get; set; }
        public string BuyerTaxRegistrationCountry { get; set; }
        public string BuyerTaxRegistrationType { get; set; }
        public string IsIba { get; set; }
        public string IsBuyerRequestedCancellation { get; set; }
        public string IsPickupPointOrder { get; set; }
    }    
}