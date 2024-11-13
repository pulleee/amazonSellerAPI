//---------------------------------------------------------------------------

#ifndef AMRestAPIH
#define AMRestAPIH
//---------------------------------------------------------------------------
enum TFlatFormat {order_id,order_item_id,purchase_date,payments_date,buyer_email,buyer_name,buyer_phone_number,sku,product_name,quantity_purchased,currency,item_price,item_tax,shipping_price,shipping_tax,ship_service_level,recipient_name,ship_address_1,ship_address_2,ship_address_3,ship_city,ship_state,ship_postal_code,ship_country,ship_phone_number,bill_name,bill_address_1,bill_address_2,bill_address_3,bill_city,bill_state,bill_postal_code,bill_country,item_promotion_discount,item_promotion_id,ship_promotion_discount,ship_promotion_id,delivery_start_date,delivery_end_date,delivery_time_zone,delivery_Instructions,sales_channel,shipment_status};
//char *cpFlatFormat[]={"order_id","order_item_id","purchase_date","payments_date","buyer_email","buyer_name","buyer_phone_number","sku","product_name","quantity_purchased","currency","item_price","item_tax","shipping_price","shipping_tax","ship_service_level","recipient_name","ship_address_1","ship_address_2","ship_address_3","ship_city","ship_state","ship_postal_code","ship_country","ship_phone_number","bill_name","bill_address_1","bill_address_2","bill_address_3","bill_city","bill_state","bill_postal_code","bill_country","item_promotion_discount","item_promotion_id","ship_promotion_discount","ship_promotion_id","delivery_start_date","delivery_end_date","delivery_time_zone","delivery_Instructions","sales_channel","shipment_status", NULL};

void __fastcall serializeXML(AnsiString strXMLDatei, TStringList *slXML);
void __fastcall serializeXMLStr(UnicodeString strXMLStr, TStringList *slXML);

AnsiString urlEncode(AnsiString sURL);
AnsiString getAWSDateTime(TDateTime td);
AnsiString getSignature(AnsiString sData, AnsiString sKey);
String __fastcall BytesToBase64( TByteDynArray _ArrayIn );
String __fastcall StringToBase64(AnsiString s);
char char2int(char input);
AnsiString hextostr(AnsiString strHex);
AnsiString buildAWSParams(AnsiString sCMD, AnsiString sAction, AnsiString sRegion, AnsiString sRessource, TStringList *slParams);

UnicodeString getInventoryFeedXML(AnsiString sSKU, int iQuantity);
UnicodeString getProduktFeedXML(AnsiString sSKU);

// Public
bool getAMOrderList(int iDaysBack, TStringList *slContent);
bool getAMOrderListCanceled(int iDaysBack, TStringList *slContent);
bool getAMOrder(AnsiString strOrderId, TStringList *slContent);
bool getAMOrderItems(AnsiString strOrderId, TStringList *slContent);
AnsiString getAMServiceStatus(void);
bool getAMFeedSubmissionList(AnsiString sFeedTypeList);

bool requestAMReport(AnsiString strReportType, int iDaysBack);
bool getAMReportRequestList(AnsiString strReportId);
bool getAMReportList(AnsiString strReportType, TStringList *slResult);
bool getAMReport(AnsiString strReportId, TStringList *slContent);
bool setAMReportAcknowledged(AnsiString strReportId);

AnsiString submitAMFeed(AnsiString sFeedType, UnicodeString sXMLBody);
bool uploadAMInvoiceEx(AnsiString strOrderId, AnsiString strInvoiceNumber, AnsiString strPDFFile);
bool uploadAMInvoice(AnsiString strOrderId, AnsiString strInvoiceNumber, AnsiString strPDFFile);

typedef struct
{
	AnsiString sAWSAccessKeyId;
	AnsiString sAWSSecret;
	AnsiString sMWSAuthToken;
	AnsiString sSellerId;
} AWSCredential;

AWSCredential aws;

TStringList *slMarketPlaces;

//---------------------------------------------------------------------------
#endif