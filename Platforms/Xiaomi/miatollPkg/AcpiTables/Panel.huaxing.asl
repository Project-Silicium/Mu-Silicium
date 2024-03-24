Name (PCFG,
    Buffer() {"<?xml version='1.0' encoding='utf-8'?>
<PanelName>ft8756</PanelName>
<PanelDescription>ft8756 video mode dsi huaxing panel</PanelDescription>
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
    <HorizontalBackPorch>20</HorizontalBackPorch>
    <HorizontalSyncPulse>4</HorizontalSyncPulse>
    <HorizontalSyncSkew>0</HorizontalSyncSkew>
    <HorizontalLeftBorder>0</HorizontalLeftBorder>
    <HorizontalRightBorder>0</HorizontalRightBorder>
    <VerticalActive>2400</VerticalActive>
    <VerticalFrontPorch>8</VerticalFrontPorch>
    <VerticalBackPorch>32</VerticalBackPorch>
    <VerticalSyncPulse>4</VerticalSyncPulse>
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
    <DSIClockHSForceRequest>1</DSIClockHSForceRequest>
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
    39 00 00
    39 FF 87 56 01
    39 00 80
    39 FF 87 56
    15 00 80
    39 CA 80 80 80 80 80 80 80 80 80 80 80 80
    15 00 90
    39 CA FE FF 66 F6 FF 66 FB FF 32
    15 51 B8
    15 53 24
    15 55 00
    39 00 B5
    39 CA 04
    05 11 00
    05 29 00
</DSIInitSequence>
<DSITermSequence>
    05 28 00
    05 10 00
    39 00 00
    39 f7 5a a5 95 27
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