707c：R2TT-14C656-FG CDX707 EQ
611c：RC5T-14C656-BF U611 EQ
718:SL7T-14C656-BC

以上每个项目对应的eq vbf，烧录完后更新DSP的DE01配置位如下定义：
Value	State Description
0x00	protection eq
0x01	U611
0x03	U718
0x05	CDX707
0x65	Thermal
0x66	EMC
0xFE	Test EQ (Used for supplier EOL testing - flat response so never use in a vehicle and when testing on bench have a power supply with high current capability)
