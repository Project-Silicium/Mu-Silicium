Name (PCFG,
    Buffer() {"<?xml version='1.0' encoding='utf-8'?>
<PanelName>nt36675</PanelName>
<PanelDescription>nt36675 video mode dsi tianma panel</PanelDescription>
<Group id='EDID Configuration'>
    <ManufactureID>0xAF0D</ManufactureID>
    <ProductCode>0x01C2</ProductCode>
    <SerialNumber>0x000000</SerialNumber>
    <WeekofManufacture>0x09</WeekofManufacture>
    <YearofManufacture>0x13</YearofManufacture>
    <EDIDVersion>1</EDIDVersion>
    <EDIDRevision>3</EDIDRevision>
    <HorizontalScreenSize>0x08</HorizontalScreenSize>
    <VerticalScreenSize>0x0e</VerticalScreenSize>
    <RedX>0xA6</RedX>
    <RedY>0x51</RedY>
    <GreenX>0x4B</GreenX>
    <GreenY>0x9E</GreenY>
    <BlueX>0x25</BlueX>
    <BlueY>0x0E</BlueY>
    <WhiteX>0x48</WhiteX>
    <WhiteY>0x4B</WhiteY>
</Group>
<Group id='Detailed Timing'>
    <HorizontalScreenSizeMM>0x53</HorizontalScreenSizeMM>
    <VerticalScreenSizeMM>0x93</VerticalScreenSizeMM>
    <HorizontalVerticalScreenSizeMM>0x00</HorizontalVerticalScreenSizeMM>
</Group>
<Group id='Active Timing'>
    <HorizontalActive>1080</HorizontalActive>
    <HorizontalFrontPorch>20</HorizontalFrontPorch>
    <HorizontalBackPorch>22</HorizontalBackPorch>
    <HorizontalSyncPulse>4</HorizontalSyncPulse>
    <HorizontalSyncSkew>0</HorizontalSyncSkew>
    <HorizontalLeftBorder>0</HorizontalLeftBorder>
    <HorizontalRightBorder>0</HorizontalRightBorder>
    <VerticalActive>2400</VerticalActive>
    <VerticalFrontPorch>10</VerticalFrontPorch>
    <VerticalBackPorch>30</VerticalBackPorch>
    <VerticalSyncPulse>2</VerticalSyncPulse>
    <VerticalSyncSkew>0</VerticalSyncSkew>
    <VerticalTopBorder>0</VerticalTopBorder>
    <VerticalBottomBorder>0</VerticalBottomBorder>
    <InvertDataPolarity>False</InvertDataPolarity>
    <InvertVsyncPolairty>False</InvertVsyncPolairty>
    <InvertHsyncPolarity>False</InvertHsyncPolarity>
    <BorderColor>0x0</BorderColor>
    <UnderflowColor>0xFF</UnderflowColor>
</Group>
<Group id='Display Interface'>
    <InterfaceType>8</InterfaceType>
    <InterfaceColorFormat>3</InterfaceColorFormat>
</Group>
<Group id='DSI Interface'>
    <DSIRefreshRate>0x3C0000</DSIRefreshRate>
    <DSILanes>4</DSILanes>
    <DSIChannelId>1</DSIChannelId>
    <DSIVirtualId>0</DSIVirtualId>
    <DSIColorFormat>36</DSIColorFormat>
    <DSIClockHSForceRequest>0</DSIClockHSForceRequest>
    <DSITrafficMode>1</DSITrafficMode>
    <DSIControllerMapping>00</DSIControllerMapping>
    <DSILP11AtInit>True</DSILP11AtInit>
    <DSIHsaHseAfterVsVe>False</DSIHsaHseAfterVsVe>
    <DSILowPowerModeInHFP>False</DSILowPowerModeInHFP>
    <DSILowPowerModeInHBP>False</DSILowPowerModeInHBP>
    <DSILowPowerModeInHSA>False</DSILowPowerModeInHSA>
    <DSILowPowerModeInBLLPEOF>True</DSILowPowerModeInBLLPEOF>
    <DSILowPowerModeInBLLP>True</DSILowPowerModeInBLLP>
    <DSIDisableEoTAfterHSXfer>True</DSIDisableEoTAfterHSXfer>
    <DSIPixelXferTiming>3</DSIPixelXferTiming>
</Group>
<DSIInitSequence>
    15 FF 10
    15 FB 01
    39 3B 03 1E 0A 04 04
    15 B0 00
    15 35 00
    15 51 B8
    15 53 24
    15 55 00
    15 FF 27
    15 FB 01
    15 07 01
    15 40 25
    15 FF 23
    15 FB 01
    15 0A 20
    15 0B 20
    15 0C 20
    15 0D 2A
    15 10 50
    15 11 01
    15 12 95
    15 15 68
    15 16 0B
    15 30 FF
    15 31 FF
    15 32 FF
    15 33 FE
    15 34 FD
    15 35 FA
    15 36 F6
    15 37 F2
    15 38 F0
    15 39 EE
    15 3A EC
    15 3B EA
    15 3D E8
    15 3F E7
    15 40 E6
    15 41 E5
    15 A0 11
    15 FF 10
    15 FB 01
    05 11 00
    05 29 00
    15 FF 27
    15 FB 01
    15 3F 01
    15 43 08
    15 FF 10
</DSIInitSequence>
<DSITermSequence>
    15 FF 10
    05 28 00
    05 10 00
</DSITermSequence>
<Group id='Backlight Configuration'>
    <BacklightType>1</BacklightType>
    <BacklightPmicControlType>2</BacklightPmicControlType>
    <BacklightPmicBankSelect>7</BacklightPmicBankSelect>
    <BacklightPmicPWMFrequency>800000</BacklightPmicPWMFrequency>
    <BacklightSteps>100</BacklightSteps>
    <BacklightPMICNum>1</BacklightPMICNum>
    <BacklightPmicModel>0x1F</BacklightPmicModel>
    <BrightnessMinLuminance>3230</BrightnessMinLuminance>
    <BrightnessMaxLuminance>420000</BrightnessMaxLuminance>
    <AdaptiveBrightnessFeature>1</AdaptiveBrightnessFeature>
    <CABLEnable>True</CABLEnable>
    <DisplayResetInfo>0 10 10000 10000 0</DisplayResetInfo>
</Group>"})