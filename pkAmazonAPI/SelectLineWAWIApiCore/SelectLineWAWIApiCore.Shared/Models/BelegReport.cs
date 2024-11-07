using System.Globalization;

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

        public static BelegReport ParseTextToBeleg(string data)
        {
            var fields = data.Split('\t');

            return new BelegReport
            {
                OrderId = fields[0],
                OrderItemId = fields[1],
                PurchaseDate = DateTime.Parse(fields[2], null, DateTimeStyles.RoundtripKind),
                PaymentsDate = DateTime.Parse(fields[3], null, DateTimeStyles.RoundtripKind),
                BuyerEmail = fields[4],
                BuyerName = fields[5],
                BuyerPhoneNumber = fields[6],
                Sku = fields[7],
                ProductName = fields[8],
                QuantityPurchased = int.Parse(fields[9]),
                Currency = fields[10],
                ItemPrice = decimal.Parse(fields[11], CultureInfo.InvariantCulture),
                ItemTax = decimal.Parse(fields[12], CultureInfo.InvariantCulture),
                ShippingPrice = decimal.Parse(fields[13], CultureInfo.InvariantCulture),
                ShippingTax = decimal.Parse(fields[14], CultureInfo.InvariantCulture),
                ShipServiceLevel = fields[15],
                RecipientName = fields[16],
                ShipAddress1 = fields[17],
                ShipAddress2 = fields[18],
                ShipAddress3 = fields[19],
                ShipCity = fields[20],
                ShipState = fields[21],
                ShipPostalCode = fields[22],
                ShipCountry = fields[23],
                ShipPhoneNumber = fields[24],
                BillName = fields[25],
                BillAddress1 = fields[26],
                BillAddress2 = fields[27],
                BillAddress3 = fields[28],
                BillCity = fields[29],
                BillState = fields[30],
                BillPostalCode = fields[31],
                BillCountry = fields[32],
                ItemPromotionDiscount = decimal.Parse(fields[33], CultureInfo.InvariantCulture),
                ItemPromotionId = fields[34],
                ShipPromotionDiscount = decimal.Parse(fields[35], CultureInfo.InvariantCulture),
                ShipPromotionId = fields[36],
                DeliveryStartDate = string.IsNullOrWhiteSpace(fields[37]) ? (DateTime?)null : DateTime.Parse(fields[37], null, DateTimeStyles.RoundtripKind),
                DeliveryEndDate = string.IsNullOrWhiteSpace(fields[38]) ? (DateTime?)null : DateTime.Parse(fields[38], null, DateTimeStyles.RoundtripKind),
                DeliveryTimeZone = fields[39],
                DeliveryInstructions = fields[40],
                SalesChannel = fields[41],
                ShipmentStatus = fields[42]
            };
        }
    }    
}
