object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'AMExchange V1.08 by pk-ware it-systeme 2021'
  ClientHeight = 510
  ClientWidth = 894
  Color = clBtnFace
  CustomTitleBar.CaptionAlignment = taCenter
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poMainFormCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 491
    Width = 894
    Height = 19
    Panels = <
      item
        Width = 150
      end
      item
        Width = 150
      end
      item
        Width = 50
      end>
  end
  object ProgressBar1: TProgressBar
    Left = 224
    Top = 312
    Width = 150
    Height = 17
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 696
    Top = 0
    Width = 198
    Height = 491
    Align = alRight
    BevelKind = bkTile
    BevelOuter = bvNone
    Caption = 'Panel1'
    ShowCaption = False
    TabOrder = 2
    DesignSize = (
      194
      487)
    object grboxShopOrders: TGroupBox
      Left = 12
      Top = 12
      Width = 169
      Height = 157
      Anchors = [akTop, akRight]
      Caption = 'Shop-Auftr'#228'ge'
      TabOrder = 0
      object Label3: TLabel
        Left = 102
        Top = 23
        Width = 22
        Height = 13
        Caption = '(min)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 99
        Top = 97
        Width = 22
        Height = 13
        Caption = '(min)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object btnGetShopOrders: TButton
        Left = 8
        Top = 48
        Width = 153
        Height = 25
        Caption = 'Shopauftr'#228'ge einlesen'
        TabOrder = 0
        OnClick = btnGetShopOrdersClick
      end
      object cbxGetOrderAuto: TCheckBox
        Left = 8
        Top = 23
        Width = 57
        Height = 17
        Caption = 'autom.'
        TabOrder = 1
        OnClick = cbxGetOrderAutoClick
      end
      object edAutoOrderTime: TAdvEdit
        Left = 130
        Top = 21
        Width = 31
        Height = 21
        EditAlign = eaRight
        EditType = etNumeric
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -11
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        TabOrder = 2
        Text = '5'
        Visible = True
        Version = '3.5.5.0'
      end
      object btnUploadVersand: TButton
        Left = 8
        Top = 119
        Width = 153
        Height = 25
        Caption = 'Versanddaten r'#252'ckmelden'
        TabOrder = 3
        OnClick = btnUploadVersandClick
      end
      object cbxSendVersandRAuto: TCheckBox
        Left = 8
        Top = 96
        Width = 57
        Height = 17
        Caption = 'autom.'
        TabOrder = 4
        OnClick = cbxSendVersandRAutoClick
      end
      object edAutoVersandRTime: TAdvEdit
        Left = 127
        Top = 94
        Width = 31
        Height = 21
        EditAlign = eaRight
        EditType = etNumeric
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -11
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        TabOrder = 5
        Text = '5'
        Visible = True
        Version = '3.5.5.0'
      end
    end
    object grboxStammdatenVeraenderung: TGroupBox
      Left = 12
      Top = 187
      Width = 169
      Height = 110
      Anchors = [akTop, akRight]
      Caption = 'Artikel / Bestand'
      TabOrder = 1
      object Label2: TLabel
        Left = 102
        Top = 76
        Width = 22
        Height = 13
        Caption = '(min)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object btnArtikelAktualisieren: TButton
        Left = 8
        Top = 45
        Width = 153
        Height = 25
        Caption = 'Artikel aktualisieren'
        TabOrder = 0
        OnClick = btnArtikelAktualisierenClick
      end
      object cbxSendArtikelAuto: TCheckBox
        Left = 8
        Top = 74
        Width = 57
        Height = 17
        Caption = 'autom.'
        TabOrder = 1
        OnClick = cbxSendArtikelAutoClick
      end
      object edAutoArtikelTime: TAdvEdit
        Left = 130
        Top = 76
        Width = 31
        Height = 21
        EditAlign = eaRight
        EditType = etNumeric
        EmptyTextStyle = []
        FlatLineColor = 11250603
        FocusColor = clWindow
        FocusFontColor = 3881787
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Font.Charset = DEFAULT_CHARSET
        Lookup.Font.Color = clWindowText
        Lookup.Font.Height = -11
        Lookup.Font.Name = 'Arial'
        Lookup.Font.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        TabOrder = 2
        Text = '5'
        Visible = True
        Version = '3.5.5.0'
      end
      object cbNurGeaenderte: TCheckBox
        Left = 8
        Top = 18
        Width = 158
        Height = 21
        Caption = 'nur ge'#228'nderte'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
    end
  end
  object memo: TMemo
    Left = 0
    Top = 0
    Width = 696
    Height = 491
    Align = alClient
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object MainMenu1: TMainMenu
    Left = 32
    Top = 40
    object Datei1: TMenuItem
      Caption = 'Datei'
      object Logleeren1: TMenuItem
        Caption = 'Log leeren'
        OnClick = Logleeren1Click
      end
      object N11: TMenuItem
        Caption = '-'
      end
      object N13: TMenuItem
        Caption = 'Manueller Dateiimport'
        OnClick = N13Click
      end
      object N12: TMenuItem
        Caption = '-'
      end
      object Beenden1: TMenuItem
        Caption = 'Beenden'
        OnClick = Beenden1Click
      end
    end
    object API1: TMenuItem
      Caption = 'API'
      object Debuglevel2: TMenuItem
        Caption = 'Debuglevel'
        OnClick = Debuglevel1Click
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object CheckServiceStatus1: TMenuItem
        Caption = 'CheckServiceStatus'
        OnClick = CheckServiceStatus1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object N2: TMenuItem
        Caption = 'RequestReport'
        OnClick = N2Click
      end
      object GetRequestReportList1: TMenuItem
        Caption = 'GetRequestReportList'
        OnClick = GetRequestReportList1Click
      end
      object GetReportList1: TMenuItem
        Caption = 'GetReportList'
        OnClick = GetReportList1Click
      end
      object GetReport1: TMenuItem
        Caption = 'GetReport'
        OnClick = GetReport1Click
      end
      object UpdateReportAcknowledgements1: TMenuItem
        Caption = 'UpdateReportAcknowledgements'
        OnClick = UpdateReportAcknowledgements1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object N4: TMenuItem
        Caption = 'SubmitFeed Inventory'
        OnClick = N4Click
      end
      object GetFeedSubmissionList1: TMenuItem
        Caption = 'GetFeedSubmissionList'
        OnClick = GetFeedSubmissionList1Click
      end
      object UploadInvoiceToOrderr1: TMenuItem
        Caption = 'UploadInvoiceToOrderr'
        OnClick = UploadInvoiceToOrderr1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object GetOrderList1: TMenuItem
        Caption = 'GetOrderList'
        OnClick = GetOrderList1Click
      end
      object GetOrder1: TMenuItem
        Caption = 'GetOrder'
        OnClick = GetOrder1Click
      end
      object GetOrderItems1: TMenuItem
        Caption = 'GetOrderItems'
        OnClick = GetOrderItems1Click
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object N7: TMenuItem
        Caption = 'CheckAndCreateOrder'
        OnClick = N7Click
      end
      object CheckAndCreatePendingOrders1: TMenuItem
        Caption = 'CheckAndCreatePendingOrders'
        OnClick = CheckAndCreatePendingOrders1Click
      end
      object ImportFLATReport1: TMenuItem
        Caption = 'Import FLAT-Report'
        OnClick = ImportFLATReport1Click
      end
      object UpdateInternalOrdeswithFLATReport1: TMenuItem
        Caption = 'Update Internal Ordes with FLAT Report'
        OnClick = UpdateInternalOrdeswithFLATReport1Click
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object N9: TMenuItem
        Caption = 'test xml'
        OnClick = N9Click
      end
      object CheckRechnungsupload1: TMenuItem
        Caption = 'Check Rechnungsupload'
        OnClick = CheckRechnungsupload1Click
      end
      object CheckProcedure1: TMenuItem
        Caption = 'Check Procedure'
        OnClick = CheckProcedure1Click
      end
    end
    object Versand1: TMenuItem
      Caption = 'Versand'
      Visible = False
      object Versand2: TMenuItem
        Caption = 'DHL-Versand exportieren'
        OnClick = Versand2Click
      end
    end
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=SQLOLEDB.1;Password=!SQLagro09;Persist Security Info=Tr' +
      'ue;User ID=FlieglDBAdmin;Initial Catalog=SL_MWAWI;Data Source=ag' +
      'ro-center.de\S15327765;Use Procedure for Prepare=1;Auto Translat' +
      'e=True;Packet Size=4096;Use Encryption for Data=False;Tag with c' +
      'olumn collation when possible=False'
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    Left = 48
    Top = 343
  end
  object RESTResponse1: TRESTResponse
    Left = 376
    Top = 376
  end
  object RESTRequest1: TRESTRequest
    AutoCreateParams = False
    Client = RESTClient1
    Params = <>
    Response = RESTResponse1
    OnAfterExecute = RESTRequest1AfterExecute
    Left = 480
    Top = 376
  end
  object RESTClient1: TRESTClient
    Params = <>
    Left = 568
    Top = 376
  end
  object timerAMGeneral: TTimer
    Enabled = False
    OnTimer = timerAMGeneralTimer
    Left = 612
    Top = 16
  end
  object OpenDialog1: TOpenDialog
    Left = 48
    Top = 272
  end
end
