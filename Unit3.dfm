object FormSysCfg: TFormSysCfg
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = #31995#32479#37197#32622#31649#29702
  ClientHeight = 382
  ClientWidth = 606
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'Segoe UI Light'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnResize = FormResize
  DesignSize = (
    606
    382)
  PixelsPerInch = 96
  TextHeight = 25
  object Label1: TLabel
    Left = 225
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
  object SG_Config: TStringGrid
    Left = 30
    Top = 31
    Width = 459
    Height = 306
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 4
    DefaultColWidth = 100
    DefaultRowHeight = 30
    DoubleBuffered = True
    RowCount = 8
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
  end
  object btnSave: TButton
    Left = 512
    Top = 264
    Width = 75
    Height = 73
    Caption = #20445#23384
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 1
    StyleElements = [seClient, seBorder]
  end
end
