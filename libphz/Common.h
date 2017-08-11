#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <time.h>

//数据类型定义
typedef          char        _tint8;     //有符号 1 字节
typedef unsigned char        _uint8;     //无符号 1 字节
typedef short                _tint16;    //有符号 2 字节
typedef unsigned short       _uint16;    //无符号 2 字节
typedef int                  _tint32;    //有符号 4 字节
typedef unsigned int         _uint32;    //无符号 4 字节
typedef			 long long   _tint64;    //有符号 8 字节
typedef unsigned long long   _uint64;    //无符号 8 字节

//数值定义
#define GAME_PLAYER					4									//游戏人数
#define MAX_WEAVE					7									//最大组合
#define MAX_INDEX					20									//最大索引
#define MAX_COUNT					21									//最大数目
#define MAX_COUNT_4					15									//4人玩最大数目
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码
#define MAX_CARDS					80									//最大牌数
#define INVALID_CHAIR  				-1	 								//无效座位号
#define INVALID_CARD				0xFF								//无效牌

//动作定义
#define ACK_NULL					0x00								//空
#define ACK_TI						0x01								//提
#define ACK_PAO						0x02								//跑
#define ACK_WEI						0x04								//偎
#define ACK_CHI						0x08								//吃
#define ACK_CHI_EX					0x10								//吃
#define ACK_PENG					0x20								//碰
#define ACK_CHIHU					0x40								//胡

//显示定义
#define ACK_KAN						0x50								//坎
#define ACK_JIANG					0x60								//将

//吃牌类型
#define CK_NULL						0x00								//无效类型
#define CK_XXD						0x01								//小小大搭
#define CK_XDD						0x02								//小大大搭
#define CK_EQS						0x04								//二七十吃
#define CK_LEFT						0x10								//靠左对齐
#define CK_CENTER					0x20								//居中对齐
#define CK_RIGHT					0x40								//靠右对齐

#define CardNum(c) ((c) & MASK_VALUE)
#define CardType(c) (((c) & MASK_COLOR)>>4)
#define CardValue(t,n) ( (t<<4) | n )


//交换两个整形数，不能传表达式，同一个变量 ns=not same
#define ns_swap(a,b) ((a)^=(b),(b)^=(a),(a)^=(b))

//三个整型数排序
#define sort3n(a)	do{	\
	((a)[0] > (a)[1]) && (ns_swap((a)[0], (a)[1]));\
	((a)[0] > (a)[2]) && (ns_swap((a)[0], (a)[2]));\
	((a)[1] > (a)[2]) && (ns_swap((a)[1], (a)[2]));}while(0)

//////////////////////////////////////////////////////////////////////////

//胡番类型
enum HuFanType
{
	MT_NULL = 0,								//空
	MT_MAO_HU = 1,								//毛胡
	MT_ZI_MO = 2,								//自摸
	MT_HONG_HU = 3,								//大红胡
	MT_HEI_HU = 4,								//黑胡
	MT_TIAN_HU = 5,								//天胡
	MT_DI_HU = 6,								//地胡
	MT_DIAN_HU = 7,								//点胡
	MT_XIAO_HONG_HU = 8							//小红胡
};


//组合子项
struct tagWeaveItem
{
	_uint8							cbWeaveKind;						//组合类型
	_uint8							cbCardCount;						//扑克数目
	_uint8							cbCenterCard;						//中心扑克
	_uint8							cbOutCard;							//打出的牌
	_uint8							cbCardList[4];						//扑克列表
	tagWeaveItem()
	{
		cbWeaveKind = 0;
		cbCardCount = 0;
		cbCenterCard = 0;
		cbOutCard = 0;
		memset(cbCardList, 0, sizeof(cbCardList));
	}
};

//吃牌信息
struct tagChiCardInfo
{
	_uint8							cbChiKind;							//吃牌类型
	_uint8							cbCenterCard;						//中心扑克
	_uint8							cbResultCount;						//结果数目
	_uint8							cbCardData[3][3];					//吃牌组合
	tagChiCardInfo()
	{
		cbChiKind = 0;
		cbCenterCard = 0;
		cbResultCount = 0;
		memset(cbCardData, 0, sizeof(cbCardData));
	}
};

//吃牌第三层
struct tagThirdChiInfo
{
	_uint8							cbChiKind;							//吃牌类型
	_uint8							cbCenterCard;						//中心牌
	_uint8							cbCardData[3];						//吃牌
	tagThirdChiInfo()
	{
		cbChiKind = 0;
		cbCenterCard = 0;
		memset(cbCardData, 0, sizeof(cbCardData));
	}
};

//吃牌第二层
struct tagSecondChiInfo
{
	_uint8							cbChiKind;							//吃牌类型
	_uint8							cbCenterCard;						//中心牌
	_uint8							cbCardData[3];						//吃牌
	_uint8							cbThirdCount;						//剩余组合数
	tagThirdChiInfo					ThirdData[6];
	tagSecondChiInfo()
	{
		cbChiKind = 0;
		cbCenterCard = 0;
		memset(cbCardData, 0, sizeof(cbCardData));
		cbThirdCount = 0;
		memset(ThirdData, 0, sizeof(ThirdData));
	}
};

//新吃牌信息
struct tagNewChiCardInfo
{
	_uint8							cbChiKind;							//吃牌类型
	_uint8							cbCenterCard;						//中心牌
	_uint8							cbCardData[3];						//吃牌
	_uint8							cbSecondCount;						//剩余组合数
	tagSecondChiInfo				SecondData[6];
	tagNewChiCardInfo()
	{
		cbChiKind = 0;
		cbCenterCard = 0;
		memset(cbCardData, 0, sizeof(cbCardData));
		cbSecondCount = 0;
		memset(SecondData, 0, sizeof(SecondData));
	}
};

//分析子项
struct tagAnalyseItem
{
	_uint8							cbCardEye;							//牌眼扑克
	_uint8							cbHuXiCount;						//胡息数目
	_uint8							cbWeaveCount;						//组合数目
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			//组合扑克
	tagAnalyseItem()
	{
		cbCardEye = 0;
		cbHuXiCount = 0;
		cbWeaveCount = 0;
		memset(WeaveItemArray, 0, sizeof(WeaveItemArray));
	}
};

//胡牌信息
struct tagHuCardInfo
{
	_uint8							cbCardEye;							//牌眼扑克
	_uint8							cbHuXiCount;						//胡息数目
	_uint8							cbWeaveCount;						//组合数目
	_uint8							cbFinalHuXiCount;
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			//组合扑克
	tagHuCardInfo()
	{
		cbCardEye = 0;
		cbHuXiCount = 0;
		cbWeaveCount = 0;
		memset(WeaveItemArray, 0, sizeof(WeaveItemArray));
	}
};

//胡番信息
struct tagHuFanInfo
{
	_uint32							wHuFanType;							//胡番类型
	_uint32							wHuFanNum;							//胡番数	
	_uint32							wHuFanXi;
	tagHuFanInfo()
	{
		wHuFanType = 0;
		wHuFanNum = 0;
		wHuFanXi = 0;
	}
};

//手牌组合
struct tagCardsWeave
{
	_uint8							cbCardCount;						//组合牌数
	_uint8							cbCardList[4];						//组合牌
	tagCardsWeave()
	{
		cbCardCount = 0;
		memset(cbCardList, 0, sizeof(cbCardList));
	}

};

enum PlayType
{
	PLAY_TYPE					= 1000,			//玩法
	BASE_HU_XI					= 1003,			//起胡胡息
	NEI_NIAO_COUNT				= 1049,			//内围打鸟个数
	PIAO_SCORE					= 1053,			//飘的分数
	XIAO_PAI_OPTION				= 1104,			//明消暗消选项
	IS_MAOHU					= 1105,			//明消暗消选项
	HU_XI_TO_TUNS				= 1107,			//胡息换算
	IS_PIAO						= 1108,			//是否能飘	
	GAME_PLAYERS				= 1109,			//游戏人数
	CHOOSE_BASE_HUXI			= 1110,			//起胡胡息选项
	ZIMO_FAN					= 1137,			//自摸翻倍
	HONGHEIDIAN					= 1138,			//红黑点
	MUST_HU						= 1139,			//必胡
};

//玩法ID
enum PlayerID
{
	CHANG_DE_QUANG_MING_TANG = 1000001, //常德全名堂
	CHANG_DE_HONG_HEI_DIAN = 1000002,   //常德红黑点
	CHANG_SHA_PAO_HU_ZI = 1000003,		//长沙跑胡子
	SHAO_YANG_ZI_PAI = 1000004,			//邵阳字牌
	HUAI_HUA_HONG_GUAI_WAN = 1000005,   //怀化红拐弯
	ZI_DING_YI_WAN_FA = 1000006,		//自定义玩法
	CHEN_ZHOU_ZI_PAI = 1000013,			//郴州字牌

	NEI_NIAO_COUNT_1 = 1049001, //内围打鸟1
	NEI_NIAO_COUNT_2 = 1049002, //内围打鸟2
	NEI_NIAO_COUNT_4 = 1049003, //内围打鸟4
	NEI_NIAO_COUNT_8 = 1049004, //内围打鸟8

	PIAO_SCORE_10 = 1053001, //十分
	PIAO_SCORE_20 = 1053002, //二十分
	PIAO_SCORE_30 = 1053003, //三十分
	PIAO_SCORE_50 = 1053004, //五十分

	XIAO_PAI_OPTION_MING = 1104001, //明消选项
	XIAO_PAI_OPTION_AN = 1104002,   //暗消选项

	WITH_MAOHU = 1105001,	//带毛胡
	WITHOUT_MAOHU = 1105002, //不带毛胡

	HU_XI_TO_TUNS_3 = 1107001, //胡息换算3
	HU_XI_TO_TUNS_1 = 1107002, //胡息换算1

	IS_PIAO_YES = 1108001, //飘
	IS_PIAO_NO = 1108002,  //不飘

	GAME_PLAYERS_3 = 1109001, //游戏人数3
	GAME_PLAYERS_4 = 1109002, //游戏人数4

	CHOOSE_BASE_HUXI_3 = 1110001, //起胡胡息3
	CHOOSE_BASE_HUXI_6 = 1110002, //起胡胡息6
	CHOOSE_BASE_HUXI_9 = 1110003, //起胡胡息9

	PID_ZIMO_FAN = 1137001,
	PID_ZIMO_NO_FAN = 1137002,

	PID_IS_HONGHEIDIAN = 1138001,
	PID_NO_HONGHEIDIAN = 1138002,

	PID_NO_MUST_HU = 1139001,
	PID_MUST_PAO_HU = 1139002,
	PID_MUST_HU = 1139003,

	//人数=1157（宜章字牌）,//	人数=1157（宜章字牌）
	PID_YZ_PLAYER_2 = 1157001, //	2人=1157001
	PID_YZ_PLAYER_3 = 1157002, //	3人=1157002
	PID_YZ_PLAYER_4 = 1157003, //	4人=1157003

	//胡牌=1158,//	胡牌=1158
	PID_YZ_BASE_HU_XI_3 = 1158001,  //	6胡息起胡=1158001
	PID_YZ_BASE_HU_XI_6 = 1158002,  //	6胡息起胡=1158001
	PID_YZ_BASE_HU_XI_9 = 1158003,  //	9胡息起胡=1158002
	PID_YZ_BASE_HU_XI_15 = 1158004, //	15胡息起胡=1158003

	//消=1159
	PID_YZ_XIAO_PAI_MING = 1159001,
	PID_YZ_XIAO_PAI_AN = 1159002,

	//息囤=1160,//	息囤=1160
	PID_YZ_HU_XI_TO_TUN_3 = 1160001, //	3息1囤=1160001
	PID_YZ_HU_XI_TO_TUN_1 = 1160002, //	1息1囤=1160002

	//飘 = 1161,	//	飘=1161
	PID_YZ_PIAO_YES = 1161001, //	是=1161001
	PID_YZ_PIAO_NO = 1161002,  //	否=1161002

	//自摸=1162,//	自摸=1162
	PID_YZ_ZI_MO_FAN_YES = 1162001, //	翻倍=1162001
	PID_YZ_ZI_MO_NOT_FAN = 1162002, //	无=1162002

	//红黑点=1163,//	红黑点=1163
	PID_YZ_HHD_YES = 1163001, //	是=1163001
	PID_YZ_HHD_NO = 1163002,  //	否=1163002

	//必胡=1164,//	必胡=1164
	PID_YZ_MUST_HU_NO = 1164001,  //	无要求=1164001
	PID_YZ_MUST_PAO_HU = 1164002, //	点炮必胡=1164002
	PID_YZ_MUST_HU_YES = 1164003, //	有胡必胡=1164003

	//人数=1165（永兴冒胡子）,//	人数=1165（永兴冒胡子）
	PID_YZ_MAO_HU_YES = 1165001, //	2人=1165001
	PID_YZ_MAO_HU_NO = 1165002, //	3人=1165002

	//天地胡=1166,//	天地胡=1166
	PID_YZ_TD_HU_YES = 1166001, //	是=1166001
	PID_YZ_TD_HU_NO = 1166002,  //	否=1166002

	PID_3_REN_15_ZHANG = 1210001, //	庄15,闲14张=1210001
	PID_3_REN_21_ZHANG = 1210002,  //	庄21,闲20张=1210002


	/*
	PID_YX_PLAYER_4 = 1165003, //	4人=1165003

	//胡牌=1166,//	胡牌=1166
	PID_YX_BASE_HU_XI_3 = 1166001, //	3胡息起胡=1166001

	//消=1167
	PID_YX_XIAO_PAI_MING = 1167001,
	PID_YX_XIAO_PAI_AN = 1167002,

	//毛胡 = 1168,  //	毛胡=1168
	PID_YX_MAO_HU_YES = 1168001, //	是=1168001
	PID_YX_MAO_HU_NO = 1168002,  //	否=1168002

	//息囤=1169,//	息囤=1169
	PID_YX_HU_XI_TO_TUN_3 = 1169001, //	3息1囤=1169001
	PID_YX_HU_XI_TO_TUN_1 = 1169002, //	1息1囤=1169002

	//飘=1170,//	飘=1170
	PID_YX_PIAO_YES = 1170001, //	是=1170001
	PID_YX_PIAO_NO = 1170002,  //	否=1170002

	//自摸=1171,//	自摸=1171
	PID_YX_ZI_MO_FAN_YES = 1171001, //	翻倍=1171001
	PID_YX_ZI_MO_FAN_NO = 1171002,  //	无=1171002

	//必胡=1172,//	必胡=1172
	PID_YX_MUST_HU_NO = 1172001,  //	无要求=1172001
	PID_YX_MUST_HU_PAO = 1172002, //	点炮必胡=1172002
	PID_YX_MUST_HU_YES = 1172003, //	有胡必胡=1172003

	//天地胡=1173,//	天地胡=1173
	PID_YX_TD_HU_YES = 1173001, //	是=1173001
	PID_YX_TD_HU_NO = 1173002,  //	否=1173002

	//人数=1174（临武字牌）,//	人数=1174（临武字牌）
	PID_LW_PLAYER_2 = 1174001, //	2人=1174001
	PID_LW_PLAYER_3 = 1174002, //	3人=1174002
	PID_LW_PLAYER_4 = 1174003, //	4人=1174003

	//胡牌=1175,//	胡牌=1175
	PID_LW_BASE_HU_XI_3 = 1175001, //	3胡息起胡=1175001
	PID_LW_BASE_HU_XI_6 = 1175002, //	6胡息起胡=1175002
	PID_LW_BASE_HU_XI_9 = 1175003, //	9胡息起胡=1175003

	//消=1176
	PID_LW_XIAO_PAI_MING = 1176001,
	PID_LW_XIAO_PAI_AN = 1176002,

	//毛胡=1177,//	毛胡=1177
	PID_LW_MAO_HU_YES = 1177001, //	是=1177001
	PID_LW_MAO_HU_NO = 1177002,  //	否=1177002

	//息囤=1178,//	息囤=1178
	PID_LW_HU_XI_TO_TUN_3 = 1178001, //	3息1囤=1178001
	PID_LW_HU_XI_TO_TUN_1 = 1178002, //	1息1囤=1178002

	//飘=1179,//	飘=1179
	PID_LW_PIAO_YES = 1179001, //	是=1179001
	PID_LW_PIAO_NO = 1179002,  //	否=1179002

	//自摸=1180,//	自摸=1180
	PID_LW_ZI_MO_FAN_YES = 1180001, //	翻倍=1180001
	PID_LW_ZI_MO_FAN_NO = 1180002,  //	无=1180002

	//必胡=1181,//	必胡=1181
	PID_LW_MUST_HU_NO = 1181001,  //	无要求=1181001
	PID_LW_MUST_HU_PAO = 1181002, //	点炮必胡=1181002
	PID_LW_MUST_HU_YES = 1181003, //	有胡必胡=1181003

	//人数=1182（马田跑胡子）,//	人数=1182（马田跑胡子）
	PID_MT_PLAYER_2 = 1182001, //	2人=1182001
	PID_MT_PLAYER_3 = 1182002, //	3人=1182002
	PID_MT_PLAYER_4 = 1182003, //	4人=1182003

	//胡牌=1183,//	胡牌=1183
	PID_MT_BASE_HU_XI_6 = 1183001,  //	6胡息起胡=1183001
	PID_MT_BASE_HU_XI_9 = 1183002,  //	9胡息起胡=1183002
	PID_MT_BASE_HU_XI_15 = 1183003, //	15胡息起胡=1183003

	//消=1184
	PID_MT_XIAO_PAI_MING = 1184001,
	PID_MT_XIAO_PAI_AN = 1184002,

	//毛胡=1185,//	毛胡=1185
	PID_MT_MAO_HU_YES = 1185001, //	是=1185001
	PID_MT_MAO_HU_NO = 1185002,  //	否=1185002

	//息囤=1186,//	息囤=1186
	PID_MT_HU_XI_TO_TUN_3 = 1186001, //	3息1囤=1186001
	PID_MT_HU_XI_TO_TUN_1 = 1186002, //	1息1囤=1186002

	//飘=1187,//	飘=1187
	PID_MT_PIAO_YES = 1187001, //	是=1187001
	PID_MT_PIAO_NO = 1187002,  //	否=1187002

	//自摸=1188,//	自摸=1188
	PID_MT_ZI_MO_FAN_YES = 1188001, //	翻倍=1188001
	PID_MT_ZI_MO_FAN_NO = 1188002,  //	无=1188002

	//红黑点=1189,//	红黑点=1189
	PID_MT_HHD_YES = 1189001, //	是=1189001
	PID_MT_HHD_NO = 1189002,  //	否=1189002

	//必胡=1190,//	必胡=1190
	PID_MT_MUST_HU_NO = 1190001,  //	无要求=1190001
	PID_MT_MUST_HU_PAO = 1190002, //	点炮必胡=1190002
	PID_MT_MUST_HU_YES = 1190003, //	有胡必胡=1190003

	//人数=1191（嘉禾字牌）,//	人数=1191（嘉禾字牌）
	PID_JH_PLAYER_2 = 1191001, //	2人=1191001
	PID_JH_PLAYER_3 = 1191002, //	3人=1191002
	PID_JH_PLAYER_4 = 1191003, //	4人=1191003

	//胡牌=1192,//	胡牌=1192
	PID_JH_BASE_HU_XI_6 = 1192001,  //	6胡息起胡=1192001
	PID_JH_BASE_HU_XI_9 = 1192002,  //	9胡息起胡=1192002
	PID_JH_BASE_HU_XI_15 = 1192003, //	15胡息起胡=1192003

	//消=1193
	PID_JH_XIAO_PAI_MING = 1193001,
	PID_JH_XIAO_PAI_AN = 1193002,

	//息囤=1194,//	息囤=1194
	PID_JH_HU_XI_TO_TUN_3 = 1194001, //	3息1囤=1194001
	PID_JH_HU_XI_TO_TUN_1 = 1194002, //	1息1囤=1194002

	//飘=1195,//	飘=1195
	PID_JH_PIAO_YES = 1195001, //	是=1195001
	PID_JH_PIAO_NO = 1195002,  //	否=1195002

	//自摸=1196,//	自摸=1196
	PID_JH_ZI_MO_FAN_YES = 1196001, //	翻倍=1196001
	PID_JH_ZI_MO_FAN_NO = 1196002,  //	无=1196002

	//必胡=1197
	PID_JH_MUST_HU_NO = 1197001,
	PID_JH_MUST_HU_PAO = 1197002,
	PID_JH_MUST_HU_YES = 1197003,*/
};

#endif
