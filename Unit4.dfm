object Form4: TForm4
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsSingle
  Caption = 'Debug'
  ClientHeight = 531
  ClientWidth = 868
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 12
    Top = 25
    Width = 36
    Height = 13
    Caption = #20018#21475#21517
  end
  object Label2: TLabel
    Left = 12
    Top = 62
    Width = 36
    Height = 13
    Caption = #27874#29305#29575
  end
  object Label3: TLabel
    Left = 12
    Top = 100
    Width = 48
    Height = 13
    Caption = #22855#20598#26657#39564
  end
  object Label4: TLabel
    Left = 12
    Top = 138
    Width = 36
    Height = 13
    Caption = #20572#27490#20301
  end
  object cbSerialPorts: TComboBox
    Left = 100
    Top = 21
    Width = 116
    Height = 21
    Style = csDropDownList
    TabOrder = 0
  end
  object cbBaudRate: TComboBox
    Left = 100
    Top = 58
    Width = 116
    Height = 21
    Style = csDropDownList
    TabOrder = 1
  end
  object cbParity: TComboBox
    Left = 100
    Top = 97
    Width = 116
    Height = 21
    Style = csDropDownList
    TabOrder = 2
  end
  object cbStopBits: TComboBox
    Left = 100
    Top = 135
    Width = 116
    Height = 21
    Style = csDropDownList
    TabOrder = 3
  end
  object btnOpenSerialPort: TButton
    Left = 12
    Top = 205
    Width = 75
    Height = 25
    Caption = #25171#24320
    TabOrder = 4
    OnClick = btnOpenSerialPortClick
  end
  object mm_ComRec: TMemo
    Left = 222
    Top = 21
    Width = 610
    Height = 316
    Lines.Strings = (
      'mm_ComRec')
    TabOrder = 5
  end
  object btnSendData: TButton
    Left = 12
    Top = 376
    Width = 75
    Height = 25
    Caption = #21457#36865
    TabOrder = 6
    OnClick = btnSendDataClick
  end
  object mm_SendBox: TMemo
    Left = 222
    Top = 378
    Width = 610
    Height = 108
    Lines.Strings = (
      'Memo1')
    TabOrder = 7
    OnKeyPress = mm_SendBoxKeyPress
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 800
    Top = 24
  end
end
