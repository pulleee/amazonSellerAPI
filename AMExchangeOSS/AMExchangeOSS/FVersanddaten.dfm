object frmVersanddaten: TfrmVersanddaten
  Left = 0
  Top = 0
  Caption = 'Versanddaten Export'
  ClientHeight = 425
  ClientWidth = 988
  Color = clBtnFace
  CustomTitleBar.CaptionAlignment = taCenter
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 988
    Height = 57
    Align = alTop
    Caption = 'Panel1'
    ShowCaption = False
    TabOrder = 0
    DesignSize = (
      988
      57)
    object Button1: TButton
      Left = 744
      Top = 17
      Width = 211
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Exportdatei erzeugen'
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 32
      Top = 17
      Width = 211
      Height = 25
      Caption = 'Versendungen ermitteln'
      TabOrder = 1
      OnClick = Button2Click
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 406
    Width = 988
    Height = 19
    Panels = <
      item
        Width = 100
      end
      item
        Width = 50
      end>
    ExplicitLeft = 352
    ExplicitTop = 336
    ExplicitWidth = 0
  end
  object grd: TAdvStringGrid
    Left = 0
    Top = 57
    Width = 988
    Height = 349
    Cursor = crDefault
    Align = alClient
    ColCount = 17
    DrawingStyle = gdsClassic
    FixedColor = clWhite
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goEditing, goFixedRowDefAlign]
    ScrollBars = ssBoth
    TabOrder = 2
    GridLineColor = 13948116
    GridFixedLineColor = 11250603
    HoverRowCells = [hcNormal, hcSelected]
    ActiveCellFont.Charset = DEFAULT_CHARSET
    ActiveCellFont.Color = 4474440
    ActiveCellFont.Height = -11
    ActiveCellFont.Name = 'Tahoma'
    ActiveCellFont.Style = [fsBold]
    ActiveCellColor = 11565130
    ActiveCellColorTo = 11565130
    BorderColor = 11250603
    ColumnHeaders.Strings = (
      'SEND_NAME1'
      'SEND_NAME2'
      'SEND_STREET'
      'SEND_HOUSNUMBER'
      'SEND_PLZ'
      'SEND_CITY'
      'SEND_COUNTRY'
      'RECV_NAME1'
      'RECV_NAME2'
      'RECV_STREET'
      'RECV_HOUSNUMBER'
      'RECV_PLZ'
      'RECV_CITY'
      'RECV_COUNTRY'
      'PRODUCT'
      'COUPON'
      'SEND_EMAIL')
    ControlLook.FixedGradientFrom = clWhite
    ControlLook.FixedGradientTo = clWhite
    ControlLook.FixedGradientMirrorFrom = clWhite
    ControlLook.FixedGradientMirrorTo = clWhite
    ControlLook.FixedGradientHoverTo = clWhite
    ControlLook.FixedGradientHoverMirrorFrom = clWhite
    ControlLook.FixedGradientHoverMirrorTo = clWhite
    ControlLook.FixedGradientHoverBorder = 11645361
    ControlLook.FixedGradientDownFrom = clWhite
    ControlLook.FixedGradientDownTo = clWhite
    ControlLook.FixedGradientDownMirrorFrom = clWhite
    ControlLook.FixedGradientDownMirrorTo = clWhite
    ControlLook.FixedGradientDownBorder = 11250603
    ControlLook.DropDownHeader.Font.Charset = DEFAULT_CHARSET
    ControlLook.DropDownHeader.Font.Color = clWindowText
    ControlLook.DropDownHeader.Font.Height = -11
    ControlLook.DropDownHeader.Font.Name = 'Tahoma'
    ControlLook.DropDownHeader.Font.Style = []
    ControlLook.DropDownHeader.Visible = True
    ControlLook.DropDownHeader.Buttons = <>
    ControlLook.DropDownFooter.Font.Charset = DEFAULT_CHARSET
    ControlLook.DropDownFooter.Font.Color = clWindowText
    ControlLook.DropDownFooter.Font.Height = -11
    ControlLook.DropDownFooter.Font.Name = 'Tahoma'
    ControlLook.DropDownFooter.Font.Style = []
    ControlLook.DropDownFooter.Visible = True
    ControlLook.DropDownFooter.Buttons = <>
    Filter = <>
    FilterDropDown.Font.Charset = DEFAULT_CHARSET
    FilterDropDown.Font.Color = clWindowText
    FilterDropDown.Font.Height = -11
    FilterDropDown.Font.Name = 'Tahoma'
    FilterDropDown.Font.Style = []
    FilterDropDown.TextChecked = 'Checked'
    FilterDropDown.TextUnChecked = 'Unchecked'
    FilterDropDownClear = '(All)'
    FilterEdit.TypeNames.Strings = (
      'Starts with'
      'Ends with'
      'Contains'
      'Not contains'
      'Equal'
      'Not equal'
      'Larger than'
      'Smaller than'
      'Clear')
    FixedRowHeight = 22
    FixedRowAlways = True
    FixedFont.Charset = DEFAULT_CHARSET
    FixedFont.Color = 3881787
    FixedFont.Height = -11
    FixedFont.Name = 'Tahoma'
    FixedFont.Style = [fsBold]
    FloatFormat = '%.2f'
    HoverButtons.Buttons = <>
    HoverButtons.Position = hbLeftFromColumnLeft
    HTMLSettings.ImageFolder = 'images'
    HTMLSettings.ImageBaseName = 'img'
    Look = glCustom
    PrintSettings.DateFormat = 'dd/mm/yyyy'
    PrintSettings.Font.Charset = DEFAULT_CHARSET
    PrintSettings.Font.Color = clWindowText
    PrintSettings.Font.Height = -11
    PrintSettings.Font.Name = 'Tahoma'
    PrintSettings.Font.Style = []
    PrintSettings.FixedFont.Charset = DEFAULT_CHARSET
    PrintSettings.FixedFont.Color = clWindowText
    PrintSettings.FixedFont.Height = -11
    PrintSettings.FixedFont.Name = 'Tahoma'
    PrintSettings.FixedFont.Style = []
    PrintSettings.HeaderFont.Charset = DEFAULT_CHARSET
    PrintSettings.HeaderFont.Color = clWindowText
    PrintSettings.HeaderFont.Height = -11
    PrintSettings.HeaderFont.Name = 'Tahoma'
    PrintSettings.HeaderFont.Style = []
    PrintSettings.FooterFont.Charset = DEFAULT_CHARSET
    PrintSettings.FooterFont.Color = clWindowText
    PrintSettings.FooterFont.Height = -11
    PrintSettings.FooterFont.Name = 'Tahoma'
    PrintSettings.FooterFont.Style = []
    PrintSettings.PageNumSep = '/'
    SearchFooter.ColorTo = clWhite
    SearchFooter.FindNextCaption = 'Find &next'
    SearchFooter.FindPrevCaption = 'Find &previous'
    SearchFooter.Font.Charset = DEFAULT_CHARSET
    SearchFooter.Font.Color = clWindowText
    SearchFooter.Font.Height = -11
    SearchFooter.Font.Name = 'Tahoma'
    SearchFooter.Font.Style = []
    SearchFooter.HighLightCaption = 'Highlight'
    SearchFooter.HintClose = 'Close'
    SearchFooter.HintFindNext = 'Find next occurrence'
    SearchFooter.HintFindPrev = 'Find previous occurrence'
    SearchFooter.HintHighlight = 'Highlight occurrences'
    SearchFooter.MatchCaseCaption = 'Match case'
    SearchFooter.ResultFormat = '(%d of %d)'
    SelectionColor = 13744549
    SortSettings.DefaultFormat = ssAutomatic
    SortSettings.HeaderColor = clWhite
    SortSettings.HeaderColorTo = clWhite
    SortSettings.HeaderMirrorColor = clWhite
    SortSettings.HeaderMirrorColorTo = clWhite
    Version = '8.5.7.2'
  end
  object SaveDialog1: TSaveDialog
    Left = 64
    Top = 257
  end
end
