namespace SelectLineWAWIApiCore.Shared.Models
{
    public class BelegReport
    {
        public string OrderId { get; set; }
        public string OrderItemId { get; set; }
        public DateTime PurchaseDate { get; set; }
        public DateTime PaymentsDate { get; set; }
        public string BuyerEmail { get; set; }
        public string BuyerName { get; set; }
        public string BuyerPhoneNumber { get; set; }
        public string Sku { get; set; }
        public string ProductName { get; set; }
        public int QuantityPurchased { get; set; }
        public string Currency { get; set; }
        public decimal ItemPrice { get; set; }
        public decimal ItemTax { get; set; }
        public decimal ShippingPrice { get; set; }
        public decimal ShippingTax { get; set; }
        public string ShipServiceLevel { get; set; }
        public string RecipientName { get; set; }
        public string ShipAddress1 { get; set; }
        public string ShipAddress2 { get; set; }
        public string ShipAddress3 { get; set; }
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
        public decimal ItemPromotionDiscount { get; set; }
        public string ItemPromotionId { get; set; }
        public decimal ShipPromotionDiscount { get; set; }
        public string ShipPromotionId { get; set; }
        public DateTime? DeliveryStartDate { get; set; }
        public DateTime? DeliveryEndDate { get; set; }
        public string DeliveryTimeZone { get; set; }
        public string DeliveryInstructions { get; set; }
        public string SalesChannel { get; set; }
        public string ShipmentStatus { get; set; }
    }
}
