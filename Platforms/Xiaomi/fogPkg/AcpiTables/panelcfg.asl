Name (PCFA, Buffer(){"
<?xml version='1.0' encoding='utf-8'?>
<PanelName>FT8006S-AA Video</PanelName>
<PanelDescription>FT8006S-AA video mode dsi xinli panel</PanelDescription>
<Group id='Active Timing'>
    <HorizontalActive>720</HorizontalActive>
    <HorizontalFrontPorch>26</HorizontalFrontPorch>
    <HorizontalBackPorch>25</HorizontalBackPorch>
    <HorizontalSyncPulse>16</HorizontalSyncPulse>
    <HorizontalSyncSkew>0</HorizontalSyncSkew>
    <HorizontalLeftBorder>0</HorizontalLeftBorder>
    <HorizontalRightBorder>0</HorizontalRightBorder>
    <VerticalActive>1650</VerticalActive>
    <VerticalFrontPorch>135</VerticalFrontPorch>
    <VerticalBackPorch>110</VerticalBackPorch>
    <VerticalSyncPulse>10</VerticalSyncPulse>
    <VerticalSyncSkew>0</VerticalSyncSkew>
    <VerticalTopBorder>0</VerticalTopBorder>
    <VerticalBottomBorder>0</VerticalBottomBorder>
</Group>
<Group id='Display Interface'>
    <InterfaceType>8</InterfaceType>
    <InterfaceColorFormat>3</InterfaceColorFormat>
</Group>
<Group id='DSI Interface'>
    <DSIChannelId>1</DSIChannelId>
    <DSIVirtualId>0</DSIVirtualId>
    <DSIColorFormat>36</DSIColorFormat>
    <DSITrafficMode>1</DSITrafficMode>
    <DSILanes>4</DSILanes>
    <DSILowPowerModeInBLLPEOF>True</DSILowPowerModeInBLLPEOF>
    <DSILowPowerModeInBLLP>True</DSILowPowerModeInBLLP>
    <DSIRefreshRate>0x3C0000</DSIRefreshRate>
    <DSICmdSwapInterface>False</DSICmdSwapInterface>
    <DSICmdUsingTrigger>False</DSICmdUsingTrigger>
    <DSIControllerMapping>
   00
 </DSIControllerMapping>
</Group>
<DSIInitSequence>
 39 41 5A 19
 39 80 E3 D4 C4 B6 AC A3 9C 96 90 8C 88 83 44 74 FB 9F
 39 90 F9 0F FE EF 9F AF 0F
 39 A0 00 00 46 00
 05 11 00
 FF 78
 05 29 00
 39 51 40 04
 39 53 2C
 39 55 00
 FF 0A
 39 41 5A 19
 39 8C 44 64
</DSIInitSequence>
<DSITermSequence>
 05 28
 FF 14
 05 10
 FF 78
</DSITermSequence>
<Group id='Backlight Configuration'>
    <BacklightType>1</BacklightType>
    <BacklightPmicControlType>2</BacklightPmicControlType>
    <DisplayResetInfo>0 10 10000 10000 0</DisplayResetInfo>
</Group>
"})
