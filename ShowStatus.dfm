object OKRightDlg: TOKRightDlg
  Left = 227
  Top = 108
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Dialog'
  ClientHeight = 331
  ClientWidth = 748
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 666
    Top = 288
    Width = 75
    Height = 25
    Caption = #30830#35748
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 666
    Top = 119
    Width = 75
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object SG_Data: TStringGrid
    Left = 8
    Top = 8
    Width = 654
    Height = 305
    ColCount = 8
    DefaultColWidth = 80
    RowCount = 12
    ScrollBars = ssNone
    TabOrder = 2
  end
end
