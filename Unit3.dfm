object FormSysCfg: TFormSysCfg
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = #31995#32479#37197#32622#31649#29702
  ClientHeight = 336
  ClientWidth = 606
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'Segoe UI Light'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnResize = FormResize
  DesignSize = (
    606
    336)
  PixelsPerInch = 96
  TextHeight = 25
  object Label1: TLabel
    Left = 220
    Top = 8
    Width = 76
    Height = 19
    Caption = #24403#21069#37197#32622
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    StyleElements = [seClient, seBorder]
  end
  object Label2: TLabel
    Left = 447
    Top = 58
    Width = 136
    Height = 16
    Anchors = [akRight, akBottom]
    Caption = #39640#30005#24179#38376#38480#30005#21387'(V)'
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    StyleElements = [seClient, seBorder]
  end
  object Label3: TLabel
    Left = 447
    Top = 130
    Width = 136
    Height = 16
    Anchors = [akRight, akBottom]
    Caption = #20302#30005#24179#38376#38480#30005#21387'(V)'
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    StyleElements = [seClient, seBorder]
  end
  object SG_Config: TStringGrid
    Left = 30
    Top = 31
    Width = 407
    Height = 283
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 4
    DefaultColWidth = 100
    DefaultRowHeight = 39
    DefaultDrawing = False
    DoubleBuffered = True
    DrawingStyle = gdsClassic
    FixedCols = 0
    RowCount = 7
    FixedRows = 0
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = #23435#20307
    Font.Style = []
    ParentDoubleBuffered = False
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 0
    StyleElements = [seClient, seBorder]
    OnDrawCell = SG_ConfigDrawCell
    OnSelectCell = SG_ConfigSelectCell
    OnSetEditText = SG_ConfigSetEditText
  end
  object btnSave: TButton
    Left = 488
    Top = 283
    Width = 95
    Height = 31
    Hint = #20445#23384#20462#25913
    Anchors = [akRight, akBottom]
    Caption = #20445#23384
    DoubleBuffered = True
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 1
    StyleElements = [seClient, seBorder]
    OnClick = btnSaveClick
  end
  object ED_HeightVoltage: TEdit
    Left = 464
    Top = 80
    Width = 121
    Height = 24
    Anchors = [akRight, akBottom]
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    StyleElements = [seClient, seBorder]
    OnChange = ED_HeightVoltageChange
  end
  object ED_LowVoltage: TEdit
    Left = 464
    Top = 152
    Width = 121
    Height = 24
    Anchors = [akRight, akBottom]
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    StyleElements = [seClient, seBorder]
    OnChange = ED_LowVoltageChange
  end
  object btnLoadDefault: TButton
    Left = 488
    Top = 217
    Width = 97
    Height = 31
    Anchors = [akRight, akBottom]
    Caption = #24674#22797#40664#35748
    DoubleBuffered = True
    Font.Charset = GB2312_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = [fsBold]
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 4
    OnClick = btnLoadDefaultClick
  end
end
