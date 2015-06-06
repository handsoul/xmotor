object UnitFormAuth: TUnitFormAuth
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsSingle
  Caption = #31649#29702#21592#26435#38480#39564#35777
  ClientHeight = 178
  ClientWidth = 397
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = #23435#20307
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnActivate = FormActivate
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  DesignSize = (
    397
    178)
  PixelsPerInch = 96
  TextHeight = 19
  object Label1: TLabel
    Left = 68
    Top = 53
    Width = 66
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    Caption = #29992#25143#21517
    ExplicitWidth = 58
  end
  object Label2: TLabel
    Left = 76
    Top = 94
    Width = 58
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    Caption = #23494' '#30721
    ExplicitWidth = 50
  end
  object LabelNew: TLabel
    Left = 68
    Top = 136
    Width = 66
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    Caption = #26032#23494#30721
    ExplicitWidth = 58
  end
  object LabelConfirmNew: TLabel
    Left = 28
    Top = 178
    Width = 106
    Height = 19
    Anchors = [akLeft, akTop, akRight]
    Caption = #30830#35748#26032#23494#30721
    ExplicitWidth = 98
  end
  object MED_Password: TMaskEdit
    Left = 142
    Top = 91
    Width = 154
    Height = 27
    Anchors = [akLeft, akTop, akRight]
    PasswordChar = '*'
    TabOrder = 0
    Text = ''
    ExplicitWidth = 136
  end
  object Edit1: TEdit
    Left = 142
    Top = 50
    Width = 154
    Height = 27
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 1
    Text = #31649#29702#21592
    StyleElements = [seClient, seBorder]
    ExplicitWidth = 121
  end
  object btnLogin: TButton
    Left = 314
    Top = 145
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
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
    ExplicitLeft = 281
    ExplicitTop = 180
  end
  object btnChangePass: TButton
    Left = 221
    Top = 145
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #20462#25913#23494#30721
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    StyleElements = [seClient, seBorder]
    OnClick = btnChangePassClick
    ExplicitLeft = 188
    ExplicitTop = 180
  end
  object MED_NewPassword: TMaskEdit
    Left = 142
    Top = 133
    Width = 154
    Height = 27
    Anchors = [akLeft, akTop, akRight]
    PasswordChar = '*'
    TabOrder = 4
    Text = ''
    ExplicitWidth = 136
  end
  object MED_ConfirmNewPassword: TMaskEdit
    Left = 142
    Top = 175
    Width = 154
    Height = 27
    Anchors = [akLeft, akTop, akRight]
    PasswordChar = '*'
    TabOrder = 5
    Text = ''
    ExplicitWidth = 136
  end
  object btnSetNewPassword: TButton
    Left = 314
    Top = 145
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #20462#25913
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    StyleElements = [seClient, seBorder]
    OnClick = btnSetNewPasswordClick
    ExplicitLeft = 306
    ExplicitTop = 215
  end
  object btnCancle: TButton
    Left = 221
    Top = 145
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #21462#28040
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    StyleElements = [seClient, seBorder]
    OnClick = btnCancleClick
  end
end
