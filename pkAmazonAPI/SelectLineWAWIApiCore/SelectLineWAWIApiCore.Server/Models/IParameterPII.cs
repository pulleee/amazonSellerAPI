using SellingPartnerAPI.SellerAPI.Model;

namespace SelectLineWAWIApiCore.Server.Models
{
    public interface IParameterBasedPII
    {
        public bool NeedRDT { get; set; }
        public CreateRestrictedDataTokenRequest RDTRequest { get; set; }
    }
}
