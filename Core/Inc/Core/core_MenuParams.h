
#ifndef CORE_MENU_PARAMS_
#define CORE_MENU_PARAMS_

#include "core_MenuGlobal.h" 	
#include "g_Ram.h" 	

#define CREATE_STRUCTS


#define MAX_LEVEL			2 
#define GROUP_COUNT			4 
#define SUBGR_INIT(GR, D)	GR, D

const struct MENU_GROUP groups[] = {
	"   1 ������     ", "���������       ", GetAdr(Status),         SIZE(TStatus),
	"   2 ������     ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
	"   3 ������     ", "��������� ����� ", GetAdr(FactoryParam),   SIZE(TFactoryParam),
	"   4 ������     ", "�������         ", GetAdr(Comands),        SIZE(TComands)
};
//
const struct MENU_GROUP subgroups[] = {
	"  1.1 �������   ", " �������        ", GetAdr(Status),         SIZE(TStatus),
	"  1.2 �������   ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
        "  1.3 �������   ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
        "  1.4 �������   ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
        //---
        "  2.1 ��������  ", "����������      ", GetAdr(UserParam),      SIZE(TUserParam),
        "  2.2 ��������  ", "����� ��������  ", GetAdr(UserParam),      SIZE(TUserParam),
        "  2.3 ��������  ", "������ ���������", GetAdr(UserParam),      SIZE(TUserParam),
        "  2.4 ��������  ", "������ ������   ", GetAdr(UserParam),      SIZE(TUserParam),
        //----
        "  3.2 �����     ", "���������       ", GetAdr(UserParam),      SIZE(TUserParam),
        "  3.2 ��������  ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
        "  3.2 ��������  ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
        "  3.2 ��������  ", "��������� ����� ", GetAdr(UserParam),      SIZE(TUserParam),
        //---    
	"  4.1 ������    ", "��������� ����� ", GetAdr(FactoryParam),   SIZE(TFactoryParam),
	"  4.2 ������    ", "�������         ", GetAdr(Comands),        SIZE(TComands)
};


#ifdef CREATE_STRUCTS

const struct MENU_PARAM params[] = {
		"A0.������       ", "    ",	0,  65530,	0, M_RUNS|M_SADR(1),						//0 00
		"A1.���� ��������", "    ",	0,  65530,	0, M_RUNS|M_SADR(18),						//1 01
		"A2.���� ����    ", "    ",	0,  65530,	0, M_RUNS|M_SADR(35),						//2 02
		"A3.���� ��������", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(52),						//3 03
		"A4.���� ��������", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(69),						//4 04
		"A5.�����  ��    ", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(86),						//5 05
		"A6.������ ��    ", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(103),						//6 06
		"A7.����� �����  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//7 07
		"A8.������       ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//8 08
		"A9.��������     ", "    ",-3600U,   3600,  	0, M_SHOW|M_SIGN|M_RMAX(3),					//9 09
		"A10.���� L1     ", "�   ", 	0,    500,  	0, M_SHOW|M_RMAX(2),						//10 10
		"A11.���� L2     ", "�   ", 	0,    500,  	0, M_SHOW|M_RMAX(2),						//11 11
		"A12.���� L3     ", "�   ", 	0,    500,  	0, M_SHOW|M_RMAX(2),						//12 12
		"A13.��� U       ", "A   ", 	0,   8000,  	0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //13 13
		"A14.��� V       ", "A   ", 	0,   8000,  	0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //14 14
		"A15.��� W       ", "A   ", 	0,   8000,  	0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //15 15
		"A16.���� ����   ", "    ", -180U,    180,  	0, M_SHOW|M_SIGN|M_RMAX(2),					//16 16
		"A17.CALIB STATE ", "    ", 	0,      3,  	0, M_STAT|M_SADR(120),						//17 17
		"A18.���������   ", "%   ",-1000U,   2000,      0, M_SHOW|M_SIGN|M_RMAX(3)|M_PREC(1),		                //18 18
		"A19.������ ���� ", "��  ",     0,   3000,      0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //19 19
		"A20.������� ����", "��  ",-3000U,   3000,      0, M_SHOW|M_SIGN|M_RMAX(3)|M_PREC(1),		                //20 20
		"A21.���� ������ ", "    ", 	0,  65530,  	0, M_SHOW|M_RMAX(4),						//21 21
		"A22.����������� ", "    ", -100U,    100,      0, M_SHOW|M_SIGN|M_RMAX(2),					//22 22
		"A23.������ ��   ", "    ", 	0,  65530,  	0, M_SHOW|M_RMAX(3)|M_PREC(3),				        //23 23
		"A24.���� VDC    ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//24 24
		"A25.������ ���� ", "    ", -999U,    999,      0, M_SHOW|M_SIGN|M_RMAX(2),					//25 25
                "A25.������ �����", "    ", -999U,    999,      0, M_SHOW|M_SIGN|M_RMAX(2),					//26 26
		"A27.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//27 27
		"A28.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//28 28
		"A29.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//29 29
		"A30.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//30 30
		"A31.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//31 31
		"A32.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//32 32
		"A33.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//33 33
		"A34.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//34 34
		"A35.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//35 35
		"A36.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//36 36
		"A37.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//37 37
		"A38.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//38 38
		"A39.RESERV      ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//39 39
		//-------------------------------------------------------------------------
		"B0.������ OTKP  ", "    ",     0,     60,      2, M_EDIT|M_KMM|M_RMAX(4),					//40
		"B1.������ 3AKP  ", "    ",	0,     60,	2, M_EDIT|M_KMM|M_RMAX(4),					//41
		"B2.��� ��� ���� ", "    ",	0,     60,	2, M_EDIT|M_KMM|M_RMAX(4),					//42
		"B3.CL START TRQ ", "    ", 	0,     60,  	4, M_EDIT|M_KMM|M_RMAX(4),					//43
		"B4.OP SEAL TRQ  ", "    ", 	0,     60,  	2, M_EDIT|M_KMM|M_RMAX(4),					//44
		"B5.OP START TRQ ", "    ", 	0,     60,  	4, M_EDIT|M_KMM|M_RMAX(4),					//45
		"   B29.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//46
		"B7.SEAL TYPE    ", "    ", 	1,      4,  	1, M_LIST|M_SADR(126),						//47
		"B8.ROD TYPE     ", "    ", 	0,      1,  	0, M_LIST|M_SADR(131),						//48
		"B9.ZONE CLOSE   ", "    ", 	0,   3000,     10, M_EDIT|M_RMAX(3)|M_PREC(1),				        //49
		"B10.ZONE OPEN   ", "    ", 	0,   3000,     10, M_EDIT|M_RMAX(3)|M_PREC(1),				        //50
		"B11.OFFSET ZONE ", "    ", 	0,    100,  	0, M_EDIT|M_RMAX(2)|M_PREC(1),				        //51
		"B12.PASSWORWD   ", "    ", 	0,  65530,  	0, M_CODE,							//52
		"   B13.TIME     ", "    ", 	0,  65530,  	0, M_TIME|M_PWP1|M_PWP1,					//53
		"   B14.DATE     ", "    ", 	0,  65530,  	0, M_DATE|M_PWP1|M_PWP1,					//54
		"   B15.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),					        //55
		"   B16.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//56
		"B17.MU/DU       ", "    ", 	0,      3,  	1, M_LIST|M_SADR(135),						//57
		"   B18.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),	
                "B19.DU SOURSE   ", "    ", 	1,      3,  	2, M_LIST|M_SADR(139),						//58
                "B20.BLOK ZALIP  ", "    ", 	0,      1,  	0, M_EDIT|M_RMAX(4),						//59
		"B21.COMMAND TIME", "S   ", 	1,    100,  	1, M_EDIT|M_RMAX(2)|M_PREC(1),				        //60
		"B22.IN TYP SIG  ", "    ", 	0,      1,  	0, M_LIST|M_SADR(145),							//61
		"B23.IN MASK     ", "    ", 	0,     31,  	0, M_BINE|M_RMAX(4),						//62
		"B24.OUT MASK    ", "    ", 	0,  65530,  	0, M_BINE|M_RMAX(7),						//63
		"B25.BAUD RATE   ", "    ", 	0,      6,  	3, M_LIST|M_SADR(147),						//64
		"B26.ADRESS      ", "    ", 	1,     32,  	1, M_EDIT|M_RMAX(1),						//65
		"B27.COMM TYPE   ", "    ", 	0,      2,  	0, M_LIST|M_SADR(225),						//66
		"B28.MUFF TIME   ", "S   ", 	3,    100,     20, M_EDIT|M_RMAX(2)|M_PREC(1),				        //67
		"B29.���� �������", "    ", 	0,    100,    100, M_EDIT|M_RMAX(2)|M_PREC(1),					//68
		"B30.���� ����   ", "��  ", 	0,    500,    500, M_EDIT|M_RMAX(2)|M_PREC(1),					//69
		"B31.���� ������ ", "    ", 	0,    100,    100, M_EDIT|M_RMAX(2)|M_PREC(1),					//70
		"   B32.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//71
		"B33.NO MOVE TIME", "S   ", 	3,    100,     20, M_EDIT|M_RMAX(2)|M_PREC(1),				        //72
		"B34.SEAL WAY    ", "OB  ", 	1,   1000,    100, M_EDIT|M_RMAX(3)|M_PREC(1),				        //73
		"B35.��� ��������", "    ", 	0,      2,  	0, M_SHOW|M_RMAX(4),						//74 0 ���� 1 �� ������ 2 �� ����
		"B36.����� ����  ", "    ", 	0,    100,  	0, M_SHOW|M_RMAX(4)|M_PREC(1),					//75
		"B37.����� ����� ", "%   ", 	0,   1000,      0, M_EDIT|M_RMAX(3)|M_PREC(1),						//76
		" B38.�� ���     ", "    ", 	0,  65535,  	30, M_EDIT|M_RMAX(4)|M_PREC(3),					//77
		"   B39.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//78
		"   B40.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//79
		"   B41.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//80
		"   B42.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//81
		"   B43.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//82
		"   B44.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//83
		"   B45.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//84
		"   B46.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//85
		"   B47.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//86
		"   B48.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//87
		"   B49.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//89
		//------------------------------------------------------------------
		" C0.PASSWORD    ", "    ",     0,  65530,      0, M_CODE,							//90
		"C1.EP TYPE      ", "    ",     0,     25,      0, M_FACT|M_RMAX(1),					        //91
		"C2.YEAR OF PROD ", "    ",     0,     99,     22, M_FACT|M_RMAX(1),						//92
		"C3.FACTORY NUM  ", "    ",     0,   9999,      0, M_FACT|M_RMAX(3),						//93
		"C4.MAX TORQ     ", "    ",    10,   6000,     10, M_FACT|M_RMAX(3)|M_PREC(2),				        //94
		"C5.NOM CURRENT  ", "A   ",     1,   1200,     10, M_FACT|M_RMAX(2)|M_PREC(1),				        //95
		"C6.KP RED       ", "    ",     1,  65530,  20000, M_FACT|M_RMAX(4)|M_PREC(2),				        //96
		"C7.INDYC TYPE   ", "    ",      0,      1,     0, M_FACT|M_RMAX(4),						//97
                "C8.INDYC RES T  ", "S   ",     0,     30,     10, M_FACT|M_RMAX(1),						//98 
                "C9.SET DEFOULT  ", "    ",     0,      1,      0, M_FCOMM|M_SADR(156),						//99
                "C10.SUBVERSION  ", "    ",     0,  65535,      0, M_SHOW|M_RMAX(4),						//100
		"C11.HALL STATE  ", "    ",     0,  65535,      0, M_SHOW|M_RMAX(4),                                            //101
                " �12.��� ���� U ", "    ",     0,  65530,   4000, M_FACT|M_RMAX(4),						//102
		" �13.��� ���� V ", "    ",     0,  65530,   4000, M_FACT|M_RMAX(4),						//103
		" �14.��� ���� W ", "    ",     0,  65530,   4000, M_FACT|M_RMAX(4),						//104
		"�15.���� ���� U ", "    ",     0,  65530,  32690, M_FACT|M_RMAX(4),						//105
		"�16.���� ���� V ", "    ",     0,  65530,  32690, M_FACT|M_RMAX(4),						//106
		"�17.���� ���� W ", "    ",     0,  65530,  32690, M_FACT|M_RMAX(4),						//107
		" �18.��� ���� R ", "    ",     0,  65530,   2000, M_FACT|M_RMAX(4),						//108
		" �19.��� ���� S ", "    ",     0,  65530,   2000, M_FACT|M_RMAX(4),						//109
		" �20.��� ���� T ", "    ",     0,  65530,   2000, M_FACT|M_RMAX(4),						//110
                "�21.���� ���� R ", "    ",     0,  65530,  32760, M_FACT|M_RMAX(4),						//111
		"�22.���� ���� S ", "    ",     0,  65530,  32760, M_FACT|M_RMAX(4),						//112
		"�23.���� ���� T ", "    ",     0,  65530,  32760, M_FACT|M_RMAX(4),						//113
                " �24.��� ��� VDC", "    ",     0,  65530,   1240, M_FACT|M_RMAX(4),						//114
                "�25.���� ��� VDC", "    ",     0,  65530,      0, M_FACT|M_RMAX(4),						//115
                " �26.��� ���� BR", "    ",     0,  65530,   1240, M_FACT|M_RMAX(4),						//116
                "�27.���� ���� BR", "    ",     0,  65530,      0, M_FACT|M_RMAX(4),						//117 
                "�28.����.��.��� ", "M�C ",     0,    100,     21, M_FACT|M_RMAX(2)|M_PREC(1),				        //118
		"�29.����.��.RMS ", "M�C ",     0,    100,      1, M_FACT|M_RMAX(2)|M_PREC(1),				        //119
                "�30.���� ���    ", "    ",    10,     60,     41, M_FACT|M_RMAX(2),					        //120
		"�31.����� ����  ", "    ", 	0,      1,  	1, M_FLST|M_SADR(124),						//121
		"�32.��� ������� ", "    ", 	0,      1,  	1, M_FLST|M_SADR(228),						//122
		"C33.��� ���� ���", "    ",     0,     16,      0, M_FACT|M_RMAX(1),						//123
		"   �34.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//124
		"   �35.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//125
		"   �36.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//126
		"   �37.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//127
		"   �38.��� ���  ", "    ", 	0,  1,  	0, M_FACT|M_RMAX(4),						//128
		"   �39.��� ���  ", "    ", 	0,  1,  	0, M_FACT|M_RMAX(4),						//129
                "�40.������1 4   ", "    ", 	0,  65535,   6700, M_FACT|M_RMAX(4),						//130
		"�41.������2 4.5 ", "    ", 	0,  65535,   7300, M_FACT|M_RMAX(4),						//131
		"�42.������3 5   ", "    ", 	0,  65535,   7900, M_FACT|M_RMAX(4),						//132
		"�43.������4 5.5 ", "    ", 	0,  65535,   8500, M_FACT|M_RMAX(4),						//133
		"�44.������5 6   ", "    ", 	0,  65535,   9100, M_FACT|M_RMAX(4),						//134
		"�45.������6 6.5 ", "    ", 	0,  65535,   9700, M_FACT|M_RMAX(4),						//135
                "�46.������7 7   ", "    ", 	0,  65535,  10300, M_FACT|M_RMAX(4),						//136
		"�47.������8 7.5 ", "    ", 	0,  65535,  10900, M_FACT|M_RMAX(4),						//137
		"�48.������9 8   ", "    ", 	0,  65535,  11490, M_FACT|M_RMAX(4),						//138
		"�49.������10 8.5", "    ", 	0,  65535,  12080, M_FACT|M_RMAX(4),						//139
		"�50.������11 9  ", "    ", 	0,  65535,  12680, M_FACT|M_RMAX(4),						//140
		"�51.������12 9.5", "    ", 	0,  65535,  13290, M_FACT|M_RMAX(4),						//141
		"�52.������13 10 ", "    ", 	0,  65535,  13880, M_FACT|M_RMAX(4),						//142
		"�53.������14 105", "    ", 	0,  65535,  14480, M_FACT|M_RMAX(4),						//143
		"�54.������15 11 ", "    ", 	0,  65535,  15070, M_FACT|M_RMAX(4),						//144
		"�55.������16 115", "    ", 	0,  65535,  15680, M_FACT|M_RMAX(4),						//145
                "�56.������17 12 ", "    ", 	0,  65535,  16270, M_FACT|M_RMAX(4),						//146
                "�57.������18 125", "    ", 	0,  65535,  16870, M_FACT|M_RMAX(4),						//147
                "�58.������19 13 ", "    ", 	0,  65535,  17460, M_FACT|M_RMAX(4),						//148
                "�59.������20 135", "    ", 	0,  65535,  18070, M_FACT|M_RMAX(4),						//149
                "�60.������21 14 ", "    ", 	0,  65535,  18660, M_FACT|M_RMAX(4),						//150
                "�61.������22 145", "    ", 	0,  65535,  19260, M_FACT|M_RMAX(4),						//151
		"�62.������23 15 ", "    ", 	0,  65535,  19860, M_FACT|M_RMAX(4),						//152
                "�63.������24 155", "    ", 	0,  65535,  20460, M_FACT|M_RMAX(4),						//153
		"�64.������25 16 ", "    ", 	0,  65535,  21050, M_FACT|M_RMAX(4),						//154
		"�65.������26 165", "    ", 	0,  65535,  21650, M_FACT|M_RMAX(4),						//155
                "�66.������27 17 ", "    ", 	0,  65535,  22250, M_FACT|M_RMAX(4),						//156
		"�67.������28 175", "    ", 	0,  65535,  22850, M_FACT|M_RMAX(4),						//157
		"�68.������29 18 ", "    ", 	0,  65535,  23450, M_FACT|M_RMAX(4),						//158
		"�69.������30 185", "    ", 	0,  65535,  24045, M_FACT|M_RMAX(4),						//159
		"�70.������31 19 ", "    ", 	0,  65535,  24650, M_FACT|M_RMAX(4),						//160
		"�71.������32 195", "    ", 	0,  65535,  25240, M_FACT|M_RMAX(4),						//161
		"�72.������33 20 ", "    ", 	0,  65535,  25840, M_FACT|M_RMAX(4),						//162
		"�73.������1 4   ", "    ", 	0,  65535,   6410, M_FACT|M_RMAX(4),						//163
                "�74.������2 4.5 ", "    ", 	0,  65535,   7218, M_FACT|M_RMAX(4),						//164
		"�75.������3 5   ", "    ", 	0,  65535,   8020, M_FACT|M_RMAX(4),						//165
                "�76.������4 5.5 ", "    ", 	0,  65535,   8820, M_FACT|M_RMAX(4),						//166
                "�77.������5 6   ", "    ", 	0,  65535,   9620, M_FACT|M_RMAX(4),						//167
                "�78.������6 6.5 ", "    ", 	0,  65535,  10420, M_FACT|M_RMAX(4),						//168
                "�79.������7 7   ", "    ", 	0,  65535,  11220, M_FACT|M_RMAX(4),						//169
                "�80.������8 7.5 ", "    ", 	0,  65535,  12020, M_FACT|M_RMAX(4),						//170
                "�81.������9 8   ", "    ", 	0,  65535,  12820, M_FACT|M_RMAX(4),						//171
		"�82.������10 8.5", "    ", 	0,  65535,  13620, M_FACT|M_RMAX(4),						//172
		"�83.������11 9  ", "    ", 	0,  65535,  14420, M_FACT|M_RMAX(4),						//173
		"�84.������12 9.5", "    ", 	0,  65535,  15220, M_FACT|M_RMAX(4),						//174
		"�85.������13 10 ", "    ", 	0,  65535,  16030, M_FACT|M_RMAX(4),						//175
                "�86.������14 105", "    ", 	0,  65535,  16830, M_FACT|M_RMAX(4),						//176
		"�87.������15 11 ", "    ", 	0,  65535,  17630, M_FACT|M_RMAX(4),						//177
		"�88.������16 115", "    ", 	0,  65535,  18430, M_FACT|M_RMAX(4),						//178
		"�89.������17 12 ", "    ", 	0,  65535,  19230, M_FACT|M_RMAX(4),						//179
		"�90.������18 125", "    ", 	0,  65535,  20040, M_FACT|M_RMAX(4),						//180
		"�91.������19 13 ", "    ", 	0,  65535,  20840, M_FACT|M_RMAX(4),						//181
		"�92.������20 135", "    ", 	0,  65535,  21640, M_FACT|M_RMAX(4),						//182
		"�93.������21 14 ", "    ", 	0,  65535,  22440, M_FACT|M_RMAX(4),						//183
		"�94.������22 145", "    ", 	0,  65535,  23240, M_FACT|M_RMAX(4),						//184
		"�95.������23 15 ", "    ", 	0,  65535,  24040, M_FACT|M_RMAX(4),						//185
                "�96.������24 155", "    ", 	0,  65535,  24840, M_FACT|M_RMAX(4),						//186
                "�97.������25 16 ", "    ", 	0,  65535,  25640, M_FACT|M_RMAX(4),						//187
                "�98.������26 165", "    ", 	0,  65535,  26440, M_FACT|M_RMAX(4),						//188
                "�99.������27 17 ", "    ", 	0,  65535,  27250, M_FACT|M_RMAX(4),						//189
                "�100.������28175", "    ", 	0,  65535,  28050, M_FACT|M_RMAX(4),						//190
                "�101.������2918 ", "    ", 	0,  65535,  28850, M_FACT|M_RMAX(4),						//191
		"�102.������30185", "    ", 	0,  65535,  29650, M_FACT|M_RMAX(4),						//192
		"�103.������3119 ", "    ", 	0,  65535,  30450, M_FACT|M_RMAX(4),						//193
		"�104.������32195", "    ", 	0,  65535,  31250, M_FACT|M_RMAX(4),						//194
		"�105.������33 20", "    ", 	0,  65535,  32060, M_FACT|M_RMAX(4),						//195
                "   �106.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//196
		"   �107.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//197
		"   �108.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//198
		"   �109.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//199
		"C110.��� ���� U ", "    ", 	 0,      1,  	 1, M_FLST|M_SADR(154),						//200
		"�111.���� ���� U", "B   ",    220,    335,    265, M_FACT|M_RMAX(2),						//201
		"�112.���� ���� U", "C   ",      1,    250,    220, M_FACT|M_RMAX(2)|M_PREC(1),				        //202		
                "C113.��� ��� U  ", "    ", 	 0,      1,  	 1, M_FLST|M_SADR(154),						//203
		"�114.���� ��� U ", "B   ",     90,    220,    115, M_FACT|M_RMAX(2),						//204
		"�115.���� ��� U ", "C   ",      1,    250,    220, M_FACT|M_RMAX(2)|M_PREC(1),				        //205              
                "C116.��� ���� U ", "    ", 	 0,      1,  	 1, M_FLST|M_SADR(154),						//206
		"�117.���� ���� U", "�   ",      0,    110,     60, M_FACT|M_RMAX(2),						//207
                "�118.���� ���� U", "C   ",      1,    250,      1, M_FACT|M_RMAX(2)|M_PREC(1),				        //208          
                "C119.��� ���� I ", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //209
		"C120.���� ���� I", "%   ",     20,    500,    150, M_FACT|M_RMAX(2)|M_PREC(1),				        //210
		"C121.���� ���� I", "C   ",      1,    100,      7, M_FACT|M_RMAX(1)|M_PREC(1),				        //211    
                "C122.��� �� ��  ", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //212
		"�123.���� �� �� ", "    ",      0,  65530,  45000, M_FACT|M_RMAX(4),						//213
		"�124.���� �� �� ", "    ",      0,  65530,  20000, M_FACT|M_RMAX(4),						//214              
                "C125.���   �����", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //215
		"�126.��    �����", "����",      0,    150,     80, M_FACT|M_RMAX(2),						//216
		"�127.����  �����", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //217           
                "C128.���  ������", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //218
		"�129.��   ������", "����",  -100U,     20,      0, M_FACT|M_SIGN|M_RMAX(2),					//219
		"�130.���� ������", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //220		
                "C131.����� ���� ", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //221
		"�132.��   ����� ", "����",      0,    200,     60, M_FACT|M_RMAX(2),					        //222
		"�133.���� ����� ", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //223		
		"C134.����� �����", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //224
		"�135.��   ������", "����",      0,    200,     60, M_FACT|M_RMAX(2),					        //225
		"�136.���� ������", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //226		
                "   �137.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//227
                "   �138.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//228
                "   �139.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//229
                "   �140.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//230
                "   �141.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//231
		"   �142.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//232
		"   �143.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//233
		"   �144.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//234
		"   �145.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//235
                "   �146.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//236
		"   �147.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//237
		"   �148.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//238
		"   �149.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//239
		"   �150.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//240
		"   �151.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//241
		"   �152.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//242
		"   �153.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//243
		"   �154.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//244
		"   �155.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//245
                "   �156.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//246
                "   �157.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//247
                "   �158.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//248
                "   �159.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//249
                "   �160.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//250
                "   �161.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//251
		"   �162.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//252
		"   �163.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//253
		"   �164.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//254
		"   �165.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//255
                "   �166.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//256
		"   �167.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//257
		"   �168.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//258
		"   �169.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//259
		"   �170.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//260
		"   �171.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//261
		"   �172.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//262
		"   �173.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//263
		"   �174.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//264
		"   �175.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//265
                "   �176.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//266
                "   �177.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//267
                "   �178.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//268
                "   �179.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//269
                "   �180.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//270
                "   �181.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//271
		"   �182.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//272
		"   �183.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//273
		"   �184.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//274
		"   �185.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//275
                "   �186.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//276
		"   �187.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//277
		"   �188.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//278
		"   �189.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//279
		"   �190.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//280
		"   �191.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//281
		"   �192.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//282
		"   �193.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//283
		"   �194.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//284
		"   �195.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//285
                "   �196.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//286
                "   �197.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//287
                "   �198.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//288
                "   �199.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//289
              
                
                //------------------------------------------------------------------
		"D0.SET CLOSE    ", "    ",      0,      2,      0, M_COMM|M_SADR(156),						//0 290
		"D1.SET OPEN     ", "    ",      0,      2,      0, M_COMM|M_SADR(156),						//1 291
		"D2.TO OPEN      ", "    ",      0,   1000,      0, M_DCOM|M_RMAX(3)|M_PREC(1),				        //2 292
		"D3.TO CLOSE     ", "    ",      0,   1000,      0, M_DCOM|M_RMAX(3)|M_PREC(1),				        //3 293
		"  D4.RESERV     ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//4 294
		"D5.CALIB RESET  ", "    ",      0,      1,      0, M_COMM|M_SADR(159),						//5 295
		"D6.COMANDS      ", "    ",      0,     19,      0, M_COMM|M_SADR(161),						//6 296
		" D7.ERR RESET   ", "    ",      0,      1,      0, M_COMM|M_SADR(161),						//7 297
		" D8.SET DEFOULT ", "    ",      0,      1,      0, M_COMM|M_SADR(156),						//8 298
		"   D9.RESERV    ", "    ",      0,  65535,  	0, M_SHOW|M_SADR(4),						//9 299
		"D10.CYCLE RESET ", "    ",      0,      1,     0, M_COMM|M_SADR(161),						//10 300
		"D11.COMM RESET  ", "    ", 	 0,  	 1,  	0, M_COMM|M_SADR(161),						//11 301
		"   D12.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//12 302
		"   D13.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//13 303
		"   D14.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//14 304
		"   D15.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//15 305
		"   D16.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//16 306
		"   D17.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//17 307
		"   D18.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//18 308
		"   D19.RESERV   ", "    ", 	 0,  65535,  	0, M_SHOW|M_RMAX(4),						//19 309
		//-------------------------------------------------------------------
		"G0.TEST MODE    ", "    ",      0,      1,      0, M_FACT|M_RMAX(1),						//0 310--------------------------------------------------------
		"G1.LED TEST     ", "    ",      0,  65530,      0, M_BINFC|M_RMAX(4),						//1 281
		"G2.DIS OUT TEST ", "    ",      0,  65530,      0, M_BINFC|M_RMAX(4),						//2 282
		"G3.DAC VALUE    ", "%   ",      0,   1000,      0, M_FDCOM|M_RMAX(3)|M_PREC(1),			        //3 283
		"G4.DISPL START  ", "    ",      0,      1,      0, M_FDCOM,							//4 284
		"   G5.RESERV    ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//18 285
		"   G6.RESERV    ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//18 286
		"   G7.RESERV    ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//7 287
		"   G8.RESERV    ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//8 288
		"   G9.RESERV    ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//9 289
		"   G10.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//10 290
		"   G11.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//11 291
		"   G12.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//12 292
		"   G13.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//13 293
		"   G14.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//14 294
		"   G15.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//15 295
		"   G16.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//16 296
		"   G17.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//17 297
		"   G18.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//18 299
		"   G20.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//18 300
		"   G21.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//18 301
		"   G22.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//22 302
		"   G23.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//23 303
		"   G24.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//24 304
		"   G25.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//25 305
		"   G26.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//26 306
		"   G27.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//27 307
		"   G28.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//28 308
		"   G29.RESERV   ", "    ",      0,  65535,      0, M_SHOW|M_RMAX(4),						//29 309
		//-------------------------------------------------------------------
		"CALIB STAUS     ", "    ",      0,      3,      0, M_HIDE,							//0 310
		"     RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//1 311
		"CLOSE POS 1     ", "    ",      0,  65535,      0, M_HIDE|M_RMAX(4),						//2 312
		"CLOSE POS 2     ", "    ",      0,  65535,      0, M_HIDE|M_RMAX(4),						//3 313
		"OPEN POS 1      ", "    ",      0,  65535,      0, M_HIDE|M_RMAX(4),						//4 314
		"OPEN POS 2      ", "    ",      0,  65535,      0, M_HIDE|M_RMAX(4),						//5 315
		"PASSWORD 1      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//6 316
		"PASSWORD 2      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//7 317
		"SHC             ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//8 318
		"MUFF            ", "    ", 	 0,  65530,  	 0, M_HIDE|M_RMAX(4),						//9 319
		"CYCLE CNT       ", "    ", 	 0,  65530,  	 0, M_HIDE|M_RMAX(4),						//10 320
		"RESERV          ", "    ", 	 0,  65535,  	 0, M_HIDE|M_RMAX(4),						//11 321
		"RESERV          ", "    ", 	 0,  65535,  	 0, M_HIDE|M_RMAX(4),						//12 322
		"RESERV          ", "    ", 	 0,  65535,  	 0, M_HIDE|M_RMAX(4),						//13 323
		"   H14.DATE     ", "    ", 	 0,  65530,  	 0, M_HIDE|M_RMAX(4),						//14 324
		"RESERV          ", "    ", 	 0,  65535,  	 0, M_HIDE|M_RMAX(4),						//15 325
		"SWITCH CURR     ", "%   ",      0,   3000,      0, M_HIDE|M_RMAX(3)|M_PREC(1),				//16 326
		" TRQ    CUR 0_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//17 327
		" TRQ    CUR 0_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//18 328
		" TRQ    CUR 0_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//19 329
		" TRQ    CUR 0_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//20 330
		" TRQ    CUR 0_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//21 331
		" TRQ    CUR 0_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//22 332
		" TRQ    CUR 1_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//23 333
		" TRQ    CUR 1_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//24 334
		" TRQ    CUR 1_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//25 335
		" TRQ    CUR 1_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//26 336
		" TRQ    CUR 1_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//27 337
		" TRQ    CUR 1_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//28 338
		" TRQ    CUR 2_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//29 339
		" TRQ    CUR 2_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//30 340
		" TRQ    CUR 2_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//31 341
		" TRQ    CUR 2_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//32 342
		" TRQ    CUR 2_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//33 343
		" TRQ    CUR 2_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//34 344
		" TRQ    CUR 3_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//35 345
		" TRQ    CUR 3_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//36 346
		" TRQ    CUR 3_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//37 347
		" TRQ    CUR 3_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//38 348
		" TRQ    CUR 3_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//39 349
		" TRQ    CUR 3_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//40 350
		" TRQ   ANGL 0_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//41 351
		" TRQ   ANGL 0_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//42 352
		" TRQ   ANGL 0_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//43 353
		" TRQ   ANGL 0_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//44 354
		" TRQ   ANGL 0_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//45 355
		" TRQ   ANGL 0_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//46 356
		" TRQ   ANGL 1_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//47 357
		" TRQ   ANGL 1_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//48 358
		" TRQ   ANGL 1_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//49 359
		" TRQ   ANGL 1_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//50 360
		" TRQ   ANGL 1_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//51 361
		" TRQ   ANGL 1_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//52 362
		" TRQ   ANGL 2_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//53 363
		" TRQ   ANGL 2_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//54 364
		" TRQ   ANGL 2_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//55 365
		" TRQ   ANGL 2_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//56 366
		" TRQ   ANGL 2_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//57 367
		" TRQ   ANGL 2_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//58 368
		" TRQ   ANGL 3_0 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//59 369
		" TRQ   ANGL 3_1 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//60 370
		" TRQ   ANGL 3_2 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//61 371
		" TRQ   ANGL 3_3 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//62 372
		" TRQ   ANGL 3_4 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//63 373
		" TRQ   ANGL 3_5 ", "%   ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//64 374
		" ANGL   SF 0_0  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//65 375
		" ANGL   SF 0_1  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//66 376
		" ANGL   SF 0_2  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//67 377
		" ANGL   SF 0_3  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//68 378
		" ANGL   SF 0_4  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//69 379
		" ANGL   SF 0_5  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//70 380
		" ANGL   SF 1_0  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//71 381
		" ANGL   SF 1_1  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//72 382
		" ANGL   SF 1_2  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//73 383
		" ANGL   SF 1_3  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//74 384
		" ANGL   SF 1_4  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//75 385
		" ANGL   SF 1_5  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//76 386
		" ANGL   SF 2_0  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//77 387
		" ANGL   SF 2_1  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//78 388
		" ANGL   SF 2_2  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//79 389
		" ANGL   SF 2_3  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//80 390
		" ANGL   SF 2_4  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//81 391
		" ANGL   SF 2_5  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//82 392
		" ANGL   SF 3_0  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//83 393
		" ANGL   SF 3_1  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//84 394
		" ANGL   SF 3_2  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//85 395
		" ANGL   SF 3_3  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//86 396
		" ANGL   SF 3_4  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//87 397
		" ANGL   SF 3_5  ", "ANGL",      0,    180,      0, M_HIDE|M_RMAX(2),						//88 398
		"COMANDS TU      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//89 399
		"AVER VOLT       ", "V   ",      0,    500,      0, M_HIDE|M_RMAX(2),						//90 400
		"131. ���� ������", "    ",      0,  65530,      0, MT_DEC|M_RMAX(4),						//91 401
		"AVER CURR %     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//92 402
		"  CURRENT  U %  ", "  % ",      0,   8000,      0, M_HIDE|M_RMAX(3)|M_PREC(1),				//93 403
		"  CURRENT  V %  ", "  % ",      0,   8000,      0, M_HIDE|M_RMAX(3)|M_PREC(1),				//94 404
		"  CURRENT  W %  ", "  % ",      0,   8000,      0, M_HIDE|M_RMAX(3)|M_PREC(1),				//95 405
		" AVER CURR      ", "A   ",      0,   8000,      0, M_HIDE|M_RMAX(3)|M_PREC(1),				//96 406
		"  POSITION      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//97 407
		"  POSITION      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//98 408
		" FULL WAY       ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//99 409
		"  FULL WAY      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//100 410
		"REVERS TYPE     ", "    ",      0,      2,      2, M_HIDE|M_RMAX(2),						//101 411
		"BAD   TASK 2 KHZ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//102 412
		"BAD   TASK 200HZ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//103 413
		"BAD   TASK 50HZ1", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//104 414
		"BAD   TASK 50HZ2", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//105 415
		"BAD   TASK 50HZ3", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//106 416
		"BAD   TASK 10 HZ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//107 417
		"BAD TASK RESET  ", "    ",      0,  65530,      0, MT_DEC|M_NVM|M_RMAX(4),					//108 418
		"CPU TIME        ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//109 419
		"TASK LIST       ", "    ",      0,  65530,      0, MT_DEC|M_NVM|M_RMAX(4),					//110 420
		"TASK NUMBER     ", "    ",      0,  65530,      0, MT_DEC|M_NVM|M_RMAX(4),					//111 421
		"     RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//112 408
		"     RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//113 408
		"     RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//114 408
		"     RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//115 408
		"H114.START INDIC", "    ",      0,     29,      8, M_HIDE|M_RMAX(4),						//116 408
		"LogEvAddr       ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//117 427
		"LogCmdAddr      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//118 428
		"LogParamAddr    ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//119 429
		"LogSimAddr      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//123 433
		"LogEvCount      ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//120 430
		"LogCmdCount     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//121 431
		"LogParamCount   ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//122 432
		"LogSimCount     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//124 434
		"    Seconds     ", "    ",      0,  	60,      0, M_HIDE|M_RMAX(1),						//125 435
		"ContGroup       ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//126 436
		"LogControlWord  ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//127 437
		"LogReset        ", "    ",      0,      1,      0, MT_DEC|M_RMAX(4),						//128 438
		"TS Indication   ", "    ",      0,  65535,      0, M_HIDE|M_RMAX(4),						//129 439
		"1    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//130 440
		"2    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//131 441
		"3    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//132 442
		"4    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//133 443
		"5    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//134 444
		"6    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//135 445
		"7    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//136 446
		"8    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//137 447
		"9    RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//138 448
		"10   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//139 449
		"11   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//140 450
		"12   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//141 451
		"13   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//142 452
		"14   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//143 453
		"15   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//144 454
		"16   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//145 455
		"17   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//146 456
		"18   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//147 457
		"19   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//148 458
		"20   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//149 459
		"21   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//150 460
		"22   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//151 461
		"23   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//152 462
		"24   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//153 463
		"25   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//154 464
		"26   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//155 465
		"27   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//156 466
		"28   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//157 467
		"29   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//158 468
		"30   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//159 469
		"31   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//160 470
		"32   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//161 471
		"33   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//162 472
		"34   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//163 473
		"35   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//164 474
		"36   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//165 475
		"37   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//166 476
		"38   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//167 477
		"39   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//168 478
		"40   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//169 479
		"41   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//170 480
		"42   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//171 481
		"43   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//172 482
		"44   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//173 483
		"45   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//174 484
		"46   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//175 485
		"47   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//176 486
		"48   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//177 487
		"49   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//178 488
		"50   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//179 489
		"51   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//180 490
		"52   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//181 491
		"53   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//182 492
		"54   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//183 493
		"55   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//184 494
		"56   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//185 495
		"57   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//186 496
		"58   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//187 497
		"59   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//188 498
		"60   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),						//189 499
		"60   RESERV     ", "    ",      0,  65530,      0, M_HIDE|M_RMAX(4),
		
		//--------------------------------------------------------------------------------------------------
		"      TIME      ", "    ",      0,  65535,      0, M_TIME|M_RONLY,							//T
		"      DATE      ", "    ",      0,  65535,     33, M_DATE|M_RONLY,							//D
		"E0.STATUS       ", "    ",      0,  65535,      0, M_RUNS|M_SADR(1),						//0
		"E1.PROCESS ERR  ", "    ",      0,  65535,      0, M_RUNS|M_SADR(18),						//1
		"E2.VOLT ERROR   ", "    ",      0,  65535,      0, M_RUNS|M_SADR(35),						//2
		"E3.CURRENT ERR  ", "    ",      0,  65535,      0, M_RUNS|M_SADR(52),						//3
		"E4.DEVICE ERR   ", "    ",      0,  65530,      0, M_RUNS|M_SADR(69),						//4
		"E5.POSITION     ", "%   ", -1000U,   2000,      0, M_SHOW|M_SIGN|M_RMAX(3)|M_PREC(1),		//5
		"    E6.TORQE    ", "    ",     10,  11000,     10, M_SHOW|M_RMAX(4),						//6
		"E7.VOLT L1      ", "V   ",      0,    500,      0, M_SHOW|M_RMAX(2),						//7
		"E8.VOLT L2      ", "V   ",      0,    500,      0, M_SHOW|M_RMAX(2),						//8
		"E9.VOLT L3      ", "V   ",      0,    500,      0, M_SHOW|M_RMAX(2),						//9
		"E10.CURRENT  U  ", "A   ", 	 0,   8000,  	 0, M_SHOW|M_RMAX(3)|M_PREC(1),				//10
		"E11.CURRENT  V  ", "A   ", 	 0,   8000,  	 0, M_SHOW|M_RMAX(3)|M_PREC(1),				//11
		"E12.CURRENT  W  ", "A   ",  	 0,   8000,  	 0, M_SHOW|M_RMAX(3)|M_PREC(1),				//12
		"E13.TEMPER      ", "    ",   -100U,    100,     0, M_SHOW|M_SIGN|M_RMAX(2),				//13
		"E14.IN STATE    ", "    ",      0,  65535,      0, M_RUNS|M_SADR(86),						//14
		"E15.OUT STATE   ", "    ",      0,  65535,      0, M_RUNS|M_SADR(103),						//15
		"   E16.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//16
		"   E17.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//17
		"   E18.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//19
		"   E20.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//20
		"   E21.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//21
		"   E22.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//22
		"   E23.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//23
		"   E24.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//24
		"   E25.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//25
		"   E26.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//26
		"   E27.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//27
		"   E28.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4),						//28
		"   E29.RESERV   ", "    ", 	 0,  65535,  	 0, M_SHOW|M_RMAX(4)						//29
};

const struct MENU_STRING values[] = {
	"    ������      ",	// 0
//----------------
	"  �� ���������  ",	// 1
	"  ����������    ",	// 2
	"     ������     ",	// 3
	" ���� ��������  ",	// 4
	" ���� ��������  ",	// 5
	"   ���� ����    ",	// 6
	"    �������     ",	// 7
	"    �������     ",	// 8
	"     �����      ",	// 9
	" ������� ������ ",	// 10
	"  ������� ���   ",	// 11
	"BLUETOOTH ���   ",	// 12
	"������ �� ����� ",	// 13
	"  ������ �� ��  ",	// 14
	"����������������",	// 15
	"��������� �� ���",	// 16
	"��������� �� �� ",	// 17
//--------------------------
	"   ��� ������   ",	// 18
	"�01 ��� ��������",	// 19
	"�02 ��� ��� ����",	// 20
	"�03 �� ��� ���� ",	// 21
	"�03 �� ��� ���� ",	// 22
	"�03 ��� ������  ",	// 23
	"�04 ��� �� ���� ",	// 24
	"�05 �����       ",	// 25
	"�06 ������ ��/��",	// 26
	"�07 ������ ���  ",	// 27
	"�08 ��� ��� ��� ",	// 28
	"     ������     ",	// 29
	"     ������     ",	// 30
	"     ������     ",	// 31
	"     ������     ",	// 32
	"     ������     ",	// 33
	"     ������     ",	// 34
//--------------------------
	"   ��� ������   ",	// 35
	"�11 ��� ���� � R",	// 36
	"�11 ��� ���� � S",	// 37
	"�11 ��� ���� � T",	// 38
	"�12 ���� ��� � R",	// 39
	"�12 ���� ��� � S",	// 40
	"�12 ���� ��� � T",	// 41
	"����� ����� ��� ",	// 42
	"�13 �����/��� U ",	// 43
	"�14 ���� ��� � R",	// 44
	"�14 ���� ��� � S",	// 45
	"�14 ���� ��� � T",	// 46
	"�15 ���� � R 47%",	// 47
	"�15 ���� � S 47%",	// 48
	"�15 ���� � T 47%",	// 49
	"  ��� ��� ����  ",	// 50
	"     ������     ",	// 51
//--------------------------
	"   ��� ������   ",	// 52
	"�21 ����� ���� U",	// 53
	"�21 ����� ���� V",	// 54
	"�21 ����� ���� W",	// 55
	"�22 ��-��� �����",	// 56
	"     ������     ",	// 57
	"�23 �� ���� U   ",	// 58
	"�23 �� ���� V   ",	// 59
	"�23 �� ���� W   ",	// 60
	"     ������     ",	// 61
	"     ������     ",	// 62
	" ��������� ���  ",	// 63
	" �������� ����  ",	// 64
	"     ������     ",	// 65
	"     ������     ",	// 66
	"     ������     ",	// 67
	"     ������     ",	// 68
	//---------------------------------------------------------------------------
	"   ��� ������   ",	// 69
	"�31 ���� � �����",	// 70
	"�32 ���� ��� 1  ",	// 71
	"�32 ���� ��� 2  ",	// 72
	"�33 ���� �����  ",	// 73
	"�34 ���� � ���� ",	// 74
	"�35 ������ ���  ",	// 75
	"�36 ������� ��� ",	// 76
	"�37 ������ ���� ",	// 77
	"�38 ������� ����",	// 78
	"���� ���        ",	// 79
	"������ ����     ",	// 80
	"�31 ��� �� � ���",	// 81
	"�41 ���������   ",	// 82
	"�42 �� ����� ���",	// 83
	"�40 ��� ���     ",	// 84
	"     ������     ",	// 85
	//---------------------------------------------------------------------------
	"  ��� ��������  ",	// 86
	"  ���� �������  ",	// 87
	"  ���� �������  ",	// 88
	"  ���� ����     ",	// 89
	"  ���� ��       ",	// 90
	"  ���� ������   ",	// 91
	"     ������     ",	// 92
	"     ������     ",	// 93
	"     ������     ",	// 94
	"     ������     ",	// 95
	"     ������     ",	// 96
	"     ������     ",	// 97
	"     ������     ",	// 98
	"     ������     ",	// 99
	"     ������     ",	// 100
	"     ������     ",	// 101
	"     ������     ",	// 102
	//---------------------------------------------------------------------------
	"  ��� ��������  ",	// 103
	"    �������     ",	// 104
	"    �������     ",	// 105
	"  �����������   ",	// 106
	"  �����������   ",	// 107
	"  ������������� ",	// 108
	"     �����      ",	// 109
	"    ������      ",	// 110
	"      ��        ",	// 111
	"     ������     ",	// 112
	"     ������     ",	// 113
	"     ������     ",	// 114
	"     ������     ",	// 115
	"     ������     ",	// 116
	"     ������     ",	// 117
	"     ������     ",	// 118
	"     ������     ",	// 119
	//---------------------------------------------------------------------------
	" ��� ���������� ",	// 120
	" ������ ������� ",	// 121
	" ������ ������� ",	// 122
	"������ ���������",	// 123
	//---------------------------------------------------------------------------
	"    1.5 ���     ",	// 124
	"    3   ���     ",	// 125
	//---------------------------------------------------------------------------
	"  �� ���������  ",	// 126
	" ��� ���������� ",	// 127
	"���������� ���� ",	// 128
	"���������� ���� ",	// 129
	"��� ���� � ���� ",	// 130
	//---------------------------------------------------------------------------
	"   ����������   ",	// 131
	"    ��������    ",	// 132
	//---------------------------------------------------------------------------
	"������ �������� ",	// 133
	"�������� �� ��� ",	// 134
	//---------------------------------------------------------------------------
	" ����� �� ����� ",	// 135
	"  ����� ������  ",	// 136
	"    ����� ��    ",	// 137
	"    ����� ��    ",	// 138
	//---------------------------------------------------------------------------
	" ��� ���������� ",	// 139
	"������ �������� ",	// 140
	"������ �������� ",	// 141
        "������ ���      ",     // 142
	//---------------------------------------------------------------------------
	"   ���������    ",	// 143
	"    ��������    ",	// 144
	//---------------------------------------------------------------------------
	"      24�       ",	// 145
	"      220�      ",	// 146
	//---------------------------------------------------------------------------
	"    2400 ���    ",	// 147
	"    4800 ���    ",	// 148
	"    9600 ���    ",	// 149
	"   19200 ���    ",	// 150
	"   38400 ���    ",	// 151
	"   57600 ���    ",	// 152
	"   115200 ���   ",	// 153
	//---------------------------------------------------------------------------
	"   ���������    ",	// 154
	"����� � ������� ",	// 155
	//---------------------------------------------------------------------------
	"  ��� �������   ",	// 156
	"     ������     ",	// 157
	"    ��������    ",	// 158
	//---------------------------------------------------------------------------
	"  ��� �������   ",	// 159
	"    ��������    ",	// 160
	//---------------------------------------------------------------------------
	"  ��� �������   ",	// 161
	"      ����      ",	// 162
	"    �������     ",	// 163
	"    �������     ",	// 164
	"  ������� ����  ",	// 165
	"  ������� ����  ",	// 166
	"     ������     ",	// 167
	"     ������     ",	// 168
	"     ������     ",	// 169
	"     ������     ",	// 170
	"     ������     ",	// 171
	"     ������     ",	// 172
	"     ������     ",	// 173
	"     ������     ",	// 174
	"     ������     ",	// 175
	"     ������     ",	// 176
	"     ������     ",	// 177
	"     ������     ",	// 178
	"     ������     ",	// 179
	"     ������     ",	// 180
	//---------------------------------------------------------------------------
	" �� ����������  ",	// 181
	"������ �������� ",	// 182
	"������ �������� ",	// 183
	//---------------------------------------------------------------------------
	"    �������     ",	// 184
	" �������������� ",	// 185
	"  ������������� ",     // 186
	//---------------------------------------------------------------------------
	"    ��������    ",	// 187
	"    �������     ",	// 188
	//---------------------------------------------------------------------------
	"���� �����������",	// 189
	" ���� ��������� ",	// 190
	//---------------------------------------------------------------------------
	" ������ ������  ",	// 191
	" ����� �������� ",	// 192
	" �������� ����  ",	// 193
	" ����� �������� ",	// 194
	" ����� ����-��  ",	// 195
	"  ���� ������   ",	// 196
	"  ���� �������  ",	// 197
	"     ������     ",	// 198
	"     ������     ",	// 199
	"    ��������    ",	// 200
	"  ������� ����  ",	// 201
	"  ������� ���   ",	// 202
	" ���� ��������  ",	// 203
	"     ������     ",	// 204
	"     ������     ",	// 205
	"     ������     ",	// 206
	"     ������     ",	// 207
	"     ������     ",	// 208
	"     ������     ",	// 209
	"     ������     ",	// 210
	"     ������     ",	// 211
	"     ������     ",	// 212
	"     ������     ",	// 213
	"     ������     ",	// 214
	"     ������     ",	// 215
	"     ������     ",	// 216
	"     ������     ",	// 217
	"     ������     ",	// 218
	"     ������     ",	// 219
	"     ������     ",	// 220
	//----------------------------------------------
	"  �� ��������   ",	// 221
	"�� ��� � �������",	// 222
	" ������ ���     ",	// 223
	" ������ ���     ",	// 224
	//-----------------------------------------------
	"  ��� ��������  ",	// 225
	" �� ����������  ",	// 226
	"  �� ��������   ",	// 227
   //-----------------------------------------------
        "380 �           ",	// 228
	"220 �           "	// 229
};

const struct MENU_STRING AddStr[] = {
	"��������� ������",	// 0
	"    �������     ",	// 1
	"    ��������    ",	// 2
	"  � 999.99%     ",	// 3
	//-----------------------------------------------
	"1111111111111111",	// 4	0
	"1111111111111111",	// 5
	"1111111111111111",	// 6	1
	"1111111111111111",	// 7
	"1111111111111111",	// 8	2
	"1111111111111111",	// 9
	"1111111111111111",	// 10	3
	"1111111111111111",	// 11
	"1111111111111111",	// 12	4
	"1111111111111111",	// 13
	"1111111111111111",	// 14	5
	"1111111111111111",	// 15
	"1111111111111111",	// 16	6
	"1111111111111111",	// 17
	"1111111111111111",	// 18	7
	"1111111111111111",     // 19
	"1111111111111111",	// 20	8
	"1111111111111111",     // 21
	"1111111111111111",	// 22	9	// Р—РґРµСЃСЊ РІС‹РІРѕРґСЏС‚СЃСЏ С‚РµРєСѓС‰РёРµ РѕР±РѕСЂРѕС‚С‹
	"                ",     // 23
	"1111111111111111",	// 24	10
	"1111111111111111",     // 25
	"1111111111111111",	// 26	11
	"1111111111111111",     // 27
	"1111111111111111",	// 28	12
	"1111111111111111",     // 29
	"1111111111111111",	// 30	13
	"1111111111111111",     // 31
};

#endif

// РњР°РєСЂРѕСЃ С‡С‚РµРЅРёСЏ СЃС‚СЂРѕРєРё РёР· С„Р»РµС€. РњР°СЃСЃРёРІ add_strings
#define ReadAddStr(Str, Index) \
	PFUNC_blkRead((int16*)&AddStr[Index], (int16*)Str, 16)

// РњР°РєСЂРѕСЃ С‡С‚РµРЅРёСЏ СЃС‚СЂРѕРєРё РёР· С„Р»РµС€. РњР°СЃСЃРёРІ add_strings
#define ReadValuesStr(Str, Index) \
	PFUNC_blkRead((int16*)&values[Index], (int16*)Str, 16)

extern const struct MENU_GROUP  groups[];
extern const struct MENU_PARAM  params[];
extern const struct MENU_STRING values[];
extern const struct MENU_STRING AddStr[];
extern const struct ADDR_PARAM_ATS48  addr_params_altistar48[];


#endif




