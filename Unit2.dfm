object Form2: TForm2
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Form2'
  ClientHeight = 219
  ClientWidth = 590
  Color = clScrollBar
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnActivate = FormActivate
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 224
    Top = 48
    Width = 96
    Height = 24
    Caption = #27979#35797#31995#32479
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = #26999#20307
    Font.Style = []
    ParentFont = False
  end
  object label_StartInfo: TLabel
    Left = 96
    Top = 128
    Width = 3
    Height = 13
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 432
    Top = 160
  end
end
