707c：R2TT-14C656-FG.vbf
718:  SL7T-14C656-BD.vbf
611： RC5T-14C656-BG U611 EQ.vbf

上面611、707和718 24ch dsp项目对应的eq vbf，烧录完后务必更新DSP的DE01配置位，具体配置位如下定义：
Value	State Description
0x00	protection eq
0x01	U611
0x03	U718
0x05	CDX707
0x65	Thermal
0x66	EMC
0xFE	Test EQ (Used for supplier EOL testing - flat response so never use in a vehicle and when testing on bench have a power supply with high current capability)
