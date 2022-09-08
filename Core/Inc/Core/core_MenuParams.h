
#ifndef CORE_MENU_PARAMS_
#define CORE_MENU_PARAMS_

#include "core_MenuGlobal.h" 	
#include "g_Ram.h" 	

#define CREATE_STRUCTS


#define MAX_LEVEL			2 
#define GROUP_COUNT			4 
#define SUBGR_INIT(GR, D)	GR, D

const struct MENU_GROUP groups[] = {
	"   1 цпсоою     ", "хмдхйюжхъ       ", GetAdr(Status),         SIZE(TStatus),
	"   2 цпсоою     ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
	"   3 цпсоою     ", "мюярпнийх гюбнд ", GetAdr(FactoryParam),   SIZE(TFactoryParam),
	"   4 цпсоою     ", "йнлюмдш         ", GetAdr(Comands),        SIZE(TComands)
};
//
const struct MENU_GROUP subgroups[] = {
	"  1.1 ярюрсяш   ", " яхярелш        ", GetAdr(Status),         SIZE(TStatus),
	"  1.2 ярюрсяш   ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
        "  1.3 ярюрсяш   ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
        "  1.4 ярюрсяш   ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
        //---
        "  2.1 мюярпийх  ", "сярпниярбю      ", GetAdr(UserParam),      SIZE(TUserParam),
        "  2.2 мюярпийх  ", "дхяйп яхцмюкнб  ", GetAdr(UserParam),      SIZE(TUserParam),
        "  2.3 мюярпийх  ", "онякед хмрмптеия", GetAdr(UserParam),      SIZE(TUserParam),
        "  2.4 мюярпийх  ", "пефхлю пюанрш   ", GetAdr(UserParam),      SIZE(TUserParam),
        //----
        "  3.2 наыхе     ", "мюярпнийх       ", GetAdr(UserParam),      SIZE(TUserParam),
        "  3.2 мюярпийх  ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
        "  3.2 мюярпийх  ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
        "  3.2 мюярпийх  ", "мюярпнийх онкэг ", GetAdr(UserParam),      SIZE(TUserParam),
        //---    
	"  4.1 цпсоою    ", "мюярпнийх гюбнд ", GetAdr(FactoryParam),   SIZE(TFactoryParam),
	"  4.2 цпсоою    ", "йнлюмдш         ", GetAdr(Comands),        SIZE(TComands)
};


#ifdef CREATE_STRUCTS

const struct MENU_PARAM params[] = {
		"A0.ярюрся       ", "    ",	0,  65530,	0, M_RUNS|M_SADR(1),						//0 00
		"A1.юбюп опнжеяяю", "    ",	0,  65530,	0, M_RUNS|M_SADR(18),						//1 01
		"A2.юбюп яерх    ", "    ",	0,  65530,	0, M_RUNS|M_SADR(35),						//2 02
		"A3.юбюп мюцпсгйх", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(52),						//3 03
		"A4.юбюп сярпнияр", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(69),						//4 04
		"A5.бундю  рс    ", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(86),						//5 05
		"A6.бшундю ря    ", "    ", 	0,  65530,  	0, M_RUNS|M_SADR(103),						//6 06
		"A7.онкнф щмйнд  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//7 07
		"A8.лнлемр       ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//8 08
		"A9.яйнпнярэ     ", "    ",-3600U,   3600,  	0, M_SHOW|M_SIGN|M_RMAX(3),					//9 09
		"A10.мюоп L1     ", "б   ", 	0,    500,  	0, M_SHOW|M_RMAX(2),						//10 10
		"A11.мюоп L2     ", "б   ", 	0,    500,  	0, M_SHOW|M_RMAX(2),						//11 11
		"A12.мюоп L3     ", "б   ", 	0,    500,  	0, M_SHOW|M_RMAX(2),						//12 12
		"A13.рнй U       ", "A   ", 	0,   8000,  	0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //13 13
		"A14.рнй V       ", "A   ", 	0,   8000,  	0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //14 14
		"A15.рнй W       ", "A   ", 	0,   8000,  	0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //15 15
		"A16.сцнк мюцп   ", "    ", -180U,    180,  	0, M_SHOW|M_SIGN|M_RMAX(2),					//16 16
		"A17.CALIB STATE ", "    ", 	0,      3,  	0, M_STAT|M_SADR(120),						//17 17
		"A18.онкнфемхе   ", "%   ",-1000U,   2000,      0, M_SHOW|M_SIGN|M_RMAX(3)|M_PREC(1),		                //18 18
		"A19.онкмши осрэ ", "на  ",     0,   3000,      0, M_SHOW|M_RMAX(3)|M_PREC(1),				        //19 19
		"A20.рейсыхи осрэ", "на  ",-3000U,   3000,      0, M_SHOW|M_SIGN|M_RMAX(3)|M_PREC(1),		                //20 20
		"A21.явер жхйкнб ", "    ", 	0,  65530,  	0, M_SHOW|M_RMAX(4),						//21 21
		"A22.релоепюрспю ", "    ", -100U,    100,      0, M_SHOW|M_SIGN|M_RMAX(2),					//22 22
		"A23.бепяхъ он   ", "    ", 	0,  65530,  	0, M_SHOW|M_RMAX(3)|M_PREC(3),				        //23 23
		"A24.мюоп VDC    ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//24 24
		"A25.релоеп дбхц ", "    ", -999U,    999,      0, M_SHOW|M_SIGN|M_RMAX(2),					//25 25
                "A25.релоеп лндск", "    ", -999U,    999,      0, M_SHOW|M_SIGN|M_RMAX(2),					//26 26
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
		"B0.лнлемр OTKP  ", "    ",     0,     60,      2, M_EDIT|M_KMM|M_RMAX(4),					//40
		"B1.лнлемр 3AKP  ", "    ",	0,     60,	2, M_EDIT|M_KMM|M_RMAX(4),					//41
		"B2.лнл сок гюйп ", "    ",	0,     60,	2, M_EDIT|M_KMM|M_RMAX(4),					//42
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
		"B29.бпел пюгцнмю", "    ", 	0,    100,    100, M_EDIT|M_RMAX(2)|M_PREC(1),					//68
		"B30.яйнп люйя   ", "цж  ", 	0,    500,    500, M_EDIT|M_RMAX(2)|M_PREC(1),					//69
		"B31.бпел рнплнф ", "    ", 	0,    100,    100, M_EDIT|M_RMAX(2)|M_PREC(1),					//70
		"   B32.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//71
		"B33.NO MOVE TIME", "S   ", 	3,    100,     20, M_EDIT|M_RMAX(2)|M_PREC(1),				        //72
		"B34.SEAL WAY    ", "OB  ", 	1,   1000,    100, M_EDIT|M_RMAX(3)|M_PREC(1),				        //73
		"B35.рхо рнплнфем", "    ", 	0,      2,  	0, M_SHOW|M_RMAX(4),						//74 0 БШЙК 1 ОН МЮОПЪФ 2 ОН РНЙС
		"B36.бпелъ рнпл  ", "    ", 	0,    100,  	0, M_SHOW|M_RMAX(4)|M_PREC(1),					//75
		"B37.гюдюм онкнф ", "%   ", 	0,   1000,      0, M_EDIT|M_RMAX(3)|M_PREC(1),						//76
		" B38.йо пец     ", "    ", 	0,  65535,  	30, M_EDIT|M_RMAX(4)|M_PREC(3),					//77
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
                " я12.йнп рнйю U ", "    ",     0,  65530,   4000, M_FACT|M_RMAX(4),						//102
		" я13.йнп рнйю V ", "    ",     0,  65530,   4000, M_FACT|M_RMAX(4),						//103
		" я14.йнп рнйю W ", "    ",     0,  65530,   4000, M_FACT|M_RMAX(4),						//104
		"я15.ялеы рнйю U ", "    ",     0,  65530,  32690, M_FACT|M_RMAX(4),						//105
		"я16.ялеы рнйю V ", "    ",     0,  65530,  32690, M_FACT|M_RMAX(4),						//106
		"я17.ялеы рнйю W ", "    ",     0,  65530,  32690, M_FACT|M_RMAX(4),						//107
		" я18.йнп мюоп R ", "    ",     0,  65530,   2000, M_FACT|M_RMAX(4),						//108
		" я19.йнп мюоп S ", "    ",     0,  65530,   2000, M_FACT|M_RMAX(4),						//109
		" я20.йнп мюоп T ", "    ",     0,  65530,   2000, M_FACT|M_RMAX(4),						//110
                "я21.ялеы мюоп R ", "    ",     0,  65530,  32760, M_FACT|M_RMAX(4),						//111
		"я22.ялеы мюоп S ", "    ",     0,  65530,  32760, M_FACT|M_RMAX(4),						//112
		"я23.ялеы мюоп T ", "    ",     0,  65530,  32760, M_FACT|M_RMAX(4),						//113
                " я24.йнп мюо VDC", "    ",     0,  65530,   1240, M_FACT|M_RMAX(4),						//114
                "я25.ялеы мюо VDC", "    ",     0,  65530,      0, M_FACT|M_RMAX(4),						//115
                " я26.йнп рнйю BR", "    ",     0,  65530,   1240, M_FACT|M_RMAX(4),						//116
                "я27.ялеы рнйю BR", "    ",     0,  65530,      0, M_FACT|M_RMAX(4),						//117 
                "я28.оняр.бп.яхм ", "MйC ",     0,    100,     21, M_FACT|M_RMAX(2)|M_PREC(1),				        //118
		"я29.оняр.бп.RMS ", "MйC ",     0,    100,      1, M_FACT|M_RMAX(2)|M_PREC(1),				        //119
                "я30.нрйк рщм    ", "    ",    10,     60,     41, M_FACT|M_RMAX(2),					        //120
		"я31.бшанп лным  ", "    ", 	0,      1,  	1, M_FLST|M_SADR(124),						//121
		"я32.лнд охрюмхъ ", "    ", 	0,      1,  	1, M_FLST|M_SADR(228),						//122
		"C33.рхо дюрв онк", "    ",     0,     16,      0, M_FACT|M_RMAX(1),						//123
		"   я34.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//124
		"   я35.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//125
		"   я36.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//126
		"   я37.RESERV   ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//127
		"   я38.пеб южо  ", "    ", 	0,  1,  	0, M_FACT|M_RMAX(4),						//128
		"   я39.пеб жюо  ", "    ", 	0,  1,  	0, M_FACT|M_RMAX(4),						//129
                "я40.южорнв1 4   ", "    ", 	0,  65535,   6700, M_FACT|M_RMAX(4),						//130
		"я41.южорнв2 4.5 ", "    ", 	0,  65535,   7300, M_FACT|M_RMAX(4),						//131
		"я42.южорнв3 5   ", "    ", 	0,  65535,   7900, M_FACT|M_RMAX(4),						//132
		"я43.южорнв4 5.5 ", "    ", 	0,  65535,   8500, M_FACT|M_RMAX(4),						//133
		"я44.южорнв5 6   ", "    ", 	0,  65535,   9100, M_FACT|M_RMAX(4),						//134
		"я45.южорнв6 6.5 ", "    ", 	0,  65535,   9700, M_FACT|M_RMAX(4),						//135
                "я46.южорнв7 7   ", "    ", 	0,  65535,  10300, M_FACT|M_RMAX(4),						//136
		"я47.южорнв8 7.5 ", "    ", 	0,  65535,  10900, M_FACT|M_RMAX(4),						//137
		"я48.южорнв9 8   ", "    ", 	0,  65535,  11490, M_FACT|M_RMAX(4),						//138
		"я49.южорнв10 8.5", "    ", 	0,  65535,  12080, M_FACT|M_RMAX(4),						//139
		"я50.южорнв11 9  ", "    ", 	0,  65535,  12680, M_FACT|M_RMAX(4),						//140
		"я51.южорнв12 9.5", "    ", 	0,  65535,  13290, M_FACT|M_RMAX(4),						//141
		"я52.южорнв13 10 ", "    ", 	0,  65535,  13880, M_FACT|M_RMAX(4),						//142
		"я53.южорнв14 105", "    ", 	0,  65535,  14480, M_FACT|M_RMAX(4),						//143
		"я54.южорнв15 11 ", "    ", 	0,  65535,  15070, M_FACT|M_RMAX(4),						//144
		"я55.южорнв16 115", "    ", 	0,  65535,  15680, M_FACT|M_RMAX(4),						//145
                "я56.южорнв17 12 ", "    ", 	0,  65535,  16270, M_FACT|M_RMAX(4),						//146
                "я57.южорнв18 125", "    ", 	0,  65535,  16870, M_FACT|M_RMAX(4),						//147
                "я58.южорнв19 13 ", "    ", 	0,  65535,  17460, M_FACT|M_RMAX(4),						//148
                "я59.южорнв20 135", "    ", 	0,  65535,  18070, M_FACT|M_RMAX(4),						//149
                "я60.южорнв21 14 ", "    ", 	0,  65535,  18660, M_FACT|M_RMAX(4),						//150
                "я61.южорнв22 145", "    ", 	0,  65535,  19260, M_FACT|M_RMAX(4),						//151
		"я62.южорнв23 15 ", "    ", 	0,  65535,  19860, M_FACT|M_RMAX(4),						//152
                "я63.южорнв24 155", "    ", 	0,  65535,  20460, M_FACT|M_RMAX(4),						//153
		"я64.южорнв25 16 ", "    ", 	0,  65535,  21050, M_FACT|M_RMAX(4),						//154
		"я65.южорнв26 165", "    ", 	0,  65535,  21650, M_FACT|M_RMAX(4),						//155
                "я66.южорнв27 17 ", "    ", 	0,  65535,  22250, M_FACT|M_RMAX(4),						//156
		"я67.южорнв28 175", "    ", 	0,  65535,  22850, M_FACT|M_RMAX(4),						//157
		"я68.южорнв29 18 ", "    ", 	0,  65535,  23450, M_FACT|M_RMAX(4),						//158
		"я69.южорнв30 185", "    ", 	0,  65535,  24045, M_FACT|M_RMAX(4),						//159
		"я70.южорнв31 19 ", "    ", 	0,  65535,  24650, M_FACT|M_RMAX(4),						//160
		"я71.южорнв32 195", "    ", 	0,  65535,  25240, M_FACT|M_RMAX(4),						//161
		"я72.южорнв33 20 ", "    ", 	0,  65535,  25840, M_FACT|M_RMAX(4),						//162
		"я73.жюпрнв1 4   ", "    ", 	0,  65535,   6410, M_FACT|M_RMAX(4),						//163
                "я74.жюпрнв2 4.5 ", "    ", 	0,  65535,   7218, M_FACT|M_RMAX(4),						//164
		"я75.жюпрнв3 5   ", "    ", 	0,  65535,   8020, M_FACT|M_RMAX(4),						//165
                "я76.жюпрнв4 5.5 ", "    ", 	0,  65535,   8820, M_FACT|M_RMAX(4),						//166
                "я77.жюпрнв5 6   ", "    ", 	0,  65535,   9620, M_FACT|M_RMAX(4),						//167
                "я78.жюпрнв6 6.5 ", "    ", 	0,  65535,  10420, M_FACT|M_RMAX(4),						//168
                "я79.жюпрнв7 7   ", "    ", 	0,  65535,  11220, M_FACT|M_RMAX(4),						//169
                "я80.жюпрнв8 7.5 ", "    ", 	0,  65535,  12020, M_FACT|M_RMAX(4),						//170
                "я81.жюпрнв9 8   ", "    ", 	0,  65535,  12820, M_FACT|M_RMAX(4),						//171
		"я82.жюпрнв10 8.5", "    ", 	0,  65535,  13620, M_FACT|M_RMAX(4),						//172
		"я83.жюпрнв11 9  ", "    ", 	0,  65535,  14420, M_FACT|M_RMAX(4),						//173
		"я84.жюпрнв12 9.5", "    ", 	0,  65535,  15220, M_FACT|M_RMAX(4),						//174
		"я85.жюпрнв13 10 ", "    ", 	0,  65535,  16030, M_FACT|M_RMAX(4),						//175
                "я86.жюпрнв14 105", "    ", 	0,  65535,  16830, M_FACT|M_RMAX(4),						//176
		"я87.жюпрнв15 11 ", "    ", 	0,  65535,  17630, M_FACT|M_RMAX(4),						//177
		"я88.жюпрнв16 115", "    ", 	0,  65535,  18430, M_FACT|M_RMAX(4),						//178
		"я89.жюпрнв17 12 ", "    ", 	0,  65535,  19230, M_FACT|M_RMAX(4),						//179
		"я90.жюпрнв18 125", "    ", 	0,  65535,  20040, M_FACT|M_RMAX(4),						//180
		"я91.жюпрнв19 13 ", "    ", 	0,  65535,  20840, M_FACT|M_RMAX(4),						//181
		"я92.жюпрнв20 135", "    ", 	0,  65535,  21640, M_FACT|M_RMAX(4),						//182
		"я93.жюпрнв21 14 ", "    ", 	0,  65535,  22440, M_FACT|M_RMAX(4),						//183
		"я94.жюпрнв22 145", "    ", 	0,  65535,  23240, M_FACT|M_RMAX(4),						//184
		"я95.жюпрнв23 15 ", "    ", 	0,  65535,  24040, M_FACT|M_RMAX(4),						//185
                "я96.жюпрнв24 155", "    ", 	0,  65535,  24840, M_FACT|M_RMAX(4),						//186
                "я97.жюпрнв25 16 ", "    ", 	0,  65535,  25640, M_FACT|M_RMAX(4),						//187
                "я98.жюпрнв26 165", "    ", 	0,  65535,  26440, M_FACT|M_RMAX(4),						//188
                "я99.жюпрнв27 17 ", "    ", 	0,  65535,  27250, M_FACT|M_RMAX(4),						//189
                "я100.жюпрнв28175", "    ", 	0,  65535,  28050, M_FACT|M_RMAX(4),						//190
                "я101.жюпрнв2918 ", "    ", 	0,  65535,  28850, M_FACT|M_RMAX(4),						//191
		"я102.жюпрнв30185", "    ", 	0,  65535,  29650, M_FACT|M_RMAX(4),						//192
		"я103.жюпрнв3119 ", "    ", 	0,  65535,  30450, M_FACT|M_RMAX(4),						//193
		"я104.жюпрнв32195", "    ", 	0,  65535,  31250, M_FACT|M_RMAX(4),						//194
		"я105.жюпрнв33 20", "    ", 	0,  65535,  32060, M_FACT|M_RMAX(4),						//195
                "   я106.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//196
		"   я107.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//197
		"   я108.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//198
		"   я109.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//199
		"C110.гюы люйя U ", "    ", 	 0,      1,  	 1, M_FLST|M_SADR(154),						//200
		"я111.спнб люйя U", "B   ",    220,    335,    265, M_FACT|M_RMAX(2),						//201
		"я112.бпел люйя U", "C   ",      1,    250,    220, M_FACT|M_RMAX(2)|M_PREC(1),				        //202		
                "C113.гюы лхм U  ", "    ", 	 0,      1,  	 1, M_FLST|M_SADR(154),						//203
		"я114.спнб лхм U ", "B   ",     90,    220,    115, M_FACT|M_RMAX(2),						//204
		"я115.бпел лхм U ", "C   ",      1,    250,    220, M_FACT|M_RMAX(2)|M_PREC(1),				        //205              
                "C116.гюы слем U ", "    ", 	 0,      1,  	 1, M_FLST|M_SADR(154),						//206
		"я117.спнб слем U", "б   ",      0,    110,     60, M_FACT|M_RMAX(2),						//207
                "я118.бпел слем U", "C   ",      1,    250,      1, M_FACT|M_RMAX(2)|M_PREC(1),				        //208          
                "C119.гюы слем I ", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //209
		"C120.спнб слем I", "%   ",     20,    500,    150, M_FACT|M_RMAX(2)|M_PREC(1),				        //210
		"C121.бпел слем I", "C   ",      1,    100,      7, M_FACT|M_RMAX(1)|M_PREC(1),				        //211    
                "C122.гюы нр йг  ", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //212
		"я123.бепу сп йг ", "    ",      0,  65530,  45000, M_FACT|M_RMAX(4),						//213
		"я124.мхфм сп йг ", "    ",      0,  65530,  20000, M_FACT|M_RMAX(4),						//214              
                "C125.гюы   оепец", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //215
		"я126.сп    оепец", "цпюд",      0,    150,     80, M_FACT|M_RMAX(2),						//216
		"я127.бпел  оепец", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //217           
                "C128.гюы  оепену", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //218
		"я129.сп   оепену", "цпюд",  -100U,     20,      0, M_FACT|M_SIGN|M_RMAX(2),					//219
		"я130.бпел оепену", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //220		
                "C131.оепец дбхц ", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //221
		"я132.сп   оепец ", "цпюд",      0,    200,     60, M_FACT|M_RMAX(2),					        //222
		"я133.бпел оепец ", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //223		
		"C134.оепец лндск", "    ",      0,      1,      1, M_FLST|M_SADR(154),					        //224
		"я135.сп   оепецп", "цпюд",      0,    200,     60, M_FACT|M_RMAX(2),					        //225
		"я136.бпел оепецп", "C   ",      1,    250,     50, M_FACT|M_RMAX(2)|M_PREC(1),				        //226		
                "   я137.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//227
                "   я138.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//228
                "   я139.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//229
                "   я140.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//230
                "   я141.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//231
		"   я142.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//232
		"   я143.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//233
		"   я144.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//234
		"   я145.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//235
                "   я146.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//236
		"   я147.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//237
		"   я148.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//238
		"   я149.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//239
		"   я150.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//240
		"   я151.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//241
		"   я152.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//242
		"   я153.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//243
		"   я154.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//244
		"   я155.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//245
                "   я156.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//246
                "   я157.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//247
                "   я158.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//248
                "   я159.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//249
                "   я160.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//250
                "   я161.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//251
		"   я162.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//252
		"   я163.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//253
		"   я164.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//254
		"   я165.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//255
                "   я166.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//256
		"   я167.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//257
		"   я168.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//258
		"   я169.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//259
		"   я170.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//260
		"   я171.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//261
		"   я172.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//262
		"   я173.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//263
		"   я174.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//264
		"   я175.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//265
                "   я176.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//266
                "   я177.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//267
                "   я178.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//268
                "   я179.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//269
                "   я180.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//270
                "   я181.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//271
		"   я182.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//272
		"   я183.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//273
		"   я184.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//274
		"   я185.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//275
                "   я186.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//276
		"   я187.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//277
		"   я188.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//278
		"   я189.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//279
		"   я190.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//280
		"   я191.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//281
		"   я192.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//282
		"   я193.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//283
		"   я194.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//284
		"   я195.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//285
                "   я196.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//286
                "   я197.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//287
                "   я198.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//288
                "   я199.RESERV  ", "    ", 	0,  65535,  	0, M_SHOW|M_RMAX(4),						//289
              
                
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
		"131. гнмю ялеыем", "    ",      0,  65530,      0, MT_DEC|M_RMAX(4),						//91 401
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
	"    гюохяэ      ",	// 0
//----------------
	"  ме нопедекем  ",	// 1
	"  нярюмнбкем    ",	// 2
	"     юбюпхъ     ",	// 3
	" хдер гюйпшрхе  ",	// 4
	" хдер нрйпшрхе  ",	// 5
	"   хдер реяр    ",	// 6
	"    гюйпшрн     ",	// 7
	"    нрйпшрн     ",	// 8
	"     лстрю      ",	// 9
	" леярмне сопюбк ",	// 10
	"  бйкчвем рем   ",	// 11
	"BLUETOOTH бйк   ",	// 12
	"юбюпхъ мю акнйе ",	// 13
	"  юбюпхъ мю ря  ",	// 14
	"опнцпюллхпнбюмхе",	// 15
	"мехяопюбм мю акй",	// 16
	"мехяопюбм мю ря ",	// 17
//--------------------------
	"   мер юбюпхи   ",	// 18
	"е01 мер дбхфемхъ",	// 19
	"е02 веп тюг дбхц",	// 20
	"е03 ме гюд гюйп ",	// 21
	"е03 ме гюд нрйп ",	// 22
	"е03 мер йюкхап  ",	// 23
	"е04 сок ме дняр ",	// 24
	"е05 лстрю       ",	// 25
	"е06 мехяоп лс/дс",	// 26
	"е07 ньхайю соо  ",	// 27
	"е08 пеф онф йнр ",	// 28
	"     пегепб     ",	// 29
	"     пегепб     ",	// 30
	"     пегепб     ",	// 31
	"     пегепб     ",	// 32
	"     пегепб     ",	// 33
	"     пегепб     ",	// 34
//--------------------------
	"   мер юбюпхи   ",	// 35
	"е11 лхм мюоп т R",	// 36
	"е11 лхм мюоп т S",	// 37
	"е11 лхм мюоп т T",	// 38
	"е12 люйя мюо т R",	// 39
	"е12 люйя мюо т S",	// 40
	"е12 люйя мюо т T",	// 41
	"меноп вепед тюг ",	// 42
	"е13 меаюк/нап U ",	// 43
	"е14 слем мюо т R",	// 44
	"е14 слем мюо т S",	// 45
	"е14 слем мюо т T",	// 46
	"е15 мюоп т R 47%",	// 47
	"е15 мюоп т S 47%",	// 48
	"е15 мюоп т T 47%",	// 49
	"  веп тюг яерх  ",	// 50
	"     пегепб     ",	// 51
//--------------------------
	"   мер юбюпхи   ",	// 52
	"е21 напшб тюгш U",	// 53
	"е21 напшб тюгш V",	// 54
	"е21 напшб тюгш W",	// 55
	"е22 бп-рнй гюыхр",	// 56
	"     пегепб     ",	// 57
	"е23 йг тюгш U   ",	// 58
	"е23 йг тюгш V   ",	// 59
	"е23 йг тюгш W   ",	// 60
	"     пегепб     ",	// 61
	"     пегепб     ",	// 62
	" лхмхлюкэм рнй  ",	// 63
	" меаюкюмя рнйю  ",	// 64
	"     пегепб     ",	// 65
	"     пегепб     ",	// 66
	"     пегепб     ",	// 67
	"     пегепб     ",	// 68
	//---------------------------------------------------------------------------
	"   мер юбюпхи   ",	// 69
	"е31 яани д онкнф",	// 70
	"е32 яани оюл 1  ",	// 71
	"е32 яани оюл 2  ",	// 72
	"е33 яани вюянб  ",	// 73
	"е34 яани д рело ",	// 74
	"е35 оепецп айо  ",	// 75
	"е36 оепенук айо ",	// 76
	"е37 оепецп айщо ",	// 77
	"е38 оепенук айщо",	// 78
	"яани жюо        ",	// 79
	"бшйкчв айщо     ",	// 80
	"е31 мер яб я айо",	// 81
	"е41 аюрюпеийю   ",	// 82
	"е42 ме гюося акч",	// 83
	"е40 рхо айо     ",	// 84
	"     пегепб     ",	// 85
	//---------------------------------------------------------------------------
	"  мер яхцмюкнб  ",	// 86
	"  бунд нрйпшрэ  ",	// 87
	"  бунд гюйпшрэ  ",	// 88
	"  бунд ярно     ",	// 89
	"  бунд дс       ",	// 90
	"  бунд деакнй   ",	// 91
	"     пегепб     ",	// 92
	"     пегепб     ",	// 93
	"     пегепб     ",	// 94
	"     пегепб     ",	// 95
	"     пегепб     ",	// 96
	"     пегепб     ",	// 97
	"     пегепб     ",	// 98
	"     пегепб     ",	// 99
	"     пегепб     ",	// 100
	"     пегепб     ",	// 101
	"     пегепб     ",	// 102
	//---------------------------------------------------------------------------
	"  мер яхцмюкнб  ",	// 103
	"    нрйпшрн     ",	// 104
	"    гюйпшрн     ",	// 105
	"  нрйпшбюеряъ   ",	// 106
	"  гюйпшбюеряъ   ",	// 107
	"  мехяопюбмнярэ ",	// 108
	"     лстрю      ",	// 109
	"    юбюпхъ      ",	// 110
	"      дс        ",	// 111
	"     пегепб     ",	// 112
	"     пегепб     ",	// 113
	"     пегепб     ",	// 114
	"     пегепб     ",	// 115
	"     пегепб     ",	// 116
	"     пегепб     ",	// 117
	"     пегепб     ",	// 118
	"     пегепб     ",	// 119
	//---------------------------------------------------------------------------
	" мер йюкхапнбйх ",	// 120
	" гюдюмн гюйпшрн ",	// 121
	" гюдюмн нрйпшрн ",	// 122
	"йюкхап бшонкмемю",	// 123
	//---------------------------------------------------------------------------
	"    1.5 йбР     ",	// 124
	"    3   йБР     ",	// 125
	//---------------------------------------------------------------------------
	"  ме нопедекем  ",	// 126
	" аег сокнрмемхъ ",	// 127
	"сокнрмемхе гюйп ",	// 128
	"сокнрмемхе нрйп ",	// 129
	"сок гюйп х нрйп ",	// 130
	//---------------------------------------------------------------------------
	"   мнплюкэмши   ",	// 131
	"    напюрмши    ",	// 132
	//---------------------------------------------------------------------------
	"деиярб гмювемхъ ",	// 133
	"опнжемрш нр мнл ",	// 134
	//---------------------------------------------------------------------------
	" пефхл нр бяецн ",	// 135
	"  бшанп пефхлю  ",	// 136
	"    пефхл лс    ",	// 137
	"    пефхл дс    ",	// 138
	//---------------------------------------------------------------------------
	" бяе хмрептеияш ",	// 139
	"рнкэйн дхяйперм ",	// 140
	"рнкэйн онякеднб ",	// 141
        "рнкэйн жюо      ",     // 142
	//---------------------------------------------------------------------------
	"   бшйкчвемю    ",	// 143
	"    бйкчвемю    ",	// 144
	//---------------------------------------------------------------------------
	"      24б       ",	// 145
	"      220б      ",	// 146
	//---------------------------------------------------------------------------
	"    2400 анд    ",	// 147
	"    4800 анд    ",	// 148
	"    9600 анд    ",	// 149
	"   19200 анд    ",	// 150
	"   38400 анд    ",	// 151
	"   57600 анд    ",	// 152
	"   115200 анд   ",	// 153
	//---------------------------------------------------------------------------
	"   бшйкчвемю    ",	// 154
	"хмдхй х нярюмнб ",	// 155
	//---------------------------------------------------------------------------
	"  мер йнлюмдш   ",	// 156
	"     гюдюрэ     ",	// 157
	"    яапняхрэ    ",	// 158
	//---------------------------------------------------------------------------
	"  мер йнлюмдш   ",	// 159
	"    яапняхрэ    ",	// 160
	//---------------------------------------------------------------------------
	"  мер йнлюмдш   ",	// 161
	"      ярно      ",	// 162
	"    гюйпшрэ     ",	// 163
	"    нрйпшрэ     ",	// 164
	"  гюйпшрэ реяр  ",	// 165
	"  нрйпшрэ реяр  ",	// 166
	"     пегепб     ",	// 167
	"     пегепб     ",	// 168
	"     пегепб     ",	// 169
	"     пегепб     ",	// 170
	"     пегепб     ",	// 171
	"     пегепб     ",	// 172
	"     пегепб     ",	// 173
	"     пегепб     ",	// 174
	"     пегепб     ",	// 175
	"     пегепб     ",	// 176
	"     пегепб     ",	// 177
	"     пегепб     ",	// 178
	"     пегепб     ",	// 179
	"     пегепб     ",	// 180
	//---------------------------------------------------------------------------
	" ме нопедекемн  ",	// 181
	"напюрм онякеднб ",	// 182
	"опълюъ онякеднб ",	// 183
	//---------------------------------------------------------------------------
	"    нярюмнб     ",	// 184
	" юбрнлюрхвеяйхи ",	// 185
	"  хцмнпхпнбюмхе ",     // 186
	//---------------------------------------------------------------------------
	"    бшйкчвем    ",	// 187
	"    бйкчвем     ",	// 188
	//---------------------------------------------------------------------------
	"мнпл пюгнлймсрши",	// 189
	" мнпл гюлймсрши ",	// 190
	//---------------------------------------------------------------------------
	" ярюрся пюанрш  ",	// 191
	" дхюцм опнжеяяю ",	// 192
	" дхюцмняр яерх  ",	// 193
	" дхюцм мюцпсгйх ",	// 194
	" дхюцм сярп-бю  ",	// 195
	"  яняр бунднб   ",	// 196
	"  яняр бшунднб  ",	// 197
	"     пегепб     ",	// 198
	"     лнлемр     ",	// 199
	"    яйнпнярэ    ",	// 200
	"  япедмее мюоп  ",	// 201
	"  япедмхи рнй   ",	// 202
	" сцнк мюцпсгйх  ",	// 203
	"     пегепб     ",	// 204
	"     пегепб     ",	// 205
	"     пегепб     ",	// 206
	"     пегепб     ",	// 207
	"     пегепб     ",	// 208
	"     пегепб     ",	// 209
	"     пегепб     ",	// 210
	"     пегепб     ",	// 211
	"     пегепб     ",	// 212
	"     пегепб     ",	// 213
	"     пегепб     ",	// 214
	"     пегепб     ",	// 215
	"     пегепб     ",	// 216
	"     пегепб     ",	// 217
	"     пегепб     ",	// 218
	"     пегепб     ",	// 219
	"     пегепб     ",	// 220
	//----------------------------------------------
	"  он сярюбйюл   ",	// 221
	"он сяр я гюдепфй",	// 222
	" бяецдю бйк     ",	// 223
	" псвмне соп     ",	// 224
	//-----------------------------------------------
	"  аег опнбепйх  ",	// 225
	" мю мевермнярэ  ",	// 226
	"  мю вермнярэ   ",	// 227
   //-----------------------------------------------
        "380 б           ",	// 228
	"220 б           "	// 229
};

const struct MENU_STRING AddStr[] = {
	"сярпниярб гюмърн",	// 0
	"    йнлюмдю     ",	// 1
	"    нрлемемю    ",	// 2
	"  о 999.99%     ",	// 3
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
	"1111111111111111",	// 22	9	// п═Б─■п═р▒п═б╣п║п┐п║п┼ п═п├п║Б─╧п═п├п═я∙п═р▒п║п▐п║Б─ п║п┐п║п▐ п║Б─ п═б╣п═я■п║я⌠п║Б─╟п═я▒п═б╣ п═я∙п═б╠п═я∙п║п┌п═я∙п║Б─ п║Б─╧
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

// п═я п═б╟п═я■п║п┌п═я∙п║п┐ п║Б─║п║Б─ п═б╣п═п┘п═я▒п║п▐ п║п┐п║Б─ п║п┌п═я∙п═я■п═я▒ п═я▒п═б╥ п║Б─·п═б╩п═б╣п║Б┌╛. п═я п═б╟п║п┐п║п┐п═я▒п═п├ add_strings
#define ReadAddStr(Str, Index) \
	PFUNC_blkRead((int16*)&AddStr[Index], (int16*)Str, 16)

// п═я п═б╟п═я■п║п┌п═я∙п║п┐ п║Б─║п║Б─ п═б╣п═п┘п═я▒п║п▐ п║п┐п║Б─ п║п┌п═я∙п═я■п═я▒ п═я▒п═б╥ п║Б─·п═б╩п═б╣п║Б┌╛. п═я п═б╟п║п┐п║п┐п═я▒п═п├ add_strings
#define ReadValuesStr(Str, Index) \
	PFUNC_blkRead((int16*)&values[Index], (int16*)Str, 16)

extern const struct MENU_GROUP  groups[];
extern const struct MENU_PARAM  params[];
extern const struct MENU_STRING values[];
extern const struct MENU_STRING AddStr[];
extern const struct ADDR_PARAM_ATS48  addr_params_altistar48[];


#endif




