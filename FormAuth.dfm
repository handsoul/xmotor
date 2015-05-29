object UnitFormAuth: TUnitFormAuth
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsSingle
  Caption = #31649#29702#21592#26435#38480#39564#35777
  ClientHeight = 213
  ClientWidth = 364
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = #23435#20307
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 19
  object Label1: TLabel
    Left = 72
    Top = 53
    Width = 57
    Height = 19
    Caption = #29992#25143#21517
  end
  object Label2: TLabel
    Left = 92
    Top = 125
    Width = 38
    Height = 19
    Caption = #23494#30721
  end
  object MED_Password: TMaskEdit
    Left = 142
    Top = 122
    Width = 121
    Height = 27
    PasswordChar = '*'
    TabOrder = 0
    Text = ''
  end
  object Edit1: TEdit
    Left = 142
    Top = 50
    Width = 121
    Height = 27
    ReadOnly = True
    TabOrder = 1
    Text = #31649#29702#21592
    StyleElements = [seClient, seBorder]
  end
  object btnLogin: TButton
    Left = 281
    Top = 180
    Width = 75
    Height = 25
    Caption = #39564#35777
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    StyleElements = [seClient, seBorder]
    OnClick = btnLoginClick
  end
  object Button1: TButton
    Left = 188
    Top = 180
    Width = 75
    Height = 25
    Caption = #20462#25913#23494#30721
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    StyleElements = [seClient, seBorder]
  end
end
