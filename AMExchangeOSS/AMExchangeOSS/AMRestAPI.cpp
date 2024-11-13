//---------------------------------------------------------------------------

#pragma hdrstop

//#include "AMRestAPI.h"
#include "Unit1.h"
#include "Xml.hpp"

#include <inifiles.hpp>
#include <strutils.hpp>
#include <System.dateutils.hpp>
#include <fstream.h>
#include <System.Hash.hpp>
#include <System.NetEncoding.hpp>

#include <map>
#include <vector>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

bool TfrmMain::getAMOrderList(int iDaysBack, TStringList *slContent)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Orders/2013-09-01";
	TDateTime td;

	TStringList *slParams=new TStringList();

	td=IncDay(DateOf(Now()), -iDaysBack);
	s=getAWSDateTime(td);
	slParams->Add("CreatedAfter="+s);
	slParams->Add("MarketplaceId.Id.1=A1PA6795UKMFR9");

	sParams=buildAWSParams("POST", "ListOrders", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		//RESTRequest1->Accept="application/json";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		try
			{
			serializeXMLStr(RESTResponse1->Content, slContent);
			}
		catch(Exception &e)
			{
			log("XML-Fehler:"+e.Message);
			log(RESTResponse1->Content);
			bSt=false;
			}
		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);
		}

	delete slParams;
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMOrderListCanceled(int iDaysBack, TStringList *slContent)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Orders/2013-09-01";
	TDateTime td;

	TStringList *slParams=new TStringList();

	td=IncDay(DateOf(Now()), -iDaysBack);
	s=getAWSDateTime(td);
	slParams->Add("OrderStatus.Status.1=Canceled");
	slParams->Add("LastUpdatedAfter="+s);
	slParams->Add("MarketplaceId.Id.1=A1PA6795UKMFR9");

	sParams=buildAWSParams("POST", "ListOrders", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		//RESTRequest1->Accept="application/json";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		try
			{
			serializeXMLStr(RESTResponse1->Content, slContent);
			}
		catch(Exception &e)
			{
			log("XML-Fehler:"+e.Message);
			log(RESTResponse1->Content);
			bSt=false;
			}
		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);
		}

	delete slParams;
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMOrder(AnsiString strOrderId, TStringList *slContent)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Orders/2013-09-01";

	TStringList *slParams=new TStringList();

	slParams->Add("AmazonOrderId.Id.1="+strOrderId);

	sParams=buildAWSParams("POST", "GetOrder", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		try
			{
			serializeXMLStr(RESTResponse1->Content, slContent);
			}
		catch(Exception &e)
			{
			log("XML-Fehler:"+e.Message);
			log(RESTResponse1->Content);
			bSt=false;
			}

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);
		}

	delete slParams;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMOrderItems(AnsiString strOrderId, TStringList *slContent)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Orders/2013-09-01";

	TStringList *slParams=new TStringList();

	slParams->Add("AmazonOrderId="+strOrderId);

	sParams=buildAWSParams("POST", "ListOrderItems", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		try
			{
			serializeXMLStr(RESTResponse1->Content, slContent);
			}
		catch(Exception &e)
			{
			log("XML-Fehler:"+e.Message);
			log(RESTResponse1->Content);
			bSt=false;
			}

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);
		}

	delete slParams;

	return(bSt);
}

//---------------------------------------------------------------------------

AnsiString TfrmMain::getAMServiceStatus(void)
{
	AnsiString s, sSignature, sStatus;
	AnsiString sParams;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Orders/2013-09-01";

	TStringList *slParams=new TStringList();

	sParams=buildAWSParams("POST", "GetServiceStatus", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);

		sStatus=slContent->Values["GetServiceStatusResponse.GetServiceStatusResult.Status"];

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);

		delete slContent;
		}

	delete slParams;

	return(sStatus);
}

//---------------------------------------------------------------------------

bool TfrmMain::requestAMReport(AnsiString strReportType, int iDaysBack)
{
	AnsiString s;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Reports/2009-01-01";
	//AnsiString sRessource="/";

	td=Now();
	td=IncDay(td, -iDaysBack);

	TStringList *slParams=new TStringList();
	slParams->Values["ReportType"]=strReportType;
	slParams->Values["StartDate"]=getAWSDateTime(td);
	slParams->Values["MarketplaceId.Id.1"]="A1PA6795UKMFR9";

	sParams=buildAWSParams("POST", "RequestReport", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);

		if(slContent->Values["RequestReportResponse.RequestReportResult.ReportRequestInfo.ReportProcessingStatus"] == "_SUBMITTED_")
			bSt=true;
		else
			bSt=false;

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);

		delete slContent;
		}

	delete slParams;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMReportRequestList(AnsiString strReportId)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Reports/2009-01-01";

	TStringList *slParams=new TStringList();
	slParams->Values["ReportProcessingStatusList.Status.1"]="_DONE_";
	slParams->Values["ReportRequestIdList.Id.1"]=strReportId;

	sParams=buildAWSParams("POST", "GetReportRequestList", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);

		//sStatus=slContent->Values["GetServiceStatusResponse.GetServiceStatusResult.Status"];

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);

		delete slContent;
		}

	delete slParams;
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMReportList(AnsiString strReportType, TStringList *slResult)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Reports/2009-01-01";

	TStringList *slParams=new TStringList();
  slParams->Values["MaxCount"]="100";
	if(strReportType != "")
		slParams->Values["ReportTypeList.Type.1"]=strReportType;


	sParams=buildAWSParams("POST", "GetReportList", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		serializeXMLStr(RESTResponse1->Content, slResult);
		if(_iDebugLevel > 0)
			memo->Lines->Add(slResult->Text);
		}

	delete slParams;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMReport(AnsiString strReportId, TStringList *slContent)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	TDateTime td;

	//slContent->LoadFromFile("C:\\pk-ware\\shopware\\teaworld\\amazon\\flat-order-report.txt", System::Sysutils::TEncoding::UTF8);
	//return(true);

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Reports/2009-01-01";

	TStringList *slParams=new TStringList();
	slParams->Values["ReportId"]=strReportId;

	sParams=buildAWSParams("POST", "GetReport", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		if(RESTResponse1->StatusCode != 200)
			{
			log("Fehler GetReport:"+RESTResponse1->StatusText);
			bSt=false;
			}
		else
			{
			if(_iDebugLevel > 1)
				log(RESTResponse1->Content);

			slContent->Text=RESTResponse1->Content;

			if(_iDebugLevel > 2)
				{
				ofstream rpt;
				s="report-"+AnsiReplaceStr(AnsiReplaceStr(Now().DateTimeString(), ":", "_"), ".", "_")+".txt";
				rpt.open(s.c_str(), ios::out|ios::binary);
				if(!rpt)
					log("report kann nicht angelegt werden!");
				else
					{
					rpt << AnsiString(RESTResponse1->Content).c_str();
					rpt.close();
					}
        }
			}
		}

	delete slParams;
	return(bSt);
}

//---------------------------------------------------------------------------

bool TfrmMain::setAMReportAcknowledged(AnsiString strReportId)
{
 	AnsiString s, sSignature;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Reports/2009-01-01";

	TStringList *slParams=new TStringList();
	if(strReportId != "")
		slParams->Values["ReportIdList.Id.1"]=strReportId;

	sParams=buildAWSParams("POST", "UpdateReportAcknowledgements", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);
		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);



		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);
		}

	delete slParams;

	return(bSt);
}

//---------------------------------------------------------------------------

AnsiString TfrmMain::submitAMFeed(AnsiString sFeedType, UnicodeString sXMLBody)
{
	AnsiString s, sSignature, sStatus, sUnHex, sMD5Param;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Feeds/2009-01-01";

	TStringList *slParams=new TStringList();
	slParams->Values["FeedType"]=sFeedType;
	//slParams->Values["MarketplaceId.Id.1"]="A1PA6795UKMFR9";

	s=THashMD5::GetHashString(sXMLBody);
	sUnHex=hextostr(s);
	sMD5Param=StringToBase64(sUnHex);
	sMD5Param=urlEncode(sMD5Param);

	slParams->Values["ContentMD5Value"]=sMD5Param;

	sParams=buildAWSParams("POST", "SubmitFeed", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		{
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);


		if(_iDebugLevel > 2)
			{
			ofstream feed;
			s="feed-"+sFeedType+"-"+AnsiReplaceStr(AnsiReplaceStr(Now().DateTimeString(), ":", "_"), ".", "_")+".xml";
			feed.open(s.c_str(), ios::out|ios::binary);
			if(!feed)
				log("feed xml kann nicht angelegt werden!");
			else
				{
				feed << AnsiString(sXMLBody).c_str();
				feed.close();
				}
			}

		}

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->AddBody(AnsiString(sXMLBody), ctTEXT_XML);
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		log(RESTResponse1->Content);

		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);

		sStatus=slContent->Values["SubmitFeedResponse.SubmitFeedResult.FeedSubmissionInfo.FeedProcessingStatus"];
		/*
		if(sStatus != "_SUBMITTED_")
			{
			;
			}
		*/

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);

		delete slContent;
		}

	delete slParams;

	return(sStatus);
}

//---------------------------------------------------------------------------

bool TfrmMain::getAMFeedSubmissionList(AnsiString strFeedType)
{
	AnsiString s, sSignature;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Feeds/2009-01-01";

	TStringList *slParams=new TStringList();
	//slParams->Values["ReportId"]=strReportId;
	slParams->Values["MaxCount"]="5";

	sParams=buildAWSParams("POST", "GetFeedSubmissionList", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->Client->ContentType="text/xml";
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		if(RESTResponse1->StatusCode != 200)
			{
			log("Fehler GetFeedSubmissionList:"+RESTResponse1->StatusText);
			bSt=false;
			}
		else
			{
			TStringList *slContent=new TStringList();

			serializeXMLStr(RESTResponse1->Content, slContent);

			if(_iDebugLevel > 0)
				memo->Lines->Add(slContent->Text);

			delete slContent;
			}
		}

	delete slParams;
	return(bSt);
}

//---------------------------------------------------------------------------

bool TfrmMain::uploadAMInvoiceEx(AnsiString strOrderId, AnsiString strInvoiceNumber, AnsiString strPDFFile)
{
	AnsiString s, sSignature, sStatus, sUnHex, sMD5Param;
	AnsiString sParams;
	TDateTime td;
	TRESTRequest *restRq=new TRESTRequest(this);
	restRq->Client=RESTClient1;
	restRq->Response=RESTResponse1;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Feeds/2009-01-01";

	TStringList *slParams=new TStringList();
	slParams->Values["FeedType"]="_UPLOAD_VAT_INVOICE_";
	slParams->Values["MarketplaceIdList.Id.1"]="A1PA6795UKMFR9";
	slParams->Values["FeedOptions"]="metadata%3AOrderId%3D"+strOrderId+"%3Bmetadata%3AInvoiceNumber%3D"+strInvoiceNumber;

	s=THashMD5::GetHashStringFromFile(strPDFFile);
	sUnHex=hextostr(s);
	sMD5Param=StringToBase64(sUnHex);
	sMD5Param=urlEncode(sMD5Param);

	slParams->Values["ContentMD5Value"]=sMD5Param;

	sParams=buildAWSParams("POST", "SubmitFeed", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	restRq->Resource=sRessource+"?"+sParams;

	restRq->Method=rmPOST;

	if(_iDebugLevel > 0)
		log("Request:"+restRq->Resource+" Method:"+restRq->Method);

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		restRq->ClearBody();
		restRq->AddFile(strPDFFile, ctAPPLICATION_OCTET_STREAM);
		restRq->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		log(RESTResponse1->Content);

		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);

		sStatus=slContent->Values["SubmitFeedResponse.SubmitFeedResult.FeedSubmissionInfo.FeedProcessingStatus"];

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);

		delete slContent;
		}

	delete slParams;
	delete restRq;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TfrmMain::uploadAMInvoice(AnsiString strOrderId, AnsiString strInvoiceNumber, AnsiString strPDFFile)
{
	AnsiString s, sSignature, sStatus, sUnHex, sMD5Param;
	AnsiString sParams;
	TDateTime td;

	AnsiString sRegion="mws.amazonservices.de";
	AnsiString sRessource="/Feeds/2009-01-01";

	TStringList *slParams=new TStringList();
	slParams->Values["FeedType"]="_UPLOAD_VAT_INVOICE_";
	slParams->Values["MarketplaceIdList.Id.1"]="A1PA6795UKMFR9";
	slParams->Values["FeedOptions"]="metadata%3AOrderId%3D"+strOrderId+"%3Bmetadata%3AInvoiceNumber%3D"+strInvoiceNumber;

	s=THashMD5::GetHashStringFromFile(strPDFFile);
	sUnHex=hextostr(s);
	sMD5Param=StringToBase64(sUnHex);
	sMD5Param=urlEncode(sMD5Param);

	slParams->Values["ContentMD5Value"]=sMD5Param;

	sParams=buildAWSParams("POST", "SubmitFeed", sRegion, sRessource, slParams);

	RESTClient1->BaseURL="https://"+sRegion;
	RESTRequest1->Resource=sRessource+"?"+sParams;

	RESTRequest1->Method=rmPOST;

	if(_iDebugLevel > 0)
		log("Request:"+RESTRequest1->Resource+" Method:"+RESTRequest1->Method);

	bool bSt=true;
	TJSONValue *jValue;
	try
		{
		RESTRequest1->ClearBody();
		RESTRequest1->AddFile(strPDFFile, ctAPPLICATION_OCTET_STREAM);
		RESTRequest1->Execute();
		}
	catch(Exception &e)
		{
		bSt=false;
		log("Exception:");
		log(e.Message);
		}
	if(bSt)
		{
		log(RESTResponse1->Content);

		TStringList *slContent=new TStringList();

		serializeXMLStr(RESTResponse1->Content, slContent);

		sStatus=slContent->Values["SubmitFeedResponse.SubmitFeedResult.FeedSubmissionInfo.FeedProcessingStatus"];

		if(_iDebugLevel > 0)
			memo->Lines->Add(slContent->Text);

		delete slContent;
		}

	delete slParams;

	return(bSt);
}

//---------------------------------------------------------------------------

UnicodeString TfrmMain::getInventoryFeedXML(AnsiString sSKU, int iQuantity)
{
	UnicodeString xml;

	xml=
		//"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
		"<?xml version=\"1.0\"?>"
		"<AmazonEnvelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"amzn-envelope.xsd\">"
		"<Header>"
		"<DocumentVersion>1.01</DocumentVersion>"
		"<MerchantIdentifier>"+aws.sSellerId+"</MerchantIdentifier>"
		"</Header>"
		"<MessageType>Inventory</MessageType>"
		"<Message>"
		"<MessageID>1</MessageID>"
		"<OperationType>Update</OperationType>"
		"<Inventory>"
		"<SKU>"+sSKU+"</SKU>"
		"<Quantity>"+IntToStr(iQuantity)+"</Quantity>"
		"<FulfillmentLatency>1</FulfillmentLatency>"
		"</Inventory>"
		/*
		"<Inventory>"
		"<SKU>V10628</SKU>"
		"<Quantity>97</Quantity>"
		"<FulfillmentLatency>1</FulfillmentLatency>"
		"</Inventory>"
		"</Message>"
		"<Message>"
		"<MessageID>2</MessageID>"
		"<OperationType>Update</OperationType>"
		"<Inventory>"
		"<SKU>V10681</SKU>"
		"<Quantity>96</Quantity>"
		"<FulfillmentLatency>1</FulfillmentLatency>"
		"</Inventory>"
		*/
		"</Message>"
		"</AmazonEnvelope>";

	return(xml);
}

//---------------------------------------------------------------------------

UnicodeString TfrmMain::getProduktFeedXML(AnsiString sSKU)
{
	UnicodeString xml;
	UnicodeString sTitle, sBrand, sDescription, sManufacturer;
	UnicodeString sItemType;
	TStringList *slBulletPoint=new TStringList();
	TStringList *slSearchTerms=new TStringList();

	slBulletPoint->Add("frisch abgepackt");

	slSearchTerms->Add("Tee");

	xml=
		"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
		"<AmazonEnvelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"amzn-envelope.xsd\">"
		"<Header>"
		"<DocumentVersion>1.01</DocumentVersion>"
		"<MerchantIdentifier>"+aws.sSellerId+"</MerchantIdentifier>"
		"</Header>"
		"<MessageType>Product</MessageType>"
		"<PurgeAndReplace>true</PurgeAndReplace>"
		"<Message>"
		"<MessageID>1</MessageID>"
		"<OperationType>Update</OperationType>"
		"<Product>"
		"<SKU>"+sSKU+"</SKU>"
		"<DescriptionData>"
		"<Title>"+sTitle+"</Title>"
		"<Brand>"+sBrand+"</Brand>"
		"<Description>"+sDescription+"</Description>";
	for(int i=0; i < slBulletPoint->Count; i++)
		xml+="<BulletPoint>"+slBulletPoint->Strings[i]+"</BulletPoint>";

	xml+=
		"<Manufacturer>"+sManufacturer+"</Manufacturer>";

	for(int i=0; i < slSearchTerms->Count; i++)
		xml+="<SearchTerms>"+slSearchTerms->Strings[i]+"</SearchTerms>";

	xml+=
		"<ItemType>"+sItemType+"</ItemType>"
		"<IsGiftWrapAvailable>false</IsGiftWrapAvailable>"
		"<IsGiftMessageAvailable>false</IsGiftMessageAvailable>"
		"</DescriptionData>"
		"<ProductData>";
		//"<Home>"
		//"<Parentage>variation-parent</Parentage>
		//"<VariationData>"
		//"<VariationTheme>"+sVariation+"</VariationTheme>"
		//"</VariationData>";
	//if(sMaterial != "")
		//xml+="<Material>"+sMaterial+"</Material>;

	xml+=
		//"<ThreadCount>500</ThreadCount>"
		//"</Home>"
		"</ProductData>"
		"</Product>"
		"</Message>"
		"<Message>"
		"</AmazonEnvelope>";

	delete slSearchTerms;
	delete slBulletPoint;

	return(xml);
}

//---------------------------------------------------------------------------

AnsiString TfrmMain::buildAWSParams(AnsiString sCMD, AnsiString sAction, AnsiString sRegion, AnsiString sRessource, TStringList *slParams)
{
	AnsiString s, sParams, sToSign, sSignature, strVersion;
	AnsiString sUnHex;
	TDateTime td;
	td=Now();

	TStringList *slAWS=new TStringList();

	strVersion=sRessource.SubString(sRessource.LastDelimiter("/")+1, 10);

	//sToSign="POST\n";
	sToSign=sCMD+"\n";
	//sToSign+="mws.amazonservices.de\n";
	sToSign+=sRegion+"\n";
	//sToSign+="/Orders/2013-09-01\n";
	sToSign+=sRessource+"\n";

	slAWS->Values["AWSAccessKeyId"]=aws.sAWSAccessKeyId;
	slAWS->Values["Action"]=sAction;
	slAWS->Values["SellerId"]=aws.sSellerId;
	slAWS->Values["MWSAuthToken"]=aws.sMWSAuthToken;
	slAWS->Values["SignatureVersion"]="2";
	//s=td.FormatString("yyyy-mm-dd")+"T"+td.FormatString("hh:nn:ss")+"Z";
	//s=ReplaceStr(s, ":", "%3A");
	s=getAWSDateTime(td);
	slAWS->Values["Timestamp"]=s;
	//slAWS->Values["Timestamp"]="2020-10-31T14%3A22%3A25Z";
	//slAWS->Values["Version"]="2013-09-01";
  slAWS->Values["Version"]=strVersion;
	slAWS->Values["SignatureMethod"]="HmacSHA256";

	for(int i=0; i < slParams->Count; i++)
		slAWS->Values[slParams->Names[i]]=slParams->ValueFromIndex[i];

	// in Map übernehmen für die alphabetische Sortierung
	std::map<AnsiString, AnsiString> mapParams;
	for(int i=0; i < slAWS->Count; i++)
		mapParams[slAWS->Names[i]]=slAWS->ValueFromIndex[i];

	// zu signierenden String aufbauen
	s="";
	for(std::map<AnsiString, AnsiString>::iterator it=mapParams.begin(); it!=mapParams.end(); ++it)
		{
		if(s != "")
			s+="&";
		s+=it->first+"="+it->second;
		}
	sToSign+=s;

	// signatur erzeugen
	s=THashSHA2::GetHMAC(sToSign, aws.sAWSSecret, THashSHA2::TSHA2Version::SHA256);
	sUnHex=hextostr(s);
	sSignature=StringToBase64(sUnHex);
	sSignature=urlEncode(sSignature);

	//slAWS->Insert(7,"Signature=ypwri7vogF41zlQesWd7KJV8S5qYh2ErJb5oFDWvr08%3D");
	slAWS->Insert(7,"Signature="+sSignature);

	s="";
	//for(std::vector<AnsiString, AnsiString>::iterator it=vecParams.begin(); it!=vecParams.end(); ++it)
	for(int i=0; i < slAWS->Count; i++)
		{
		if(s != "")
			s+="&";
		//s+=it->first+"="+it->second;
		s+=slAWS->Names[i]+"="+slAWS->ValueFromIndex[i];
		}
	sParams=s;

	delete slAWS;
	return(sParams);
}

//---------------------------------------------------------------------------

AnsiString TfrmMain::urlEncode(AnsiString sURL)
{
	sURL=ReplaceStr(sURL, "/", "%2F");
	sURL=ReplaceStr(sURL, "=", "%3D");
	sURL=ReplaceStr(sURL, ":", "%3A");
  sURL=ReplaceStr(sURL, "+", "%2B");

	return(sURL);
}

//---------------------------------------------------------------------------

AnsiString TfrmMain::getAWSDateTime(TDateTime td)
{
	AnsiString s;
	unsigned long lTZInfo;
	_TIME_ZONE_INFORMATION tz;
	lTZInfo=GetTimeZoneInformation(&tz);
	//0: { Unknown };
	//1: { Standard Time };
	//2: { Daylight Time };

	//td=IncHour(td, -2);

	if(lTZInfo == 1)
		td=IncHour(td, -1);  // Amazon Irland (-1)
	else
		td=IncHour(td, -2);

	s=DateToISO8601(td, true);

	//s=td.FormatString("yyyy-mm-dd")+"T"+td.FormatString("hh:nn:ss")+"Z";
	s=ReplaceStr(s, ":", "%3A");

	return(s);
}

//---------------------------------------------------------------------------

String __fastcall TfrmMain::BytesToBase64( TByteDynArray _ArrayIn )
{
	TBase64Encoding * Encoding = new TBase64Encoding( 64, '\n' );
	String Result = Encoding->EncodeBytesToString( &_ArrayIn[0], _ArrayIn.High );
	delete Encoding;
	return Result;
}

//---------------------------------------------------------------------------

String __fastcall TfrmMain::StringToBase64(AnsiString s)
{
	int length;
	unsigned char *data;

	data=s.c_str();
	length=s.Length();

	if(length<=0)
		return L"";
	static const char set[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char *in=(unsigned char*)data;
	char *pos,*out=pos=new char[((length-1)/3+1)<<2];
	while ((length-=3)>=0)
		{
		pos[0]=set[in[0]>>2];
		pos[1]=set[((in[0]&0x03)<<4)|(in[1]>>4)];
		pos[2]=set[((in[1]&0x0F)<<2)|(in[2]>>6)];
		pos[3]=set[in[2]&0x3F];
		pos+=4;
		in+=3;
		};
	if((length&2)!=0)
		{
		pos[0]=set[in[0]>>2];
		if((length&1)!=0)
			{
		 pos[1]=set[((in[0]&0x03)<<4)|(in[1]>>4)];
		 pos[2]=set[(in[1]&0x0F)<<2];
			}
		else
			{
			pos[1]=set[(in[0]&0x03)<<4];
			pos[2]='=';
			};
		pos[3]='=';
		pos+=4;
		};

	UnicodeString code=UnicodeString(out,pos-out);
	delete[] out;

	return code;
}

//---------------------------------------------------------------------------

char TfrmMain::char2int(char input)
{
	if(input >= '0' && input <= '9')
		return input - '0';
	if(input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if(input >= 'a' && input <= 'f')
		return input - 'a' + 10;

	throw std::runtime_error("Incorrect symbol in hex string");
};

//---------------------------------------------------------------------------

AnsiString TfrmMain::hextostr(AnsiString strHex)
{
		AnsiString sResult;
		char c;

		for(int i=0; i < strHex.Length(); i+=2)
			{
			c=char2int(strHex[i+1]);
			c <<= 4;
			c |= char2int(strHex[i+2]);

			sResult+=c;
			}

		return sResult;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::serializeXML(AnsiString strXMLDatei, TStringList *slXML)
{
	UnicodeString s, sPath, sElement;
	TStringList *slElement=new TStringList();

	TXmlReader *XmlReader = new TXmlReader(strXMLDatei, "");
	__try
	{
		while (XmlReader->Read())
		{
			switch (XmlReader->Node)
			{
				case xnElement:
					sElement=XmlReader->Name;
					if(sPath != "")
						sPath+=".";
					sPath+=sElement;

					XmlReader->Element();
				break;
				case xnText:
					s=sPath+"="+XmlReader->Value;
					slXML->Add(s);
				break;
				case xnEndElement:
					sPath.SetLength(sPath.Length()-(XmlReader->Name.Length()+1));
				break;
			}
		}
	}
	__finally
	{
		XmlReader->Free();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::serializeXMLStr(UnicodeString strXMLStr, TStringList *slXML)
{
	UnicodeString s, sPath, sElement;
	TStringList *slElement=new TStringList();

	// xml-definition um Sonderzeichen erweitern
	strXMLStr=ReplaceStr(strXMLStr, "<?xml version=\"1.0\"?>", "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");

	TStringStream *pss=new TStringStream(strXMLStr);
	pss->Position=0;

	TXmlReader *XmlReader = new TXmlReader(pss, "");
	__try
	{
	while (XmlReader->Read())
		{
		switch (XmlReader->Node)
			{
				case xnElement:
					sElement=XmlReader->Name;
					if(sPath != "")
						sPath+=".";
					sPath+=sElement;

					//XmlReader->Element();
				break;
				case xnText:
					s=sPath+"="+XmlReader->Value;
					slXML->Add(s);
				break;
				case xnEndElement:
					sPath.SetLength(sPath.Length()-(XmlReader->Name.Length()+1));
				break;
			}
		}
	}
	__finally
	{
		XmlReader->Free();
	}
}

//---------------------------------------------------------------------------


