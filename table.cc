#include "table.h"

extern ZJH zjh;
extern Log log;

Table::Table() :
	start_timer_stamp(0.1),
	preready_timer_stamp(5),
	dispatch_show_timer_stamp(0.9),
	out_show_timer_stamp(0.9),
	dispatch_delay_timer_stamp(0.1),
	out_delay_timer_stamp(0.1),
	dissolve_room_tstamp(90)
{
	//timer 1
	start_timer.data = this;
	ev_timer_init(&start_timer, Table::start_timer_cb, start_timer_stamp, start_timer_stamp);

	//timer 2
	preready_timer.data = this;
	ev_timer_init(&preready_timer, Table::preready_timer_cb, preready_timer_stamp, preready_timer_stamp);

	//timer 3
	dispatch_show_timer.data = this;
	ev_timer_init(&dispatch_show_timer, Table::dispatch_show_timer_cb, dispatch_show_timer_stamp, dispatch_show_timer_stamp);

	//timer 4
	out_show_timer.data = this;
	ev_timer_init(&out_show_timer, Table::out_show_timer_cb, out_show_timer_stamp, out_show_timer_stamp);

	//timer 5
	dispatch_delay_timer.data = this;
	ev_timer_init(&dispatch_delay_timer, Table::dispatch_delay_timer_cb, dispatch_delay_timer_stamp, dispatch_delay_timer_stamp);

	//timer 6
	out_delay_timer.data = this;
	ev_timer_init(&out_delay_timer, Table::out_delay_timer_cb, out_delay_timer_stamp, out_delay_timer_stamp);

	//timer 7
	dissolve_room_timer.data = this;
	ev_timer_init(&dissolve_room_timer, Table::dissolve_room_timer_cb, dissolve_room_tstamp, dissolve_room_tstamp);

	//timer 8
	send_videodata_timer.data = this;
	send_videodata_timer_stamp = 0.4;
	ev_timer_init(&send_videodata_timer, Table::Send_Video_Data_Timer_cb, send_videodata_timer_stamp, send_videodata_timer_stamp);

	game_restart_timer.data = this;
	game_restart_timer_stamp = 3;
	ev_timer_init(&game_restart_timer, Table::game_restart_timer_cb, game_restart_timer_stamp, game_restart_timer_stamp);

	InitData();
}

Table::~Table()
{
	ev_timer_stop(zjh.loop, &start_timer);
	ev_timer_stop(zjh.loop, &preready_timer);
	ev_timer_stop(zjh.loop, &dispatch_show_timer);
	ev_timer_stop(zjh.loop, &out_show_timer);
	ev_timer_stop(zjh.loop, &dispatch_delay_timer);
	ev_timer_stop(zjh.loop, &out_delay_timer);
	ev_timer_stop(zjh.loop, &dissolve_room_timer);
	ev_timer_stop(zjh.loop, &send_videodata_timer);
	ev_timer_stop(zjh.loop, &game_restart_timer);
}

void Table::InitData()
{
	vid = 0;
	zid = 0;
	type = 0;
	base_money = 1;
	players.clear();
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		seats[i].clear();
	}
	state = GAME_FREE;
	stand_money = 0;

	m_wTakeIn = 0;
	m_wPlayID = 0;
	m_cbRoomStatus = 0;
	m_wRoomUserId = 0;
	m_bDaNiao = false;
	m_bPiao = false;
	m_bMingXiao = false;
	voters_players.clear();
	dissolve_applyer = 0;
	dissovle_state = 0;
	is_dissolved = 0;

	m_wBankerUser = INVALID_CHAIR;
	m_wHuangZhuangCount = 0;
	m_GamePlayer = 3;
	m_wBaseHuXi = 9;
	m_wHuXiToTun = 3;
	m_wCurrentNum = 0;
	m_wAllNum = 0;
	m_strInnings.clear();

	m_bWithMaoHu = false;
	m_bZiMoFan = true;
	m_cbMustHu = 1;//默认点炮必胡
	m_bHHD = false;
	m_bTianHu3Ti5Kan = false;
	m_b3Ren15Zhang = false;
	
	//数据统计
	memset(m_cbHuPaiCount, 0, sizeof(m_cbHuPaiCount));
	memset(m_cbDianPaoCount, 0, sizeof(m_cbDianPaoCount));
	memset(m_wBestHuXi, 0, sizeof(m_wBestHuXi));
	memset(m_wBestFanShu, 0, sizeof(m_wBestFanShu));
	memset(m_wBestScore, 0, sizeof(m_wBestScore));
	memset(m_wTotalScore, 0, sizeof(m_wTotalScore));
	memset(m_UserZiMoCnt, 0, sizeof(m_UserZiMoCnt));
	memset(m_UserAllPiaoScore, 0, sizeof(m_UserAllPiaoScore));

	m_vecGains.clear();

	m_strLastEndInfo.clear();
	Reset();
}

//重置数据
void Table::Reset()
{
	m_bLeftCardCount = 0;
	m_bDispatchHu = false;
	m_bTiWeiPaoHu = false;
	m_bDispatch = false;
	m_bCardOperated = false;
	m_bTiCard = false;
	m_bIsWeiTiCard = false;
	m_bPaoCard = false;
	m_bFirstCard = false;
	m_bIsTianHuShow = false;
	m_bIsDiHuOut = false;
	m_bAfterWeave = false;
	m_bOutCard = false;
	m_wCurrentUser = INVALID_CHAIR;
	m_cbCurrentCard = INVALID_CARD;
	m_wOutCardUser = INVALID_CHAIR;
	m_cbOutCardData = INVALID_CARD;
	m_cbBankerCard = INVALID_CARD;
	m_bIsTianHu3Ti5Kan = false;
	m_bCurrentMustHu = false;

	memset(m_cbRepertoryCard, 0, sizeof(m_cbRepertoryCard));
	memset(m_cbCardIndex, 0, sizeof(m_cbCardIndex));
	memset(m_cbCardsWeaveCount, 0, sizeof(m_cbCardsWeaveCount));
	memset(m_CardsWeaveInfo, 0, sizeof(m_CardsWeaveInfo));
	memset(m_cbBestWeaveCount, 0, sizeof(m_cbBestWeaveCount));
	memset(m_BestWeaveInfo, 0, sizeof(m_BestWeaveInfo));
	memset(m_bWeaveHuXiCount, 0, sizeof(m_bWeaveHuXiCount));
	memset(m_bWeaveShowHuXiCount, 0, sizeof(m_bWeaveShowHuXiCount));
	memset(m_bWeaveItemCount, 0, sizeof(m_bWeaveItemCount));
	memset(m_WeaveItemArray, 0, sizeof(m_WeaveItemArray));
	memset(m_bHandsHuXiCount, 0, sizeof(m_bHandsHuXiCount));
	memset(m_bResponse, false, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_bPengAbandon, false, sizeof(m_bPengAbandon));
	memset(m_bChiAbandon, false, sizeof(m_bChiAbandon));
	memset(m_bChouWeiCard, false, sizeof(m_bChouWeiCard));
	memset(m_bXiangGong, false, sizeof(m_bXiangGong));
	memset(m_bDongZhang, false, sizeof(m_bDongZhang));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));

	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_UserAbandonCount, 0, sizeof(m_UserAbandonCount));
	memset(m_UserAbandonCards, 0, sizeof(m_UserAbandonCards));
	memset(m_UserAbandonType, 0, sizeof(m_UserAbandonType));
	memset(m_bCommonOrBest, 0x00, sizeof(m_bCommonOrBest));
	memset(m_UserNiaoCount, -1, sizeof(m_UserNiaoCount));
	memset(m_UserPiaoScore, -1, sizeof(m_UserPiaoScore));

}

int Table::split(const std::string& str, std::vector<std::string>& ret_, std::string sep)
{
	if (str.empty())
	{
		return 0;
	}

	std::string tmp;
	std::string::size_type pos_begin = str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;

	while (pos_begin != std::string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

void Table::init_made_info(Json::Value &val)
{
	if (!val["baseGold"].isNull() && val["baseGold"].isNumeric())
	{
		base_money = val["baseGold"].asInt();
	}
	if (!val["standMoney"].isNull() && val["standMoney"].isNumeric())
	{
		stand_money = val["standMoney"].asInt();
	}
	if (!val["roomGolden"].isNull() && val["roomGolden"].isNumeric())
	{
		m_wTakeIn = val["roomGolden"].asInt();
	}
	if (!val["roomStatus"].isNull() && val["roomStatus"].isNumeric())
	{
		m_cbRoomStatus = val["roomStatus"].asInt();
	}
	if (!val["roomUserId"].isNull() && val["roomUserId"].isNumeric())
	{
		m_wRoomUserId = val["roomUserId"].asInt();
	}
	if (!val["dataMap"].isNull())
	{
		if (!val["dataMap"]["innerWayList"].isNull())
		{
			//房间类型
			if (!val["dataMap"]["innerWayList"]["1000"].isNull() && val["dataMap"]["innerWayList"]["1000"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1000"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					m_wPlayID = atoi(strPlayID.c_str());
				}
			}
			//房间总局数
			if (!val["dataMap"]["roomCount"].isNull() && val["dataMap"]["roomCount"].isNumeric())
			{
				m_wAllNum = val["dataMap"]["roomCount"].asInt();
			}
			//起胡胡息
			/*if (!val["dataMap"]["innerWayList"]["1003"].isNull() && val["dataMap"]["innerWayList"]["1003"].isString())
			  {
			  std::string str = val["dataMap"]["innerWayList"]["1003"].asString().c_str();
			  _uint32 tmpValue = atoi( str.c_str() );
			  if( tmpValue >= 0 && tmpValue < 255 ) m_wBaseHuXi = tmpValue;
			  }*/
			  //内围打鸟个数

			#define PID_COUNT 53 
			const char cstrOptions[PID_COUNT][5] =
			{
				"1049",//1
				"1053",//2
				"1104",//3
				"1105",//4
				"1107",//5
				"1108",//6
				"1109",//7
				"1110",//8
				"1137",//9
				"1138",//10
				"1139",//11
				"1157",//12
				"1158",//13
				"1159",//14
				"1160",//15
				"1161",//16
				"1162",//17
				"1163",//18
				"1164",//19
				"1165",//
				"1166",//
				"1167",//
				"1168",//
				"1169",//
				"1170",//
				"1171",//
				"1172",//
				"1173",//
				"1174",//
				"1175",//
				"1176",//
				"1177",//
				"1178",//
				"1179",//
				"1180",//
				"1181",//
				"1182",//
				"1183",//
				"1184",//
				"1185",//
				"1186",//
				"1187",//
				"1188",//
				"1189",//
				"1190",//
				"1191",//
				"1192",//
				"1193",//
				"1194",//
				"1195",//
				"1196",//
				"1197",//
				"1210",//玩法=1210单选  （郴州三人）
			};
			for (int i = 0; i < PID_COUNT; i++)
			{
				if (!val["dataMap"]["innerWayList"][cstrOptions[i]].isNull() && val["dataMap"]["innerWayList"][cstrOptions[i]].isString())
				{
					std::string str = val["dataMap"]["innerWayList"][cstrOptions[i]].asString().c_str();
					std::vector<std::string> vct;
					split(str, vct, ",");
					for (vector<int>::size_type j = 0; j < vct.size(); j++)
					{
						std::string strPlayID = vct[j];
						check_playid(atoi(strPlayID.c_str()));
					}
				}
			}
			#undef PID_COUNT
			/*
			//飘玩法分数
			if (!val["dataMap"]["innerWayList"][].isNull() && val["dataMap"]["innerWayList"]["1053"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1053"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					check_playid(atoi(strPlayID.c_str()));
				}
			}
			//明消暗消选项
			if (!val["dataMap"]["innerWayList"][].isNull() && val["dataMap"]["innerWayList"]["1104"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1104"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					check_playid(atoi(strPlayID.c_str()));
				}
			}
			//胡息换算
			if (!val["dataMap"]["innerWayList"][].isNull() && val["dataMap"]["innerWayList"]["1107"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1107"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					check_playid(atoi(strPlayID.c_str()));
				}
			}
			//飘或者不飘
			if (!val["dataMap"]["innerWayList"][].isNull() && val["dataMap"]["innerWayList"]["1108"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1108"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					check_playid(atoi(strPlayID.c_str()));
				}
			}
			//房间人数
			if (!val["dataMap"]["innerWayList"][].isNull() && val["dataMap"]["innerWayList"]["1109"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1109"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					check_playid(atoi(strPlayID.c_str()));
				}
			}
			//起胡胡息选择
			if (!val["dataMap"]["innerWayList"][].isNull() && val["dataMap"]["innerWayList"]["1110"].isString())
			{
				std::string str = val["dataMap"]["innerWayList"]["1110"].asString().c_str();
				std::vector<std::string> vct;
				split(str, vct, ",");
				for (vector<int>::size_type j = 0; j < vct.size(); j++)
				{
					std::string strPlayID = vct[j];
					check_playid(atoi(strPlayID.c_str()));
				}
			}*/
		}
	}
}

void Table::check_playid(int nPlayID)
{

	switch (nPlayID)
	{
		/*case PID_JH_HU_XI_TO_TUN_3:
		case PID_LW_HU_XI_TO_TUN_3:
		case PID_MT_HU_XI_TO_TUN_3:
		case PID_YX_HU_XI_TO_TUN_3:*/
		case PID_YZ_HU_XI_TO_TUN_3:
		case HU_XI_TO_TUNS_3:									//3胡息算一屯
		{
			m_wHuXiToTun = 3;
		}break;
		/*case PID_JH_HU_XI_TO_TUN_1:
		case PID_LW_HU_XI_TO_TUN_1:
		case PID_MT_HU_XI_TO_TUN_1:
		case PID_YX_HU_XI_TO_TUN_1:*/
		case PID_YZ_HU_XI_TO_TUN_1:
		case HU_XI_TO_TUNS_1:									//1胡息算一屯
		{
			m_wHuXiToTun = 1;
		}break;
		case NEI_NIAO_COUNT_1:									//内围打鸟1
		{
			m_bDaNiao = true;
			if (std::find(m_vecNiaoCount.begin(), m_vecNiaoCount.end(), 1) == m_vecNiaoCount.end())
				m_vecNiaoCount.push_back(1);
		}break;
		case NEI_NIAO_COUNT_2:									//内围打鸟2
		{
			m_bDaNiao = true;
			if (std::find(m_vecNiaoCount.begin(), m_vecNiaoCount.end(), 2) == m_vecNiaoCount.end())
				m_vecNiaoCount.push_back(2);
		}break;
		case NEI_NIAO_COUNT_4:									//内围打鸟4
		{
			m_bDaNiao = true;
			if (std::find(m_vecNiaoCount.begin(), m_vecNiaoCount.end(), 4) == m_vecNiaoCount.end())
				m_vecNiaoCount.push_back(4);
		}break;
		case NEI_NIAO_COUNT_8:									//内围打鸟8
		{
			m_bDaNiao = true;
			if (std::find(m_vecPiaoScore.begin(), m_vecPiaoScore.end(), 8) == m_vecPiaoScore.end())
				m_vecPiaoScore.push_back(8);
		}break;

		/*case PID_JH_PIAO_YES:
		case PID_LW_PIAO_YES:
		case PID_MT_PIAO_YES:
		case PID_YX_PIAO_YES:*/
		case PID_YZ_PIAO_YES:
		case IS_PIAO_YES:
		{
			m_bPiao = true;
			if (std::find(m_vecPiaoScore.begin(), m_vecPiaoScore.end(), 2) == m_vecPiaoScore.end())
				m_vecPiaoScore.push_back(2);
			if (std::find(m_vecPiaoScore.begin(), m_vecPiaoScore.end(), 5) == m_vecPiaoScore.end())
				m_vecPiaoScore.push_back(5);
			if (std::find(m_vecPiaoScore.begin(), m_vecPiaoScore.end(), 8) == m_vecPiaoScore.end())
				m_vecPiaoScore.push_back(8);
		}break;
		/*case PID_JH_PLAYER_2:
		case PID_LW_PLAYER_2:
		case PID_MT_PLAYER_2:
		case PID_YX_PLAYER_2:*/
		case PID_YZ_PLAYER_2:
		{
			m_GamePlayer = 2;
		}break;

		/*case PID_JH_PLAYER_3:
		case PID_LW_PLAYER_3:
		case PID_MT_PLAYER_3:																					
		case PID_YX_PLAYER_3:*/																										
		case PID_YZ_PLAYER_3:
		case GAME_PLAYERS_3:
		{
			m_GamePlayer = 3;
		}break;
		
		/*case PID_JH_PLAYER_4:
		case PID_LW_PLAYER_4:
		case PID_MT_PLAYER_4:
		case PID_YX_PLAYER_4:*/
		case PID_YZ_PLAYER_4:
		case GAME_PLAYERS_4:
		{
			m_GamePlayer = 4;
		}break;

		/*case PID_JH_XIAO_PAI_MING:
		case PID_MT_XIAO_PAI_MING:
		case PID_YX_XIAO_PAI_MING:
		case PID_LW_XIAO_PAI_MING:*/
		case PID_YZ_XIAO_PAI_MING:
		case XIAO_PAI_OPTION_MING:
		{
			m_bMingXiao = true;
		}break;

		/*case PID_JH_XIAO_PAI_AN:
		case PID_MT_XIAO_PAI_AN:
		case PID_YX_XIAO_PAI_AN:
		case PID_LW_XIAO_PAI_AN:*/
		case PID_YZ_XIAO_PAI_AN:
		case XIAO_PAI_OPTION_AN:
		{
			m_bMingXiao = false;
		}break;

		//case PID_LW_BASE_HU_XI_3:
		case PID_YZ_BASE_HU_XI_3:
		case CHOOSE_BASE_HUXI_3:
		{
			m_wBaseHuXi = 3;
		}break;

		/*case PID_MT_BASE_HU_XI_6:
		case PID_LW_BASE_HU_XI_6:
		case PID_JH_BASE_HU_XI_6:*/
		case PID_YZ_BASE_HU_XI_6:
		case CHOOSE_BASE_HUXI_6:
		{
			m_wBaseHuXi = 6;
		}break;

		/*case PID_JH_BASE_HU_XI_9:
		case PID_MT_BASE_HU_XI_9:
		case PID_LW_BASE_HU_XI_9:*/
		case PID_YZ_BASE_HU_XI_9:
		case CHOOSE_BASE_HUXI_9:
		{
			m_wBaseHuXi = 9;
		}break;

		/*case PID_JH_BASE_HU_XI_15:
		case PID_MT_BASE_HU_XI_15:*/
		case PID_YZ_BASE_HU_XI_15:
		{
			m_wBaseHuXi = 15;
		}break;
		
		/*case PID_LW_MAO_HU_YES:
		case PID_MT_MAO_HU_YES:*/
		case PID_YZ_MAO_HU_YES:
		case WITH_MAOHU:
		{
			m_bWithMaoHu = true;
		}break;

		/*case PID_LW_MAO_HU_NO:
		case PID_MT_MAO_HU_NO:
		case PID_YX_MAO_HU_NO:*/
		case PID_YZ_MAO_HU_NO:
		case WITHOUT_MAOHU:
		{
			m_bWithMaoHu = false;
		}break;

		/*case PID_JH_ZI_MO_FAN_YES:
		case PID_LW_ZI_MO_FAN_YES:
		case PID_YX_ZI_MO_FAN_YES:
		case PID_MT_ZI_MO_FAN_YES:*/
		case PID_YZ_ZI_MO_FAN_YES:
		case PID_ZIMO_FAN:
		{
			m_bZiMoFan = true;
		}break;

		/*case PID_JH_ZI_MO_FAN_NO:
		case PID_LW_ZI_MO_FAN_NO:
		case PID_YX_ZI_MO_FAN_NO:
		case PID_MT_ZI_MO_FAN_NO:*/
		case PID_YZ_ZI_MO_NOT_FAN:
		case PID_ZIMO_NO_FAN:
		{
			m_bZiMoFan = false;
		}break;
		
		//case PID_MT_HHD_YES:
		case PID_YZ_HHD_YES:
		case PID_IS_HONGHEIDIAN:
		{
			m_bHHD = true;
		}break;

		//case PID_MT_HHD_NO:
		case PID_YZ_HHD_NO:
		case PID_NO_HONGHEIDIAN:
		{
			m_bHHD = false;
		}break;
		/*case PID_LW_MUST_HU_NO:
		case PID_MT_MUST_HU_NO:
		case PID_YX_MUST_HU_NO:
		case PID_JH_MUST_HU_NO:*/
		case PID_YZ_MUST_HU_NO:
		case PID_NO_MUST_HU:
		{
			m_cbMustHu = 0;
		}break;

		/*case PID_LW_MUST_HU_PAO:
		case PID_YX_MUST_HU_PAO:
		case PID_MT_MUST_HU_PAO:
		case PID_JH_MUST_HU_PAO:*/
		case PID_YZ_MUST_PAO_HU:
		case PID_MUST_PAO_HU:
		{
			m_cbMustHu = 1;
		}break;

		/*case PID_LW_MUST_HU_YES:
		case PID_MT_MUST_HU_YES:
		case PID_YX_MUST_HU_YES:
		case PID_JH_MUST_HU_YES:*/
		case PID_YZ_MUST_HU_YES:
		case PID_MUST_HU:
		{
			m_cbMustHu = 2;
		}break;

		case PID_YZ_TD_HU_YES:
		{
			m_bTianHu3Ti5Kan = true;
		}break;

		case PID_YZ_TD_HU_NO:
		{
			m_bTianHu3Ti5Kan = false;
		}
		break;
		case PID_3_REN_15_ZHANG:
		{
			if (1000036 == m_wPlayID)//郴州3人玩法ID
			{
				m_b3Ren15Zhang = true;
			}
		}
		break;
		case PID_3_REN_21_ZHANG:
		{
			if (1000036 == m_wPlayID)
			{
				m_b3Ren15Zhang = false;
			}
		}
		break;

		default:
			break;
	}
}

int Table::init(Json::Value &valCfg, Client* client)
{
	int my_vid = zjh.conf["tables"]["vid"].asInt();
	int my_zid = zjh.conf["tables"]["zid"].asInt();
	int my_type = zjh.conf["tables"]["type"].asInt();
	tid = client->roomid;
	vid = (vid > 0) ? vid : my_vid;
	zid = (zid > 0) ? zid : my_zid;
	type = (0 == my_type) ? type : my_type;

	init_made_info(valCfg);

	log.info("Table init tid[%s] base_money[%d] stand_money[%d] m_wTakeIn[%d] m_wPlayID[%d] m_cbRoomStatus[%d] m_wRoomUserId[%d] m_wBaseHuXi[%d] m_GamePlayer[%d] .\n",
		tid.c_str(), base_money, stand_money, m_wTakeIn, m_wPlayID, m_cbRoomStatus, m_wRoomUserId, m_wBaseHuXi, m_GamePlayer);
	log.info("Table init tid[%s]  m_bDaNiao[%d] m_vecNiaoCount.size[%d] m_bPiao[%d] m_vecPiaoScore.size[%d] m_wHuXiToTun[%d] m_bMingXiao[%d].\n",
		tid.c_str(), m_bDaNiao, m_vecNiaoCount.size(), m_bPiao, m_vecPiaoScore.size(), m_wHuXiToTun, m_bMingXiao);
	return 0;
}

int Table::broadcast(Player *p, const std::string &packet)
{
	Player *player;
	std::map<int, Player*>::iterator it;
	for (it = players.begin(); it != players.end(); it++) {
		player = it->second;
		if (player == p || player->client == NULL) {
			continue;
		}
		player->client->send(packet);
	}
	return 0;
}

int Table::unicast(Player *p, const std::string &packet)
{
	if (p->client) {
		return p->client->send(packet);
	}
	return -1;
}

int Table::random(int start, int end, int seed)
{
	return start + rand() % (end - start + 1);
}

int Table::sit_down(Player *player)
{
	std::vector<int> tmp;
	for (int i = 0; i < m_GamePlayer; i++) {
		if (seats[i].occupied == 0) {
			tmp.push_back(i);
		}
	}

	int len = tmp.size();
	if (len == m_GamePlayer)
	{
		int index = random(0, len - 1, player->uid);
		int i = tmp[index];
		log.debug("len[%d] index[%d] i[%d]\n", len, index, i);
		seats[i].occupied = 1;
		seats[i].player = player;
		return i;
	}
	else if (len > 0)
	{
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].occupied == 1)
			{
				for (int j = 0; j < m_GamePlayer; ++j)
				{
					int temp_id = (i + j) % m_GamePlayer;
					if (seats[temp_id].occupied == 0)
					{
						seats[temp_id].occupied = 1;
						seats[temp_id].player = player;
						return temp_id;
					}
				}
			}
		}
	}

	return -1;
}

void Table::stand_up(Player *player)
{
	seats[player->seatid].clear();
}

int Table::handler_user_operate(int cmd, Player* player)
{
	switch (cmd)
	{
	case CLIENT_READY_REQ:
		handler_ready(player);
		break;
	case CLIENT_BETTING_REQ:
		handler_betting(player);
		break;
	case CLIENT_LOGOUT_REQ:
		zjh.game->del_player(player);
		break;
	case CLIENT_TABLE_INFO_REQ:
		handler_table_info(player);
		break;
	case CLIENT_CHAT_REQ:
		handler_chat(player);
		break;
	case CLIENT_FACE_REQ:
		handler_face(player);
		break;
	case CLIENT_EMOTION_REQ:
		handler_interaction_emotion(player);
		break;
	case CLIENT_UPTABLE_APPLY_REQ:
		handler_apply_uptable(player);
		break;
	case CLIENT_DOWNTABLE_REQ:
		handler_downtable(player);
		break;
	case CLINET_HEART_BEAT_REQ:
		heart_beat_res(player);
		break;
	case CLIENT_PHZ_OUT_CARD_REQ:			//玩家出牌消息
		OnUserOutCard(player);
		break;
	case CLIENT_PHZ_OPERATE_CARD_REQ:		//玩家操作牌
		OnUserOperateCard(player);
		break;
	case CLIENT_PHZ_CHANGE_CARD_REQ:		//换牌	
		OnUserChangeCard(player);
		break;
	case CLIENT_PHZ_UPDATE_CARD_REQ:		//整理
		OnUserUpdateCard(player);
		break;
	case CLIENT_DISSOLVE_ROOM_REQ:
		handler_dissolve_room(player);		//解散房间请求
		break;
	case CLIENT_DISSOLVE_ACTION_REQ:
		handler_dissolve_action(player);	//操作
		break;
	case CLIENT_PHZ_PIAO_REQ:
		OnUserPiaoOperate(player);		//飘操作
		break;
	case CLIENT_PHZ_FINAL_END_REQ:
		OnFinalEndReq(player);			//总结算请求
		break;
	case CLIENT_PHZ_PRE_OPERATE_REQ:
		OnUserPreOperateCard(player);		//预操作请求
		break;
	case CLIENT_PHZ_OFFLINE_INFO_REQ:
		OnOfflineInfoReq(player);			//断线信息请求
		break;
	case CLIENT_PHZ_HAND_DETAIL_REQ:
		OnHandDetailInfo(player);			//手牌详情
		break;
	case CLIENT_PHZ_GAINS_REQ:
		OnUserGainsReq(player);
		break;
	default:
		log.error("user operate error invalid cmd[%d] uid[%d] tid[%s] .\n", cmd, player->uid, player->tid.c_str());
		return -1;
	}

	return 0;
}

int Table::del_player(Player *player)
{
	if (player == NULL)
	{
		log.error("del player error player Null tid[%s] .\n", tid.c_str());
		return -1;
	}
	if (players.find(player->uid) == players.end())
	{
		log.error("del player error player not find uid[%d] tid[%s].\n", player->uid, tid.c_str());
		return -1;
	}
	player->stop_offline_timer();
	players.erase(player->uid);
	log.info("del player uid[%d] tid[%d].\n", player->uid, tid.c_str());
	if (players.empty())
	{
		return 1;
	}
	return 0;
}

int Table::handler_login(Player *player)
{
	if (players.find(player->uid) != players.end())
	{
		log.error("handler login error player exsit uid[%d] tid[%s] .\n", player->uid, tid.c_str());
		return -1;
	}
	players[player->uid] = player;
	player->tid = tid;

	handler_login_succ_uc(player);	//单播玩家登陆消息
	handler_apply_uptable(player);	//登录后自动上桌	
	handler_table_info(player);		//单播桌子信息

	log.info("handler login succ uid[%d] tid[%s] size[%d] .\n", player->uid, tid.c_str(), players.size());
	return 0;
}

int Table::handler_login_succ_uc(Player *player)
{
	proto::game::AckLoginSuccessUc stLoginuc;
	stLoginuc.set_seatid(player->seatid);
	stLoginuc.set_uid(player->uid);
	stLoginuc.set_money(player->money);
	if (player->name != "")
	{
		stLoginuc.set_name(player->name.c_str());
	}
	if (player->avatar != "")
	{
		stLoginuc.set_avatar(player->avatar.c_str());
	}
	stLoginuc.set_sex(player->sex);
	stLoginuc.set_exp(player->exp);
	stLoginuc.set_rmb(player->rmb);
	Ppacket ppack;
	stLoginuc.SerializeToString(&ppack.body);
	ppack.pack(SERVER_LOGIN_SUCC_UC);
	unicast(player, ppack.data);
	log.info("handler login succ uc uid[%d] tid[%s] .\n", player->uid, tid.c_str());
	return 0;
}

int Table::handler_login_succ_bc(Player *player)
{
	proto::game::AckLoginSuccessBc stLoginbc;
	stLoginbc.set_seatid(player->seatid);
	stLoginbc.set_uid(player->uid);
	stLoginbc.set_money(player->money);
	if (player->name != "")
	{
		stLoginbc.set_name(player->name.c_str());
	}
	if (player->avatar.size() != 0)
	{
		stLoginbc.set_avatar(player->avatar.c_str());
	}
	stLoginbc.set_sex(player->sex);
	Ppacket ppack;
	stLoginbc.SerializeToString(&ppack.body);
	ppack.pack(SERVER_LOGIN_SUCC_BC);
	broadcast(player, ppack.data);
	log.info("handler login succ bc uid[%d] tid[%s] .\n", player->uid, tid.c_str());
	return 0;
}

int Table::handler_table_info(Player *player, int req_count)
{
	proto::game::AckTableInfo stTableInfo;
	stTableInfo.set_seatid(player->seatid);
	stTableInfo.set_state(state);
	stTableInfo.set_operate_timer_stamp(15);
	stTableInfo.set_left_card_count(m_bLeftCardCount);
	stTableInfo.set_banker_user(m_wBankerUser);
	stTableInfo.set_current_user(m_wCurrentUser);
	_uint8 cbCurrentCard = (m_bMingXiao == false && m_bIsWeiTiCard == true) ? INVALID_CARD : m_cbCurrentCard;
	stTableInfo.set_current_card(cbCurrentCard);
	stTableInfo.set_current_outcard_user(m_wOutCardUser);
	stTableInfo.set_current_outcard(m_cbOutCardData);
	stTableInfo.set_is_dispatch(m_bDispatch);
	stTableInfo.set_card_operated(m_bCardOperated);
	stTableInfo.set_is_da_niao(m_bDaNiao);
	stTableInfo.set_is_out_card(m_bOutCard);
	stTableInfo.set_is_first_card(m_bFirstCard);
	stTableInfo.set_huang_count(m_wHuangZhuangCount);
	stTableInfo.set_take_in(m_wTakeIn);
	stTableInfo.set_play_id(m_wPlayID);
	stTableInfo.set_base_huxi(m_wBaseHuXi);
	stTableInfo.set_room_user_id(m_wRoomUserId);

	stTableInfo.set_is_with_maohu(m_bWithMaoHu);
	stTableInfo.set_is_must_hu(m_cbMustHu);
	stTableInfo.set_is_zimo_fan(m_bZiMoFan);
	stTableInfo.set_is_hhd(m_bHHD);
	stTableInfo.set_huxi_to_tun(m_wHuXiToTun);
	stTableInfo.set_current_num(m_wCurrentNum);
	stTableInfo.set_all_num(m_wAllNum);


	if (m_bDaNiao)
	{
		for (_uint8 j = 0; j < m_vecNiaoCount.size(); ++j)
			stTableInfo.add_niao_count(m_vecNiaoCount[j]);
	}
	if (m_bPiao)
	{
		for (_uint8 j = 0; j < m_vecPiaoScore.size(); ++j)
			stTableInfo.add_piao_count(m_vecPiaoScore[j]);
	}

	_tint32 wChairID = player->seatid;
	if (wChairID >= 0 && wChairID < m_GamePlayer)
	{
		if (req_count >= 0 && req_count < m_cbOfflineCount[wChairID])
		{
			for (_uint8 i = req_count; i < m_cbOfflineCount[wChairID] && i < MaxOfflineLen; ++i)
			{
				stTableInfo.add_offline_info(m_strOfflineInfo[wChairID][i]);
			}
		}
		stTableInfo.set_dong_zhang(m_bDongZhang[wChairID]);
		stTableInfo.set_user_action(m_bUserAction[wChairID]);
		stTableInfo.set_b_must_hu(m_bCurrentMustHu);
		if (m_bUserAction[wChairID] != ACK_NULL)
		{
			_tint32 wFirstUser = INVALID_CHAIR;
			bool bFirstOperate = IsFirstUser(wChairID, wFirstUser);
			stTableInfo.set_is_first_operate(bFirstOperate);
		}
		stTableInfo.set_response(m_bResponse[wChairID]);
		stTableInfo.set_perform_action(m_bPerformAction[wChairID]);
		if (m_bCommonOrBest[wChairID] == false)
		{
			for (_uint8 i = 0; i < m_cbCardsWeaveCount[wChairID]; ++i)
			{
				proto::game::tagHandCards* pstHandCards = stTableInfo.add_hand_cards();
				for (_uint8 j = 0; j < m_CardsWeaveInfo[wChairID][i].cbCardCount; ++j)
				{
					pstHandCards->add_card(m_CardsWeaveInfo[wChairID][i].cbCardList[j]);
				}
			}
		}
		else
		{
			for (_uint8 i = 0; i < m_cbBestWeaveCount[wChairID]; ++i)
			{
				proto::game::tagHandCards* pstHandCards = stTableInfo.add_hand_cards();
				for (_uint8 j = 0; j < m_BestWeaveInfo[wChairID][i].cbCardCount; ++j)
				{
					pstHandCards->add_card(m_BestWeaveInfo[wChairID][i].cbCardList[j]);
				}
			}
		}
		std::vector<_uint8> vecListenCards;
		if (IsListen(wChairID, vecListenCards))
		{
			for (_uint8 i = 0; i < vecListenCards.size(); ++i)
				stTableInfo.add_listen_cards(vecListenCards[i]);
		}
		_uint8 cbTotalHuXi = m_bHandsHuXiCount[wChairID] + m_bWeaveHuXiCount[wChairID];
		stTableInfo.set_total_huxi(cbTotalHuXi);
		if (m_bUserAction[wChairID] & ACK_CHI)
		{
			for (_uint8 j = 0; j < m_cbUserChiCardCount[wChairID]; ++j)
			{
				proto::game::tagChiCardInfo* pstChiCardInfo = stTableInfo.add_chicard_info();
				pstChiCardInfo->set_chi_kind(m_UserChiCardInfo[wChairID][j].cbChiKind);
				pstChiCardInfo->set_center_card(m_UserChiCardInfo[wChairID][j].cbCenterCard);
				for (_uint8 c = 0; c < 3; c++) pstChiCardInfo->add_cards(m_UserChiCardInfo[wChairID][j].cbCardData[c]);
				for (_uint8 k = 0; k < m_UserChiCardInfo[wChairID][j].cbSecondCount; ++k)
				{
					proto::game::tagSecondInfo* pstSecondInfo = pstChiCardInfo->add_second_info();
					pstSecondInfo->set_chi_kind(m_UserChiCardInfo[wChairID][j].SecondData[k].cbChiKind);
					pstSecondInfo->set_center_card(m_UserChiCardInfo[wChairID][j].SecondData[k].cbCenterCard);
					for (_uint8 c = 0; c < 3; c++) pstSecondInfo->add_cards(m_UserChiCardInfo[wChairID][j].SecondData[k].cbCardData[c]);
					for (_uint8 m = 0; m < m_UserChiCardInfo[wChairID][j].SecondData[k].cbThirdCount; ++m)
					{
						proto::game::tagThirdInfo* pstThirdInfo = pstSecondInfo->add_third_info();
						pstThirdInfo->set_chi_kind(m_UserChiCardInfo[wChairID][j].SecondData[k].ThirdData[m].cbChiKind);
						pstThirdInfo->set_center_card(m_UserChiCardInfo[wChairID][j].SecondData[k].ThirdData[m].cbCenterCard);
						for (_uint8 c = 0; c < 3; c++) pstThirdInfo->add_cards(m_UserChiCardInfo[wChairID][j].SecondData[k].ThirdData[m].cbCardData[c]);
					}
				}

			}
		}
	}
	stTableInfo.set_base_money(base_money);
	stTableInfo.set_stand_money(stand_money);
	proto::game::tagDissolveRoomInfo* pstDissolveInfo = stTableInfo.mutable_dissolve_room_info();
	pstDissolveInfo->set_state(dissovle_state);
	if (dissovle_state == 1)
	{
		pstDissolveInfo->set_uid(dissolve_applyer);
		for (std::map<int, Player*>::iterator it = voters_players.begin(); it != voters_players.end(); it++)
		{
			pstDissolveInfo->add_voters_uid(it->first);
			proto::game::tagPlayers* pstDissPlayers = pstDissolveInfo->add_players();
			pstDissPlayers->set_uid(it->second->uid);
			pstDissPlayers->set_action(it->second->dissovle_state);
		}
		_uint32 wRemainTime = (_uint32)ev_timer_remaining(zjh.loop, &dissolve_room_timer);
		pstDissolveInfo->set_remain_time(wRemainTime);
	}

	for (_uint8 c = 0; c < m_GamePlayer; ++c)
	{
		Player* p = seats[c].player;
		if (p == NULL) continue;
		proto::game::tagPlayers* pstTablePlayers = stTableInfo.add_players();
		pstTablePlayers->set_uid(p->uid);
		pstTablePlayers->set_seatid(p->seatid);
		pstTablePlayers->set_ready(seats[c].ready);
		pstTablePlayers->set_money(p->money);
		pstTablePlayers->set_name(p->name);
		pstTablePlayers->set_sex(p->sex);
		pstTablePlayers->set_avatar(p->avatar);
		pstTablePlayers->set_hu_xi(m_bWeaveShowHuXiCount[c]);
		for (int j = 0; j < m_bWeaveItemCount[c]; ++j)
		{
			proto::game::tagWeaveCards* pstWeaveCards = pstTablePlayers->add_weaves();
			pstWeaveCards->set_weave_kind(m_WeaveItemArray[c][j].cbWeaveKind);
			pstWeaveCards->set_card_count(m_WeaveItemArray[c][j].cbCardCount);
			if (m_bMingXiao == false && (m_WeaveItemArray[c][j].cbWeaveKind == ACK_WEI || m_WeaveItemArray[c][j].cbWeaveKind == ACK_TI) && player->seatid != c)
			{
				if (m_WeaveItemArray[c][j].cbWeaveKind == ACK_WEI && m_bChouWeiCard[c][m_GameLogic.SwitchToCardIndex(m_WeaveItemArray[c][j].cbCenterCard)] == true)
				{
					pstWeaveCards->set_center_card(m_WeaveItemArray[c][j].cbCenterCard);
					for (int k = 0; k < m_WeaveItemArray[c][j].cbCardCount; ++k)
					{
						pstWeaveCards->add_card_list(m_WeaveItemArray[c][j].cbCardList[k]);
					}
				}
			}
			else
			{
				pstWeaveCards->set_center_card(m_WeaveItemArray[c][j].cbCenterCard);
				for (int k = 0; k < m_WeaveItemArray[c][j].cbCardCount; ++k)
				{
					pstWeaveCards->add_card_list(m_WeaveItemArray[c][j].cbCardList[k]);
				}
			}
		}
		for (_uint8 j = 0; j < m_UserAbandonCount[c]; ++j)
		{
			pstTablePlayers->add_abandon_cards(m_UserAbandonCards[c][j]);
			pstTablePlayers->add_abandon_type(m_UserAbandonType[c][j]);
		}
		if (m_bDaNiao)
		{
			pstTablePlayers->set_niao(m_UserNiaoCount[c]);
		}
		if (m_bPiao)
		{
			pstTablePlayers->set_piao(m_UserPiaoScore[c]);
		}
		pstTablePlayers->set_xiang_gong(m_bXiangGong[c]);
		pstTablePlayers->set_offline(p->is_offline);
	}
	stTableInfo.set_game_player(m_GamePlayer);
	stTableInfo.set_game_name("chenzhouphz");
	Ppacket ppack;
	stTableInfo.SerializeToString(&ppack.body);
	ppack.pack(SERVER_TABLE_INFO_UC);
	unicast(player, ppack.data);
	log.debug("uid[%d] tid[%s] stTableInfo[%s] .\n", player->uid, tid.c_str(), stTableInfo.DebugString().c_str());
	log.info("handler table info uid[%d] tid[%s].\n", player->uid, tid.c_str());
	return 0;
}

int Table::handler_preready()
{
	for (std::map<int, Player*>::iterator iter = players.begin(); iter != players.end(); iter++)
	{
		iter->second->reset();
	}
	log.info("handler preready tid[%s] .\n", tid.c_str());
	return 0;
}

void Table::start_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->start_timer);
	table->m_cbCurTimer = 0;
	table->OnOperateStart();
}

void Table::preready_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->preready_timer);
	table->m_cbCurTimer = 0;
	table->handler_preready();
}

void Table::dispatch_show_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->dispatch_show_timer);
	table->m_cbCurTimer = 0;
	if (table->m_bCardOperated == false)
	{
		table->UpdateAbandonCard();
	}
	table->m_wCurrentUser = (table->m_wCurrentUser + 1) % (table->m_GamePlayer);
	table->DispatchCardData(table->m_wCurrentUser);
}

void Table::out_show_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->out_show_timer);
	table->m_cbCurTimer = 0;
	if (table->m_bCardOperated == false)
	{
		table->UpdateAbandonCard();
	}
	table->m_wCurrentUser = (table->m_wCurrentUser + 1) % (table->m_GamePlayer);
	table->DispatchCardData(table->m_wCurrentUser);
}

void Table::dispatch_delay_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->dispatch_delay_timer);
	table->m_cbCurTimer = 0;
	bool bEstimate = table->DispatchEstimateUserRespond(table->m_wCurrentUser, table->m_cbCurrentCard);
	if (bEstimate == false)
	{
		ev_timer_again(zjh.loop, &table->dispatch_show_timer);
		table->m_cbCurTimer = 3;
	}
	//保存数据到redis
	table->SerializeTableData();
}

void Table::out_delay_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->out_delay_timer);
	table->m_cbCurTimer = 0;
	bool bEstimate = table->OutEstimateUserRespond(table->m_wCurrentUser, table->m_cbCurrentCard);
	if (bEstimate == false)
	{
		ev_timer_again(zjh.loop, &table->out_show_timer);
		table->m_cbCurTimer = 4;
	}
	//保存数据到redis
	table->SerializeTableData();
}

int Table::handler_logout(Player *player)
{
	if (state != GAME_FREE || m_cbRoomStatus != 0 )
	{
		for (int i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].uid == player->uid)
			{
				log.error("handler logout error player in seat playing. uid[%d] state[%d] tid[%s].\n", player->uid, state, tid.c_str());
				return -1;
			}
		}
	}
	handler_downtable(player);
	//广播退出房间协议
	proto::game::AckLogoutBc stLogout;
	stLogout.set_seatid(player->seatid);
	stLogout.set_uid(player->uid);
	stLogout.set_type(player->logout_type);
	Ppacket ppack;
	stLogout.SerializeToString(&ppack.body);
	ppack.pack(SERVER_LOGOUT_SUCC_BC);
	unicast(player, ppack.data);
	log.info("handler logout uid[%d] seatid[%d] money[%ld] tid[%s].\n", player->uid, player->seatid, player->money, tid.c_str());
	return 0;
}

int Table::handler_chat(Player *player)
{
	proto::game::AckChat stChat;
	stChat.ParseFromString(player->client->ppacket.body);
	proto::game::AckChat stChatRes;
	stChatRes.set_seatid(player->seatid);
	stChatRes.set_text(stChat.text());
	Ppacket ppack;
	stChatRes.SerializeToString(&ppack.body);
	ppack.pack(SERVER_CHAT_BC);
	broadcast(NULL, ppack.data);
	log.debug("handler chat stChat[%s] stChatRes[%s] .\n", stChat.DebugString().c_str(), stChatRes.DebugString().c_str());
	return 0;
}

int Table::handler_face(Player *player)
{
	proto::game::AckFace stFace;
	stFace.ParseFromString(player->client->ppacket.body);
	proto::game::AckFace stFaceRes;
	stFaceRes.set_seatid(player->seatid);
	stFaceRes.set_faceid(stFace.faceid());
	Ppacket ppack;
	stFaceRes.SerializeToString(&ppack.body);
	ppack.pack(SERVER_FACE_BC);
	broadcast(NULL, ppack.data);
	return 0;
}

int Table::handler_interaction_emotion(Player *player)
{
	proto::game::ReqEmotion stEmotion;
	stEmotion.ParseFromString(player->client->ppacket.body);
	int target_seatid = stEmotion.target_seatid();
	int itype = stEmotion.type();
	proto::game::AckEmotionBC stEmotionBC;
	stEmotionBC.Clear();
	stEmotionBC.set_seatid(player->seatid);
	stEmotionBC.set_money(player->money);
	stEmotionBC.set_target_seatid(target_seatid);
	stEmotionBC.set_type(itype);
	Ppacket ppack;
	stEmotionBC.SerializeToString(&ppack.body);
	ppack.pack(SERVER_EMOTION_BC);
	broadcast(NULL, ppack.data);
	log.info("handler interaction emotion uid[%d] type[%d] tid[%s].\n ", player->uid, itype, tid.c_str());
	return 0;
}

void Table::betting_error_uc(Player *player, int code)
{
	proto::game::AckBettingErr stBettingErr;
	stBettingErr.set_uid(player->uid);
	stBettingErr.set_seatid(player->seatid);
	stBettingErr.set_code(code);
	Ppacket ppack;
	stBettingErr.SerializeToString(&ppack.body);
	ppack.pack(SERVER_BETTING_ERR_UC);
	unicast(player, ppack.data);
}

int Table::handler_betting(Player *player)
{
	if (state != GAME_NIAO)
	{
		log.error("handler betting error uid[%d] state[%d]  .\n", player->uid, state);
		betting_error_uc(player, 101);
		return -1;
	}

	_tint32 wChairID = player->seatid;
	if (wChairID < 0 || wChairID >= m_GamePlayer)
	{
		log.error("handler betting error uid[%d] seatid[%d] wrong .\n", player->uid, wChairID);
		betting_error_uc(player, 102);
		return -1;
	}

	if (m_UserNiaoCount[wChairID] >= 0)
	{
		log.error("handler betting error uid[%d] m_UserNiaoCount[%d] already niao .\n", player->uid, m_UserNiaoCount[wChairID]);
		betting_error_uc(player, 103);
		return -1;
	}
	proto::game::ReqBetting stReqBetting;
	stReqBetting.ParseFromString(player->client->ppacket.body);
	_uint8 my_count = stReqBetting.cout();
	if (my_count < 0 || (my_count > 0 && std::find(m_vecNiaoCount.begin(), m_vecNiaoCount.end(), my_count) == m_vecNiaoCount.end()))
	{
		log.error("handler betting error uid[%d] my_count[%d] wrong or not find .\n", player->uid, my_count);
		betting_error_uc(player, 104);
		return -1;
	}
	m_UserNiaoCount[wChairID] = my_count;

	proto::game::AckBettingSucc stBettingSucc;
	stBettingSucc.set_uid(player->uid);
	stBettingSucc.set_seatid(player->seatid);
	stBettingSucc.set_name(player->name);
	stBettingSucc.set_avatar(player->avatar);
	stBettingSucc.set_sex(player->sex);
	stBettingSucc.set_niao(m_UserNiaoCount[wChairID]);
	Ppacket ppack;
	stBettingSucc.SerializeToString(&ppack.body);
	ppack.pack(SERVER_BETTING_SUCC_UC);
	broadcast(NULL, ppack.data);
	log.debug("uid[%d] tid[%s] stBettingSucc[%s] .\n", player->uid, tid.c_str(), stBettingSucc.DebugString().c_str());
	log.info("handler betting uid[%d] state[%d] seatid[%d] my_count[%d] tid[%s].\n ", player->uid, state, player->seatid, my_count, tid.c_str());

	//保存数据到redis
	SerializeTableData();

	_uint8 cbNiaoCount = 0;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (m_UserNiaoCount[i] >= 0) cbNiaoCount++;
	}
	if (cbNiaoCount == m_GamePlayer)
	{
		if (m_bPiao) //飘
		{
			state = GAME_PIAO;
			StartPiao();
		}
		else
		{
			GameStart();
		}
	}

	return 0;
}

void Table::uptable_error_uc(Player *player, int code)
{
	if (NULL == player)
	{
		log.info("uptable error uc error player is null tid[%s] .\n", tid.c_str());
		return;
	}
	proto::game::AckUpTableErrorUc stUpTable;
	stUpTable.set_uid(player->uid);
	stUpTable.set_money(player->money);
	stUpTable.set_code(code);
	Ppacket ppack;
	stUpTable.SerializeToString(&ppack.body);
	ppack.pack(SERVER_UPTABLE_ERR_UC);
	unicast(player, ppack.data);
}

int Table::handler_apply_uptable(Player *player)
{
	handler_uptable(player);
	return 0;
}

int Table::handler_uptable(Player *player)
{
	for (int i = 0; i < m_GamePlayer; ++i)
	{
		if (seats[i].uid == player->uid)
		{
			log.error("handler uptable error uid[%d] already in seat tid[%s].\n", player->uid, tid.c_str());
			uptable_error_uc(player, 102);
			return -1;
		}
	}

	if (player->money < stand_money)
	{
		log.error("handler uptable error uid[%d] money[%ld] too less stand_money[%d] tid[%s].\n", player->uid, player->money, stand_money, tid.c_str());
		uptable_error_uc(player, 104);
		return -1;
	}

	int SeatNum = 0;
	for (int i = 0; i < m_GamePlayer; i++)
	{
		if (seats[i].player != NULL)
			SeatNum++;
	}
	if (SeatNum >= m_GamePlayer)
	{
		log.error("handler uptable error uid[%d] SeatNum[%d] tid[%s] .\n", player->uid, SeatNum, tid.c_str());
		uptable_error_uc(player, 105);
		return -1;
	}
	player->stop_offline_timer();
	player->seatid = sit_down(player);
	if (player->seatid < 0)
	{
		log.error("handler uptable error uid[%d] seatid[%d] tid[%s] .\n", player->uid, player->seatid, tid.c_str());
		uptable_error_uc(player, 105);
		return -1;
	}
	Seat &seat = seats[player->seatid];
	seat.uid = player->uid;

	proto::game::AckUpTableSuccess stUpTable;
	stUpTable.set_uid(player->uid);
	stUpTable.set_seatid(player->seatid);
	if (player->name.size() != 0)
	{
		stUpTable.set_name(player->name.c_str());
	}
	stUpTable.set_sex(player->sex);
	if (player->avatar.size() != 0)
	{
		stUpTable.set_avatar(player->avatar.c_str());
	}
	stUpTable.set_money(player->money);
	stUpTable.set_exp(player->exp);
	Ppacket ppack;
	stUpTable.SerializeToString(&ppack.body);
	ppack.pack(SERVER_UPTABLE_SUCC_BC);
	broadcast(NULL, ppack.data);

	log.info("handler uptable succ uid[%d] seaitd[%d] money[%ld] tid[%s] .\n", player->uid, player->seatid, player->money, tid.c_str());
	return 0;
}

int Table::handler_downtable(Player *player)
{
	bool bFind = false;
	for (int i = 0; i < m_GamePlayer; ++i)
	{
		if (seats[i].uid == player->uid)
		{
			bFind = true;
			break;
		}
	}
	if (!bFind)
	{
		log.error("handler downtable error uid[%d] not in seat tid[%s].\n", player->uid, tid.c_str());
		return -1;
	}
	if (state != GAME_FREE || m_cbRoomStatus != 0 )
	{
		player->down_tag = (player->down_tag == 0) ? 1 : 0;
		proto::game::AckAheadDownTable stHeadDownTable;
		stHeadDownTable.set_uid(player->uid);
		stHeadDownTable.set_seatid(player->seatid);
		stHeadDownTable.set_down_tag(player->down_tag);
		Ppacket ppack;
		stHeadDownTable.SerializeToString(&ppack.body);
		ppack.pack(SERVER_AHEAD_DOWNTABLE_UC);
		unicast(player, ppack.data);
		log.info("handler downtable next round down uid[%d] money[%ld] state[%d] tid[%s].\n", player->uid, player->money, state, tid.c_str());
		return 0;
	}

	if (player->seatid == m_wBankerUser)
		m_wBankerUser = INVALID_CHAIR;
	proto::game::AckDownTableSuccess stDownTableSuccess;
	stDownTableSuccess.set_uid(player->uid);
	stDownTableSuccess.set_seatid(player->seatid);
	stDownTableSuccess.set_money(player->money);
	player->down_tag = 0;
	Ppacket ppack;
	stDownTableSuccess.SerializeToString(&ppack.body);
	ppack.pack(SERVER_DOWNTABLE_SUCC_BC);
	broadcast(NULL, ppack.data);
	//stand_up
	stand_up(player);
	player->clear();

	log.info("handler downtable succ uid[%d] money[%ld] tid[%s].\n", player->uid, player->money, tid.c_str());
	return 0;
}

void Table::update_user_info_to_datasvr(int UserScore[GAME_PLAYER])
{
	Jpacket packet_data;
	packet_data.val["cmd"] = SERVER_DZ_UPDATE_USER_INFO;
	for (int i = 0; i < m_GamePlayer; i++)
	{
		packet_data.val["players"][i]["uid"] = seats[i].uid;
		packet_data.val["players"][i]["alter_money"] = UserScore[i];
		packet_data.val["players"][i]["al_board"] = 1;
		if (UserScore[i] > 0) {
			packet_data.val["players"][i]["al_win"] = 1;
			packet_data.val["players"][i]["alter_exp"] = 0;
		}
		else {
			packet_data.val["players"][i]["al_win"] = 0;
			packet_data.val["players"][i]["alter_exp"] = 0;
		}
	}
	packet_data.val["vid"] = vid;
	packet_data.val["roomid"] = tid;
	packet_data.val["innings"] = m_strInnings;
	packet_data.end();
	zjh.game->send_to_datasvr(packet_data.tostring());
}

int Table::game_start_req()
{
	Jpacket req_packet;
	req_packet.val["cmd"] = SERVER_DZ_GAME_START_REQ;
	req_packet.val["vid"] = zid;
	req_packet.val["roomid"] = tid;
	req_packet.val["innings"] = m_strInnings;
	req_packet.end();
	zjh.game->send_to_datasvr(req_packet.tostring());
	log.info("game start req roomid[%s] .\n", tid.c_str());
	return 0;
}

int Table::game_start_res(Jpacket &packet)
{
	int can_start = 0;
	if (!packet.val["can_start"].isNull() && packet.val["can_start"].isNumeric())
	{
		can_start = packet.val["can_start"].asInt();
		m_wCurrentNum = packet.val["currentCount"].asInt();
	}
	if (0 == can_start)
	{
		int code = 0;
		if (!packet.val["code"].isNull() && packet.val["code"].isNumeric())
		{
			code = packet.val["code"].asInt();
		}
		proto::game::AckGameStartFail stGameStartFail;
		stGameStartFail.set_vid(vid);
		stGameStartFail.set_roomid(tid);
		stGameStartFail.set_can_start(0);
		stGameStartFail.set_code(code);
		int ntime = (int)time(NULL);
		stGameStartFail.set_ts(ntime);
		Ppacket ppack;
		stGameStartFail.SerializeToString(&ppack.body);
		ppack.pack(SERVER_DZ_GAME_START_RES);
		broadcast(NULL, ppack.data);
		advance_game_end();
		log.error("game start res fail roomid[%s] can_start[%d] code[%d] .\n", tid.c_str(), can_start, code);
		return -1;
	}
	ev_timer_stop(zjh.loop, &game_restart_timer);
	if (m_bAlreadyResp == true)
	{
		log.error("game start res tid[%s] m_bAlreadyResp[%d] .\n", tid.c_str(), m_bAlreadyResp);
		return -1;
	}
	m_bAlreadyResp = true;

	int cReadyNum = 0;
	for (int i = 0; i < m_GamePlayer; ++i)
	{
		if (seats[i].ready == 1)
			cReadyNum++;
	}
	if (cReadyNum == m_GamePlayer)
	{
		//开始游戏
		proto::game::AckPrereadyBC stPre;
		stPre.set_roomid(tid);
		Ppacket ppack;
		stPre.SerializeToString(&ppack.body);
		ppack.pack(SERVER_GAME_PREREADY_BC);
		broadcast(NULL, ppack.data);

		m_cbRoomStatus = 1;
		if (m_bDaNiao) //打鸟
		{
			state = GAME_NIAO;
			if (m_wBankerUser < 0 || m_wBankerUser >= m_GamePlayer)
			{
				bool IsFind = false;
				for (_uint8 i = 0; i < m_GamePlayer; ++i)
				{
					if (seats[i].player != NULL && seats[i].player->uid == (_tint32)m_wRoomUserId)
					{
						m_wBankerUser = i;
						IsFind = true;
						break;
					}

				}
				if (!IsFind) m_wBankerUser = rand() % m_GamePlayer;
			}
			StartBetting();
		}
		else if (m_bPiao) //飘
		{
			state = GAME_PIAO;
			if (m_wBankerUser < 0 || m_wBankerUser >= m_GamePlayer)
			{
				bool IsFind = false;
				for (_uint8 i = 0; i < m_GamePlayer; ++i)
				{
					if (seats[i].player != NULL && seats[i].player->uid == (_tint32)m_wRoomUserId)
					{
						m_wBankerUser = i;
						IsFind = true;
						break;
					}

				}
				if (!IsFind) m_wBankerUser = rand() % m_GamePlayer;
			}
			StartPiao();
		}
		else
		{
			GameStart();
		}
	}
	else //不能开赛，发0结算给平台
	{
		_tint32 UserScore[GAME_PLAYER] = { 0 };
		update_user_info_to_datasvr(UserScore);
	}

	log.info("game start res succ roomid[%s] can_start[%d] .\n", tid.c_str(), can_start);
	return 0;
}

void Table::handler_ready(Player* player)
{
	if (player->seatid < 0 || player->seatid >= m_GamePlayer)
	{
		log.error("handler ready error seatid[%d] wrong .\n", player->seatid);
		return;
	}

	if (seats[player->seatid].ready == 1)
	{
		log.error("handler ready error seatid[%d] already ready[ .\n", player->seatid);
		return;
	}
	seats[player->seatid].ready = 1;

	proto::game::AckUserReady stReady;
	stReady.set_seatid(player->seatid);
	stReady.set_uid(player->uid);
	Ppacket ppack;
	stReady.SerializeToString(&ppack.body);
	ppack.pack(SERVER_READY_SUCC_BC);
	broadcast(NULL, ppack.data);
	log.info("handler ready succ uid[%d] seatid[%d] tid[%s] .\n", player->uid, player->seatid, tid.c_str());

	//请求开始游戏
	_uint8 cReadyNum = 0;
	for (int i = 0; i < m_GamePlayer; i++)
	{
		if (seats[i].ready == 1)
			cReadyNum++;
	}
	if (state == GAME_FREE && cReadyNum == m_GamePlayer)
	{
		//m_wCurrentNum++;
		m_bAlreadyResp = false;
		std::ostringstream oss;
		oss << tid << "_" << m_wCurrentNum;
		m_strInnings = oss.str();
		game_start_req();
		ev_timer_again(zjh.loop, &game_restart_timer);
	}
}

//开始打鸟
bool Table::StartBetting()
{
	proto::game::AckStartBetting stStartBetting;
	stStartBetting.set_state(state);
	stStartBetting.set_banker(m_wBankerUser);
	Ppacket ppack;
	stStartBetting.SerializeToString(&ppack.body);
	ppack.pack(SERVER_START_BET_BC);
	broadcast(NULL, ppack.data);
	//保存数据到redis
	SerializeTableData();
	return true;
}

//游戏开始
bool Table::GameStart()
{
	log.info("Game Start tid[%s] m_strInnings[%s].\n", tid.c_str(), m_strInnings.c_str());
	state = GAME_PLAYING;
	//gamestart数据初始化
	m_videostr.clear();
	m_strLastEndInfo.clear();
	memset(m_cbOfflineCount, 0, sizeof(m_cbOfflineCount));
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		for (_uint8 j = 0; j < MaxOfflineLen; ++j)
		{
			m_strOfflineInfo[i][j].clear();
		}
	}
	//确定庄家
	if (m_wBankerUser < 0 || m_wBankerUser >= m_GamePlayer)
	{
		bool IsFind = false;
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].player != NULL && seats[i].player->uid == (_tint32)m_wRoomUserId)
			{
				m_wBankerUser = i;
				IsFind = true;
				break;
			}

		}
		if (!IsFind) m_wBankerUser = rand() % m_GamePlayer;
	}

	//保存录像
	save_table_info_video();
	//洗牌
	m_bLeftCardCount = MAX_CARDS;
	m_GameLogic.RandCardData(m_cbRepertoryCard, MAX_CARDS);
	//分发扑克
	_uint8 cbMaxCount = (m_GamePlayer != 4) ? (m_b3Ren15Zhang ? MAX_COUNT_4 : MAX_COUNT) : MAX_COUNT_4;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		m_bLeftCardCount -= (cbMaxCount - 1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_bLeftCardCount], cbMaxCount - 1, m_cbCardIndex[i]);
	}
	//庄家首张
	m_cbBankerCard = m_cbRepertoryCard[--m_bLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
	m_bFirstCard = true;

	/************************************test***********************************/
#if 0
	m_wBankerUser = 2;
	_uint8 buf0[] = { 2,2,3,4,4,5,5,6,7,8,8,10,17,17,17,18,19,19,19,24,};
	_uint8 buf1[] = { 3,4,6,7,7,8,9,10,18,18,19,20,20,21,21,22,24,24,25,26,};
	_uint8 buf2[] = { 1,1,3,3,5,6,8,9,10,17,20,21,21,22,22,23,23,25,26,26,26,};

	m_GameLogic.SwitchToCardIndex(buf0, MAX_COUNT - 1, m_cbCardIndex[0]);
	m_GameLogic.SwitchToCardIndex(buf1, MAX_COUNT - 1, m_cbCardIndex[1]);
	m_GameLogic.SwitchToCardIndex(buf2, MAX_COUNT - 1, m_cbCardIndex[2]);

	m_cbBankerCard = 0x16;
	m_cbRepertoryCard[18] = 0x2;
	m_cbRepertoryCard[17] = 0x16;
	m_cbRepertoryCard[16] = 0x17;
	m_cbRepertoryCard[15] = 0x1;
	m_cbRepertoryCard[14] = 0x3;
	m_cbRepertoryCard[13] = 0x8;
	m_cbRepertoryCard[12] = 0xA;
	m_cbRepertoryCard[11] = 0x1;
	m_cbRepertoryCard[10] = 0x16;
	m_cbRepertoryCard[9] = 0x5;
	m_cbRepertoryCard[8] = 0x17;
	m_cbRepertoryCard[7] = 0x4;
	m_cbRepertoryCard[6] = 0x14;
	m_cbRepertoryCard[5] = 0x4;
	m_cbRepertoryCard[4] = 0x4;
	m_cbRepertoryCard[3] = 0x4;
	m_cbRepertoryCard[2] = 0x4;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
#endif 
#if 0
	m_wBankerUser = 0;
	//_uint8 buf1[] = { 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x01, 0x02, 0x16 };
	_uint8 buf0[] = { 0x14, 0x14, 0x14, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x02, 0x03 };
	_uint8 buf1[] = { 0x01, 0x11, 0x11, 0x02, 0x02, 0x12, 0x12, 0x03, 0x04, 0x04, 0x06, 0x07, 0x05, 0x15, 0x16, 0x17, 0x17, 0x18, 0x19, 0x09 };
	_uint8 buf2[] = { 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x08, 0x08, 0x09, 0x09 };
	//_uint8 buf2[] = { 0x01, 0x02, 0x07, 0x0a, 0x08, 0x08, 0x15, 0x6, 0x16, 0x11, 0x18, 0x19, 0x18, 0x17, 0x1a, 0x09, 0x09, 0x09, 0x09, 0x12 };

	m_GameLogic.SwitchToCardIndex(buf0, MAX_COUNT - 1, m_cbCardIndex[0]);
	m_GameLogic.SwitchToCardIndex(buf1, MAX_COUNT - 1, m_cbCardIndex[1]);
	m_GameLogic.SwitchToCardIndex(buf2, MAX_COUNT - 1, m_cbCardIndex[2]);

	m_cbBankerCard = 0x13;
	m_cbRepertoryCard[18] = 0x08;
	m_cbRepertoryCard[17] = 0x15;
	m_cbRepertoryCard[16] = 0x1a;
	m_cbRepertoryCard[15] = 0x1a;
	m_cbRepertoryCard[14] = 0x17;
	m_cbRepertoryCard[13] = 0x17;
	m_cbRepertoryCard[12] = 0x17;
	m_cbRepertoryCard[11] = 0x18;
	m_cbRepertoryCard[10] = 0x18;
	m_cbRepertoryCard[9] = 0x18;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
#endif 
#if 0
	m_wBankerUser = 0;
	_uint8 buf0[] = { 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x16, 0x16, 0x16, 0x18, 0x19, 0x18, 0x19, 0x1a, 0x11, 0x11, 0x11, 0x12, 0x12 };
	//_uint8 buf1[] = { 0x14, 0x15, 0x16, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x01, 0x02, 0x16 };
	_uint8 buf1[] = { 0x11, 0x11, 0x11, 0x15, 0x15, 0x13, 0x13, 0x13, 0x14, 0x14, 0x14, 0x15, 0x19, 0x19, 0x19, 0x19, 0x02, 0x04, 0x03, 0x18 };
	_uint8 buf2[] = { 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x08, 0x08, 0x08, 0x08 };
	//_uint8 buf2[] = { 0x01, 0x02, 0x07, 0x0a, 0x08, 0x08, 0x15, 0x6, 0x16, 0x11, 0x18, 0x19, 0x18, 0x17, 0x1a, 0x09, 0x09, 0x09, 0x09, 0x12 };

	m_GameLogic.SwitchToCardIndex(buf0, MAX_COUNT - 1, m_cbCardIndex[0]);
	m_GameLogic.SwitchToCardIndex(buf1, MAX_COUNT - 1, m_cbCardIndex[1]);
	m_GameLogic.SwitchToCardIndex(buf2, MAX_COUNT - 1, m_cbCardIndex[2]);

	m_cbBankerCard = 0x13;
	m_cbRepertoryCard[18] = 0x07;
	m_cbRepertoryCard[17] = 0x17;
	m_cbRepertoryCard[16] = 0x17;
	m_cbRepertoryCard[15] = 0x18;
	m_cbRepertoryCard[14] = 0x03;
	m_cbRepertoryCard[13] = 0x04;
	m_cbRepertoryCard[12] = 0x03;
	m_cbRepertoryCard[11] = 0x04;
	m_cbRepertoryCard[10] = 0x03;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
#endif 
#if 0
	m_wBankerUser = 0;
	_uint8 buf0[] = { 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x16, 0x16, 0x16, 0x18, 0x19, 0x18, 0x19, 0x1a, 0x11, 0x11, 0x11, 0x12, 0x12 };
	_uint8 buf1[] = { 0x02, 0x02, 0x02, 0x02, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1a, 0x1a, 0x1a, 0x17, 0x1a, 0x16 };
	_uint8 buf2[] = { 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x08, 0x08, 0x08, 0x08 };
	//_uint8 buf2[] = { 0x01, 0x02, 0x07, 0x0a, 0x08, 0x08, 0x15, 0x6, 0x16, 0x11, 0x18, 0x19, 0x18, 0x17, 0x1a, 0x09, 0x09, 0x09, 0x09, 0x12 };

	m_GameLogic.SwitchToCardIndex(buf0, MAX_COUNT - 1, m_cbCardIndex[0]);
	m_GameLogic.SwitchToCardIndex(buf1, MAX_COUNT - 1, m_cbCardIndex[1]);
	m_GameLogic.SwitchToCardIndex(buf2, MAX_COUNT - 1, m_cbCardIndex[2]);

	m_cbBankerCard = 0x13;
	m_cbRepertoryCard[18] = 0x07;
	m_cbRepertoryCard[17] = 0x16;
	m_cbRepertoryCard[16] = 0x16;
	m_cbRepertoryCard[15] = 0x18;
	m_cbRepertoryCard[14] = 0x03;
	m_cbRepertoryCard[13] = 0x04;
	m_cbRepertoryCard[12] = 0x03;
	m_cbRepertoryCard[11] = 0x04;
	m_cbRepertoryCard[10] = 0x03;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
#endif 
#if 0
	m_wBankerUser = 0;
	_uint8 buf0[] = { 4,4,5,5,6,25,8,9,10,10,10,17,18,19,20,21,21,22,23,25};
	_uint8 buf1[] = { 1,1,2,4,5,5,7,8,9,17,18,20,21,24,24,24,25,26,26,26 };
	_uint8 buf2[] = { 1,1,2,3,4,6,6,8,9,10,17,17,19,21,22,23,23,24,25,26 };
	//_uint8 buf2[] = { 0x01, 0x02, 0x07, 0x0a, 0x08, 0x08, 0x15, 0x6, 0x16, 0x11, 0x18, 0x19, 0x18, 0x17, 0x1a, 0x09, 0x09, 0x09, 0x09, 0x12 };

	m_GameLogic.SwitchToCardIndex(buf0, MAX_COUNT - 1, m_cbCardIndex[0]);
	m_GameLogic.SwitchToCardIndex(buf1, MAX_COUNT - 1, m_cbCardIndex[1]);
	m_GameLogic.SwitchToCardIndex(buf2, MAX_COUNT - 1, m_cbCardIndex[2]);

	m_cbBankerCard = 0x07;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
#endif 
#if 0
	m_wBankerUser = 1;
	_uint8 buf0[] = { 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x16, 0x16, 0x16, 0x18, 0x18, 0x18, 0x19, 0x1a, 0x11, 0x11, 0x11, 0x12, 0x12 };
	_uint8 buf1[] = { 0x01, 0x01, 0x01, 0x11, 0x11, 0x11, 0x03, 0x03, 0x03, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x13, 0x13, 0x13, 0x15, 0x15 };
	_uint8 buf2[] = { 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x08, 0x08, 0x08, 0x08 };
	//_uint8 buf2[] = { 0x01, 0x02, 0x07, 0x0a, 0x08, 0x08, 0x15, 0x6, 0x16, 0x11, 0x18, 0x19, 0x18, 0x17, 0x1a, 0x09, 0x09, 0x09, 0x09, 0x12 };

	m_GameLogic.SwitchToCardIndex(buf0, MAX_COUNT - 1, m_cbCardIndex[0]);
	m_GameLogic.SwitchToCardIndex(buf1, MAX_COUNT - 1, m_cbCardIndex[1]);
	m_GameLogic.SwitchToCardIndex(buf2, MAX_COUNT - 1, m_cbCardIndex[2]);

	m_cbBankerCard = 0x17;
	m_cbRepertoryCard[18] = 0x1a;
	m_cbRepertoryCard[17] = 0x15;
	m_cbRepertoryCard[16] = 0x15;
	m_cbRepertoryCard[15] = 0x18;
	m_cbRepertoryCard[14] = 0x03;
	m_cbRepertoryCard[13] = 0x04;
	m_cbRepertoryCard[12] = 0x03;
	m_cbRepertoryCard[11] = 0x04;
	m_cbRepertoryCard[10] = 0x03;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbBankerCard)]++;
#endif 
	/************************************test***********************************/

	log.info("game start m_wBankerUser[%d] m_cbBankerCard[0x%02x] tid[%s] .\n", m_wBankerUser, m_cbBankerCard, tid.c_str());
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		char s[300] = { 0 };
		_uint8 slen = 0;
		for (_uint8 j = 0; j < MAX_INDEX; ++j)
		{
			if (m_cbCardIndex[i][j] == 0) continue;
			for (_uint8 k = 0; k < m_cbCardIndex[i][j]; ++k)
			{
				slen += snprintf(s + slen, 300 - slen, "%d,", m_GameLogic.SwitchToCardData(j));
			}
		}
		log.info("game start handcards seatid[%d] uid[%d] handcards[%s] tid[%s].\n", i, seats[i].uid, s, tid.c_str());
	}

	//广播游戏开始
	std::map<int, Player*>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter)
	{
		Player* player = iter->second;
		if (NULL == player) continue;
		_tint32 wCurChairID = player->seatid;
		proto::game::AckGameStart stGameStart;
		stGameStart.set_banker(m_wBankerUser);
		stGameStart.set_current_num(m_wCurrentNum);
		if (wCurChairID == m_wBankerUser)
		{
			stGameStart.set_banker_card(m_cbBankerCard);
		}
		else
		{
			stGameStart.set_banker_card(INVALID_CARD);
		}
		stGameStart.set_left_card_count(m_bLeftCardCount);
		if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
		{
			UpdateCommonCardsWeave(wCurChairID);
			UpdateBestCardsWeave(wCurChairID);
			if (m_bCommonOrBest[wCurChairID] == false) {
				for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stGameStart.add_hand_cards();
					for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			else {
				for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stGameStart.add_hand_cards();
					for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			std::vector<_uint8> vecListenCards;
			if (IsListen(wCurChairID, vecListenCards))
			{
				for (_uint8 i = 0; i < vecListenCards.size(); ++i)
					stGameStart.add_listen_cards(vecListenCards[i]);
			}
			_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
			stGameStart.set_total_huxi(cbTotalHuXi);
		}
		Ppacket ppack;
		stGameStart.SerializeToString(&ppack.body);
		ppack.pack(SERVER_GAME_START_BC);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stGameStart[%s] .\n", player->uid, tid.c_str(), stGameStart.DebugString().c_str());
	}

	ev_timer_again(zjh.loop, &start_timer);
	m_cbCurTimer = 1;
	//保存录像
	save_game_start_video();
	//保存数据到redis
	SerializeTableData();
	return true;
}

bool Table::OnOperateStart()
{
	//设置变量
	m_wCurrentUser = m_wBankerUser;
	m_cbCurrentCard = m_cbBankerCard;

	//提牌处理
	_uint8 cbTiCardCount[GAME_PLAYER] = { 0 };
	_uint8 cbTiCardIndex[GAME_PLAYER][MAX_WEAVE];
	memset(cbTiCardIndex, 0, sizeof(cbTiCardIndex));
	_uint8 cbWeiCardCount[GAME_PLAYER] = { 0 };
	_uint8 cbWeiCardIndex[GAME_PLAYER][MAX_WEAVE];
	memset(cbWeiCardIndex, 0, sizeof(cbWeiCardIndex));

	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		//类型统计
		cbTiCardCount[i] = m_GameLogic.GetAcitonTiCard(m_cbCardIndex[i], cbTiCardIndex[i]);
		cbWeiCardCount[i] = m_GameLogic.GetActionWeiCard(m_cbCardIndex[i], cbWeiCardIndex[i]);
	}
	//3提5坎天胡判断
	if( m_bTianHu3Ti5Kan )
	{
		for( _uint8 i = 0; i < GAME_PLAYER; ++i )
		{
			_uint8 cChairID = (m_wBankerUser + i) % GAME_PLAYER;
			if( cbTiCardCount[cChairID] >= 3 
						|| (cbTiCardCount[cChairID] + cbWeiCardCount[cChairID]) >= 5 )
			{
				m_bIsTianHu3Ti5Kan = true;
				//m_cbUserCanHuCount[cChairID]++;
				GameEnd( cChairID );
				return true;
			}
		}
	}
	//庄家提牌处理
	_tint32 wChairID = m_wBankerUser;
	if (cbTiCardCount[wChairID] > 0)
	{
		for (_uint8 j = 0; j < cbTiCardCount[wChairID]; j++)
		{
			_uint8 cbRemoveCount = m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]];
			_uint8 cbTiCardData = m_GameLogic.SwitchToCardData(cbTiCardIndex[wChairID][j]);
			m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]] = 0;
			//设置组合
			_uint8 cbIndex = m_bWeaveItemCount[wChairID]++;
			m_WeaveItemArray[wChairID][cbIndex].cbCardCount = 4;
			m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind = ACK_TI;
			m_WeaveItemArray[wChairID][cbIndex].cbCenterCard = cbTiCardData;
			m_WeaveItemArray[wChairID][cbIndex].cbCardList[0] = cbTiCardData;
			m_WeaveItemArray[wChairID][cbIndex].cbCardList[1] = cbTiCardData;
			m_WeaveItemArray[wChairID][cbIndex].cbCardList[2] = cbTiCardData;
			m_WeaveItemArray[wChairID][cbIndex].cbCardList[3] = cbTiCardData;
			//更新胡息
			//UpdateUserHuXiCount(wChairID);
			UpdateCardsWeave(wChairID, m_WeaveItemArray[wChairID][cbIndex].cbCardList, cbRemoveCount);
			//广播提牌
			std::map<int, Player*>::iterator iter = players.begin();
			for (; iter != players.end(); ++iter)
			{
				Player* player = iter->second;
				if (NULL == player) continue;
				_tint32 wCurChairID = player->seatid;
				//打包
				proto::game::AckOperateCard stOperateCard;
				stOperateCard.set_ack_code(ACK_TI);
				stOperateCard.set_seatid(wChairID);
				if (m_bMingXiao)
				{
					stOperateCard.set_action_card(cbTiCardData);
				}
				stOperateCard.set_remove_count(cbRemoveCount);
				if (wCurChairID == wChairID) {
					stOperateCard.set_action_card(cbTiCardData);
					if (m_bCommonOrBest[wCurChairID] == false) {
						for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j)
							{
								pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					else {
						for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					std::vector<_uint8> vecListenCards;
					if (IsListen(wCurChairID, vecListenCards))
					{
						for (_uint8 i = 0; i < vecListenCards.size(); ++i)
							stOperateCard.add_listen_cards(vecListenCards[i]);
					}
					_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
					stOperateCard.set_total_huxi(cbTotalHuXi);
				}
				stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wChairID]);
				if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
				{
					stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
				}
				Ppacket ppack;
				stOperateCard.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
				unicast(player, ppack.data);
				log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
				//保存离线数据
				save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
			}
			log.info("on operate start ti card wChairID[%d] cbTiCardData[0x%02x] tid[%s].\n", wChairID, cbTiCardData, tid.c_str());
			//保存录像
			save_operate_video(wChairID, ACK_TI, cbTiCardData);
		}
		//如果有多个提牌
		if (cbTiCardCount[wChairID] > 1)
		{
			for (_uint8 i = 0; i < cbTiCardCount[wChairID] - 1; ++i)
			{
				_uint8 bSendCardData = m_cbRepertoryCard[--m_bLeftCardCount];
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(bSendCardData)]++;
				//广播翻牌
				std::map<int, Player*>::iterator iter = players.begin();
				for (; iter != players.end(); ++iter)
				{
					Player* player = iter->second;
					if (NULL == player) continue;
					_tint32 wCurChairID = player->seatid;

					proto::game::AckDispatchCard stDispachCard;
					stDispachCard.set_seatid(wChairID);
					stDispachCard.set_card(bSendCardData);
					stDispachCard.set_left_card_count(m_bLeftCardCount);
					stDispachCard.set_to_seatid(wChairID);
					if (wCurChairID == wChairID)
					{
						UpdateCommonCardsWeave(wCurChairID);
						UpdateBestCardsWeave(wCurChairID);
						if (m_bCommonOrBest[wCurChairID] == false) {
							for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
								for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						else {
							for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
								for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						std::vector<_uint8> vecListenCards;
						if (IsListen(wCurChairID, vecListenCards))
						{
							for (_uint8 i = 0; i < vecListenCards.size(); ++i)
								stDispachCard.add_listen_cards(vecListenCards[i]);
						}
						_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
						stDispachCard.set_total_huxi(cbTotalHuXi);
					}
					if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
					{
						stDispachCard.set_off_count(m_cbOfflineCount[wCurChairID]);
					}
					Ppacket ppack;
					stDispachCard.SerializeToString(&ppack.body);
					ppack.pack(SERVER_PHZ_SEND_CARD);
					unicast(player, ppack.data);
					log.debug("uid[%d] tid[%s] stDispachCard[%s] .\n", player->uid, tid.c_str(), stDispachCard.DebugString().c_str());
					//保存离线数据
					save_offline_info(wCurChairID, SERVER_PHZ_SEND_CARD, stDispachCard.SerializeAsString());
				}
				//保存录像
				save_dispatch_card_video(wChairID, bSendCardData, wChairID);
			}
			_uint8 cbTempTiCount = 0;
			_uint8 cbTempTiIndex[MAX_WEAVE];
			memset(cbTempTiIndex, 0, sizeof(cbTempTiIndex));
			do
			{
				cbTempTiCount = m_GameLogic.GetAcitonTiCard(m_cbCardIndex[wChairID], cbTempTiIndex);
				if (cbTempTiCount == 0) break;
				for (_uint8 j = 0; j < cbTempTiCount; ++j)
				{
					_uint8 cbRemoveCount = m_cbCardIndex[wChairID][cbTempTiIndex[j]];
					_uint8 cbTiCardData = m_GameLogic.SwitchToCardData(cbTempTiIndex[j]);
					m_cbCardIndex[wChairID][cbTempTiIndex[j]] = 0;
					//设置组合
					_uint8 cbIndex = m_bWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbIndex].cbCardCount = 4;
					m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind = ACK_TI;
					m_WeaveItemArray[wChairID][cbIndex].cbCenterCard = cbTiCardData;
					m_WeaveItemArray[wChairID][cbIndex].cbCardList[0] = cbTiCardData;
					m_WeaveItemArray[wChairID][cbIndex].cbCardList[1] = cbTiCardData;
					m_WeaveItemArray[wChairID][cbIndex].cbCardList[2] = cbTiCardData;
					m_WeaveItemArray[wChairID][cbIndex].cbCardList[3] = cbTiCardData;
					//更新胡息
					//UpdateUserHuXiCount(wChairID);
					UpdateCardsWeave(wChairID, m_WeaveItemArray[wChairID][cbIndex].cbCardList, cbRemoveCount);
					//广播提牌
					std::map<int, Player*>::iterator iter = players.begin();
					for (; iter != players.end(); ++iter)
					{
						Player* player = iter->second;
						if (NULL == player) continue;
						_tint32 wCurChairID = player->seatid;
						//打包
						proto::game::AckOperateCard stOperateCard;
						stOperateCard.set_ack_code(ACK_TI);
						stOperateCard.set_seatid(wChairID);
						if (m_bMingXiao)
						{
							stOperateCard.set_action_card(cbTiCardData);
						}
						stOperateCard.set_remove_count(cbRemoveCount);
						if (wCurChairID == wChairID) {
							stOperateCard.set_action_card(cbTiCardData);
							if (m_bCommonOrBest[wCurChairID] == false) {
								for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
									proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
									for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
										pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
									}
								}
							}
							else {
								for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
									proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
									for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
										pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
									}
								}
							}
							std::vector<_uint8> vecListenCards;
							if (IsListen(wCurChairID, vecListenCards))
							{
								for (_uint8 i = 0; i < vecListenCards.size(); ++i)
									stOperateCard.add_listen_cards(vecListenCards[i]);
							}
							_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
							stOperateCard.set_total_huxi(cbTotalHuXi);
						}
						stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wChairID]);
						if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
						{
							stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
						}
						Ppacket ppack;
						stOperateCard.SerializeToString(&ppack.body);
						ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
						unicast(player, ppack.data);
						log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
						//保存离线数据
						save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
					}
					log.info("on operate start ti card wChairID[%d] cbTiCardData[0x%02x] tid[%s].\n", wChairID, cbTiCardData, tid.c_str());
					//保存录像
					save_operate_video(wChairID, ACK_TI, cbTiCardData);
				}
				for (_uint8 i = 0; i < cbTempTiCount; ++i)
				{
					_uint8 bSendCardData = m_cbRepertoryCard[--m_bLeftCardCount];
					m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(bSendCardData)]++;
					//广播翻牌
					std::map<int, Player*>::iterator iter = players.begin();
					for (; iter != players.end(); ++iter)
					{
						Player* player = iter->second;
						if (NULL == player) continue;
						_tint32 wCurChairID = player->seatid;

						proto::game::AckDispatchCard stDispachCard;
						stDispachCard.set_seatid(wChairID);
						stDispachCard.set_card(bSendCardData);
						stDispachCard.set_left_card_count(m_bLeftCardCount);
						stDispachCard.set_to_seatid(wChairID);
						if (wCurChairID == wChairID)
						{
							UpdateCommonCardsWeave(wCurChairID);
							UpdateBestCardsWeave(wCurChairID);
							if (m_bCommonOrBest[wCurChairID] == false) {
								for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
									proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
									for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
										pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
									}
								}
							}
							else {
								for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
									proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
									for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
										pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
									}
								}
							}
							std::vector<_uint8> vecListenCards;
							if (IsListen(wCurChairID, vecListenCards))
							{
								for (_uint8 i = 0; i < vecListenCards.size(); ++i)
									stDispachCard.add_listen_cards(vecListenCards[i]);
							}
							_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
							stDispachCard.set_total_huxi(cbTotalHuXi);
						}
						if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
						{
							stDispachCard.set_off_count(m_cbOfflineCount[wCurChairID]);
						}
						Ppacket ppack;
						stDispachCard.SerializeToString(&ppack.body);
						ppack.pack(SERVER_PHZ_SEND_CARD);
						unicast(player, ppack.data);
						log.debug("uid[%d] tid[%s] stDispachCard[%s] .\n", player->uid, tid.c_str(), stDispachCard.DebugString().c_str());
						//保存离线数据
						save_offline_info(wCurChairID, SERVER_PHZ_SEND_CARD, stDispachCard.SerializeAsString());
					}
					//保存录像
					save_dispatch_card_video(wChairID, bSendCardData, wChairID);
				}
			} while (true);
		}
	}

	//庄家天胡判断，可操作
	tagHuCardInfo stHuInfo;
	bool bZimo = false;
	if (m_bDispatch && m_wBankerUser == m_wCurrentUser) bZimo = true;			//是否自摸
	std::vector<tagHuFanInfo> vecHuFanInfo;
	bool bHu = m_GameLogic.GetHuCardInfoFinal(m_cbCardIndex[m_wBankerUser], INVALID_CARD, m_bWeaveItemCount[m_wBankerUser], m_WeaveItemArray[m_wBankerUser], stHuInfo, m_wBaseHuXi, vecHuFanInfo, bZimo, m_bDispatch, m_bHHD, false, m_bWithMaoHu);
	if (bHu && !m_bXiangGong[m_wBankerUser])
	{ 	
		m_bCurrentMustHu = false;
		m_bDispatchHu = false;
		m_bUserAction[m_wBankerUser] |= ACK_CHIHU;
		m_bIsTianHuShow = true;
		if (seats[m_wBankerUser].player != NULL)
		{
			/*if (2 == m_cbMustHu)//必胡
			{
				return GameEnd(m_wBankerUser);
			}*/
			//发送操作提示
			proto::game::AckOperateNotify stNotity;
			stNotity.set_operate_type(m_bUserAction[m_wBankerUser]);
			stNotity.set_operate_card(m_cbBankerCard);
			stNotity.set_off_count(m_cbOfflineCount[m_wBankerUser]);
			m_bCurrentMustHu = (2 == m_cbMustHu);
			stNotity.set_is_must_hu(m_bCurrentMustHu);

			Ppacket ppack;
			stNotity.SerializeToString(&ppack.body);
			ppack.pack(SERVER_PHZ_OPERATE_NOTIFY);
			unicast(seats[m_wBankerUser].player, ppack.data);
			log.debug("uid[%d] tid[%s] stNotity[%s] .\n", seats[m_wBankerUser].player->uid, tid.c_str(), stNotity.DebugString().c_str());
			//保存离线数据
			save_offline_info(m_wBankerUser, SERVER_PHZ_OPERATE_NOTIFY, stNotity.SerializeAsString());
			log.info("on operate start tian hu m_wBankerUser[%d] m_bUserAction[0x%02x] m_cbBankerCard[0x%02x] tid[%s] .\n",
				m_wBankerUser, m_bUserAction[m_wBankerUser], m_cbBankerCard, tid.c_str());
			//保存录像
			save_operate_notify_video();
			//保存数据到redis
			SerializeTableData();
			return true;
		}
	}

	//设置变量
	SendOutCardNotify(m_wCurrentUser, true);
	//保存数据到redis
	SerializeTableData();
	return true;
}

void Table::Do3Ti5Kan(proto::game::AckGameEnd& stGameEnd, _uint32& wTotalHuXi, _tint32 wChairID)
{
	//3提5坎胡牌提牌偎牌处理
	_uint8 cbTiCardIndex[MAX_WEAVE];
	memset(cbTiCardIndex, 0, sizeof(cbTiCardIndex));
	_uint8 cbTiCardCount = m_GameLogic.GetAcitonTiCard(m_cbCardIndex[wChairID], cbTiCardIndex);
	for (_uint8 j = 0; j < cbTiCardCount; j++)
	{
		_uint8 cbTiCardData = m_GameLogic.SwitchToCardData(cbTiCardIndex[j]);
		m_cbCardIndex[wChairID][cbTiCardIndex[j]] = 0;
		_uint8 cbIndex = m_bWeaveItemCount[wChairID]++;
		m_WeaveItemArray[wChairID][cbIndex].cbCardCount = 4;
		m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind = ACK_TI;
		m_WeaveItemArray[wChairID][cbIndex].cbCenterCard = cbTiCardData;
		for (_uint8 i = 0; i < 4; ++i)
			m_WeaveItemArray[wChairID][cbIndex].cbCardList[i] = cbTiCardData;
		//更新胡息
		//UpdateUserHuXiCount(wChairID);
		//UpdateCardsWeave(wChairID, m_WeaveItemArray[wChairID][cbIndex].cbCardList, cbRemoveCount);
	}
	_uint8 cbWeiCardIndex[MAX_WEAVE];
	memset(cbWeiCardIndex, 0, sizeof(cbWeiCardIndex));
	_uint8 cbWeiCardCount = m_GameLogic.GetActionWeiCard(m_cbCardIndex[wChairID], cbWeiCardIndex);
	for (_uint8 j = 0; j < cbWeiCardCount; j++)
	{
		_uint8 cbWeiCardData = m_GameLogic.SwitchToCardData(cbWeiCardIndex[j]);
		m_cbCardIndex[wChairID][cbWeiCardIndex[j]] = 0;
		_uint8 cbIndex = m_bWeaveItemCount[wChairID]++;
		m_WeaveItemArray[wChairID][cbIndex].cbCardCount = 3;
		m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind = ACK_WEI;
		m_WeaveItemArray[wChairID][cbIndex].cbCenterCard = cbWeiCardData;
		for (_uint8 i = 0; i < 3; ++i)
			m_WeaveItemArray[wChairID][cbIndex].cbCardList[i] = cbWeiCardData;
		//更新胡息
		//UpdateUserHuXiCount(wChairID);
		//UpdateCardsWeave(wChairID, m_WeaveItemArray[wChairID][cbIndex].cbCardList, cbRemoveCount);
	}
	//手牌组合
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (m_cbCardIndex[wChairID][i] == 0)
			continue;
		_uint8 cbCardData = m_GameLogic.SwitchToCardData(i);
		if ((cbCardData & MASK_VALUE) == 0x02) //二七十吃
		{
			for (_uint8 j = 0; j <= m_cbCardIndex[wChairID][i]; j++)
			{
				if ((m_cbCardIndex[wChairID][i + 5] >= 1) && (m_cbCardIndex[wChairID][i + 8] >= 1))
				{
					m_cbCardIndex[wChairID][i]--;
					m_cbCardIndex[wChairID][i + 5]--;
					m_cbCardIndex[wChairID][i + 8]--;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCardCount = 3;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbWeaveKind = ACK_CHI;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCenterCard = cbCardData;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCardList[0] = cbCardData;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCardList[1] = cbCardData + 5;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]++].cbCardList[2] = cbCardData + 8;
				}
			}
		}
		if ((cbCardData & MASK_VALUE) == 0x01) //一二三吃
		{
			for (_uint8 j = 0; j <= m_cbCardIndex[wChairID][i]; j++)
			{
				if ((m_cbCardIndex[wChairID][i + 1] >= 1) && (m_cbCardIndex[wChairID][i + 2] >= 1))
				{
					m_cbCardIndex[wChairID][i]--;
					m_cbCardIndex[wChairID][i + 1]--;
					m_cbCardIndex[wChairID][i + 2]--;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCardCount = 3;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbWeaveKind = ACK_CHI;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCenterCard = cbCardData;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCardList[0] = cbCardData;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]].cbCardList[1] = cbCardData + 1;
					m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID]++].cbCardList[2] = cbCardData + 2;
				}
			}
		}
	}
	UpdateUserHuXiCount(wChairID);
	
	memset(m_BestWeaveInfo[wChairID], 0, sizeof(tagCardsWeave)*MAX_INDEX);

	m_GameLogic.SettleBestWeaves(m_cbCardIndex[wChairID], m_BestWeaveInfo[wChairID], m_cbBestWeaveCount[wChairID]);
	m_bHandsHuXiCount[wChairID] = m_GameLogic.GetCardsWeaveHuXi(m_BestWeaveInfo[wChairID], m_cbBestWeaveCount[wChairID]);

	wTotalHuXi = m_bWeaveHuXiCount[wChairID] + m_bHandsHuXiCount[wChairID] ;
	///
	for (_uint8 j = 0; j < m_bWeaveItemCount[wChairID]; ++j)
	{
		proto::game::tagWeaveCards *pstWeaves = stGameEnd.add_weaves();
		_uint8 cbKind = (m_WeaveItemArray[wChairID][j].cbWeaveKind == ACK_WEI) ? ACK_KAN : m_WeaveItemArray[wChairID][j].cbWeaveKind;
		pstWeaves->set_weave_kind(cbKind);
		pstWeaves->set_hu_xi(m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][j]));
		for (int k = 0; k < m_WeaveItemArray[wChairID][j].cbCardCount; ++k)
		{
			pstWeaves->add_card_list(m_WeaveItemArray[wChairID][j].cbCardList[k]);
		}
	}
	
	for (_uint8 j = 0; j < m_cbBestWeaveCount[wChairID]; ++j)
	{
		proto::game::tagWeaveCards *pstWeaves = stGameEnd.add_weaves();
		//_uint8 cbKind = (m_BestWeaveInfo[wChairID][j].cbWeaveKind == ACK_WEI) ? ACK_KAN : m_BestWeaveInfo[wChairID][j].cbWeaveKind;
		//pstWeaves->set_weave_kind(cbKind);
		//pstWeaves->set_hu_xi(m_GameLogic.GetWeaveHuXi(m_BestWeaveInfo[wChairID][j]));
		for (int k = 0; k < m_BestWeaveInfo[wChairID][j].cbCardCount; ++k)
		{
			pstWeaves->add_card_list(m_BestWeaveInfo[wChairID][j].cbCardList[k]);
		}
	}
	

	log.info("Game End TianHu3Ti5Kan wChairID[%d]  wTotalHuXi[%d] .\n", wChairID, wTotalHuXi);
}

//游戏结束
bool Table::GameEnd(_tint32 wChairID)
{
	//操作清零
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));
	log.info("game end wChairID[%d] uid[%d] tid[%s] m_strInnings[%s].\n", wChairID, seats[wChairID].uid, tid.c_str(), m_strInnings.c_str());

	state = GAME_END;
	_uint8 cbEndState = wChairID == INVALID_CHAIR ? 0 : 1; //0：黄庄 1：胡牌结束
	if (cbEndState)
	{
		_uint32 wTotalHuXi = 0;										//总胡息
		_uint32 wTotalTunCount = 0;									//总囤数
		_tint32 UserScore[GAME_PLAYER] = { 0 };						//玩牌得分
		_tint32 UserNiaoScore[GAME_PLAYER] = { 0 };					//打鸟得分
		_tint32 UserPiaoScore[GAME_PLAYER] = { 0 };					//飘得分
		bool bZiMo = (wChairID == m_wCurrentUser) ? true : false;	//是否自摸
		bool bDianPao = false;
		if (!m_bIsTianHuShow && !m_bDispatch && !m_bIsTianHu3Ti5Kan) bDianPao = true;
		//结算打包
		proto::game::AckGameEnd stGameEnd;
		stGameEnd.set_end_type(cbEndState);
		stGameEnd.set_win_seatid(wChairID);
		stGameEnd.set_hu_card(m_cbCurrentCard);
		//牌墩牌
		_uint8 cbLeftCardCount = m_bLeftCardCount;
		_uint8 cbRepertoryCard[MAX_CARDS];
		memcpy(cbRepertoryCard, m_cbRepertoryCard, sizeof(cbRepertoryCard));
		while (cbLeftCardCount > 0)
		{
			stGameEnd.add_left_cards(cbRepertoryCard[--cbLeftCardCount]);
		}
		//统计胡息
		tagHuCardInfo stHuCardInfo;
		std::vector<tagHuFanInfo> vecHuFanInfo;
		vecHuFanInfo.clear();
		memset(&stHuCardInfo, 0, sizeof(stHuCardInfo));
		
		_uint8 tmpFanNum = 1;
		_uint8 cbHuCard = (m_bDispatchHu == true) ? m_cbCurrentCard : INVALID_CARD;

		if (m_bTianHu3Ti5Kan && m_bIsTianHu3Ti5Kan)
		{
			Do3Ti5Kan(stGameEnd, wTotalHuXi, wChairID);
		}
		else
		{
			m_GameLogic.GetHuCardInfoFinal(m_cbCardIndex[wChairID], cbHuCard, m_bWeaveItemCount[wChairID], m_WeaveItemArray[wChairID], stHuCardInfo, m_wBaseHuXi, vecHuFanInfo, bZiMo, m_bDispatch, m_bHHD, true, m_bWithMaoHu);
			//更新胡息
			UpdateUserHuXiCount(wChairID);
			//统计胡番
			/*//毛胡算15胡
			if ((stHuCardInfo.cbHuXiCount + m_bWeaveHuXiCount[wChairID]) == 0)
			{
				stHuCardInfo.cbHuXiCount = 15;
				tagHuFanInfo stHuFan;
				stHuFan.wHuFanType = MT_MAO_HU;
				stHuFan.wHuFanNum = stHuCardInfo.cbHuXiCount;
				vecHuFanInfo.push_back(stHuFan);
			}*/


			wTotalHuXi = stHuCardInfo.cbFinalHuXiCount;

			for (_uint8 pos = 0; pos < vecHuFanInfo.size(); ++pos)
			{
				if (MT_MAO_HU == vecHuFanInfo[pos].wHuFanType)
				{
					//wTotalHuXi = vecHuFanInfo[pos].wHuFanXi;
				}
				else
				{
					tmpFanNum *= vecHuFanInfo[pos].wHuFanNum;
				}
			}
		}
		//计算囤数
		if((1000036 == m_wPlayID)&&(m_wHuXiToTun==1))
		{
			wTotalTunCount=wTotalHuXi;
		}
		else
		{
			wTotalTunCount = (wTotalHuXi - m_wBaseHuXi) / m_wHuXiToTun + base_money;
		}
		log.info("Game end wChairID[%d] uid[%d] bZiMo[%d] bDianPao[%d] wTotalHuXi[%d] wTotalTunCount[%d] tid[%s].\n",
			wChairID, seats[wChairID].uid, bZiMo, bDianPao, wTotalHuXi, wTotalTunCount, tid.c_str());
		//算分
		_uint32 wScore = wTotalTunCount * tmpFanNum;
		if (m_bTianHu3Ti5Kan && m_bIsTianHu3Ti5Kan)
		{
			tagHuFanInfo stHuFan;
			stHuFan.wHuFanType = MT_TIAN_HU;
			stHuFan.wHuFanNum = 0;
			vecHuFanInfo.push_back(stHuFan);
		}
		//自摸分数乘二
		else if (m_bZiMoFan && bZiMo)
		{
			tagHuFanInfo stHuFan;
			stHuFan.wHuFanType = MT_ZI_MO;
			stHuFan.wHuFanNum = 2;
			vecHuFanInfo.push_back(stHuFan);
			wScore *= 2;
			++m_UserZiMoCnt[wChairID];
		}
		if (bDianPao)
		{
			stGameEnd.set_dian_pao(bDianPao);
			stGameEnd.set_dian_pao_user(m_wCurrentUser);
			UserScore[wChairID] = wScore*m_GamePlayer;
			UserScore[m_wCurrentUser] = -wScore*m_GamePlayer;
		}
		else
		{
			UserScore[wChairID] = wScore * (m_GamePlayer - 1);
			for (_uint8 i = 0; i < m_GamePlayer; ++i)
			{
				if (i == wChairID) continue;
				UserScore[i] = -wScore;
			}
		}
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			log.info("Game End Before tid[%s] seatid[%d] uid[%d] score[%d] .\n", tid.c_str(), i, seats[i].uid, UserScore[i]);
		}
		//胡息番数打包	
		stGameEnd.set_total_huxi(stHuCardInfo.cbHuXiCount + m_bWeaveHuXiCount[wChairID]);
		stGameEnd.set_hu_xis(wTotalHuXi);
		stGameEnd.set_tuns(wTotalTunCount);
		for (_uint8 pos = 0; pos < vecHuFanInfo.size(); ++pos)
		{
			proto::game::tagFanInfo* pstFanInfo = stGameEnd.add_fan_info();
			pstFanInfo->set_fan_type(vecHuFanInfo[pos].wHuFanType);
			pstFanInfo->set_fan_num(vecHuFanInfo[pos].wHuFanNum);
		}
		//组合牌
		/*for (_uint8 j = 0; j < m_bWeaveItemCount[wChairID]; ++j)
		{
			proto::game::tagWeaveCards* pstWeaves = stGameEnd.add_weaves();
			pstWeaves->set_weave_kind(m_WeaveItemArray[wChairID][j].cbWeaveKind);
			pstWeaves->set_hu_xi(m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][j]));
			pstWeaves->set_out_card(m_WeaveItemArray[wChairID][j].cbOutCard);
			for (int k = 0; k < m_WeaveItemArray[wChairID][j].cbCardCount; ++k)
			{
				pstWeaves->add_card_list(m_WeaveItemArray[wChairID][j].cbCardList[k]);
			}
		}*/
		//手牌组合
		for (_uint8 j = 0; j < stHuCardInfo.cbWeaveCount; ++j)
		{
			proto::game::tagWeaveCards* pstWeaves = stGameEnd.add_weaves();
			_uint8 cbKind = (stHuCardInfo.WeaveItemArray[j].cbWeaveKind == ACK_WEI) ? ACK_KAN : stHuCardInfo.WeaveItemArray[j].cbWeaveKind;
			pstWeaves->set_weave_kind(cbKind);
			pstWeaves->set_hu_xi(m_GameLogic.GetWeaveHuXi(stHuCardInfo.WeaveItemArray[j]));
			for (int k = 0; k < stHuCardInfo.WeaveItemArray[j].cbCardCount; ++k)
			{
				pstWeaves->add_card_list(stHuCardInfo.WeaveItemArray[j].cbCardList[k]);
			}
		}
		//将牌
		if (stHuCardInfo.cbCardEye != 0)
		{
			proto::game::tagWeaveCards* pstWeaves = stGameEnd.add_weaves();
			pstWeaves->set_weave_kind(ACK_JIANG);
			pstWeaves->set_hu_xi(0);
			for (_uint8 k = 0; k < 2; ++k)
			{
				pstWeaves->add_card_list(stHuCardInfo.cbCardEye);
			}
		}
		for (_uint8 pos = 0; pos < m_GamePlayer; ++pos)
		{
			proto::game::tagPlayers* pstEndPlayers = stGameEnd.add_players();
			pstEndPlayers->set_seatid(pos);
			if (seats[pos].player != NULL)
			{
				pstEndPlayers->set_uid(seats[pos].player->uid);
				pstEndPlayers->set_name(seats[pos].player->name);
				pstEndPlayers->set_sex(seats[pos].player->sex);
				pstEndPlayers->set_avatar(seats[pos].player->avatar);
			}
			_uint8 cbHongCount = 0;
			_uint8 cbHeiCount = 0;
			/*if (!m_bCommonOrBest[pos] && !(pos == wChairID && m_bTianHu3Ti5Kan && m_bIsTianHu3Ti5Kan)) 
			{
				for (_uint8 i = 0; i < m_cbCardsWeaveCount[pos]; ++i) {
					proto::game::tagHandCards* pstHandCards = pstEndPlayers->add_hand_cards();
					for (_uint8 j = 0; j < m_CardsWeaveInfo[pos][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_CardsWeaveInfo[pos][i].cbCardList[j]);
						_uint8 cbCard = m_CardsWeaveInfo[pos][i].cbCardList[j];
						if (CardNum(cbCard) == 0x02 || CardNum(cbCard) == 0x07 || CardNum(cbCard) == 0x0a) cbHongCount++;
						else cbHeiCount++;
					}
				}
			}
			else */
			{
				UpdateBestCardsWeave(pos);
				for (_uint8 i = 0; i < m_cbBestWeaveCount[pos]; ++i) {
					proto::game::tagHandCards* pstHandCards = pstEndPlayers->add_hand_cards();
					for (_uint8 j = 0; j < m_BestWeaveInfo[pos][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_BestWeaveInfo[pos][i].cbCardList[j]);
						_uint8 cbCard = m_BestWeaveInfo[pos][i].cbCardList[j];
						if (CardNum(cbCard) == 0x02 || CardNum(cbCard) == 0x07 || CardNum(cbCard) == 0x0a) cbHongCount++;
						else cbHeiCount++;
					}
				}
			}
			for (_uint8 j = 0; j < m_bWeaveItemCount[pos]; ++j)
			{
				proto::game::tagWeaveCards* pstWeaveCards = pstEndPlayers->add_weaves();
				pstWeaveCards->set_weave_kind(m_WeaveItemArray[pos][j].cbWeaveKind);
				pstWeaveCards->set_card_count(m_WeaveItemArray[pos][j].cbCardCount);
				pstWeaveCards->set_center_card(m_WeaveItemArray[pos][j].cbCenterCard);
				pstWeaveCards->set_out_card(m_WeaveItemArray[pos][j].cbOutCard);
				for (_uint8 k = 0; k < m_WeaveItemArray[pos][j].cbCardCount; ++k)
				{
					pstWeaveCards->add_card_list(m_WeaveItemArray[pos][j].cbCardList[k]);
					_uint8 cbCard = m_WeaveItemArray[pos][j].cbCardList[k];
					if (CardNum(cbCard) == 0x02 || CardNum(cbCard) == 0x07 || CardNum(cbCard) == 0x0a) cbHongCount++;
					else cbHeiCount++;
				}
			}
			if (pos == wChairID && cbHuCard != INVALID_CARD)
			{
				if (CardNum(cbHuCard) == 0x02 || CardNum(cbHuCard) == 0x07 || CardNum(cbHuCard) == 0x0a) cbHongCount++;
				else cbHeiCount++;
			}
			pstEndPlayers->set_hong_count(cbHongCount);
			pstEndPlayers->set_hei_count(cbHeiCount);
			pstEndPlayers->set_total_huxi(m_bHandsHuXiCount[pos] + m_bWeaveHuXiCount[pos]);
		}
		//打鸟计算
		if (m_bDaNiao)
		{
			UserNiaoScore[wChairID] += UserScore[wChairID] * m_UserNiaoCount[wChairID];
			for (_uint8 i = 0; i < m_GamePlayer; ++i)
			{
				if (i == wChairID) continue;
				UserNiaoScore[i] -= m_UserNiaoCount[wChairID] * (std::abs(UserScore[i]));
			}
			for (_uint8 i = 0; i < m_GamePlayer; ++i)
			{
				if (i == wChairID) continue;
				UserNiaoScore[i] -= (std::abs(UserScore[i]))*m_UserNiaoCount[i];
				UserNiaoScore[wChairID] += (std::abs(UserScore[i]))*m_UserNiaoCount[i];
			}
			for (_uint8 i = 0; i < m_GamePlayer; ++i)
			{
				proto::game::tagPlayers* pstEndPlayers = stGameEnd.mutable_players(i);
				pstEndPlayers->set_niao(m_UserNiaoCount[i]);
				log.info("Game End Niao seatid[%d] uid[%d] niao[%d] niao_score[%d] tid[%s] .\n", i, seats[i].uid, m_UserNiaoCount[i], UserNiaoScore[i], tid.c_str());
			}
		}
		//飘计算
		if (m_bPiao)
		{
			if (bDianPao) //点炮时其他玩家不算飘分
			{
				UserPiaoScore[wChairID] += (m_UserPiaoScore[wChairID] + m_UserPiaoScore[m_wCurrentUser]);
				UserPiaoScore[m_wCurrentUser] -= (m_UserPiaoScore[wChairID] + m_UserPiaoScore[m_wCurrentUser]);
			}
			else
			{
				UserPiaoScore[wChairID] += m_UserPiaoScore[wChairID] * (m_GamePlayer - 1);
				for (_uint8 i = 0; i < m_GamePlayer; ++i)
				{
					if (i == wChairID) continue;
					UserPiaoScore[i] -= m_UserPiaoScore[wChairID];
				}
				for (_uint8 i = 0; i < m_GamePlayer; ++i)
				{
					if (i == wChairID) continue;
					UserPiaoScore[i] -= m_UserPiaoScore[i];
					UserPiaoScore[wChairID] += m_UserPiaoScore[i];
				}
			}
			for (_uint8 i = 0; i < m_GamePlayer; ++i)
			{
				proto::game::tagPlayers* pstEndPlayers = stGameEnd.mutable_players(i);
				pstEndPlayers->set_piao(m_UserPiaoScore[i]);
				log.info("Game End Piao seatid[%d] uid[%d] piao[%d] piao_score[%d] tid[%s] .\n", i, seats[i].uid, m_UserPiaoScore[i], UserPiaoScore[i], tid.c_str());
			}
		}

		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			m_UserAllPiaoScore[i] += UserPiaoScore[i];
			UserScore[i] = UserScore[i] + UserNiaoScore[i] + UserPiaoScore[i];
			proto::game::tagPlayers* pstEndPlayers = stGameEnd.mutable_players(i);
			if (seats[i].player != NULL)
			{
				seats[i].player->money += UserScore[i];
				pstEndPlayers->set_money(seats[i].player->money);
			}
			pstEndPlayers->set_total_score(UserScore[i]);
		}
		//广播结算
		std::map<int, Player*>::iterator iter = players.begin();
		for (; iter != players.end(); ++iter)
		{
			Player* player = iter->second;
			if (NULL == player) continue;
			_tint32 wCurChairID = player->seatid;
			if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
			{
				stGameEnd.set_off_count(m_cbOfflineCount[wCurChairID]);
			}
			Ppacket ppack;
			stGameEnd.SerializeToString(&ppack.body);
			ppack.pack(SERVER_GAME_END_BC);
			unicast(player, ppack.data);
			log.debug("uid[%d] tid[%s] stGameEnd[%s] .\n", player->uid, tid.c_str(), stGameEnd.DebugString().c_str());
			//保存离线数据
			save_offline_info(wCurChairID, SERVER_GAME_END_BC, stGameEnd.SerializeAsString());
		}
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].player != NULL)
			{
				log.info("Game End Totalscore tid[%s] seatid[%d] uid[%d] score[%d] money[%d] .\n", tid.c_str(), i, seats[i].player->uid, UserScore[i], seats[i].player->money);
			}
		}
		//数据统计
		m_cbHuPaiCount[wChairID]++;
		if (bDianPao) m_cbDianPaoCount[m_wCurrentUser]++;
		if (wTotalHuXi > m_wBestHuXi[wChairID]) m_wBestHuXi[wChairID] = wTotalHuXi;
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			m_wTotalScore[i] += UserScore[i];
			if (UserScore[i] > (_tint32)m_wBestScore[i])
			{
				m_wBestScore[i] = UserScore[i];
			}
		}
		tagGain stGain;
		for( _uint8 i = 0; i < GAME_PLAYER && i < m_GamePlayer; ++i )
		{
			stGain.Gain[i] = UserScore[i];
		}
		m_vecGains.push_back(stGain);
		for(_uint8 i = 0; i < GAME_PLAYER && i < m_GamePlayer; ++i )
		{
			log.info("Data Count chairid[%d] m_cbHuPaiCount[%d] m_wBestHuXi[%d] m_wBestFanShu[%d] m_wTotalScore[%d] tid[%s] .\n",
				i, m_cbHuPaiCount[i], m_wBestHuXi[i], m_wBestFanShu[i], m_wTotalScore[i], tid.c_str());
		}
		//保存录像
		save_game_end_video(SERVER_GAME_END_BC, stGameEnd.SerializeAsString());
		//更换赢家为庄家
		m_wHuangZhuangCount = 0;
		m_wBankerUser = wChairID;
		//更新数据
		update_user_info_to_datasvr(UserScore);
	}
	else
	{
		_tint32 UserScore[GAME_PLAYER] = { 0 };						//玩牌得分
		m_wHuangZhuangCount++;
		//黄庄后下家坐庄
		m_wBankerUser = (m_wBankerUser + 1) % m_GamePlayer;
		//黄庄结算
		proto::game::AckGameEnd stGameEnd;
		stGameEnd.set_end_type(cbEndState);
		//牌墩牌
		_uint8 cbLeftCardCount = m_bLeftCardCount;
		_uint8 cbRepertoryCard[MAX_CARDS];
		memcpy(cbRepertoryCard, m_cbRepertoryCard, sizeof(cbRepertoryCard));
		while (cbLeftCardCount > 0)
		{
			stGameEnd.add_left_cards(cbRepertoryCard[--cbLeftCardCount]);
		}
		for( _uint8 i=0;i<GAME_PLAYER && i < m_GamePlayer;++i)
		{
			if(seats[i].player!=NULL) seats[i].player->money+=UserScore[i];
			log.info( "Game End HuangZhuang seatid[%d] UserScore[%d] tid[%s] .\n", i, UserScore[i], tid.c_str() );
		}
		tagGain stGain;
		for( _uint8 i = 0; i < GAME_PLAYER && i < m_GamePlayer; ++i )
		{
			stGain.Gain[i] = UserScore[i];
		}
		m_vecGains.push_back(stGain);
		
		//玩家剩余手牌
		for(_uint8 pos=0;pos<GAME_PLAYER && pos < m_GamePlayer;++pos)
		{
			proto::game::tagPlayers* pstEndPlayers = stGameEnd.add_players();
			pstEndPlayers->set_seatid(pos);
			if (seats[pos].player != NULL)
			{
				pstEndPlayers->set_money(seats[pos].player->money);
			}
			if (m_bCommonOrBest[pos] == false) {
				for (_uint8 i = 0; i < m_cbCardsWeaveCount[pos]; ++i) {
					proto::game::tagHandCards* pstHandCards = pstEndPlayers->add_hand_cards();
					for (_uint8 j = 0; j < m_CardsWeaveInfo[pos][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_CardsWeaveInfo[pos][i].cbCardList[j]);
					}
				}
			}
			else {
				for (_uint8 i = 0; i < m_cbBestWeaveCount[pos]; ++i) {
					proto::game::tagHandCards* pstHandCards = pstEndPlayers->add_hand_cards();
					for (_uint8 j = 0; j < m_BestWeaveInfo[pos][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_BestWeaveInfo[pos][i].cbCardList[j]);
					}
				}
			}
		}
		//广播结算
		std::map<int, Player*>::iterator iter = players.begin();
		for (; iter != players.end(); ++iter)
		{
			Player* player = iter->second;
			if (NULL == player) continue;
			_tint32 wCurChairID = player->seatid;
			if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
			{
				stGameEnd.set_off_count(m_cbOfflineCount[wCurChairID]);
			}
			Ppacket ppack;
			stGameEnd.SerializeToString(&ppack.body);
			ppack.pack(SERVER_GAME_END_BC);
			unicast(player, ppack.data);
			log.debug("uid[%d] tid[%s] stGameEnd[%s] .\n", player->uid, tid.c_str(), stGameEnd.DebugString().c_str());
			//保存离线数据
			save_offline_info(wCurChairID, SERVER_GAME_END_BC, stGameEnd.SerializeAsString());
		}
		//保存录像
		save_game_end_video(SERVER_GAME_END_BC, stGameEnd.SerializeAsString());
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].player != NULL)
			{
				log.info("Game End HuangZhuang tid[%s] seatid[%d] uid[%d] score[%d] money[%d] .\n", tid.c_str(), i, seats[i].player->uid, UserScore[i], seats[i].player->money);
			}
		}
		//更新数据
		update_user_info_to_datasvr(UserScore);
	}

	//发送录像
	ev_timer_again(zjh.loop, &send_videodata_timer);
	m_cbCurTimer = 8;
	//游戏初始化
	state = GAME_FREE;
	Reset();
	for (int i = 0; i < m_GamePlayer; i++) seats[i].reset();
	ev_timer_again(zjh.loop, &preready_timer);
	m_cbCurTimer = 2;
	//保存数据到redis
	SerializeTableData();
	return true;
}

//用户出牌
bool Table::OnUserOutCard(Player *player)
{
	time_t begin = time(NULL);
	
	proto::game::ReqOutCard stReqOutCard;
	stReqOutCard.ParseFromString(player->client->ppacket.body);
	log.debug("uid[%d] tid[%s] stReqOutCard[%s] .\n", player->uid, tid.c_str(), stReqOutCard.DebugString().c_str());
	_uint8 cbCardData = stReqOutCard.out_card();	//用户出的牌数据
	_uint8 cbRow = stReqOutCard.row();	//出牌的行下标
	_uint8 cbCol = stReqOutCard.col();	//出牌的列下标
	_uint8 wChairID = player->seatid;
	if (!m_GameLogic.IsValidCard(cbCardData))
	{
		log.error("On user out card cbCardData[%d] error .\n", cbCardData);
		return false;
	}
	if (wChairID != m_wCurrentUser || !m_bOutCard || wChairID < 0 || wChairID >= m_GamePlayer)
	{
		log.error("On user out card wChairID[%d] m_wCurrentUser[%d] m_bOutCard[%d] error .\n",
			wChairID, m_wCurrentUser, m_bOutCard);
		return false;
	}

	//一坎牌不能打
	if (m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(cbCardData)] >= 3)
	{
		log.error("On user out card wChairID[%d] cbCardData[0x%02x] more than 3 .\n", wChairID, cbCardData);
		return false;
	}

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID], cbCardData) == false)
	{
		log.error("On user out card remove cbCardData[%d] error .\n ", cbCardData);
		return false;
	}
	//更新胡息
	_uint8 cbCardList[3] = { 0 };
	cbCardList[0] = cbCardData;
	UpdateCardsWeave(wChairID, cbCardList, 1, cbRow, cbCol);
	//广播出牌
	std::map<int, Player*>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter)
	{
		Player* player = iter->second;
		if (NULL == player) continue;
		_tint32 wCurChairID = player->seatid;

		proto::game::AckOutCard stOutCard;
		stOutCard.set_seatid(wChairID);
		stOutCard.set_out_card(cbCardData);
		if (wCurChairID == wChairID)
		{
			if (m_bCommonOrBest[wCurChairID] == false) {
				for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stOutCard.add_hand_cards();
					for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			else {
				for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stOutCard.add_hand_cards();
					for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			std::vector<_uint8> vecListenCards;
			if (IsListen(wCurChairID, vecListenCards))
			{
				for (_uint8 i = 0; i < vecListenCards.size(); ++i)
					stOutCard.add_listen_cards(vecListenCards[i]);
			}
			_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
			stOutCard.set_total_huxi(cbTotalHuXi);
		}
		if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
		{
			stOutCard.set_off_count(m_cbOfflineCount[wCurChairID]);
		}
		Ppacket ppack;
		stOutCard.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_OUT_CARD_RES);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stOutCard[%s] .\n", player->uid, tid.c_str(), stOutCard.DebugString().c_str());
		//保存离线数据
		save_offline_info(wCurChairID, SERVER_PHZ_OUT_CARD_RES, stOutCard.SerializeAsString());
	}

	log.info("On user out card Succ wChairID[%d] uid[%d] cbCardData[0x%02x] cbRow[%d] cbCol[%d] tid[%s] .\n", wChairID, seats[wChairID].uid, cbCardData, cbRow, cbCol, tid.c_str());

	//设置变量
	m_bFirstCard = false;
	m_bOutCard = false;
	m_bDispatch = false;
	m_bCardOperated = false;
	m_bIsWeiTiCard = false;
	m_wCurrentUser = wChairID;
	m_cbCurrentCard = cbCardData;
	m_wOutCardUser = wChairID;
	m_cbOutCardData = cbCardData;

	//臭牌记录
	m_bPengAbandon[wChairID][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)] = true;
	m_bChiAbandon[wChairID][m_GameLogic.SwitchToCardIndex(m_cbOutCardData)] = true;
	//动张记录
	for (_uint8 i = 0; i < GAME_PLAYER; ++i) {
		if (i == wChairID) continue;
		for (_uint8 j = 0; j < m_bWeaveItemCount[i]; ++j) {
			if (m_WeaveItemArray[i][j].cbWeaveKind == ACK_WEI && m_WeaveItemArray[i][j].cbCardList[0] == cbCardData) {
				m_bDongZhang[wChairID] = true;
				break;
			}
		}
	}
	//动作判断
	ev_timer_again(zjh.loop, &out_delay_timer);
	m_cbCurTimer = 6;
	//保存录像
	save_out_card_video(wChairID, cbCardData);
	//更新组合后打牌
	if (m_bAfterWeave)
	{
		UpdateWeaveOutCard(wChairID, cbCardData);
		m_bAfterWeave = false;
	}
	//保存数据到redis
	SerializeTableData();
	
	time_t end = time(NULL);
	int total = end - begin;
	if (total >= 1)
	{
		log.error("slow cmd:[%s] second[%d]\n",__FUNCTION__, total);
	}
	return true;
}

//吃牌
bool Table::UserOperateChi(_tint32 wTargetUser)
{
	//吃牌操作
	if (m_cbUserChiKind[wTargetUser] == 0)
	{
		log.error("user operate chi error m_cbUserChiKind[0x%02x] wrong .\n", m_cbUserChiKind[wTargetUser]);
		return false;
	}
	//第一层
	_uint8 j = 0;
	for (; j < m_cbUserChiCardCount[wTargetUser]; j++)
	{
		if (m_UserChiCardInfo[wTargetUser][j].cbChiKind == m_cbUserChiKind[wTargetUser]) break;
	}
	if (j == m_cbUserChiCardCount[wTargetUser])
	{
		log.error("user operate chi error m_cbUserChiKind[0x%02x] no find .\n", m_cbUserChiKind[wTargetUser]);
		return false;
	}

	//第二层
	bool bSecondFind = false;
	bool bThirdFind = false;
	_uint8 k = 0;
	_uint8 m = 0;
	if (m_cbUserSecondChiKind[wTargetUser] == 0 && m_UserChiCardInfo[wTargetUser][j].cbSecondCount != 0)
	{
		log.error("user operate chi error m_cbUserSecondChiKind[0x%02x] wrong .\n", m_cbUserSecondChiKind[wTargetUser]);
		return false;
	}
	if (m_cbUserSecondChiKind[wTargetUser] != 0)
	{
		for (; k < m_UserChiCardInfo[wTargetUser][j].cbSecondCount; ++k)
		{
			if (m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbChiKind == m_cbUserSecondChiKind[wTargetUser]) break;
		}
		if (k == m_UserChiCardInfo[wTargetUser][j].cbSecondCount)
		{
			log.error("user operate chi error m_cbUserSecondChiKind[0x%02] no find .\n", m_cbUserSecondChiKind[wTargetUser]);
			return false;
		}
		bSecondFind = true;
		//第三层
		if (m_cbUserThirdChiKind[wTargetUser] == 0 && m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbThirdCount != 0)
		{
			log.error("user operate chi error m_cbUserThirdChiKind[0x%02x] wrong .\n", m_cbUserThirdChiKind[wTargetUser]);
			return false;
		}
		if (m_cbUserThirdChiKind[wTargetUser] != 0)
		{
			for (; m < m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbThirdCount; ++m)
			{
				if (m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbChiKind == m_cbUserThirdChiKind[wTargetUser]) break;
			}
			if (m == m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbThirdCount)
			{
				log.error("user operate chi error m_cbUserThirdChiKind[0x%02x] no find .\n", m_cbUserThirdChiKind[wTargetUser]);
				return false;
			}
			bThirdFind = true;
		}
	}

	//设置组合 第一层
	m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)]++;
	_uint8 cbIndex = m_bWeaveItemCount[wTargetUser]++;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount = 3;
	m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind = ACK_CHI;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard = m_cbCurrentCard;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0] = m_UserChiCardInfo[wTargetUser][j].cbCardData[0];
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1] = m_UserChiCardInfo[wTargetUser][j].cbCardData[1];
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2] = m_UserChiCardInfo[wTargetUser][j].cbCardData[2];
	//删除扑克
	for (_uint8 l = 0; l < 3; l++)
	{
		_uint8 cbCurrentCard = m_UserChiCardInfo[wTargetUser][j].cbCardData[l];
		m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(cbCurrentCard)]--;
	}
	//更新手牌组合和胡息
	UpdateCardsWeave(wTargetUser, m_UserChiCardInfo[wTargetUser][j].cbCardData, 3);
	if (bSecondFind)
	{
		//设置组合 第二层
		_uint8 cbIndex = m_bWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount = 3;
		m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind = ACK_CHI;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard = m_cbCurrentCard;
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0] = m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbCardData[0];
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1] = m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbCardData[1];
		m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2] = m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbCardData[2];
		//删除扑克 
		for (_uint8 l = 0; l < 3; l++)
		{
			_uint8 cbCurrentCard = m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbCardData[l];
			m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(cbCurrentCard)]--;
		}
		//更新手牌组合和胡息
		UpdateCardsWeave(wTargetUser, m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbCardData, 3);
		if (bThirdFind)
		{
			//设置组合 第三层
			_uint8 cbIndex = m_bWeaveItemCount[wTargetUser]++;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount = 3;
			m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind = ACK_CHI;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard = m_cbCurrentCard;
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0] = m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbCardData[0];
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1] = m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbCardData[1];
			m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2] = m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbCardData[2];
			//删除扑克 
			for (_uint8 l = 0; l < 3; l++)
			{
				_uint8 cbCurrentCard = m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbCardData[l];
				m_cbCardIndex[wTargetUser][m_GameLogic.SwitchToCardIndex(cbCurrentCard)]--;
			}
			//更新手牌组合和胡息
			UpdateCardsWeave(wTargetUser, m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbCardData, 3);
		}
	}
	//更新胡息
	//UpdateUserHuXiCount(wTargetUser);
	//广播吃牌
	std::map<int, Player*>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter)
	{
		Player* player = iter->second;
		if (NULL == player) continue;
		_tint32 wCurChairID = player->seatid;

		proto::game::AckOperateCard stOperateCard;
		stOperateCard.set_ack_code(ACK_CHI);
		stOperateCard.set_seatid(wTargetUser);
		stOperateCard.set_action_card(m_cbCurrentCard);
		proto::game::tagHandCards* pstChiResults = stOperateCard.add_chi_results();
		for (_uint8 l = 0; l < 3; l++)
		{
			pstChiResults->add_card(m_UserChiCardInfo[wTargetUser][j].cbCardData[l]);
		}
		if (bSecondFind)
		{
			proto::game::tagHandCards* pstChiResults = stOperateCard.add_chi_results();
			for (_uint8 l = 0; l < 3; l++)
			{
				pstChiResults->add_card(m_UserChiCardInfo[wTargetUser][j].SecondData[k].cbCardData[l]);
			}
		}
		if (bThirdFind)
		{
			proto::game::tagHandCards* pstChiResults = stOperateCard.add_chi_results();
			for (_uint8 l = 0; l < 3; l++)
			{
				pstChiResults->add_card(m_UserChiCardInfo[wTargetUser][j].SecondData[k].ThirdData[m].cbCardData[l]);
			}
		}
		if (wCurChairID == wTargetUser)
		{
			if (m_bCommonOrBest[wCurChairID] == false) {
				for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
					for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			else {
				for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
					for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			std::vector<_uint8> vecListenCards;
			/*if (IsListen(wCurChairID, vecListenCards))
			{
				for (_uint8 i = 0; i < vecListenCards.size(); ++i)
					stOperateCard.add_listen_cards(vecListenCards[i]);
			}*/
			_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
			stOperateCard.set_total_huxi(cbTotalHuXi);
		}
		stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wTargetUser]);
		if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
		{
			stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
		}
		Ppacket ppack;
		stOperateCard.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
		//保存离线数据
		save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
	}
	log.info("user operate chi chairid[%d] uid[%d] m_cbCurrentCard[0x%02x] tid[%s].\n", wTargetUser, seats[wTargetUser].uid, m_cbCurrentCard, tid.c_str());
	//保存录像
	save_operate_video(wTargetUser, ACK_CHI, m_cbCurrentCard);
	//操作清零
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));
	log.info("user operate chi clear operate .\n");
	m_bCardOperated = true;
	//出牌提示
	SendOutCardNotify(wTargetUser, true);
	m_bAfterWeave = true;
	return true;
}

//碰牌
bool Table::UserOperatePeng(_tint32 wTargetUser)
{
	_uint8 cbRemoveIndex = m_GameLogic.SwitchToCardIndex(m_cbCurrentCard);
	_uint8 cbRemoveCount = m_cbCardIndex[wTargetUser][cbRemoveIndex];
	m_cbCardIndex[wTargetUser][cbRemoveIndex] = 0;
	//设置组合
	_uint8 cbIndex = m_bWeaveItemCount[wTargetUser]++;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardCount = 3;
	m_WeaveItemArray[wTargetUser][cbIndex].cbWeaveKind = ACK_PENG;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCenterCard = m_cbCurrentCard;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[0] = m_cbCurrentCard;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[1] = m_cbCurrentCard;
	m_WeaveItemArray[wTargetUser][cbIndex].cbCardList[2] = m_cbCurrentCard;
	//更新胡息
	//UpdateUserHuXiCount(wTargetUser);
	UpdateCardsWeave(wTargetUser, m_WeaveItemArray[wTargetUser][cbIndex].cbCardList, cbRemoveCount);
	//广播碰牌
	std::map<int, Player*>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter)
	{
		Player* player = iter->second;
		if (NULL == player) continue;
		_tint32 wCurChairID = player->seatid;

		proto::game::AckOperateCard stOperateCard;
		stOperateCard.set_ack_code(ACK_PENG);
		stOperateCard.set_seatid(wTargetUser);
		stOperateCard.set_action_card(m_cbCurrentCard);
		stOperateCard.set_remove_count(cbRemoveCount);
		if (wCurChairID == wTargetUser)
		{
			if (m_bCommonOrBest[wCurChairID] == false) {
				for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
					for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			else {
				for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
					proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
					for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
						pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
					}
				}
			}
			std::vector<_uint8> vecListenCards;
			/*if (IsListen(wCurChairID, vecListenCards))
			{
				for (_uint8 i = 0; i < vecListenCards.size(); ++i)
					stOperateCard.add_listen_cards(vecListenCards[i]);
			}*/
			_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
			stOperateCard.set_total_huxi(cbTotalHuXi);
		}
		stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wTargetUser]);
		if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
		{
			stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
		}
		Ppacket ppack;
		stOperateCard.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
		//保存离线数据
		save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
	}
	log.info("user operate peng chairid[%d] uid[%d] m_cbCurrentCard[0x%02x] tid[%s].\n", wTargetUser, seats[wTargetUser].uid, m_cbCurrentCard, tid.c_str());
	//保存录像
	save_operate_video(wTargetUser, ACK_PENG, m_cbCurrentCard);
	//操作清零
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));
	log.info("user operate peng clear operate .\n");
	m_bCardOperated = true;
	//出牌提示
	SendOutCardNotify(wTargetUser, true);
	m_bAfterWeave = true;
	return true;
}

//操作后判断跑牌
bool Table::EstimatePaoCard()
{
	//跑牌判断
	_tint32 wPaoChairID = INVALID_CHAIR;
	m_bPaoCard = false;
	_uint8 cbCenterCard = m_cbCurrentCard;
	for (_uint8 i = 0; i < m_GamePlayer; i++)
	{
		if (i == m_wCurrentUser) continue;
		if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[i], cbCenterCard) == true)
		{
			_tint32 wActionUser = i;
			_uint8 cbRemoveIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
			_uint8 cbRemoveCount = m_cbCardIndex[wActionUser][cbRemoveIndex];
			m_cbCardIndex[wActionUser][cbRemoveIndex] = 0;
			//设置组合
			_uint8 cbIndex = m_bWeaveItemCount[wActionUser]++;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount = 4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind = ACK_PAO;
			m_WeaveItemArray[wActionUser][cbIndex].cbCenterCard = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[0] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[1] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[2] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3] = cbCenterCard;
			//更新胡息
			//UpdateUserHuXiCount(wActionUser);
			UpdateCardsWeave(wActionUser, m_WeaveItemArray[wActionUser][cbIndex].cbCardList, cbRemoveCount);
			//广播跑牌
			std::map<int, Player*>::iterator iter = players.begin();
			for (; iter != players.end(); ++iter)
			{
				Player* player = iter->second;
				if (NULL == player) continue;
				_tint32 wCurChairID = player->seatid;

				proto::game::AckOperateCard stOperateCard;
				stOperateCard.set_ack_code(ACK_PAO);
				stOperateCard.set_seatid(wActionUser);
				stOperateCard.set_action_card(cbCenterCard);
				stOperateCard.set_remove_count(cbRemoveCount);
				if (wCurChairID == wActionUser)
				{
					if (m_bCommonOrBest[wCurChairID] == false) {
						for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					else {
						for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					std::vector<_uint8> vecListenCards;
					if (IsListen(wCurChairID, vecListenCards))
					{
						for (_uint8 i = 0; i < vecListenCards.size(); ++i)
							stOperateCard.add_listen_cards(vecListenCards[i]);
					}
					_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
					stOperateCard.set_total_huxi(cbTotalHuXi);
				}
				stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wActionUser]);
				if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
				{
					stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
				}
				Ppacket ppack;
				stOperateCard.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
				unicast(player, ppack.data);
				log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
				//保存离线数据
				save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
			}
			log.info("estimate pao card pao wActionUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wActionUser, cbCenterCard, tid.c_str());
			//保存录像
			save_operate_video(wActionUser, ACK_PAO, cbCenterCard);
			wPaoChairID = i;
			m_bPaoCard = true;
			m_bCardOperated = true;
			break;
		}
	}
	//跑牌转换
	if (m_bPaoCard == false)
	{
		for (_uint8 i = 0; i < m_GamePlayer; i++)
		{
			for (_uint8 cbIndex = 0; cbIndex < m_bWeaveItemCount[i]; cbIndex++)
			{
				_uint8 cbWeaveKind = m_WeaveItemArray[i][cbIndex].cbWeaveKind;
				_uint8 cbWeaveCard = m_WeaveItemArray[i][cbIndex].cbCardList[0];
				bool bChangeWeave = false;
				if (cbCenterCard == cbWeaveCard)
				{
					if (cbWeaveKind == ACK_WEI && i != m_wCurrentUser) bChangeWeave = true;
					else if (cbWeaveKind == ACK_PENG && m_bDispatch) bChangeWeave = true; //碰牌转跑必须是派发的牌
				}
				if (bChangeWeave == false) continue;
				_tint32 wActionUser = i;
				//设置组合
				m_WeaveItemArray[wActionUser][cbIndex].cbCardCount = 4;
				m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind = ACK_PAO;
				m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3] = cbCenterCard;
				//更新胡息
				UpdateUserHuXiCount(wActionUser);
				//广播跑牌
				std::map<int, Player*>::iterator iter = players.begin();
				for (; iter != players.end(); ++iter)
				{
					Player* player = iter->second;
					if (NULL == player) continue;
					_tint32 wCurChairID = player->seatid;

					proto::game::AckOperateCard stOperateCard;
					stOperateCard.set_ack_code(ACK_PAO);
					stOperateCard.set_seatid(wActionUser);
					stOperateCard.set_action_card(cbCenterCard);
					stOperateCard.set_remove_count(0);
					if (wCurChairID == wActionUser)
					{
						if (m_bCommonOrBest[wCurChairID] == false) {
							for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						else {
							for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						std::vector<_uint8> vecListenCards;
						if (IsListen(wCurChairID, vecListenCards))
						{
							for (_uint8 i = 0; i < vecListenCards.size(); ++i)
								stOperateCard.add_listen_cards(vecListenCards[i]);
						}
						_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
						stOperateCard.set_total_huxi(cbTotalHuXi);
					}
					stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wActionUser]);
					if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
					{
						stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
					}
					Ppacket ppack;
					stOperateCard.SerializeToString(&ppack.body);
					ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
					unicast(player, ppack.data);
					log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
					//保存离线数据
					save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
				}
				log.info("estimate pao card weipengpao wActionUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wActionUser, cbCenterCard, tid.c_str());
				//保存录像
				save_operate_video(wActionUser, ACK_PAO, cbCenterCard);
				wPaoChairID = i;
				m_bPaoCard = true;
				m_bCardOperated = true;
				break;
			}
			if (m_bPaoCard == true) break;
		}
	}

	//操作清零
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));
	log.info("estimate pao card clear operate .\n");

	//跑后判断
	if (m_bPaoCard && wPaoChairID != INVALID_CHAIR)
	{
		//出牌提示
		if (IsAllowOutCard(wPaoChairID))
		{
			SendOutCardNotify(wPaoChairID, true);
			m_bAfterWeave = true;
		}
		else
		{
			m_wCurrentUser = (wPaoChairID + 1) % m_GamePlayer;
			DispatchCardData(m_wCurrentUser);
		}
		return true;
	}
	return false;
}

//单操作
bool Table::DoSingleOperate(_tint32 wTargetUser, _uint8 cbTargetAction)
{
	log.info("Do single operate wTargetUser[%d] uid[%d] cbTargetAction[%d] tid[%s].\n", wTargetUser, seats[wTargetUser].uid, cbTargetAction, tid.c_str());
	switch (cbTargetAction)
	{
	case ACK_CHIHU:
	{
		GameEnd(wTargetUser);
	}break;
	case ACK_PENG:
	{
		UserOperatePeng(wTargetUser);
	}break;
	case ACK_CHI:
	{
		UserOperateChi(wTargetUser);
	}break;
	case ACK_NULL:
	{
		if ((m_bUserAction[wTargetUser] & ACK_CHI) || (m_bUserAction[wTargetUser] & ACK_PENG))
		{
			//臭牌记录
			if (m_bUserAction[wTargetUser] & ACK_PENG)//臭偎记录
			{
				m_bChouWeiCard[wTargetUser][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;
				m_bPengAbandon[wTargetUser][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;//臭牌记录
			}
			if (m_bUserAction[wTargetUser] & ACK_CHI)
			{
				m_bChiAbandon[wTargetUser][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;//臭牌记录
			}
		}
		//如果是提偎跑胡
		if (m_bTiWeiPaoHu)
		{
			m_bTiWeiPaoHu = false;
			if (m_bWeiHu) //偎胡
			{
				m_bWeiHu = false;
				SendOutCardNotify(wTargetUser, true);
				m_bAfterWeave = true;
			}
			else if (IsAllowOutCard(wTargetUser))
			{
				SendOutCardNotify(wTargetUser, true);
				m_bAfterWeave = true;
			}
			else
			{
				m_wCurrentUser = (m_wCurrentUser + 1) % m_GamePlayer;
				DispatchCardData(m_wCurrentUser);
			}
			return true;
		}
		else
		{
			//操作后判断跑牌
			if (EstimatePaoCard() == true) return true;
			else
			{
				//没人要，更新弃牌信息
				UpdateAbandonCard();
				m_wCurrentUser = (m_wCurrentUser + 1) % m_GamePlayer;
				DispatchCardData(m_wCurrentUser);
			}
		}
	}break;
	default:
		break;
	}

	return true;
}

//多操作
bool Table::DoMultiOperate(_tint32 wTargetUser, _uint8 cbTargetAction)
{
	log.info("Do multi operate wTargetUser[%d] uid[%d] cbTargetAction[%d] tid[%s] .\n", wTargetUser, seats[wTargetUser].uid, cbTargetAction, tid.c_str());
	//操作过牌
	if (cbTargetAction == ACK_NULL)
	{
		//判断最优
		_tint32 wFirstUser = INVALID_CHAIR;
		bool bCanOperate = IsFirstUser(wTargetUser, wFirstUser);
		//预操作提示
		if (!bCanOperate && wFirstUser != INVALID_CHAIR)
		{
			if (seats[wFirstUser].player != NULL)
			{
				Ppacket ppack;
				ppack.pack(SERVER_PHZ_PRE_OPERATE_NOTIFY);
				unicast(seats[wFirstUser].player, ppack.data);
			}
		}
		log.info(" Do multi operate wTargetUser[%d] cbTargetAction[0x%02x] bCanOperate[%d] wFirstUser[%d] tid[%s] .\n", wTargetUser, cbTargetAction, bCanOperate, wFirstUser, tid.c_str());
		HandlerChouCard();
	}
	//处理操作优先级
	_tint32 cbSortChairID = m_GamePlayer;
	_uint32	wSortCodes[GAME_PLAYER] = { 0 };
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		_tint32 wTempChairID = (m_wCurrentUser + i) % m_GamePlayer;
		if (m_bUserAction[wTempChairID] == ACK_NULL) continue;
		_uint8 cbCode = 0;
		if (!m_bResponse[wTempChairID]) //如果没有操作，取出最优操作
		{
			if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_CHIHU) cbCode = ACK_CHIHU;
			else if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_PENG) cbCode = ACK_PENG;
			else if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_CHI) cbCode = ACK_CHI;
		}
		else if (m_bResponse[wTempChairID] && m_bPerformAction[wTempChairID] != ACK_NULL) //操作过后取操作的
		{
			cbCode = m_bPerformAction[wTempChairID];
		}
		wSortCodes[wTempChairID] = (cbCode * 10 + cbSortChairID) * 10 + wTempChairID;
		if (m_bResponse[wTempChairID] && m_bPerformAction[wTempChairID] == ACK_NULL) wSortCodes[wTempChairID] = 0;
		cbSortChairID--;
	}
	//排序
	for (_uint8 m = 0; m < m_GamePlayer - 1; ++m)
	{
		for (_uint8 n = m + 1; n < m_GamePlayer; ++n)
		{
			if (wSortCodes[m] < wSortCodes[n])
			{
				_uint32 cbTemp = wSortCodes[m];
				wSortCodes[m] = wSortCodes[n];
				wSortCodes[n] = cbTemp;
			}
		}
	}
	//判断最优
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (wSortCodes[i] == 0) continue;
		_tint32 wTempChairID = wSortCodes[i] % 10;
		if (!m_bResponse[wTempChairID]) break;
		if (m_bResponse[wTempChairID] && m_bPerformAction[wTempChairID] != ACK_NULL)
		{
			if (m_bPerformAction[wTempChairID] == ACK_CHIHU)
			{
				GameEnd(wTempChairID);
				return true;
			}
			else if (m_bPerformAction[wTempChairID] == ACK_PENG)
			{
				UserOperatePeng(wTempChairID);
				return true;
			}
			else if (m_bPerformAction[wTempChairID] == ACK_CHI)
			{
				UserOperateChi(wTempChairID);
				return true;
			}
		}
	}
	_uint8 cbCurOperateCount = 0;
	_uint8 cbTotalOperateCount = 0;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (m_bUserAction[i] != ACK_NULL) cbTotalOperateCount++;
		if (m_bResponse[i]) cbCurOperateCount++;
	}
	if (cbCurOperateCount == cbTotalOperateCount)  //多操作结束
	{
		log.info("Do multi operate multiOpetate over cbTotalOperateCount[%d] all pass .\n ", cbTotalOperateCount);
		//全部过牌，所有过得人吃碰臭牌记录
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (m_bUserAction[i] != ACK_NULL && m_bResponse[i] && m_bPerformAction[i] == ACK_NULL)
			{
				if (m_bUserAction[i] & ACK_PENG)//臭偎记录
				{
					m_bChouWeiCard[i][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;
					m_bPengAbandon[i][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;//臭牌记录
					log.info(" Do multi operate choupeng chairid[%d] card[0x%02x] tid[%s] .\n", i, m_cbCurrentCard, tid.c_str());
				}
				if (m_bUserAction[i] & ACK_CHI)//臭偎记录
				{
					m_bChiAbandon[i][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;//臭牌记录
					log.info(" Do multi operate chouchi chairid[%d] card[0x%02x] tid[%s] .\n", i, m_cbCurrentCard, tid.c_str());
				}
			}
		}
		//操作后判断跑牌
		if (EstimatePaoCard() == true)
		{
			return true;
		}
		else
		{
			//没人要，更新弃牌信息
			UpdateAbandonCard();
			m_wCurrentUser = (m_wCurrentUser + 1) % m_GamePlayer;
			DispatchCardData(m_wCurrentUser);
		}
		return true;
	}

	return false;
}

//用户预操作
bool Table::OnUserPreOperateCard(Player* player)
{
	proto::game::ReqPreOperate stReqPreOp;
	stReqPreOp.ParseFromString(player->client->ppacket.body);
	log.debug("uid[%d] tid[%s] stReqPreOp[%s] .\n", player->uid, tid.c_str(), stReqPreOp.DebugString().c_str());
	_uint8 cbOperateCode = stReqPreOp.operate_type();//动作代码
	_tint32 wChairID = player->seatid;
	log.info("on user pre operate para wChairID[%d] uid[%d] cbOperateCode[0x%02x] tid[%s] .\n", wChairID, seats[wChairID].uid, cbOperateCode, tid.c_str());
	if (wChairID < 0 || wChairID >= m_GamePlayer)
	{
		log.error("on user pre operate card error wChairID[%d] wrong.\n", wChairID);
		return false;
	}

	if ((m_bUserAction[wChairID] == ACK_NULL)
		|| (m_bResponse[wChairID] == true)
		|| ((cbOperateCode != ACK_NULL) && (m_bUserAction[wChairID] & cbOperateCode) == 0))
	{
		log.error("on user operate card error wChairID[%d] m_bResponse[%d] m_bUserAction[0x%02x] cbOperateCode[0x%02x] tid[%s] .\n", wChairID, m_bResponse[wChairID], m_bUserAction[wChairID], cbOperateCode, tid.c_str());
		return false;
	}
	
	//标记臭牌
	if (cbOperateCode == ACK_NULL)
	{
		_uint8 cardIndex = m_GameLogic.SwitchToCardIndex(m_cbCurrentCard);
		if (m_bUserAction[wChairID] & ACK_PENG) //臭偎记录
		{
			m_bChouWeiCard[wChairID][cardIndex] = true;
			m_bPengAbandon[wChairID][cardIndex] = true; //臭牌记录
			log.info("[%s] choupeng chairid[%d] card[0x%02x] tid[%s] .\n", __FUNCTION__, wChairID, m_cbCurrentCard, tid.c_str());
		}
		if (m_bUserAction[wChairID] & ACK_CHI) //臭偎记录
		{
			m_bChiAbandon[wChairID][cardIndex] = true; //臭牌记录
			log.info("[%s] chouchi chairid[%d] card[0x%02x] tid[%s] .\n", __FUNCTION__, wChairID, m_cbCurrentCard, tid.c_str());
		}
	}
	else if (cbOperateCode == ACK_CHI)
	{
		_uint8 cardIndex = m_GameLogic.SwitchToCardIndex(m_cbCurrentCard);
		if (m_bUserAction[wChairID] & ACK_PENG) //臭偎记录
		{
			m_bChouWeiCard[wChairID][cardIndex] = true;
			m_bPengAbandon[wChairID][cardIndex] = true; //臭牌记录
			log.info("[%s] choupeng chairid[%d] card[0x%02x] tid[%s] .\n", __FUNCTION__, wChairID, m_cbCurrentCard, tid.c_str());
		}
	}

	m_bUserAction[wChairID] = cbOperateCode;
	//操作过直接成功
	if (cbOperateCode == ACK_NULL)
	{
		proto::game::AckPreOperate stAckPreOp;
		stAckPreOp.set_operate_type(m_bUserAction[wChairID]);
		stAckPreOp.set_can_operate(true);
		Ppacket ppack;
		stAckPreOp.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_PRE_OPERATE_RES);
		unicast(player, ppack.data);
		return true;
	}
	//判断最优
	_tint32 wFirstUser = INVALID_CHAIR;
	bool bCanOperate = IsFirstUser(wChairID, wFirstUser);
	log.info("on user pre operate wChairID[%d] uid[%d] cbOperateCode[0x%02x] bCanOperate[%d] wFirstUser[%d] tid[%s] .\n", wChairID, seats[wChairID].uid, cbOperateCode, bCanOperate, wFirstUser, tid.c_str());
	if (bCanOperate)
	{
		OnUserOperateCard(player);
	}
	else
	{
		//预操作返回
		proto::game::AckPreOperate stAckPreOp;
		stAckPreOp.set_operate_type(m_bUserAction[wChairID]);
		stAckPreOp.set_can_operate(bCanOperate);
		Ppacket ppack;
		stAckPreOp.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_PRE_OPERATE_RES);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stAckPreOp[%s] .\n", player->uid, tid.c_str(), stAckPreOp.DebugString().c_str());
		//预操作提示
		if (!bCanOperate && wFirstUser != INVALID_CHAIR)
		{
			if (seats[wFirstUser].player != NULL)
			{
				Ppacket ppack;
				ppack.pack(SERVER_PHZ_PRE_OPERATE_NOTIFY);
				unicast(seats[wFirstUser].player, ppack.data);
			}
		}
	}
	//保存数据到redis
	SerializeTableData();
	return true;
}

//是否为最优玩家, 返回最优玩家
bool Table::IsFirstUser(_tint32 wChairID, _tint32& wFirstUser)
{
	//处理操作优先级
	_tint32 cbSortChairID = m_GamePlayer;
	_uint32	wSortCodes[GAME_PLAYER] = { 0 };
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		_tint32 wTempChairID = (m_wCurrentUser + i) % m_GamePlayer;
		if (m_bUserAction[wTempChairID] == ACK_NULL) continue;
		_uint8 cbCode = 0;
		if (!m_bResponse[wTempChairID]) //如果没有操作，取出最优操作
		{
			if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_CHIHU) cbCode = ACK_CHIHU;
			else if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_PENG) cbCode = ACK_PENG;
			else if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_CHI) cbCode = ACK_CHI;
		}
		wSortCodes[wTempChairID] = (cbCode * 10 + cbSortChairID) * 10 + wTempChairID;
		if (m_bResponse[wTempChairID] && m_bPerformAction[wTempChairID] == ACK_NULL) wSortCodes[wTempChairID] = 0;
		cbSortChairID--;
	}
	//排序
	for (_uint8 m = 0; m < m_GamePlayer - 1; ++m)
	{
		for (_uint8 n = m + 1; n < m_GamePlayer; ++n)
		{
			if (wSortCodes[m] < wSortCodes[n])
			{
				_uint32 cbTemp = wSortCodes[m];
				wSortCodes[m] = wSortCodes[n];
				wSortCodes[n] = cbTemp;
			}
		}
	}
	//判断最优
	bool bCanOperate = false;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (wSortCodes[i] == 0) continue;
		wFirstUser = wSortCodes[i] % 10;
		if (wFirstUser == wChairID) bCanOperate = true;
		break;
	}

	return bCanOperate;
}
//用户操作
bool Table::OnUserOperateCard(Player* player)
{
	time_t begin = time(NULL);
	_tint32 err_no = 0;
	proto::game::ReqOperateCard stReqOp;
	stReqOp.ParseFromString(player->client->ppacket.body);
	log.debug("uid[%d] tid[%s] stReqOp[%s] .\n", player->uid, tid.c_str(), stReqOp.DebugString().c_str());
	_uint8 cbOperateCode = stReqOp.operate_type();//动作代码
	_uint8 cbChiKind = stReqOp.chi_kind();		//吃牌类型
	_uint8 cbSecondKind = stReqOp.second_kind();	//二层类型
	_uint8 cbThirdKind = stReqOp.third_kind();	//三层类型
	_tint32 wChairID = player->seatid;
	log.info("on user operate card para wChairID[%d] uid[%d] cbOperateCode[0x%02x] cbChiKind[0x%02x] cbSecondKind[0x%02x] cbThirdKind[0x%02x] tid[%s] .\n", wChairID, seats[wChairID].uid, cbOperateCode, cbChiKind, cbSecondKind, cbThirdKind, tid.c_str());
	if (wChairID < 0 || wChairID >= m_GamePlayer)
	{
		log.error("on user operate card error wChairID[%d] wrong.\n", wChairID);
		return false;
	}

	if ((m_bUserAction[wChairID] == ACK_NULL) || (m_bResponse[wChairID] == true) || ((cbOperateCode != ACK_NULL) && (m_bUserAction[wChairID] & cbOperateCode) == 0))
	{
		log.error("on user operate card error wChairID[%d] m_bResponse[%d] m_bUserAction[0x%02x] cbOperateCode[0x%02x] tid[%s] .\n", wChairID, m_bResponse[wChairID], m_bUserAction[wChairID], cbOperateCode, tid.c_str());
		return false;
	}

	if(m_bCurrentMustHu)
	{
		if(m_bUserAction[wChairID] & ACK_CHIHU)
		{
			if (cbOperateCode != ACK_CHIHU)
			{
				err_no = 0x01;
			}
		}
	}

	if(0 == err_no)
	{
		//标记臭牌
		if (cbOperateCode == ACK_NULL)
		{
			_uint8 cardIndex = m_GameLogic.SwitchToCardIndex(m_cbCurrentCard);
			if (m_bUserAction[wChairID] & ACK_PENG) //臭偎记录
			{
				m_bChouWeiCard[wChairID][cardIndex] = true;
				m_bPengAbandon[wChairID][cardIndex] = true; //臭牌记录
				log.info("[%s] choupeng chairid[%d] card[0x%02x] tid[%s] .\n", __FUNCTION__, wChairID, m_cbCurrentCard, tid.c_str());
			}
			if (m_bUserAction[wChairID] & ACK_CHI) //臭偎记录
			{
				m_bChiAbandon[wChairID][cardIndex] = true; //臭牌记录
				log.info("[%s] chouchi chairid[%d] card[0x%02x] tid[%s] .\n", __FUNCTION__, wChairID, m_cbCurrentCard, tid.c_str());
			}
		}
		else if (cbOperateCode == ACK_CHI)
		{
			_uint8 cardIndex = m_GameLogic.SwitchToCardIndex(m_cbCurrentCard);
			if (m_bUserAction[wChairID] & ACK_PENG) //臭偎记录
			{
				m_bChouWeiCard[wChairID][cardIndex] = true;
				m_bPengAbandon[wChairID][cardIndex] = true; //臭牌记录
				log.info("[%s] choupeng chairid[%d] card[0x%02x] tid[%s] .\n", __FUNCTION__, wChairID, m_cbCurrentCard, tid.c_str());
			}
		}
	}
	//吃操作判断
	if (cbOperateCode == ACK_CHI)
	{
		//第一层
		if (cbChiKind == 0)
		{
			log.error("on user operate chi error cbChiKind[%d] wrong .\n", cbChiKind);
			return false;
		}
		_uint8 j = 0;
		for (; j < m_cbUserChiCardCount[wChairID]; j++)
		{
			if (m_UserChiCardInfo[wChairID][j].cbChiKind == cbChiKind) break;
		}
		if (j == m_cbUserChiCardCount[wChairID])
		{
			log.error("on user operate chi error cbChiKind[%d] no find .\n", cbChiKind);
			return false;
		}
		//第二层
		if (cbSecondKind == 0 && m_UserChiCardInfo[wChairID][j].cbSecondCount != 0)
		{
			log.error("on user operate chi error cbSecondKind[%d] wrong .\n", cbSecondKind);
			return false;
		}
		if (cbSecondKind != 0)
		{
			_uint8 k = 0;
			for (; k < m_UserChiCardInfo[wChairID][j].cbSecondCount; ++k)
			{
				if (m_UserChiCardInfo[wChairID][j].SecondData[k].cbChiKind == cbSecondKind) break;
			}
			if (k == m_UserChiCardInfo[wChairID][j].cbSecondCount)
			{
				log.error("on user operate chi error cbSecondKind[%d] no find .\n", cbSecondKind);
				return false;
			}
			//第三层
			if (cbThirdKind == 0 && m_UserChiCardInfo[wChairID][j].SecondData[k].cbThirdCount != 0)
			{
				log.error("on user operate chi error cbThirdKind[%d] wrong .\n", cbThirdKind);
				return false;
			}
			if (cbThirdKind != 0)
			{
				_uint8 m = 0;
				for (; m < m_UserChiCardInfo[wChairID][j].SecondData[k].cbThirdCount; ++m)
				{
					if (m_UserChiCardInfo[wChairID][j].SecondData[k].ThirdData[m].cbChiKind == cbThirdKind) break;
				}
				if (m == m_UserChiCardInfo[wChairID][j].SecondData[k].cbThirdCount)
				{
					log.error("on user operate chi error cbThirdKind[%d] no find .\n", cbThirdKind);
					return false;
				}
			}
		}
		//保存玩家吃操作
		m_cbUserChiKind[wChairID] = cbChiKind;
		m_cbUserSecondChiKind[wChairID] = cbSecondKind;
		m_cbUserThirdChiKind[wChairID] = cbThirdKind;
		log.info("on user operate chi save chi info wChairID[%d] m_cbUserChiKind[0x%02x] m_cbUserSecondChiKind[0x%02x] m_cbUserThirdChiKind[0x%02x] tid[%s].\n ", wChairID, m_cbUserChiKind[wChairID], m_cbUserSecondChiKind[wChairID], m_cbUserThirdChiKind[wChairID], tid.c_str());
	}

	//操作返回
	proto::game::AckOperateRes stOpRes;
	stOpRes.set_seatid(wChairID);
	stOpRes.set_err_no(err_no);
	Ppacket ppack;
	stOpRes.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_OPERATE_RES);
	unicast(player, ppack.data);
	log.debug("uid[%d] tid[%s] stOpRes[%s] .\n", player->uid, tid.c_str(), stOpRes.DebugString().c_str());

	if(err_no)
	{
		log.error("[%s] error err_no[%d] .\n",__FUNCTION__, err_no);
		return false;
	}
	//保存操作录像
	save_operate_res_video(wChairID, m_bPerformAction[wChairID]);

	//保存操作信息
	_uint8 cbTargetAction = cbOperateCode;
	_tint32 wTargetUser = wChairID;
	m_bResponse[wChairID] = true;
	m_bPerformAction[wChairID] = cbOperateCode;

	_uint8 cbTotalOperateCount = 0;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (m_bUserAction[i] != ACK_NULL) cbTotalOperateCount++;
	}
	if (cbTotalOperateCount > 1) //多操作
	{
		DoMultiOperate(wTargetUser, cbTargetAction);
	}
	else //单操作
	{
		DoSingleOperate(wTargetUser, cbTargetAction);
	}

	//保存数据到redis
	SerializeTableData();
	
	time_t end = time(NULL);
	int total = end - begin;
	if (total >= 1)
	{
		log.error("slow cmd:[%s] second[%d]\n",__FUNCTION__, total);
	}
	
	return true;
}

//派发扑克
bool Table::DispatchCardData(_tint32 wCenterUser)
{
	if (wCenterUser == INVALID_CHAIR)
	{
		log.error("DispatchCardData wCenterUser[%d] is invalid . \n", wCenterUser);
		return false;
	}

	if (m_bLeftCardCount == 0)
	{
		GameEnd(INVALID_CHAIR);
		return true;
	}

	_uint8 bSendCardData = m_cbRepertoryCard[--m_bLeftCardCount];
	m_bIsWeiTiCard = false;
	m_bIsWeiTiCard = (IsWeiTiCard(wCenterUser, bSendCardData) == true) ? true : false; //明消 

	//广播翻牌
	std::map<int, Player*>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter)
	{
		Player* player = iter->second;
		if (NULL == player) continue;
		_tint32 wCurChairID = player->seatid;

		proto::game::AckDispatchCard stDispachCard;
		stDispachCard.set_seatid(wCenterUser);
		if (m_bMingXiao == false && m_bIsWeiTiCard == true)
		{
			stDispachCard.set_card(INVALID_CARD);
		}
		else
		{
			stDispachCard.set_card(bSendCardData);
		}
		stDispachCard.set_left_card_count(m_bLeftCardCount);
		if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
		{
			stDispachCard.set_off_count(m_cbOfflineCount[wCurChairID]);
		}
		Ppacket ppack;
		stDispachCard.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_SEND_CARD);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stDispachCard[%s] .\n", player->uid, tid.c_str(), stDispachCard.DebugString().c_str());
		//保存离线数据
		save_offline_info(wCurChairID, SERVER_PHZ_SEND_CARD, stDispachCard.SerializeAsString());
	}
	log.info("dispatch card data seatid[%d] uid[%d] card[0x%02x] tid[%s]. \n", wCenterUser, seats[wCenterUser].uid, bSendCardData, tid.c_str());
	//设置变量
	m_bFirstCard = false;
	m_bDispatch = true;
	m_bCardOperated = false;
	m_wCurrentUser = wCenterUser;
	m_cbCurrentCard = bSendCardData;

	//动作判断
	ev_timer_again(zjh.loop, &dispatch_delay_timer);
	m_cbCurTimer = 5;
	//保存录像
	save_dispatch_card_video(wCenterUser, bSendCardData);
	//保存数据到redis
	SerializeTableData();
	return true;
}

//出牌提示
bool Table::SendOutCardNotify(_tint32 wCurrentUser, bool bOutCard)
{
	if (wCurrentUser == INVALID_CHAIR)
	{
		log.error("SendOutCardNotify wCurrentUser[%d] error .\n", wCurrentUser);
		return false;
	}

	//过牌判断
	if (bOutCard == true)
	{
		int i;
		for (i = 0; i < MAX_INDEX; i++)
		{
			if ((m_cbCardIndex[wCurrentUser][i] > 0) && (m_cbCardIndex[wCurrentUser][i] < 3)) break;
		}
		if (i == MAX_INDEX) bOutCard = false;
	}

	if (bOutCard == false)
	{
		m_bXiangGong[wCurrentUser] = true;
		//发送数据
		proto::game::AckXiangGong stXG;
		stXG.set_seatid(wCurrentUser);
		Ppacket ppack;
		stXG.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_XIANG_GONG);
		broadcast(NULL, ppack.data);

		m_wCurrentUser = (wCurrentUser + 1) % m_GamePlayer;
		DispatchCardData(m_wCurrentUser);
		return true;
	}

	//设置变量
	m_bOutCard = true;
	m_wCurrentUser = wCurrentUser;

	//发送数据
	proto::game::AckOutNotify stOutNotify;
	stOutNotify.set_seatid(wCurrentUser);
	stOutNotify.set_is_out_card(m_bOutCard);
	stOutNotify.set_off_count(m_cbOfflineCount[wCurrentUser]);
	Ppacket ppack;
	stOutNotify.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_OUT_CARD_NOTIFY);
	broadcast(NULL, ppack.data);
	log.debug("uid[%d] tid[%s] stOutNotify[%s] .\n", m_wCurrentUser, tid.c_str(), stOutNotify.DebugString().c_str());
	log.info("send out card notify wCurrentUser[%d] bOutCard[%d] tid[%s] .\n", wCurrentUser, bOutCard, tid.c_str());

	return true;
}

//派发响应判断
bool Table::DispatchEstimateUserRespond(_tint32 wCenterUser, _uint8 cbCenterCard)
{
	time_t begin = time(NULL);
	
	log.info("dispatch estimate user respond para wCenterUser[%d] cbCenterCard[0x%02x] tid[%s] .\n", wCenterUser, cbCenterCard, tid.c_str());
	//执行变量
	bool bPerformAction = false;
	m_bTiCard = false;
	//提牌判断
	if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[wCenterUser], cbCenterCard) == true)
	{
		//变量定义
		_uint8 cbRemoveIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
		_uint8 cbRemoveCount = m_cbCardIndex[wCenterUser][cbRemoveIndex];
		//删除扑克
		m_cbCardIndex[wCenterUser][cbRemoveIndex] = 0;
		//设置组合
		_uint8 cbIndex = m_bWeaveItemCount[wCenterUser]++;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount = 4;
		m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind = ACK_TI;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCenterCard = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0] = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[1] = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[2] = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[3] = cbCenterCard;
		//更新胡息
		//UpdateUserHuXiCount(wCenterUser);
		UpdateCardsWeave(wCenterUser, m_WeaveItemArray[wCenterUser][cbIndex].cbCardList, cbRemoveCount);
		//广播提牌
		std::map<int, Player*>::iterator iter = players.begin();
		for (; iter != players.end(); ++iter)
		{
			Player* player = iter->second;
			if (NULL == player) continue;
			_tint32 wCurChairID = player->seatid;

			proto::game::AckOperateCard stOperateCard;
			stOperateCard.set_ack_code(ACK_TI);
			stOperateCard.set_seatid(wCenterUser);
			if (m_bMingXiao)
			{
				stOperateCard.set_action_card(cbCenterCard);
			}
			stOperateCard.set_remove_count(cbRemoveCount);
			if (wCurChairID == wCenterUser)
			{
				stOperateCard.set_action_card(cbCenterCard);
				if (m_bCommonOrBest[wCurChairID] == false) {
					for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
						proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
						for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
							pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
						}
					}
				}
				else {
					for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
						proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
						for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
							pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
						}
					}
				}
				std::vector<_uint8> vecListenCards;
				if (IsListen(wCurChairID, vecListenCards))
				{
					for (_uint8 i = 0; i < vecListenCards.size(); ++i)
						stOperateCard.add_listen_cards(vecListenCards[i]);
				}
				_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
				stOperateCard.set_total_huxi(cbTotalHuXi);
			}
			stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wCenterUser]);
			if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
			{
				stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
			}
			Ppacket ppack;
			stOperateCard.SerializeToString(&ppack.body);
			ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
			unicast(player, ppack.data);
			log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
			//保存离线数据
			save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
		}
		log.info("dispatch estimate user respond ti wCenterUser[%d] cbCenterCard[0x%02x] tid[%s] .\n", wCenterUser, cbCenterCard, tid.c_str());
		//保存录像
		save_operate_video(wCenterUser, ACK_TI, cbCenterCard);
		//设置变量
		bPerformAction = true;
		m_bTiCard = true;
		m_bCardOperated = true;
	}

	//偎牌变提
	if (bPerformAction == false)
	{
		for (_uint8 cbIndex = 0; cbIndex < m_bWeaveItemCount[wCenterUser]; cbIndex++)
		{
			//变量定义
			_uint8 cbWeaveKind = m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind;
			_uint8 cbWeaveCard = m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0];
			//转换判断
			if ((cbCenterCard != cbWeaveCard) || (cbWeaveKind != ACK_WEI)) continue;
			//设置组合
			m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount = 4;
			m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind = ACK_TI;
			m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[3] = cbCenterCard;
			//更新胡息
			UpdateUserHuXiCount(wCenterUser);
			//广播提牌
			std::map<int, Player*>::iterator iter = players.begin();
			for (; iter != players.end(); ++iter)
			{
				Player* player = iter->second;
				if (NULL == player) continue;
				_tint32 wCurChairID = player->seatid;

				proto::game::AckOperateCard stOperateCard;
				stOperateCard.set_ack_code(ACK_TI);
				stOperateCard.set_seatid(wCenterUser);
				if (m_bMingXiao)
				{
					stOperateCard.set_action_card(cbCenterCard);
				}
				stOperateCard.set_remove_count(0);
				if (wCurChairID == wCenterUser)
				{
					stOperateCard.set_action_card(cbCenterCard);
					if (m_bCommonOrBest[wCurChairID] == false) {
						for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					else {
						for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					std::vector<_uint8> vecListenCards;
					if (IsListen(wCurChairID, vecListenCards))
					{
						for (_uint8 i = 0; i < vecListenCards.size(); ++i)
							stOperateCard.add_listen_cards(vecListenCards[i]);
					}
					_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
					stOperateCard.set_total_huxi(cbTotalHuXi);
				}
				stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wCenterUser]);
				if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
				{
					stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
				}
				Ppacket ppack;
				stOperateCard.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
				unicast(player, ppack.data);
				log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
				//保存离线数据
				save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
			}
			log.info("dispatch estimate user respond weitoti wCenterUser[%d] cbCenterCard[0x%02x] tid[%s] .\n", wCenterUser, cbCenterCard, tid.c_str());

			//保存录像
			save_operate_video(wCenterUser, ACK_TI, cbCenterCard);
			//设置变量
			bPerformAction = true;
			m_bTiCard = true;
			m_bCardOperated = true;
			break;
		}
	}

	//偎牌判断
	if ((bPerformAction == false) && (m_GameLogic.IsWeiPengCard(m_cbCardIndex[wCenterUser], cbCenterCard) == true))
	{
		//变量定义
		_uint8 cbRemoveIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
		_uint8 cbRemoveCount = m_cbCardIndex[wCenterUser][cbRemoveIndex];
		//设置扑克
		m_cbCardIndex[wCenterUser][cbRemoveIndex] = 0;
		//设置组合
		_uint8 cbIndex = m_bWeaveItemCount[wCenterUser]++;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardCount = 3;
		m_WeaveItemArray[wCenterUser][cbIndex].cbWeaveKind = ACK_WEI;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCenterCard = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[0] = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[1] = cbCenterCard;
		m_WeaveItemArray[wCenterUser][cbIndex].cbCardList[2] = cbCenterCard;
		//更新胡息
		//UpdateUserHuXiCount(wCenterUser);
		UpdateCardsWeave(wCenterUser, m_WeaveItemArray[wCenterUser][cbIndex].cbCardList, cbRemoveCount);
		//广播偎牌
		std::map<int, Player*>::iterator iter = players.begin();
		for (; iter != players.end(); ++iter)
		{
			Player* player = iter->second;
			if (NULL == player) continue;
			_tint32 wCurChairID = player->seatid;

			proto::game::AckOperateCard stOperateCard;
			stOperateCard.set_ack_code(ACK_WEI);
			stOperateCard.set_seatid(wCenterUser);
			if (m_bChouWeiCard[wCenterUser][m_GameLogic.SwitchToCardIndex(cbCenterCard)] == true || m_bMingXiao)
			{
				stOperateCard.set_action_card(cbCenterCard);
			}
			stOperateCard.set_remove_count(cbRemoveCount);
			if (wCurChairID == wCenterUser)
			{
				stOperateCard.set_action_card(cbCenterCard);
				if (m_bCommonOrBest[wCurChairID] == false) {
					for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
						proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
						for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
							pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
						}
					}
				}
				else {
					for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
						proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
						for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
							pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
						}
					}
				}
				std::vector<_uint8> vecListenCards;
				if (IsListen(wCurChairID, vecListenCards))
				{
					for (_uint8 i = 0; i < vecListenCards.size(); ++i)
						stOperateCard.add_listen_cards(vecListenCards[i]);
				}
				_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
				stOperateCard.set_total_huxi(cbTotalHuXi);
			}
			stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wCenterUser]);
			if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
			{
				stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
			}
			Ppacket ppack;
			stOperateCard.SerializeToString(&ppack.body);
			ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
			unicast(player, ppack.data);
			log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
			//保存离线数据
			save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
		}
		log.info("dispatch estimate user respond wei wCenterUser[%d] cbCenterCard[0x%02x] tid[%s] .\n", wCenterUser, cbCenterCard, tid.c_str());
		//保存录像
		save_operate_video(wCenterUser, ACK_WEI, cbCenterCard);
		//设置变量
		bPerformAction = true;
		m_bTiCard = false;
		m_bCardOperated = true;
	}

	//操作清零
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));
	log.info("dispatch estimate clear operate .\n");

	bool bAction = false;
	//提偎之后胡牌判断
	if (bPerformAction)
	{
		tagHuCardInfo stHuInfo;
		std::vector<tagHuFanInfo> vecHuFanInfo;
		bool bZimo = false;
		if (m_bDispatch && wCenterUser == m_wCurrentUser) bZimo = true;			//是否自摸
		bool bHu = m_GameLogic.GetHuCardInfoFinal(m_cbCardIndex[wCenterUser], INVALID_CARD, m_bWeaveItemCount[wCenterUser], m_WeaveItemArray[wCenterUser], stHuInfo, m_wBaseHuXi, vecHuFanInfo, bZimo, m_bDispatch, m_bHHD, false, m_bWithMaoHu);
		if (bHu && !m_bXiangGong[wCenterUser])
		{
			m_bCurrentMustHu = false;
			m_bDispatchHu = false;
			m_bTiWeiPaoHu = true;
			if (m_bTiCard == false) m_bWeiHu = true;
			m_bUserAction[wCenterUser] |= ACK_CHIHU;
			bAction = true;
			/*if (2 == m_cbMustHu)
			{
				GameEnd(wCenterUser); //自摸必胡
				return true;
			}
			else*/
			{	//发送操作提示
				proto::game::AckOperateNotify stNotity;
				stNotity.set_operate_type(m_bUserAction[wCenterUser]);
				stNotity.set_operate_card(cbCenterCard);
				stNotity.set_off_count(m_cbOfflineCount[wCenterUser]);
				m_bCurrentMustHu = (2 == m_cbMustHu);
				stNotity.set_is_must_hu(m_bCurrentMustHu);

				Ppacket ppack;
				stNotity.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_NOTIFY);
				unicast(seats[wCenterUser].player, ppack.data);
				log.debug("uid[%d] tid[%s] stNotity[%s] .\n", seats[wCenterUser].player->uid, tid.c_str(), stNotity.DebugString().c_str());
				//保存离线数据
				save_offline_info(wCenterUser, SERVER_PHZ_OPERATE_NOTIFY, stNotity.SerializeAsString());
				log.info("on operate start tian hu wCenterUser[%d] m_bUserAction[0x%02x] cbCenterCard[0x%02x] tid[%s] .\n",
					wCenterUser, m_bUserAction[wCenterUser], cbCenterCard, tid.c_str());
				//保存录像
				save_operate_notify_video();
				//保存数据到redis
				SerializeTableData();
				return true;
			}
		}

		//出牌提示
		if (m_bTiCard == false || (m_bTiCard && IsAllowOutCard(wCenterUser)))
		{
			SendOutCardNotify(wCenterUser, true);
			m_bTiCard = false;
			m_bAfterWeave = true;
			return true;
		}
		else
		{
			m_wCurrentUser = wCenterUser;
			return false;
		}
	}

	//操作判断
	_uint8 tmpUser = wCenterUser;
	for (_uint8 i = 0; i < m_GamePlayer; i++)
	{
		//胡牌判断
		tagHuCardInfo stHuInfo;
		std::vector<tagHuFanInfo> vecHuFanInfo;
		bool bZimo = false;
		tmpUser = (wCenterUser + i) % m_GamePlayer;
		if (m_bDispatch && tmpUser == m_wCurrentUser) bZimo = true;			//是否自摸
		bool bHu = m_GameLogic.GetHuCardInfoFinal(m_cbCardIndex[tmpUser], cbCenterCard, m_bWeaveItemCount[tmpUser], m_WeaveItemArray[tmpUser], stHuInfo, m_wBaseHuXi, vecHuFanInfo, bZimo, m_bDispatch, m_bHHD, false, m_bWithMaoHu);
		if (bHu && !m_bXiangGong[tmpUser])
		{
			m_bCurrentMustHu = false;
			m_bDispatchHu = true;
			m_bUserAction[tmpUser] |= ACK_CHIHU;
			bAction = true;

			m_bCurrentMustHu = (2 == m_cbMustHu);
			if (m_bCurrentMustHu)
			{
				proto::game::AckOperateNotify stNotity;
				stNotity.set_operate_type(m_bUserAction[tmpUser]);
				stNotity.set_operate_card(cbCenterCard);
				stNotity.set_off_count(m_cbOfflineCount[tmpUser]);
				stNotity.set_is_must_hu(m_bCurrentMustHu);

				Ppacket ppack;
				stNotity.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_NOTIFY);
				unicast(seats[tmpUser].player, ppack.data);
				log.debug("uid[%d] tid[%s] stNotity[%s] .\n", seats[tmpUser].player->uid, tid.c_str(), stNotity.DebugString().c_str());
				//保存离线数据
				save_offline_info(tmpUser, SERVER_PHZ_OPERATE_NOTIFY, stNotity.SerializeAsString());
				log.info("on operate start tian hu tmpUser[%d] m_bUserAction[0x%02x] cbCenterCard[0x%02x] tid[%s] .\n",
						tmpUser, m_bUserAction[tmpUser], cbCenterCard, tid.c_str());
				//保存录像
				save_operate_notify_video();
				//保存数据到redis
				SerializeTableData();
				return true;
			}
			/*
			if (2 == m_cbMustHu)
			{
				GameEnd(tmpUser); 
				return true;
			}*/
		}
		//吃碰判断, 如果是跑牌，则不判断吃碰
		if (IsPaoCard(wCenterUser, cbCenterCard) == false)
		{
			_uint8 cbType = proto::game::CHOU_CODE_NULL;
			_uint8 cbOperateIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
			if (m_bPengAbandon[tmpUser][cbOperateIndex] == false && IsAllowChiPeng(tmpUser) && !m_bDongZhang[tmpUser]) //臭牌不能碰吃
			{
				if ((m_GameLogic.IsWeiPengCard(m_cbCardIndex[tmpUser], cbCenterCard) == true) && (IsPassPengCard(tmpUser, cbCenterCard) == false))
				{
					m_bUserAction[tmpUser] |= ACK_PENG;
					bAction = true;
				}
			}
			else //臭牌提示
			{
				if ((m_GameLogic.IsWeiPengCard(m_cbCardIndex[tmpUser], cbCenterCard) == true)
					&& m_bPengAbandon[tmpUser][cbOperateIndex] == true) cbType = proto::game::CHOU_CODE_PENG;
			}
			if (IsPassCard(tmpUser, cbCenterCard) == false && m_bChiAbandon[tmpUser][cbOperateIndex] == false && IsAllowChiPeng(tmpUser) && !m_bDongZhang[tmpUser]) //臭牌不能碰吃
			{
				_tint32 wEatUser = (wCenterUser + 1) % m_GamePlayer;
				if (tmpUser == wCenterUser || tmpUser == wEatUser)
				{
					m_cbUserChiCardCount[tmpUser] = m_GameLogic.GetChiCardInfo(m_cbCardIndex[tmpUser], cbCenterCard, m_UserChiCardInfo[tmpUser]);
					if (m_cbUserChiCardCount[tmpUser] > 0)
					{
						m_bUserAction[tmpUser] |= ACK_CHI;
						bAction = true;
					}
				}
			}
			else //臭牌提示
			{
				_uint8 cbTmpChiCount = 0;
				tagNewChiCardInfo stTmpChiInfo[6];		//玩家吃牌组合
				_tint32 wTmpEatUser = (wCenterUser + 1) % m_GamePlayer;
				if (tmpUser == wCenterUser || tmpUser == wTmpEatUser)
				{
					cbTmpChiCount = m_GameLogic.GetChiCardInfo(m_cbCardIndex[tmpUser], cbCenterCard, stTmpChiInfo);
					if (cbTmpChiCount > 0 && m_bChiAbandon[tmpUser][cbOperateIndex] == true)
					{
						if (cbType == proto::game::CHOU_CODE_PENG) cbType = proto::game::CHOU_CODE_CHIPENG;
						else cbType = proto::game::CHOU_CODE_CHI;
					}
				}
			}
			if (cbType != proto::game::CHOU_CODE_NULL) SendChouNotify(tmpUser, cbType);
		}
	}
	//发送操作提示
	if (bAction == true)
	{
		for (_uint8 i = 0; i < m_GamePlayer; i++)
		{
			Player* player = seats[i].player;
			if (player == NULL) continue;
			if (m_bUserAction[i] == ACK_NULL) continue;
			//发送操作提示
			proto::game::AckOperateNotify stNotity;
			stNotity.set_operate_type(m_bUserAction[i]);
			stNotity.set_operate_card(cbCenterCard);
			if (m_bUserAction[i] & ACK_CHI)
			{
				for (_uint8 j = 0; j < m_cbUserChiCardCount[i]; ++j)
				{
					proto::game::tagChiCardInfo* pstChiCardInfo = stNotity.add_chicard_info();
					pstChiCardInfo->set_chi_kind(m_UserChiCardInfo[i][j].cbChiKind);
					pstChiCardInfo->set_center_card(m_UserChiCardInfo[i][j].cbCenterCard);
					for (_uint8 c = 0; c < 3; c++) pstChiCardInfo->add_cards(m_UserChiCardInfo[i][j].cbCardData[c]);
					for (_uint8 k = 0; k < m_UserChiCardInfo[i][j].cbSecondCount; ++k)
					{
						proto::game::tagSecondInfo* pstSecondInfo = pstChiCardInfo->add_second_info();
						pstSecondInfo->set_chi_kind(m_UserChiCardInfo[i][j].SecondData[k].cbChiKind);
						pstSecondInfo->set_center_card(m_UserChiCardInfo[i][j].SecondData[k].cbCenterCard);
						for (_uint8 c = 0; c < 3; c++) pstSecondInfo->add_cards(m_UserChiCardInfo[i][j].SecondData[k].cbCardData[c]);
						for (_uint8 m = 0; m < m_UserChiCardInfo[i][j].SecondData[k].cbThirdCount; ++m)
						{
							proto::game::tagThirdInfo* pstThirdInfo = pstSecondInfo->add_third_info();
							pstThirdInfo->set_chi_kind(m_UserChiCardInfo[i][j].SecondData[k].ThirdData[m].cbChiKind);
							pstThirdInfo->set_center_card(m_UserChiCardInfo[i][j].SecondData[k].ThirdData[m].cbCenterCard);
							for (_uint8 c = 0; c < 3; c++) pstThirdInfo->add_cards(m_UserChiCardInfo[i][j].SecondData[k].ThirdData[m].cbCardData[c]);
						}
					}
				}
			}
			stNotity.set_off_count(m_cbOfflineCount[i]);
			Ppacket ppack;
			stNotity.SerializeToString(&ppack.body);
			ppack.pack(SERVER_PHZ_OPERATE_NOTIFY);
			unicast(player, ppack.data);
			log.debug("uid[%d] tid[%s] stNotity[%s] .\n", player->uid, tid.c_str(), stNotity.DebugString().c_str());
			//保存离线数据
			save_offline_info(i, SERVER_PHZ_OPERATE_NOTIFY, stNotity.SerializeAsString());
			log.info("dispatch estimate after hupengchi seatid[%d] m_bUserAction[0x%02x] tid[%s] .\n", i, m_bUserAction[i], tid.c_str());
		}
		//保存录像
		save_operate_notify_video();
		time_t end = time(NULL);
		int total = end - begin;
		if (total >= 1)
		{
			log.error("slow cmd:[%s] second[%d]\n", __FUNCTION__, total);
		}

		return true;
	}

	//跑牌判断
	_tint32 wPaoChairID = INVALID_CHAIR;
	m_bPaoCard = false;
	for (_uint8 i = 0; i < m_GamePlayer; i++)
	{
		if (i == wCenterUser) continue;
		if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[i], cbCenterCard) == true)
		{
			_tint32 wActionUser = i;
			_uint8 cbRemoveIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
			_uint8 cbRemoveCount = m_cbCardIndex[wActionUser][cbRemoveIndex];
			m_cbCardIndex[wActionUser][cbRemoveIndex] = 0;
			//设置组合
			_uint8 cbIndex = m_bWeaveItemCount[wActionUser]++;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount = 4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind = ACK_PAO;
			m_WeaveItemArray[wActionUser][cbIndex].cbCenterCard = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[0] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[1] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[2] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3] = cbCenterCard;
			//更新胡息
			//UpdateUserHuXiCount(wActionUser);
			UpdateCardsWeave(wActionUser, m_WeaveItemArray[wActionUser][cbIndex].cbCardList, cbRemoveCount);
			//广播跑牌
			std::map<int, Player*>::iterator iter = players.begin();
			for (; iter != players.end(); ++iter)
			{
				Player* player = iter->second;
				if (NULL == player) continue;
				_tint32 wCurChairID = player->seatid;

				proto::game::AckOperateCard stOperateCard;
				stOperateCard.set_ack_code(ACK_PAO);
				stOperateCard.set_seatid(wActionUser);
				stOperateCard.set_action_card(cbCenterCard);
				stOperateCard.set_remove_count(cbRemoveCount);
				if (wCurChairID == wActionUser)
				{
					if (m_bCommonOrBest[wCurChairID] == false) {
						for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					else {
						for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					std::vector<_uint8> vecListenCards;
					if (IsListen(wCurChairID, vecListenCards))
					{
						for (_uint8 i = 0; i < vecListenCards.size(); ++i)
							stOperateCard.add_listen_cards(vecListenCards[i]);
					}
					_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
					stOperateCard.set_total_huxi(cbTotalHuXi);
				}
				stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wActionUser]);
				if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
				{
					stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
				}
				Ppacket ppack;
				stOperateCard.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
				unicast(player, ppack.data);
				log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
				//保存离线数据
				save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
			}
			log.info("dispatch estimate user respond pao wActionUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wActionUser, cbCenterCard, tid.c_str());
			//保存录像
			save_operate_video(wActionUser, ACK_PAO, cbCenterCard);
			wPaoChairID = i;
			m_bPaoCard = true;
			m_bCardOperated = true;
			break;
		}
	}

	//跑牌转换
	if (m_bPaoCard == false)
	{
		for (_uint8 i = 0; i < m_GamePlayer; i++)
		{
			for (_uint8 cbIndex = 0; cbIndex < m_bWeaveItemCount[i]; cbIndex++)
			{
				_uint8 cbWeaveKind = m_WeaveItemArray[i][cbIndex].cbWeaveKind;
				_uint8 cbWeaveCard = m_WeaveItemArray[i][cbIndex].cbCardList[0];
				bool bChangeWeave = false;
				if (cbCenterCard == cbWeaveCard)
				{
					if (cbWeaveKind == ACK_WEI && i != wCenterUser) bChangeWeave = true;
					else if (cbWeaveKind == ACK_PENG) bChangeWeave = true;
				}
				if (bChangeWeave == false) continue;
				_tint32 wActionUser = i;
				//设置组合
				m_WeaveItemArray[wActionUser][cbIndex].cbCardCount = 4;
				m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind = ACK_PAO;
				m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3] = cbCenterCard;
				//更新胡息
				UpdateUserHuXiCount(wActionUser);
				//广播跑牌
				std::map<int, Player*>::iterator iter = players.begin();
				for (; iter != players.end(); ++iter)
				{
					Player* player = iter->second;
					if (NULL == player) continue;
					_tint32 wCurChairID = player->seatid;

					proto::game::AckOperateCard stOperateCard;
					stOperateCard.set_ack_code(ACK_PAO);
					stOperateCard.set_seatid(wActionUser);
					stOperateCard.set_action_card(cbCenterCard);
					stOperateCard.set_remove_count(0);
					if (wCurChairID == wActionUser)
					{
						if (m_bCommonOrBest[wCurChairID] == false) {
							for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						else {
							for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						std::vector<_uint8> vecListenCards;
						if (IsListen(wCurChairID, vecListenCards))
						{
							for (_uint8 i = 0; i < vecListenCards.size(); ++i)
								stOperateCard.add_listen_cards(vecListenCards[i]);
						}
						_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
						stOperateCard.set_total_huxi(cbTotalHuXi);
					}
					stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wActionUser]);
					if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
					{
						stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
					}
					Ppacket ppack;
					stOperateCard.SerializeToString(&ppack.body);
					ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
					unicast(player, ppack.data);
					log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
					//保存离线数据
					save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
				}
				log.info("dispatch estimate user respond weipengpao wActionUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wActionUser, cbCenterCard, tid.c_str());

				//保存录像
				save_operate_video(wActionUser, ACK_PAO, cbCenterCard);
				wPaoChairID = i;
				m_bPaoCard = true;
				m_bCardOperated = true;
				break;
			}
			if (m_bPaoCard == true) break;
		}
	}
	
	time_t end = time(NULL);
	int total = end - begin;
	if (total >= 1)
	{
		log.error("slow cmd:[%s] second[%d]\n",__FUNCTION__, total);
	}

	if (m_bPaoCard && wPaoChairID != INVALID_CHAIR)
	{
		//出牌提示
		if (IsAllowOutCard(wPaoChairID))
		{
			SendOutCardNotify(wPaoChairID, true);
			m_bAfterWeave = true;
			return true;
		}
		else
		{
			m_wCurrentUser = wPaoChairID;
			return false;
		}
	}

	return false;
}

//打牌响应判断
bool Table::OutEstimateUserRespond(_tint32 wCenterUser, _uint8 cbCenterCard)
{
	time_t begin = time(NULL);
	//闲家提牌处理
	//提牌处理
	_uint8 cbTiCardCount[GAME_PLAYER] = { 0 };
	_uint8 cbTiCardIndex[GAME_PLAYER][MAX_WEAVE];
	memset(cbTiCardIndex, 0, sizeof(cbTiCardIndex));
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		//类型统计
		cbTiCardCount[i] = m_GameLogic.GetAcitonTiCard(m_cbCardIndex[i], cbTiCardIndex[i]);
	}
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (i == m_wBankerUser) continue;
		_tint32 wChairID = i;
		if (cbTiCardCount[wChairID] > 0)
		{
			for (_uint8 j = 0; j < cbTiCardCount[wChairID]; j++)
			{
				_uint8 cbRemoveCount = m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]];
				_uint8 cbTiCardData = m_GameLogic.SwitchToCardData(cbTiCardIndex[wChairID][j]);
				m_cbCardIndex[wChairID][cbTiCardIndex[wChairID][j]] = 0;
				//设置组合
				_uint8 cbIndex = m_bWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbIndex].cbCardCount = 4;
				m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind = ACK_TI;
				m_WeaveItemArray[wChairID][cbIndex].cbCenterCard = cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[0] = cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[1] = cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[2] = cbTiCardData;
				m_WeaveItemArray[wChairID][cbIndex].cbCardList[3] = cbTiCardData;
				//更新胡息
				//UpdateUserHuXiCount(wChairID);
				UpdateCardsWeave(wChairID, m_WeaveItemArray[wChairID][cbIndex].cbCardList, cbRemoveCount);
				//广播提牌
				std::map<int, Player*>::iterator iter = players.begin();
				for (; iter != players.end(); ++iter)
				{
					Player* player = iter->second;
					if (NULL == player) continue;
					_tint32 wCurChairID = player->seatid;

					proto::game::AckOperateCard stOperateCard;
					stOperateCard.set_ack_code(ACK_TI);
					stOperateCard.set_seatid(wChairID);
					if (m_bMingXiao)
					{
						stOperateCard.set_action_card(cbTiCardData);
					}
					stOperateCard.set_remove_count(cbRemoveCount);
					if (wCurChairID == wChairID)
					{
						stOperateCard.set_action_card(cbTiCardData);
						if (m_bCommonOrBest[wCurChairID] == false) {
							for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						else {
							for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						std::vector<_uint8> vecListenCards;
						if (IsListen(wCurChairID, vecListenCards))
						{
							for (_uint8 i = 0; i < vecListenCards.size(); ++i)
								stOperateCard.add_listen_cards(vecListenCards[i]);
						}
						_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
						stOperateCard.set_total_huxi(cbTotalHuXi);
					}
					stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wChairID]);
					if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
					{
						stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
					}
					Ppacket ppack;
					stOperateCard.SerializeToString(&ppack.body);
					ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
					unicast(player, ppack.data);
					log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
					//保存离线数据
					save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
				}
				log.info("on operate start ti card wChairID[%d] cbTiCardData[0x%02x] tid[%s].\n", wChairID, cbTiCardData, tid.c_str());
				//保存录像
				save_operate_video(wChairID, ACK_TI, cbTiCardData);
			}
			//如果有多个提牌
			if (cbTiCardCount[wChairID] > 1)
			{
				for (_uint8 i = 0; i < cbTiCardCount[wChairID] - 1; ++i)
				{
					_uint8 bSendCardData = m_cbRepertoryCard[--m_bLeftCardCount];
					m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(bSendCardData)]++;
					//广播翻牌
					std::map<int, Player*>::iterator iter = players.begin();
					for (; iter != players.end(); ++iter)
					{
						Player* player = iter->second;
						if (NULL == player) continue;
						_tint32 wCurChairID = player->seatid;

						proto::game::AckDispatchCard stDispachCard;
						stDispachCard.set_seatid(wChairID);
						stDispachCard.set_card(bSendCardData);
						stDispachCard.set_left_card_count(m_bLeftCardCount);
						stDispachCard.set_to_seatid(wChairID);
						if (wCurChairID == wChairID)
						{
							UpdateCommonCardsWeave(wCurChairID);
							UpdateBestCardsWeave(wCurChairID);
							if (m_bCommonOrBest[wCurChairID] == false) {
								for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
									proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
									for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
										pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
									}
								}
							}
							else {
								for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
									proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
									for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
										pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
									}
								}
							}
							std::vector<_uint8> vecListenCards;
							if (IsListen(wCurChairID, vecListenCards))
							{
								for (_uint8 i = 0; i < vecListenCards.size(); ++i)
									stDispachCard.add_listen_cards(vecListenCards[i]);
							}
							_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
							stDispachCard.set_total_huxi(cbTotalHuXi);
						}
						if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
						{
							stDispachCard.set_off_count(m_cbOfflineCount[wCurChairID]);
						}
						Ppacket ppack;
						stDispachCard.SerializeToString(&ppack.body);
						ppack.pack(SERVER_PHZ_SEND_CARD);
						unicast(player, ppack.data);
						log.debug("uid[%d] tid[%s] stDispachCard[%s] .\n", player->uid, tid.c_str(), stDispachCard.DebugString().c_str());
						//保存离线数据
						save_offline_info(wCurChairID, SERVER_PHZ_SEND_CARD, stDispachCard.SerializeAsString());
					}
					//保存录像
					save_dispatch_card_video(wChairID, bSendCardData, wChairID);
				}
				_uint8 cbTempTiCount = 0;
				_uint8 cbTempTiIndex[MAX_WEAVE];
				memset(cbTempTiIndex, 0, sizeof(cbTempTiIndex));
				do
				{
					cbTempTiCount = m_GameLogic.GetAcitonTiCard(m_cbCardIndex[wChairID], cbTempTiIndex);
					if (cbTempTiCount == 0) break;
					for (_uint8 j = 0; j < cbTempTiCount; ++j)
					{
						_uint8 cbRemoveCount = m_cbCardIndex[wChairID][cbTempTiIndex[j]];
						_uint8 cbTiCardData = m_GameLogic.SwitchToCardData(cbTempTiIndex[j]);
						m_cbCardIndex[wChairID][cbTempTiIndex[j]] = 0;
						//设置组合
						_uint8 cbIndex = m_bWeaveItemCount[wChairID]++;
						m_WeaveItemArray[wChairID][cbIndex].cbCardCount = 4;
						m_WeaveItemArray[wChairID][cbIndex].cbWeaveKind = ACK_TI;
						m_WeaveItemArray[wChairID][cbIndex].cbCenterCard = cbTiCardData;
						m_WeaveItemArray[wChairID][cbIndex].cbCardList[0] = cbTiCardData;
						m_WeaveItemArray[wChairID][cbIndex].cbCardList[1] = cbTiCardData;
						m_WeaveItemArray[wChairID][cbIndex].cbCardList[2] = cbTiCardData;
						m_WeaveItemArray[wChairID][cbIndex].cbCardList[3] = cbTiCardData;
						//更新胡息
						//UpdateUserHuXiCount(wChairID);
						UpdateCardsWeave(wChairID, m_WeaveItemArray[wChairID][cbIndex].cbCardList, cbRemoveCount);
						//广播提牌
						std::map<int, Player*>::iterator iter = players.begin();
						for (; iter != players.end(); ++iter)
						{
							Player* player = iter->second;
							if (NULL == player) continue;
							_tint32 wCurChairID = player->seatid;

							proto::game::AckOperateCard stOperateCard;
							stOperateCard.set_ack_code(ACK_TI);
							stOperateCard.set_seatid(wChairID);
							if (m_bMingXiao)
							{
								stOperateCard.set_action_card(cbTiCardData);
							}
							stOperateCard.set_remove_count(cbRemoveCount);
							if (wCurChairID == wChairID)
							{
								stOperateCard.set_action_card(cbTiCardData);
								if (m_bCommonOrBest[wCurChairID] == false) {
									for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
										proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
										for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
											pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
										}
									}
								}
								else {
									for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
										proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
										for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
											pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
										}
									}
								}
								std::vector<_uint8> vecListenCards;
								if (IsListen(wCurChairID, vecListenCards))
								{
									for (_uint8 i = 0; i < vecListenCards.size(); ++i)
										stOperateCard.add_listen_cards(vecListenCards[i]);
								}
								_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
								stOperateCard.set_total_huxi(cbTotalHuXi);
							}
							stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wChairID]);
							if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
							{
								stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
							}
							Ppacket ppack;
							stOperateCard.SerializeToString(&ppack.body);
							ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
							unicast(player, ppack.data);
							log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
							//保存离线数据
							save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
						}
						log.info("on operate start ti card wChairID[%d] cbTiCardData[0x%02x] tid[%s].\n", wChairID, cbTiCardData, tid.c_str());
						//保存录像
						save_operate_video(wChairID, ACK_TI, cbTiCardData);
					}
					for (_uint8 i = 0; i < cbTempTiCount; ++i)
					{
						_uint8 bSendCardData = m_cbRepertoryCard[--m_bLeftCardCount];
						m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(bSendCardData)]++;
						//广播翻牌
						std::map<int, Player*>::iterator iter = players.begin();
						for (; iter != players.end(); ++iter)
						{
							Player* player = iter->second;
							if (NULL == player) continue;
							_tint32 wCurChairID = player->seatid;

							proto::game::AckDispatchCard stDispachCard;
							stDispachCard.set_seatid(wChairID);
							stDispachCard.set_card(bSendCardData);
							stDispachCard.set_left_card_count(m_bLeftCardCount);
							stDispachCard.set_to_seatid(wChairID);
							if (wCurChairID == wChairID)
							{
								UpdateCommonCardsWeave(wCurChairID);
								UpdateBestCardsWeave(wCurChairID);
								if (m_bCommonOrBest[wCurChairID] == false)
								{
									for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i)
									{
										proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
										for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j)
										{
											pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
										}
									}
								}
								else
								{
									for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i)
									{
										proto::game::tagHandCards* pstHandCards = stDispachCard.add_hand_cards();
										for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j)
										{
											pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
										}
									}
								}
								std::vector<_uint8> vecListenCards;
								if (IsListen(wCurChairID, vecListenCards))
								{
									for (_uint8 i = 0; i < vecListenCards.size(); ++i)
										stDispachCard.add_listen_cards(vecListenCards[i]);
								}
								_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
								stDispachCard.set_total_huxi(cbTotalHuXi);
							}
							if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
							{
								stDispachCard.set_off_count(m_cbOfflineCount[wCurChairID]);
							}
							Ppacket ppack;
							stDispachCard.SerializeToString(&ppack.body);
							ppack.pack(SERVER_PHZ_SEND_CARD);
							unicast(player, ppack.data);
							log.debug("uid[%d] tid[%s] stDispachCard[%s] .\n", player->uid, tid.c_str(), stDispachCard.DebugString().c_str());
							//保存离线数据
							save_offline_info(wCurChairID, SERVER_PHZ_SEND_CARD, stDispachCard.SerializeAsString());
						}
						//保存录像
						save_dispatch_card_video(wChairID, bSendCardData, wChairID);
					}
				} while (true);
			}
		}
	}
	log.info("out estimate user respond para wCenterUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wCenterUser, cbCenterCard, tid.c_str());
	//操作清零
	memset(m_bResponse, 0, sizeof(m_bResponse));
	memset(m_bUserAction, 0, sizeof(m_bUserAction));
	memset(m_bPerformAction, 0, sizeof(m_bPerformAction));
	memset(m_cbUserChiCardCount, 0, sizeof(m_cbUserChiCardCount));
	memset(m_UserChiCardInfo, 0, sizeof(m_UserChiCardInfo));
	memset(m_cbUserChiKind, 0, sizeof(m_cbUserChiKind));
	memset(m_cbUserSecondChiKind, 0, sizeof(m_cbUserSecondChiKind));
	memset(m_cbUserThirdChiKind, 0, sizeof(m_cbUserThirdChiKind));
	log.info("out estimate user respond clear operate .\n");

	bool bAction = false;
	//判断点炮
	bool isPaoPai = IsPaoCard(wCenterUser, cbCenterCard);
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		_tint32 wTmpSeatid = (wCenterUser + i) % m_GamePlayer;
		if (wTmpSeatid == wCenterUser) continue;
		//胡牌判断
		tagHuCardInfo stHuInfo;
		std::vector<tagHuFanInfo> vecHuFanInfo;
		bool bZimo = false;
		if (m_bDispatch && wTmpSeatid == m_wCurrentUser) bZimo = true;			//是否自摸
		bool bHu = m_GameLogic.GetHuCardInfoFinal(m_cbCardIndex[wTmpSeatid], cbCenterCard, m_bWeaveItemCount[wTmpSeatid], m_WeaveItemArray[wTmpSeatid], stHuInfo, m_wBaseHuXi, vecHuFanInfo, bZimo, m_bDispatch, m_bHHD, false, m_bWithMaoHu);
		if (bHu && !m_bXiangGong[wTmpSeatid])
		{
			m_bCurrentMustHu = false;
			m_bDispatchHu = true;
			m_bUserAction[wTmpSeatid] |= ACK_CHIHU;
			m_bIsDiHuOut = true;
			bAction = true;
			
			m_bCurrentMustHu = (1 == m_cbMustHu || 2 == m_cbMustHu);
			if (m_bCurrentMustHu)
			{
				proto::game::AckOperateNotify stNotity;
				stNotity.set_operate_type(m_bUserAction[wTmpSeatid]);
				stNotity.set_operate_card(cbCenterCard);
				stNotity.set_off_count(m_cbOfflineCount[wTmpSeatid]);
				m_bCurrentMustHu = (1 == m_cbMustHu || 2 == m_cbMustHu);
				stNotity.set_is_must_hu(m_bCurrentMustHu);

				Ppacket ppack;
				stNotity.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_NOTIFY);
				unicast(seats[wTmpSeatid].player, ppack.data);
				log.debug("uid[%d] tid[%s] stNotity[%s] .\n", seats[wTmpSeatid].player->uid, tid.c_str(), stNotity.DebugString().c_str());
				//保存离线数据
				save_offline_info(wTmpSeatid, SERVER_PHZ_OPERATE_NOTIFY, stNotity.SerializeAsString());
				log.info("on operate start tian hu wTmpSeatid[%d] m_bUserAction[0x%02x] cbCenterCard[0x%02x] tid[%s] .\n",
						 wTmpSeatid, m_bUserAction[wTmpSeatid], cbCenterCard, tid.c_str());
				//保存录像
				save_operate_notify_video();
				//保存数据到redis
				SerializeTableData();
				return true;
			}
			/*
			if (1 == m_cbMustHu || 2 == m_cbMustHu)
			{
				GameEnd(wTmpSeatid); //点炮必胡
				return true;
			}*/
		}

		//吃碰判断
		if (!isPaoPai)
		{
			_uint8 cbType = proto::game::CHOU_CODE_NULL;
			_uint8 cbOperateIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
			if (m_bPengAbandon[wTmpSeatid][cbOperateIndex] == false && IsAllowChiPeng(wTmpSeatid) && !m_bDongZhang[wTmpSeatid]) //臭牌不能吃碰
			{
				if ((m_GameLogic.IsWeiPengCard(m_cbCardIndex[wTmpSeatid], cbCenterCard) == true) && (IsPassPengCard(wTmpSeatid, cbCenterCard) == false))
				{
					m_bUserAction[wTmpSeatid] |= ACK_PENG;
					bAction = true;
				}
			}
			else //臭牌提示
			{
				if ((m_GameLogic.IsWeiPengCard(m_cbCardIndex[wTmpSeatid], cbCenterCard) == true)
					&& m_bPengAbandon[wTmpSeatid][cbOperateIndex] == true) cbType = proto::game::CHOU_CODE_PENG;
			}
			if (IsPassCard(wTmpSeatid, cbCenterCard) == false && m_bChiAbandon[wTmpSeatid][cbOperateIndex] == false && IsAllowChiPeng(wTmpSeatid) && !m_bDongZhang[wTmpSeatid]) //臭牌不能吃碰
			{
				_tint32 wEatUser = (wCenterUser + 1) % m_GamePlayer;
				if (wTmpSeatid == wEatUser)
				{
					m_cbUserChiCardCount[wTmpSeatid] = m_GameLogic.GetChiCardInfo(m_cbCardIndex[wTmpSeatid], cbCenterCard, m_UserChiCardInfo[wTmpSeatid]);
					if (m_cbUserChiCardCount[wTmpSeatid] > 0)
					{
						m_bUserAction[wTmpSeatid] |= ACK_CHI;
						bAction = true;
					}
				}
			}
			else //臭牌提示
			{
				_uint8 cbTmpChiCount = 0;
				tagNewChiCardInfo stTmpChiInfo[6];		//玩家吃牌组合
				_tint32 wTmpEatUser = (wCenterUser + 1) % m_GamePlayer;
				if (wTmpSeatid == wTmpEatUser)
				{
					cbTmpChiCount = m_GameLogic.GetChiCardInfo(m_cbCardIndex[wTmpSeatid], cbCenterCard, stTmpChiInfo);
					if (cbTmpChiCount > 0 && m_bChiAbandon[wTmpSeatid][cbOperateIndex] == true)
					{
						if (cbType == proto::game::CHOU_CODE_PENG) cbType = proto::game::CHOU_CODE_CHIPENG;
						else cbType = proto::game::CHOU_CODE_CHI;
					}
				}
			}
			if (cbType != proto::game::CHOU_CODE_NULL) SendChouNotify(wTmpSeatid, cbType);
		}
	}
	//发送操作提示
	if (bAction == true)
	{
		for (_uint8 i = 0; i < m_GamePlayer; i++)
		{
			Player* player = seats[i].player;
			if (player == NULL) continue;
			if (m_bUserAction[i] == ACK_NULL) continue;
			proto::game::AckOperateNotify stNotity;
			stNotity.set_operate_type(m_bUserAction[i]);
			stNotity.set_operate_card(m_cbCurrentCard);
			if (m_bUserAction[i] & ACK_CHI)
			{
				for (_uint8 j = 0; j < m_cbUserChiCardCount[i]; ++j)
				{
					proto::game::tagChiCardInfo* pstChiCardInfo = stNotity.add_chicard_info();
					pstChiCardInfo->set_chi_kind(m_UserChiCardInfo[i][j].cbChiKind);
					pstChiCardInfo->set_center_card(m_UserChiCardInfo[i][j].cbCenterCard);
					for (_uint8 c = 0; c < 3; c++) pstChiCardInfo->add_cards(m_UserChiCardInfo[i][j].cbCardData[c]);
					for (_uint8 k = 0; k < m_UserChiCardInfo[i][j].cbSecondCount; ++k)
					{
						proto::game::tagSecondInfo* pstSecondInfo = pstChiCardInfo->add_second_info();
						pstSecondInfo->set_chi_kind(m_UserChiCardInfo[i][j].SecondData[k].cbChiKind);
						pstSecondInfo->set_center_card(m_UserChiCardInfo[i][j].SecondData[k].cbCenterCard);
						for (_uint8 c = 0; c < 3; c++) pstSecondInfo->add_cards(m_UserChiCardInfo[i][j].SecondData[k].cbCardData[c]);
						for (_uint8 m = 0; m < m_UserChiCardInfo[i][j].SecondData[k].cbThirdCount; ++m)
						{
							proto::game::tagThirdInfo* pstThirdInfo = pstSecondInfo->add_third_info();
							pstThirdInfo->set_chi_kind(m_UserChiCardInfo[i][j].SecondData[k].ThirdData[m].cbChiKind);
							pstThirdInfo->set_center_card(m_UserChiCardInfo[i][j].SecondData[k].ThirdData[m].cbCenterCard);
							for (_uint8 c = 0; c < 3; c++) pstThirdInfo->add_cards(m_UserChiCardInfo[i][j].SecondData[k].ThirdData[m].cbCardData[c]);
						}
					}
				}
			}
			stNotity.set_off_count(m_cbOfflineCount[i]);
			Ppacket ppack;
			stNotity.SerializeToString(&ppack.body);
			ppack.pack(SERVER_PHZ_OPERATE_NOTIFY);
			unicast(player, ppack.data);
			log.debug("uid[%d] tid[%s] stNotity[%s] .\n", player->uid, tid.c_str(), stNotity.DebugString().c_str());
			//保存离线数据
			save_offline_info(i, SERVER_PHZ_OPERATE_NOTIFY, stNotity.SerializeAsString());
			log.info("out estimate chipeng seatid[%d] m_bUserAction[0x%02x] tid[%s] .\n", i, m_bUserAction[i], tid.c_str());
		}
		//保存录像
		save_operate_notify_video();

		time_t end = time(NULL);
		int total = end - begin;
		if (total >= 1)
		{
			log.error("slow cmd:[%s] second[%d]\n", __FUNCTION__, total);
		}

		return true;
	}

	//跑牌判断
	_tint32 wPaoChairID = INVALID_CHAIR;
	m_bPaoCard = false;
	for (_uint8 i = 0; i < m_GamePlayer; i++)
	{
		if (i == wCenterUser) continue;
		if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[i], cbCenterCard) == true)
		{
			_tint32 wActionUser = i;
			_uint8 cbRemoveIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
			_uint8 cbRemoveCount = m_cbCardIndex[wActionUser][cbRemoveIndex];
			m_cbCardIndex[wActionUser][cbRemoveIndex] = 0;
			//设置组合
			_uint8 cbIndex = m_bWeaveItemCount[wActionUser]++;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardCount = 4;
			m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind = ACK_PAO;
			m_WeaveItemArray[wActionUser][cbIndex].cbCenterCard = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[0] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[1] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[2] = cbCenterCard;
			m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3] = cbCenterCard;
			//更新胡息
			//UpdateUserHuXiCount(wActionUser);
			UpdateCardsWeave(wActionUser, m_WeaveItemArray[wActionUser][cbIndex].cbCardList, cbRemoveCount);
			//广播跑牌
			std::map<int, Player*>::iterator iter = players.begin();
			for (; iter != players.end(); ++iter)
			{
				Player* player = iter->second;
				if (NULL == player) continue;
				_tint32 wCurChairID = player->seatid;

				proto::game::AckOperateCard stOperateCard;
				stOperateCard.set_ack_code(ACK_PAO);
				stOperateCard.set_seatid(wActionUser);
				stOperateCard.set_action_card(cbCenterCard);
				stOperateCard.set_remove_count(cbRemoveCount);
				if (wCurChairID == wActionUser)
				{
					if (m_bCommonOrBest[wCurChairID] == false) {
						for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					else {
						for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
							proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
							for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
								pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
							}
						}
					}
					std::vector<_uint8> vecListenCards;
					if (IsListen(wCurChairID, vecListenCards))
					{
						for (_uint8 i = 0; i < vecListenCards.size(); ++i)
							stOperateCard.add_listen_cards(vecListenCards[i]);
					}
					_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
					stOperateCard.set_total_huxi(cbTotalHuXi);
				}
				stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wActionUser]);
				if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
				{
					stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
				}
				Ppacket ppack;
				stOperateCard.SerializeToString(&ppack.body);
				ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
				unicast(player, ppack.data);
				log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
				//保存离线数据
				save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
			}
			log.info("out estimate user respond pao wActionUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wActionUser, cbCenterCard, tid.c_str());
			//保存录像
			save_operate_video(wActionUser, ACK_PAO, cbCenterCard);
			wPaoChairID = i;
			m_bPaoCard = true;
			m_bCardOperated = true;
			break;
		}
	}

	//跑牌转换
	if (m_bPaoCard == false)
	{
		for (_uint8 i = 0; i < m_GamePlayer; i++)
		{
			if (i == wCenterUser) continue;
			for (_uint8 cbIndex = 0; cbIndex < m_bWeaveItemCount[i]; cbIndex++)
			{
				_uint8 cbWeaveKind = m_WeaveItemArray[i][cbIndex].cbWeaveKind;
				_uint8 cbWeaveCard = m_WeaveItemArray[i][cbIndex].cbCardList[0];
				bool bChangeWeave = false;
				if (cbCenterCard == cbWeaveCard)
				{
					if (cbWeaveKind == ACK_WEI) bChangeWeave = true;
				}
				if (bChangeWeave == false) continue;
				_tint32 wActionUser = i;
				//设置组合
				m_WeaveItemArray[wActionUser][cbIndex].cbCardCount = 4;
				m_WeaveItemArray[wActionUser][cbIndex].cbWeaveKind = ACK_PAO;
				m_WeaveItemArray[wActionUser][cbIndex].cbCardList[3] = cbCenterCard;
				//更新胡息
				UpdateUserHuXiCount(wActionUser);
				//广播跑牌
				std::map<int, Player*>::iterator iter = players.begin();
				for (; iter != players.end(); ++iter)
				{
					Player* player = iter->second;
					if (NULL == player) continue;
					_tint32 wCurChairID = player->seatid;

					proto::game::AckOperateCard stOperateCard;
					stOperateCard.set_ack_code(ACK_PAO);
					stOperateCard.set_seatid(wActionUser);
					stOperateCard.set_action_card(cbCenterCard);
					stOperateCard.set_remove_count(0);
					if (wCurChairID == wActionUser)
					{
						if (m_bCommonOrBest[wCurChairID] == false) {
							for (_uint8 i = 0; i < m_cbCardsWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_CardsWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_CardsWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						else {
							for (_uint8 i = 0; i < m_cbBestWeaveCount[wCurChairID]; ++i) {
								proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
								for (_uint8 j = 0; j < m_BestWeaveInfo[wCurChairID][i].cbCardCount; ++j) {
									pstHandCards->add_card(m_BestWeaveInfo[wCurChairID][i].cbCardList[j]);
								}
							}
						}
						std::vector<_uint8> vecListenCards;
						if (IsListen(wCurChairID, vecListenCards))
						{
							for (_uint8 i = 0; i < vecListenCards.size(); ++i)
								stOperateCard.add_listen_cards(vecListenCards[i]);
						}
						_uint8 cbTotalHuXi = m_bHandsHuXiCount[wCurChairID] + m_bWeaveHuXiCount[wCurChairID];
						stOperateCard.set_total_huxi(cbTotalHuXi);
					}
					stOperateCard.set_hu_xi(m_bWeaveShowHuXiCount[wActionUser]);
					if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
					{
						stOperateCard.set_off_count(m_cbOfflineCount[wCurChairID]);
					}
					Ppacket ppack;
					stOperateCard.SerializeToString(&ppack.body);
					ppack.pack(SERVER_PHZ_OPERATE_CARD_RES);
					unicast(player, ppack.data);
					log.debug("uid[%d] tid[%s] stOperateCard[%s] .\n", player->uid, tid.c_str(), stOperateCard.DebugString().c_str());
					//保存离线数据
					save_offline_info(wCurChairID, SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
					log.debug("stOperate Pao tid[%s] content[%s] .\n", tid.c_str(), stOperateCard.DebugString().c_str());
				}
				log.info("out estimate user respond weipeng to pao wActionUser[%d] cbCenterCard[0x%02x] tid[%s].\n", wActionUser, cbCenterCard, tid.c_str());
				//保存录像
				save_operate_video(wActionUser, ACK_PAO, cbCenterCard);
				wPaoChairID = i;
				m_bPaoCard = true;
				m_bCardOperated = true;
				break;
			}
			if (m_bPaoCard == true) break;
		}
	}

	time_t end = time(NULL);
	int total = end - begin;
	if (total >= 1)
	{
		log.error("slow cmd:[%s] second[%d]\n",__FUNCTION__, total);
	}

	//跑后判断
	if (m_bPaoCard && wPaoChairID != INVALID_CHAIR)
	{
		//出牌提示
		if (IsAllowOutCard(wPaoChairID))
		{
			SendOutCardNotify(wPaoChairID, true);
			m_bAfterWeave = true;
			return true;
		}
		else
		{
			m_wCurrentUser = wPaoChairID;
			return false;
		}
	}

	return false;
}

//出牌判断
bool Table::IsAllowOutCard(_tint32 wChairID)
{
	//跑提计算
	_uint8 cbTiPaoCount = 0;
	for (int i = 0; i < m_bWeaveItemCount[wChairID]; i++)
	{
		if (m_WeaveItemArray[wChairID][i].cbCardCount == 4) cbTiPaoCount++;
	}

	return (cbTiPaoCount <= 1) ? true : false;
}

//更新胡息
bool Table::UpdateUserHuXiCount(_tint32 wChairID)
{
	//胡息计算
	_uint8 bUserHuXiCount = 0;
	_uint8 bUserShowHuXiCount = 0;
	for (_uint8 i = 0; i < m_bWeaveItemCount[wChairID]; i++)
	{
		bUserHuXiCount += m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][i]);
		if ((m_WeaveItemArray[wChairID][i].cbWeaveKind == ACK_WEI
			|| m_WeaveItemArray[wChairID][i].cbWeaveKind == ACK_TI) && m_bMingXiao == false) continue;
		bUserShowHuXiCount += m_GameLogic.GetWeaveHuXi(m_WeaveItemArray[wChairID][i]);
	}

	//设置胡息
	m_bWeaveHuXiCount[wChairID] = bUserHuXiCount;
	m_bWeaveShowHuXiCount[wChairID] = bUserShowHuXiCount;
	
	m_bHandsHuXiCount[wChairID] = m_GameLogic.GetCardsWeaveHuXi(m_bCommonOrBest[wChairID] ? m_BestWeaveInfo[wChairID] : m_CardsWeaveInfo[wChairID],
																m_bCommonOrBest[wChairID] ? m_cbBestWeaveCount[wChairID] : m_cbCardsWeaveCount[wChairID]);


	return true;
}

int Table::heart_beat_res(Player* player)
{
	Ppacket ppack;
	ppack.pack(SERVER_HEART_BEAT_RES);
	unicast(player, ppack.data);

	return 0;
}

//获取番类型和番数
_uint32 Table::GetHuFanType(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbWeaveItemCount, tagWeaveItem WeaveItemArray[MAX_WEAVE], tagHuCardInfo HuCardInfo, _uint8 cbHuCard, std::vector<tagHuFanInfo> &vecHuFanInfo)
{
	_uint32 wTotalFanNum = 0;
	_uint8 cbTotalCardCount = 0;
	_uint8 cbHongZiCount = 0;
	_uint8 cbHeiZiCount = 0;
	_uint8 cbDaZiCount = 0;
	_uint8 cbXiaoZiCount = 0;
	_uint8 cbChiWeaveCount = 0;
	_uint8 cbHandCardCount = 0;
	std::vector<_uint8> vecHongCards;
	vecHongCards.clear();
	tagHuFanInfo HuFanInfo;

	//组合牌
	for (_uint8 i = 0; i < cbWeaveItemCount && i < MAX_WEAVE; ++i)
	{
		cbTotalCardCount += WeaveItemArray[i].cbCardCount;
		if (WeaveItemArray[i].cbWeaveKind == ACK_CHI) cbChiWeaveCount++;
		for (_uint8 j = 0; j < WeaveItemArray[i].cbCardCount && j < 4; ++j)
		{
			_uint8 cbCardData = WeaveItemArray[i].cbCardList[j];
			if (cbCardData == INVALID_CARD)
			{
				cbTotalCardCount--;
				continue;
			}
			if (CardNum(cbCardData) == 2 || CardNum(cbCardData) == 7 || CardNum(cbCardData) == 10)
			{
				cbHongZiCount++;
				vecHongCards.push_back(cbCardData);
			}
			else cbHeiZiCount++;
			if (CardType(cbCardData) == 1) cbDaZiCount++;
			else cbXiaoZiCount++;
		}
	}
	//手牌
	for (_uint8 i = 0; i < MAX_INDEX; ++i)
	{
		cbHandCardCount += cbCardIndex[i];
	}
	//手牌组合牌
	for (_uint8 i = 0; i < HuCardInfo.cbWeaveCount; ++i)
	{
		cbTotalCardCount += HuCardInfo.WeaveItemArray[i].cbCardCount;
		for (_uint8 j = 0; j < HuCardInfo.WeaveItemArray[i].cbCardCount; ++j)
		{
			_uint8 cbCardData = HuCardInfo.WeaveItemArray[i].cbCardList[j];
			if (CardNum(cbCardData) == 0x02 || CardNum(cbCardData) == 0x07 || CardNum(cbCardData) == 0x0a)
			{
				cbHongZiCount++;
				vecHongCards.push_back(cbCardData);
			}
			else cbHeiZiCount++;
			if (CardType(cbCardData) == 1) cbDaZiCount++;
			else cbXiaoZiCount++;
		}

		if (HuCardInfo.WeaveItemArray[i].cbWeaveKind == ACK_CHI) cbChiWeaveCount++;
	}
	//牌眼统计
	if (HuCardInfo.cbCardEye != 0 && HuCardInfo.cbCardEye != INVALID_CARD)
	{
		cbTotalCardCount += 2;
		_uint8 cbCardData = HuCardInfo.cbCardEye;
		if (CardNum(cbCardData) == 0x02 || CardNum(cbCardData) == 0x07 || CardNum(cbCardData) == 0x0a)
		{
			cbHongZiCount += 2;
			vecHongCards.push_back(cbCardData);
			vecHongCards.push_back(cbCardData);
		}
		else cbHeiZiCount += 2;
		if (CardType(cbCardData) == 1) cbDaZiCount += 2;
		else cbXiaoZiCount += 2;
	}

	return wTotalFanNum;
}

bool Table::OnUserChangeCard(Player* player)
{
	if (player->seatid < 0 || player->seatid >= m_GamePlayer)
	{
		log.info("OnUserChangeCard error uid[%d] seatid[%d] .\n", player->uid, player->seatid);
		return false;
	}
	_uint8 wChairID = player->seatid;

	proto::game::ReqChangeCard stReqChange;
	stReqChange.ParseFromString(player->client->ppacket.body);
	log.debug("uid[%d] tid[%s] stReqChange[%s] .\n", player->uid, tid.c_str(), stReqChange.DebugString().c_str());
	_uint8 cbChangeCount = stReqChange.hand_cards_size();
	_uint8 cbCardsCount = 0;
	tagCardsWeave stCardsWeave[MAX_INDEX];
	memset(stCardsWeave, 0, sizeof(stCardsWeave));
	_uint8 cbCardIndexTemp[MAX_INDEX]; //统计手牌
	memset(cbCardIndexTemp, 0, sizeof(cbCardIndexTemp));
	for (_uint8 i = 0; i < cbChangeCount; ++i)
	{
		const proto::game::tagHandCards& stHandCards = stReqChange.hand_cards(i);
		_uint8 cbListCount = stHandCards.card_size();
		for (_uint8 j = 0; j < cbListCount; ++j)
		{
			_uint8 cbCardData = stHandCards.card(j);;
			if (!m_GameLogic.IsValidCard(cbCardData)) //牌值是否正确
			{
				log.error("OnUserChangeCard error cbCardData[0x%02x] is InValid .\n", cbCardData);
				return false;
			}
			if (m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(cbCardData)] == 0) //手牌中是否有这张牌
			{
				log.error("OnUserChangeCard error cbCardData[0x%02x] is NotExsit .\n", cbCardData);
				return false;
			}
			stCardsWeave[i].cbCardList[stCardsWeave[i].cbCardCount++] = cbCardData;
			cbCardIndexTemp[m_GameLogic.SwitchToCardIndex(cbCardData)]++;
			cbCardsCount++;
		}
	}
	//牌数是否和手牌数相等
	_uint8 cbCardIndexCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; ++i)
	{
		if (cbCardIndexTemp[i] != m_cbCardIndex[wChairID][i])
		{
			log.error("on user change i[%d] num not same .\n", i);
			return false;
		}
		cbCardIndexCount += m_cbCardIndex[wChairID][i];
	}
	if (cbCardsCount != cbCardIndexCount)
	{
		log.error("on user change card cbCardsCount[%d] cbCardIndexCount[%d] not same.\n", cbCardsCount, cbCardIndexCount);
		return false;
	}

	m_bCommonOrBest[wChairID] = false;
	m_cbCardsWeaveCount[wChairID] = cbChangeCount;
	memcpy(m_CardsWeaveInfo[wChairID], stCardsWeave, sizeof(m_CardsWeaveInfo[wChairID]));

	UpdateUserHuXiCount(wChairID);
	
	proto::game::AckChangeCard stAckChange;
	for (_uint8 i = 0; i < m_cbCardsWeaveCount[wChairID]; ++i)
	{
		proto::game::tagHandCards* pstHandCards = stAckChange.add_hand_cards();
		for (_uint8 j = 0; j < m_CardsWeaveInfo[wChairID][i].cbCardCount; ++j)
		{
			pstHandCards->add_card(m_CardsWeaveInfo[wChairID][i].cbCardList[j]);
		}
	}
	stAckChange.set_total_huxi(m_bHandsHuXiCount[wChairID] + m_bWeaveHuXiCount[wChairID]);
	Ppacket ppack;
	stAckChange.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_CHANGE_CARD_RES);
	unicast(player, ppack.data);
	log.debug("uid[%d] tid[%s] stAckChange[%s] .\n", player->uid, tid.c_str(), stAckChange.DebugString().c_str());
	return true;
}

bool Table::UpdateCommonCardsWeave(_tint32 wChairID)
{
	m_cbCardsWeaveCount[wChairID] = 0;
	memset(m_CardsWeaveInfo[wChairID], 0, sizeof(m_CardsWeaveInfo[wChairID]));

	//m_GameLogic.SettleCommonWeaves(m_cbCardIndex[wChairID], m_CardsWeaveInfo[wChairID], m_cbCardsWeaveCount[wChairID]);
	m_GameLogic.SettleOrderWeaves(m_cbCardIndex[wChairID], m_CardsWeaveInfo[wChairID], m_cbCardsWeaveCount[wChairID]);
	return true;
}

bool Table::UpdateBestCardsWeave(_tint32 wChairID)
{
	m_cbBestWeaveCount[wChairID] = 0;
	memset(m_BestWeaveInfo[wChairID], 0, sizeof(m_BestWeaveInfo[wChairID]));

	m_GameLogic.SettleBestWeaves(m_cbCardIndex[wChairID], m_BestWeaveInfo[wChairID], m_cbBestWeaveCount[wChairID]);
	//m_bHandsHuXiCount[wChairID] = m_GameLogic.GetCardsWeaveHuXi(m_BestWeaveInfo[wChairID], m_cbBestWeaveCount[wChairID]);

	UpdateUserHuXiCount(wChairID);

	log.info("update best cards weave wChairID[%d] cbHandsHuXiCount[%d] .\n", wChairID, m_bHandsHuXiCount[wChairID]);
	return true;
}

bool Table::OnUserUpdateCard(Player* player)
{
	if (player->seatid < 0 || player->seatid >= m_GamePlayer)
	{
		log.info("OnUserUpdateCard error uid[%d] seatid[%d] .\n", player->uid, player->seatid);
		return false;
	}
	_tint32 wChairID = player->seatid;
	m_bCommonOrBest[wChairID] = (m_bCommonOrBest[wChairID] == true) ? false : true;
	UpdateBestCardsWeave(wChairID);

	proto::game::AckUpdateCard stUpdate;
	if (m_bCommonOrBest[wChairID] == false) {
		for (_uint8 i = 0; i < m_cbCardsWeaveCount[wChairID]; ++i) {
			proto::game::tagHandCards* pstHandCards = stUpdate.add_hand_cards();
			for (_uint8 j = 0; j < m_CardsWeaveInfo[wChairID][i].cbCardCount; ++j) {
				pstHandCards->add_card(m_CardsWeaveInfo[wChairID][i].cbCardList[j]);
			}
		}
	}
	else {
		for (_uint8 i = 0; i < m_cbBestWeaveCount[wChairID]; ++i) {
			proto::game::tagHandCards* pstHandCards = stUpdate.add_hand_cards();
			for (_uint8 j = 0; j < m_BestWeaveInfo[wChairID][i].cbCardCount; ++j) {
				pstHandCards->add_card(m_BestWeaveInfo[wChairID][i].cbCardList[j]);
			}
		}
	}
	stUpdate.set_total_huxi(m_bHandsHuXiCount[wChairID] + m_bWeaveHuXiCount[wChairID]);
	std::vector<_uint8> vecListenCards;
	if (IsListen(wChairID, vecListenCards))
	{
		for (_uint8 i = 0; i < vecListenCards.size(); ++i)
		{
			stUpdate.add_listen_cards(vecListenCards[i]);
		}
	}
	Ppacket ppack;
	stUpdate.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_UPDATE_CARD_RES);
	unicast(player, ppack.data);
	log.debug("uid[%d] tid[%s] stUpdate[%s] .\n", player->uid, tid.c_str(), stUpdate.DebugString().c_str());

	return true;
}

bool Table::UpdateCardsWeave(_tint32 wChairID, _uint8 cbRemoveCards[4], _uint8 cbRemoveCount, _uint8 cbRow, _uint8 cbCol)
{
	if (m_cbCardsWeaveCount[wChairID] == 0)
	{
		log.error("UpdateCardsWeave error cbCardsWeaveCount[%d] wrong .\n", m_cbCardsWeaveCount[wChairID]);
		return false;
	}

	m_GameLogic.UpdateCardsWeave(m_CardsWeaveInfo[wChairID], m_cbCardsWeaveCount[wChairID], cbRemoveCards, cbRemoveCount, cbRow, cbCol);
	m_GameLogic.UpdateCardsWeave(m_BestWeaveInfo[wChairID], m_cbBestWeaveCount[wChairID], cbRemoveCards, cbRemoveCount, cbRow, cbCol);
/*
	_uint8 cbWeaveCount = 0;
	tagCardsWeave WeaveItem[MAX_INDEX];
	memset(WeaveItem, 0, sizeof(tagCardsWeave)*MAX_INDEX);

	m_GameLogic.SettleBestWeaves(m_cbCardIndex[wChairID], WeaveItem, cbWeaveCount);
	m_bHandsHuXiCount[wChairID] = m_GameLogic.GetCardsWeaveHuXi(WeaveItem, cbWeaveCount);
*/

	UpdateUserHuXiCount(wChairID);
	
	return true;
}

bool Table::UpdateAbandonCard()
{
	if (m_wCurrentUser == INVALID_CHAIR || m_cbCurrentCard == INVALID_CARD)
	{
		log.error("update abandon card error m_wCurrentUser[%d] m_cbCurrentCard[0x%02x] .\n", m_wCurrentUser, m_cbCurrentCard);
		return false;
	}

	m_UserAbandonType[m_wCurrentUser][m_UserAbandonCount[m_wCurrentUser]] = (m_bDispatch == true) ? 1 : 0;
	m_UserAbandonCards[m_wCurrentUser][m_UserAbandonCount[m_wCurrentUser]++] = m_cbCurrentCard;
	std::map<int, Player*>::iterator iter = players.begin();
	for (; iter != players.end(); ++iter)
	{
		Player* player = iter->second;
		if (NULL == player) continue;
		_tint32 wCurChairID = player->seatid;
		proto::game::AckUpdateAbandon stAbandon;
		stAbandon.set_seatid(m_wCurrentUser);
		stAbandon.set_card(m_cbCurrentCard);
		for (_uint8 i = 0; i < m_UserAbandonCount[m_wCurrentUser]; ++i)
		{
			stAbandon.add_abandon_cards(m_UserAbandonCards[m_wCurrentUser][i]);
			stAbandon.add_abandon_type(m_UserAbandonType[m_wCurrentUser][i]);
		}
		if (wCurChairID >= 0 && wCurChairID < m_GamePlayer)
		{
			stAbandon.set_off_count(m_cbOfflineCount[wCurChairID]);
		}
		Ppacket ppack;
		stAbandon.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_UPDATE_ABANDON_CARD);
		unicast(player, ppack.data);
		log.debug("uid[%d] tid[%s] stAbandon[%s] .\n", player->uid, tid.c_str(), stAbandon.DebugString().c_str());
		//保存离线数据
		save_offline_info(wCurChairID, SERVER_PHZ_UPDATE_ABANDON_CARD, stAbandon.SerializeAsString());
	}
	//保存录像
	save_abandon_card_video(m_wCurrentUser);
	return true;
}

bool Table::IsPaoCard(_tint32 wCenterUser, _uint8 cbCenterCard)
{
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (i == wCenterUser) continue;
		if (m_GameLogic.IsTiPaoCard(m_cbCardIndex[i], cbCenterCard) == true) return true;
	}

	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		for (_uint8 cbIndex = 0; cbIndex < m_bWeaveItemCount[i]; cbIndex++)
		{
			_uint8 cbWeaveKind = m_WeaveItemArray[i][cbIndex].cbWeaveKind;
			_uint8 cbWeaveCard = m_WeaveItemArray[i][cbIndex].cbCardList[0];
			bool bChangeWeave = false;
			if (cbCenterCard == cbWeaveCard)
			{
				if (cbWeaveKind == ACK_WEI && i != wCenterUser) bChangeWeave = true;
				else if (cbWeaveKind == ACK_PENG && m_bDispatch) bChangeWeave = true;
			}
			if (bChangeWeave == false) continue;
			return true;
		}
	}

	return false;
}

void Table::user_offline(Player * player)
{
	//检测是否所有玩家都已经断线
	bool bIsAll = true;
	map<int, Player *>::iterator iter = players.begin();
	for (; iter != players.end(); iter++)
	{
		Player *player = iter->second;
		if (player)
		{
			if (player->client != NULL)
			{
				bIsAll = false;
				break;
			}
		}
	}
	if (bIsAll)
	{
		Jpacket packet;
		packet.val["cmd"] = SERVER_ALL_PLAYER_DOWN;
		packet.val["vid"] = zjh.game->server_vid;
		packet.val["roomid"] = player->tid;
		packet.end();
		zjh.game->send_to_datasvr(packet.tostring());
	}
}

int Table::advance_game_end()
{
	log.debug("advance game_end\n");
	state = GAME_END;
	ev_timer_stop(zjh.loop, &start_timer);
	ev_timer_stop(zjh.loop, &preready_timer);
	ev_timer_stop(zjh.loop, &dispatch_show_timer);
	ev_timer_stop(zjh.loop, &out_show_timer);
	ev_timer_stop(zjh.loop, &dissolve_room_timer);
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (seats[i].player != NULL) seats[i].player->down_tag = 1;
	}
	is_dissolved = 1;
	m_cbCurTimer = 0;
	return 0;
}

int Table::handler_dissolve_room(Player *player)
{
	if (dissovle_state == 1 || player->seatid < 0 || player->seatid >= m_GamePlayer || is_dissolved == 1)
	{
		log.error("apply dissovling room error uid[%d] seatid[%d] dissovle_state[%d] is_dissolved[%d] tid[%s] .\n", player->uid, player->seatid, dissovle_state, is_dissolved, tid.c_str());
		return -1;
	}

	//如果是房主申请解散，并且房间状态不是进行中，直接解散房间
	//if( m_cbRoomStatus != 1 && player->uid == (int)m_wRoomUserId ) 
	if (m_cbRoomStatus != 1) //房间不是进行中，就直接解散 
	{
		proto::game::AckDissolveResult stResult;
		stResult.set_result(1);
		Ppacket ppack;
		stResult.SerializeToString(&ppack.body);
		ppack.pack(SERVER_DISSOLVE_ROOM_RESULT_BC);
		broadcast(NULL, ppack.data);
		//发送给平台
		Jpacket datapac;
		datapac.val["cmd"] = SERVER_DZ_DISSOLVE_ROOM_REQ;
		datapac.val["roomid"] = tid;
		datapac.val["vid"] = vid;
		datapac.val["innings"] = m_strInnings;
		datapac.end();
		zjh.game->send_to_datasvr(datapac.tostring());
		dissovle_state = 0;
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].player != NULL) seats[i].player->dissovle_state = 2;
		}
		log.info("handler dissolve room player[%d] apply dissolve room[%s] \n", player->uid, tid.c_str());
		return 0;
	}

	if (player->dissolve_cnt >= 2)
	{
		proto::game::AckDissolveErr stErr;
		stErr.set_code(101);
		Ppacket ppack;
		stErr.SerializeToString(&ppack.body);
		ppack.pack(SERVER_APPLY_DISSOLVE_ROOM_ERR_UC);
		unicast(player, ppack.data);
		return 0;
	}

	player->dissolve_cnt++;
	dissovle_state = 1;
	dissolve_applyer = player->uid;
	voters_players.clear();

	log.info("handler dissolve room player[%d] apply dissolve room[%s] \n", player->uid, tid.c_str());

	proto::game::AckDissolveSucc stSucc;
	stSucc.set_uid(player->uid);
	stSucc.set_remain_time(dissolve_room_tstamp);
	for (_uint8 j = 0; j < m_GamePlayer; ++j)
	{
		if (seats[j].player == NULL) continue;
		if (player->uid == seats[j].uid) //|| seats[j].player->is_offline == 1 ) 
			continue;
		stSucc.add_voters_uid(seats[j].uid);
		voters_players[seats[j].uid] = seats[j].player;
	}
	Ppacket ppack;
	stSucc.SerializeToString(&ppack.body);
	ppack.pack(SERVER_APPLY_DISSOLVE_ROOM_SUCC_BC);
	broadcast(NULL, ppack.data);

	ev_timer_again(zjh.loop, &dissolve_room_timer);
	m_cbCurTimer = 7;
	check_dissovle_result();
	return 0;
}

int Table::handler_dissolve_action(Player *player)
{
	if (dissovle_state != 1) {
		log.error("dissovle_state[%d] is error\n", dissovle_state);
		return -1;
	}
	if (player->dissovle_state != 2) {
		log.error("player[%d] vote error\n", player->uid);
		return -1;
	}
	proto::game::ReqDissolveAct stAct;
	stAct.ParseFromString(player->client->ppacket.body);

	int uid = stAct.uid();
	int act = stAct.action();
	if (voters_players.find(uid) == voters_players.end())
	{
		log.error("voters_uid not cantain uid[%d]\n", uid);
		return -1;
	}
	log.info("handler dissovle action player[%d] act[%d] room[%s] \n", player->uid, act, tid.c_str());
	player->dissovle_state = act;

	proto::game::AckDissolveActSucc stActSucc;
	stActSucc.set_uid(uid);
	stActSucc.set_action(act);
	Ppacket ppack;
	stActSucc.SerializeToString(&ppack.body);
	ppack.pack(SERVER_DISSOLVE_ACTION_SUCC_BC);
	broadcast(NULL, ppack.data);

	check_dissovle_result();
	return 0;
}

void Table::check_dissovle_result()
{
	unsigned int _cnt_yes = 0;
	unsigned int _cnt_no = 0;
	std::map<int, Player*>::iterator it = voters_players.begin();
	for (; it != voters_players.end(); it++)
	{
		if (it->second->dissovle_state == 0) {
			_cnt_no++;
		}
		else if (it->second->dissovle_state == 1) {
			_cnt_yes++;
		}
	}
	if (_cnt_yes + _cnt_no == voters_players.size() || _cnt_no > 0)
	{
		ev_timer_stop(zjh.loop, &dissolve_room_timer);
		m_cbCurTimer = 0;

		proto::game::AckDissolveResult stResult;
		stResult.set_result(_cnt_yes == voters_players.size() ? 1 : 0);
		Ppacket ppack;
		stResult.SerializeToString(&ppack.body);
		ppack.pack(SERVER_DISSOLVE_ROOM_RESULT_BC);
		broadcast(NULL, ppack.data);

		if (_cnt_yes == voters_players.size())
		{
			//发送录像
			if (state == GAME_PLAYING)
			{
				tagGain stGain;
				for( _uint8 i = 0; i < GAME_PLAYER; ++i )
				{
					stGain.Gain[i] = 0;
				}
				m_vecGains.push_back(stGain);

				ev_timer_again(zjh.loop, &send_videodata_timer);
			}
			m_cbCurTimer = 8;
			Jpacket pac;
			pac.val["cmd"] = SERVER_DZ_DISSOLVE_ROOM_REQ;
			pac.val["roomid"] = tid;
			pac.val["vid"] = vid;
			pac.val["innings"] = m_strInnings;
			pac.end();
			zjh.game->send_to_datasvr(pac.tostring());
		}
		dissovle_state = 0;
		for (_uint8 i = 0; i < m_GamePlayer; ++i)
		{
			if (seats[i].player != NULL) seats[i].player->dissovle_state = 2;
		}
	}
}

void Table::dissolve_room_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->dissolve_room_timer);
	table->m_cbCurTimer = 0;
	std::map<int, Player*>::iterator it = table->voters_players.begin();
	for (; it != table->voters_players.end(); it++) {
		if (it->second->dissovle_state == 2) {
			it->second->dissovle_state = 1;
			table->check_dissovle_result();
		}
	}
}

//开始飘
bool Table::StartPiao()
{
	proto::game::AckStartPiao stStartPiao;
	stStartPiao.set_state(state);
	stStartPiao.set_banker(m_wBankerUser);
	Ppacket ppack;
	stStartPiao.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_START_PIAO);
	broadcast(NULL, ppack.data);
	//保存数据到redis
	SerializeTableData();
	return true;
}

void Table::PiaoError(Player *player, int code)
{
	proto::game::AckPiaoErr stPiaoErr;
	stPiaoErr.set_uid(player->uid);
	stPiaoErr.set_seatid(player->seatid);
	stPiaoErr.set_code(code);
	Ppacket ppack;
	stPiaoErr.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_PIAO_ERR);
	unicast(player, ppack.data);
}

bool Table::OnUserPiaoOperate(Player* player)
{
	if (state != GAME_PIAO)
	{
		log.error("on user piao operate error uid[%d] state[%d]  .\n", player->uid, state);
		PiaoError(player, 101);
		return -1;
	}

	_tint32 wChairID = player->seatid;
	if (wChairID < 0 || wChairID >= m_GamePlayer)
	{
		log.error("on user piao operate error uid[%d] seatid[%d] wrong .\n", player->uid, wChairID);
		PiaoError(player, 102);
		return -1;
	}

	if (m_UserPiaoScore[wChairID] >= 0)
	{
		log.error("on user piao operate error uid[%d] m_UserPiaoScore[%d] already piao .\n", player->uid, m_UserPiaoScore[wChairID]);
		PiaoError(player, 103);
		return -1;
	}

	proto::game::ReqPiao stReqPiao;
	stReqPiao.ParseFromString(player->client->ppacket.body);
	_uint8 my_count = stReqPiao.cout();
	if (my_count < 0 || (my_count > 0 && std::find(m_vecPiaoScore.begin(), m_vecPiaoScore.end(), my_count) == m_vecPiaoScore.end()))
	{
		log.error("on user piao operate uid[%d] my_count[%d] wrong or not find .\n", player->uid, my_count);
		PiaoError(player, 104);
		return -1;
	}
	m_UserPiaoScore[wChairID] = my_count;

	proto::game::AckPiaoSucc stPiaoSucc;
	stPiaoSucc.set_uid(player->uid);
	stPiaoSucc.set_seatid(player->seatid);
	stPiaoSucc.set_name(player->name);
	stPiaoSucc.set_avatar(player->avatar);
	stPiaoSucc.set_sex(player->sex);
	stPiaoSucc.set_piao(m_UserPiaoScore[wChairID]);
	Ppacket ppack;
	stPiaoSucc.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_PIAO_SUCC);
	broadcast(NULL, ppack.data);
	log.debug("uid[%d] tid[%s] stPiaoSucc[%s] .\n", player->uid, tid.c_str(), stPiaoSucc.DebugString().c_str());
	log.info("on user piao operate uid[%d] state[%d] seatid[%d] my_count[%d] tid[%s].\n ", player->uid, state, player->seatid, my_count, tid.c_str());

	//保存数据到redis
	SerializeTableData();

	_uint8 cbPiaoCount = 0;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (m_UserPiaoScore[i] >= 0) cbPiaoCount++;
	}
	if (cbPiaoCount == m_GamePlayer)
	{
		GameStart();
	}
	return 0;
}

bool Table::IsListen(_tint32 wChairID, std::vector<_uint8>& vecListenCards)
{
	//return false; //听牌会导致卡顿，暂时屏蔽
	
	time_t begin = time(NULL);
#if 1
	vecListenCards.clear();
	_uint8 cbCardDataArray[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A };
	_uint8 cbSize = sizeof(cbCardDataArray) / sizeof(cbCardDataArray[0]);
	for (_uint8 i = 0; i < cbSize; ++i)
	{
		tagHuCardInfo stHuInfo;
		std::vector<tagHuFanInfo> vecHuFanInfo;
		bool bHu = m_GameLogic.GetHuCardInfoFinal(m_cbCardIndex[wChairID], cbCardDataArray[i], m_bWeaveItemCount[wChairID], m_WeaveItemArray[wChairID], stHuInfo, m_wBaseHuXi, vecHuFanInfo, true, true, false, m_bWithMaoHu);
		if (bHu /*&& (IsListenOver(wChairID, cbCardDataArray[i]) == false)*/)
		{
			vecListenCards.push_back(cbCardDataArray[i]);
			log.info("Is listen wChairID[%d] listenCard[0x%02x] .\n", wChairID, cbCardDataArray[i]);
		}
	}
#else
	vecListenCards.clear();
	_uint8 cbCardDataArray[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A };
	_uint8 cbSize = sizeof(cbCardDataArray) / sizeof(cbCardDataArray[0]);
	//牌库牌
	for (_uint8 i = 0; i < cbSize; ++i)
	{
		tagHuCardInfo stHuInfo;
		if (m_GameLogic.GetHuCardInfo(m_cbCardIndex[wChairID], cbCardDataArray[i], m_bWeaveHuXiCount[wChairID], stHuInfo, m_wBaseHuXi, true, m_bWithMaoHu) == true)
		{
			//if (IsListenOver(wChairID, cbCardDataArray[i]) == false)
			{
				vecListenCards.push_back(cbCardDataArray[i]);
			}
		}
	}
	//去除偎牌
	_uint8 cbCardIndexTemp1[MAX_INDEX];
	memcpy(cbCardIndexTemp1, m_cbCardIndex[wChairID], sizeof(cbCardIndexTemp1));
	_uint8 cbWeaveCountTemp1 = m_bWeaveItemCount[wChairID];
	tagWeaveItem WeaveItemArrayTemp1[MAX_WEAVE];
	memcpy(WeaveItemArrayTemp1, m_WeaveItemArray[wChairID], sizeof(WeaveItemArrayTemp1));
	for (_uint8 i = 0; i < MAX_INDEX; ++i)
	{
		if (cbCardIndexTemp1[i] == 3)
		{
			cbCardIndexTemp1[i] = 0;
			_uint8 cbCardData = m_GameLogic.SwitchToCardData(i);
			WeaveItemArrayTemp1[cbWeaveCountTemp1].cbCardCount = 3;
			WeaveItemArrayTemp1[cbWeaveCountTemp1].cbWeaveKind = ACK_WEI;
			WeaveItemArrayTemp1[cbWeaveCountTemp1].cbCenterCard = cbCardData;
			WeaveItemArrayTemp1[cbWeaveCountTemp1].cbCardList[0] = cbCardData;
			WeaveItemArrayTemp1[cbWeaveCountTemp1].cbCardList[1] = cbCardData;
			WeaveItemArrayTemp1[cbWeaveCountTemp1++].cbCardList[2] = cbCardData;
		}
	}
	_uint8 cbWeaveHuXiCountTemp1 = 0;
	for (_uint8 j = 0; j < cbWeaveCountTemp1; ++j)
	{
		cbWeaveHuXiCountTemp1 += m_GameLogic.GetWeaveHuXi(WeaveItemArrayTemp1[j]);
	}
	for (_uint8 i = 0; i < cbSize; ++i)
	{
		tagHuCardInfo stHuInfo;
		if (m_GameLogic.GetHuCardInfo(cbCardIndexTemp1, cbCardDataArray[i], cbWeaveHuXiCountTemp1, stHuInfo, m_wBaseHuXi, true, m_bWithMaoHu) == true)
		{
			if (std::find(vecListenCards.begin(), vecListenCards.end(), cbCardDataArray[i]) == vecListenCards.end()
				/*&& (IsListenOver(wChairID, cbCardDataArray[i]) == false)*/)
			{
				vecListenCards.push_back(cbCardDataArray[i]);
			}
		}
	}

	//碰偎牌
	tagHuCardInfo stHuInfo;
	if (m_GameLogic.GetHuCardInfo(m_cbCardIndex[wChairID], INVALID_CARD, m_bWeaveHuXiCount[wChairID], stHuInfo, m_wBaseHuXi, true, m_bMingXiao) == true)
	{
		for (_uint8 i = 0; i < m_bWeaveItemCount[wChairID]; ++i)
		{
			if ((m_WeaveItemArray[wChairID][i].cbWeaveKind == ACK_PENG || m_WeaveItemArray[wChairID][i].cbWeaveKind == ACK_WEI)
				&& std::find(vecListenCards.begin(), vecListenCards.end(), m_WeaveItemArray[wChairID][i].cbCenterCard) == vecListenCards.end()
				/*&& (IsListenOver(wChairID, m_WeaveItemArray[wChairID][i].cbCenterCard) == false)*/)
			{
				vecListenCards.push_back(m_WeaveItemArray[wChairID][i].cbCenterCard);
			}
		}
		for (_uint8 i = 0; i < stHuInfo.cbWeaveCount; ++i)
		{
			if (stHuInfo.WeaveItemArray[i].cbWeaveKind == ACK_WEI
				&& std::find(vecListenCards.begin(), vecListenCards.end(), stHuInfo.WeaveItemArray[i].cbCenterCard) == vecListenCards.end()
				/*&& (IsListenOver(wChairID, stHuInfo.WeaveItemArray[i].cbCenterCard) == false)*/)
			{
				vecListenCards.push_back(stHuInfo.WeaveItemArray[i].cbCenterCard);
			}
		}
	}
	//偎变提,碰变跑
	_uint8 cbWeaveCountTemp = m_bWeaveItemCount[wChairID];
	for (_uint8 i = 0; i < cbWeaveCountTemp; ++i)
	{
		_uint8 cbWeaveHuXiCountTemp = 0;
		tagWeaveItem WeaveItemArrayTemp[MAX_WEAVE];
		memcpy(WeaveItemArrayTemp, m_WeaveItemArray[wChairID], sizeof(WeaveItemArrayTemp));
		if (WeaveItemArrayTemp[i].cbWeaveKind == ACK_WEI || WeaveItemArrayTemp[i].cbWeaveKind == ACK_PENG)
		{
			WeaveItemArrayTemp[i].cbCardCount = 4;
			WeaveItemArrayTemp[i].cbWeaveKind = (WeaveItemArrayTemp[i].cbWeaveKind == ACK_WEI) ? ACK_TI : ACK_PAO;
			WeaveItemArrayTemp[i].cbCardList[3] = WeaveItemArrayTemp[i].cbCenterCard;
			//更新胡息
			for (_uint8 j = 0; j < cbWeaveCountTemp; ++j)
			{
				cbWeaveHuXiCountTemp += m_GameLogic.GetWeaveHuXi(WeaveItemArrayTemp[j]);
			}
			tagHuCardInfo stHuInfo;
			if (m_GameLogic.GetHuCardInfo(m_cbCardIndex[wChairID], INVALID_CARD, cbWeaveHuXiCountTemp, stHuInfo, m_wBaseHuXi, true, m_bWithMaoHu) == true
				&& std::find(vecListenCards.begin(), vecListenCards.end(), WeaveItemArrayTemp[i].cbCenterCard) == vecListenCards.end()
				/*&& (IsListenOver(wChairID, WeaveItemArrayTemp[i].cbCenterCard) == false)*/)
			{
				vecListenCards.push_back(WeaveItemArrayTemp[i].cbCenterCard);
			}
		}
	}
	//坎变提
	for (_uint8 i = 0; i < MAX_INDEX; ++i)
	{
		_uint8 cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp, m_cbCardIndex[wChairID], sizeof(cbCardIndexTemp));
		_uint8 cbWeaveHuXiCountTemp = 0;
		_uint8 cbWeaveCountTemp = m_bWeaveItemCount[wChairID];
		tagWeaveItem WeaveItemArrayTemp[MAX_WEAVE];
		memcpy(WeaveItemArrayTemp, m_WeaveItemArray[wChairID], sizeof(WeaveItemArrayTemp));
		if (cbCardIndexTemp[i] == 3)
		{
			cbCardIndexTemp[i] = 0;
			_uint8 cbCardData = m_GameLogic.SwitchToCardData(i);
			WeaveItemArrayTemp[cbWeaveCountTemp].cbCardCount = 4;
			WeaveItemArrayTemp[cbWeaveCountTemp].cbWeaveKind = ACK_TI;
			WeaveItemArrayTemp[cbWeaveCountTemp].cbCenterCard = cbCardData;
			WeaveItemArrayTemp[cbWeaveCountTemp].cbCardList[0] = cbCardData;
			WeaveItemArrayTemp[cbWeaveCountTemp].cbCardList[1] = cbCardData;
			WeaveItemArrayTemp[cbWeaveCountTemp].cbCardList[2] = cbCardData;
			WeaveItemArrayTemp[cbWeaveCountTemp++].cbCardList[3] = cbCardData;
			//更新胡息
			for (_uint8 j = 0; j < cbWeaveCountTemp; ++j)
			{
				cbWeaveHuXiCountTemp += m_GameLogic.GetWeaveHuXi(WeaveItemArrayTemp[j]);
			}
			tagHuCardInfo stHuInfo;
			if (m_GameLogic.GetHuCardInfo(cbCardIndexTemp, INVALID_CARD, cbWeaveHuXiCountTemp, stHuInfo, m_wBaseHuXi, true, m_bWithMaoHu) == true
				&& std::find(vecListenCards.begin(), vecListenCards.end(), cbCardData) == vecListenCards.end()
				/*&& (IsListenOver(wChairID, cbCardData) == false)*/)
			{
				vecListenCards.push_back(cbCardData);
			}
		}
	}

#endif
	time_t end = time(NULL);
	int total = end - begin;
	if (total >= 1)
	{
		log.error("slow cmd:[%s] second[%d]\n",__FUNCTION__, total);
	}
	return vecListenCards.size() > 0;
}

bool Table::IsAllowChiPeng(_tint32 wChairID)
{
	_uint8 cbCardCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; ++i) cbCardCount += m_cbCardIndex[wChairID][i];
	return cbCardCount > 2;
}

void Table::handler_offline(Player *player)
{
	proto::game::AckOffline stOffline;
	stOffline.set_uid(player->uid);
	stOffline.set_seatid(player->seatid);
	Ppacket ppack;
	stOffline.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PLAYER_OFFLINE_BC);
	broadcast(NULL, ppack.data);
}

void Table::OnFinalEndReq(Player* player)
{
	proto::game::AckFinalEnd stFEnd;
	stFEnd.set_roomid(tid);
	stFEnd.set_room_user_id(m_wRoomUserId);
	stFEnd.set_ts((int)time(NULL));
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		std::map<int, Player*>::iterator iter = players.find(seats[i].uid);
		if (iter != players.end() && iter->second)
		{
			proto::game::tagPlayers* pstPlayer = stFEnd.add_players();
			pstPlayer->set_seatid(i);
			pstPlayer->set_uid(iter->second->uid);
			pstPlayer->set_name(iter->second->name);
			pstPlayer->set_avatar(iter->second->avatar);
			pstPlayer->set_sex(iter->second->sex);
			pstPlayer->set_hu_count(m_cbHuPaiCount[i]);
			pstPlayer->set_best_huxi(m_wBestHuXi[i]);
			pstPlayer->set_dian_pao_count(m_cbDianPaoCount[i]);
			pstPlayer->set_best_score(m_wBestScore[i]);
			pstPlayer->set_total_score(m_wTotalScore[i]);
			pstPlayer->set_all_piao_score(m_UserAllPiaoScore[i]);
			pstPlayer->set_zimo_count(m_UserZiMoCnt[i]);
		}
	}
	Ppacket ppack;
	stFEnd.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_FINAL_END_RES);
	unicast(player, ppack.data);
}

void Table::save_video_data(_tint32 cmd, std::string str)
{
	ostringstream oss;
	oss << cmd << ":S:" << str << ":E:";
	m_videostr += oss.str();
}

void Table::send_video_to_svr()
{
	//zlib压缩
	/*static unsigned char zbuf[MaxVideoBufLen] ={0};
	  uLong nzlen = MaxVideoBufLen;
	  int iRet = compress((Bytef*)zbuf, (uLongf *)&nzlen, (const Bytef*)m_videostr.c_str(), (uLong)m_videostr.size());
	  if (iRet != Z_OK)
	  {
	  log.error("send video to svr compress error iRet:%d .\n", iRet);
	  return;
	  }
	//base64编码
	static unsigned char buf[MaxVideoBufLen] ={0};
	int noutlen = MaxVideoBufLen;
	iRet = base64_encode((const unsigned char *)zbuf, nzlen, (unsigned char *)buf, &noutlen);
	if (iRet != CRYPT_OK)
	{
	log.error("send video to svr base64_encode is error iRet:%d .\n", iRet);
	return;
	}*/
	//base64编码
	static unsigned char buf[MaxVideoBufLen] = { 0 };
	int noutlen = MaxVideoBufLen;
	int iRet = base64_encode((const unsigned char *)m_videostr.c_str(), m_videostr.size(), (unsigned char *)buf, &noutlen);
	if (iRet != CRYPT_OK)
	{
		log.error("send video to svr base64_encode is error iRet:%d .\n", iRet);
		return;
	}

	Jpacket packet_data;
	packet_data.val["cmd"] = SERVER_VIDEO_DATA_SAVE;
	packet_data.val["roomid"] = tid;
	std::string packStr((char *)buf, noutlen);
	packet_data.val["content"] = packStr;
	packet_data.val["innings"] = m_strInnings;
	packet_data.end();
	zjh.game->send_to_videosvr(packet_data.tostring());
	log.info("send video to svr m_videostr.size[%d] noutlen[%d] tid[%s].\n", m_videostr.size(), noutlen, tid.c_str());
}

void Table::Send_Video_Data_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	log.info("Send_Video_Data_Timer_cb roomID:%s \n", table->tid.c_str());
	ev_timer_stop(zjh.loop, &table->send_videodata_timer);
	table->m_cbCurTimer = 0;
	table->send_video_to_svr();
}

void Table::save_table_info_video()
{
	proto::game::TableInfoVideo stTableInfo;
	stTableInfo.set_banker(m_wBankerUser);
	stTableInfo.set_take_in(m_wTakeIn);
	stTableInfo.set_play_id(m_wPlayID);
	stTableInfo.set_base_huxi(m_wBaseHuXi);
	stTableInfo.set_room_user_id(m_wRoomUserId);
	stTableInfo.set_game_player(m_GamePlayer);
	stTableInfo.set_game_name("chenzhouphz");

	stTableInfo.set_is_with_maohu(m_bWithMaoHu);
	stTableInfo.set_is_must_hu(m_cbMustHu);
	stTableInfo.set_is_zimo_fan(m_bZiMoFan);
	stTableInfo.set_is_hhd(m_bHHD);
	stTableInfo.set_huxi_to_tun(m_wHuXiToTun);
	stTableInfo.set_current_num(m_wCurrentNum);
	stTableInfo.set_all_num(m_wAllNum);
	
	for (int c = 0; c < m_GamePlayer; ++c)
	{
		Player *p = seats[c].player;
		if (p == NULL) continue;
		proto::game::tagPlayers* pstTablePlayers = stTableInfo.add_players();
		pstTablePlayers->set_uid(p->uid);
		pstTablePlayers->set_seatid(p->seatid);
		pstTablePlayers->set_ready(seats[c].ready);
		pstTablePlayers->set_money(p->money);
		pstTablePlayers->set_name(p->name);
		pstTablePlayers->set_sex(p->sex);
		pstTablePlayers->set_avatar(p->avatar);
		pstTablePlayers->set_hu_xi(m_bWeaveShowHuXiCount[c]);
		if (m_bDaNiao)
		{
			pstTablePlayers->set_niao(m_UserNiaoCount[c]);
		}
		if (m_bPiao)
		{
			pstTablePlayers->set_piao(m_UserPiaoScore[c]);
		}
	}
	save_video_data(SERVER_TABLE_INFO_UC, stTableInfo.SerializeAsString());
}

void Table::save_game_start_video()
{
	proto::game::GameStartVideo stGameStart;
	stGameStart.set_banker(m_wBankerUser);
	stGameStart.set_banker_card(m_cbBankerCard);
	stGameStart.set_left_card_count(m_bLeftCardCount);
	for (_uint8 c = 0; c < m_GamePlayer; ++c)
	{
		proto::game::tagPlayers* pstPlayer = stGameStart.add_players();
		pstPlayer->set_seatid(c);
		pstPlayer->set_total_huxi(m_bHandsHuXiCount[c] + m_bWeaveHuXiCount[c]);
		for (_uint8 i = 0; i < m_cbCardsWeaveCount[c]; ++i)
		{
			proto::game::tagHandCards* pstHandCards = pstPlayer->add_hand_cards();
			for (_uint8 j = 0; j < m_CardsWeaveInfo[c][i].cbCardCount; ++j)
			{
				pstHandCards->add_card(m_CardsWeaveInfo[c][i].cbCardList[j]);
			}
		}
		std::vector<_uint8> vecListenCards;
		if (IsListen(c, vecListenCards))
		{
			for (_uint8 i = 0; i < vecListenCards.size(); ++i)
				pstPlayer->add_listen_cards(vecListenCards[i]);
		}
	}
	save_video_data(SERVER_GAME_START_BC, stGameStart.SerializeAsString());
}

void Table::save_operate_video(_tint32 wChairID, _uint8 cbAckCode, _uint8 cbActionCard)
{
	proto::game::OperateVideo stOperateCard;
	stOperateCard.set_ack_code(cbAckCode);
	stOperateCard.set_seatid(wChairID);
	stOperateCard.set_action_card(cbActionCard);
	std::vector<_uint8> vecListenCards;
	if (IsListen(wChairID, vecListenCards))
	{
		for (_uint8 i = 0; i < vecListenCards.size(); ++i)
			stOperateCard.add_listen_cards(vecListenCards[i]);
	}
	stOperateCard.set_total_huxi(m_bHandsHuXiCount[wChairID] + m_bWeaveHuXiCount[wChairID]);
	for (_uint8 i = 0; i < m_cbCardsWeaveCount[wChairID]; ++i)
	{
		proto::game::tagHandCards* pstHandCards = stOperateCard.add_hand_cards();
		for (_uint8 j = 0; j < m_CardsWeaveInfo[wChairID][i].cbCardCount; ++j)
		{
			pstHandCards->add_card(m_CardsWeaveInfo[wChairID][i].cbCardList[j]);
		}
	}
	for (int j = 0; j < m_bWeaveItemCount[wChairID]; ++j)
	{
		proto::game::tagWeaveCards* pstWeaves = stOperateCard.add_weaves();
		pstWeaves->set_weave_kind(m_WeaveItemArray[wChairID][j].cbWeaveKind);
		pstWeaves->set_card_count(m_WeaveItemArray[wChairID][j].cbCardCount);
		pstWeaves->set_center_card(m_WeaveItemArray[wChairID][j].cbCenterCard);
		for (int k = 0; k < m_WeaveItemArray[wChairID][j].cbCardCount; ++k)
		{
			pstWeaves->add_card_list(m_WeaveItemArray[wChairID][j].cbCardList[k]);
		}
	}
	save_video_data(SERVER_PHZ_OPERATE_CARD_RES, stOperateCard.SerializeAsString());
}

void Table::save_dispatch_card_video(_tint32 wChairID, _uint8 cbCardData, _tint32 wToSeatid)
{
	proto::game::AckDispatchCard stDispachCard;
	stDispachCard.set_seatid(wChairID);
	stDispachCard.set_card(cbCardData);
	stDispachCard.set_left_card_count(m_bLeftCardCount);
	if (wToSeatid != INVALID_CHAIR) stDispachCard.set_to_seatid(wToSeatid);
	save_video_data(SERVER_PHZ_SEND_CARD, stDispachCard.SerializeAsString());
}

void Table::save_operate_notify_video()
{
	proto::game::OperateNotify stOpNotify;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		proto::game::tagPlayers* pstPlayers = stOpNotify.add_players();
		pstPlayers->set_seatid(i);
		pstPlayers->set_operate_type(m_bUserAction[i]);
	}
	save_video_data(SERVER_PHZ_OPERATE_NOTIFY, stOpNotify.SerializeAsString());
}

void Table::save_operate_res_video(_tint32 wChairID, _uint8 cbOpCode)
{
	proto::game::AckOperateRes stOpRes;
	stOpRes.set_seatid(wChairID);
	stOpRes.set_operate_type(cbOpCode);
	save_video_data(SERVER_PHZ_OPERATE_RES, stOpRes.SerializeAsString());
}

void Table::save_out_card_video(_tint32 wChairID, _uint8 cbCardData)
{
	proto::game::AckOutCard stOutCard;
	stOutCard.set_seatid(wChairID);
	stOutCard.set_out_card(cbCardData);
	if (m_bCommonOrBest[wChairID] == false) {
		for (_uint8 i = 0; i < m_cbCardsWeaveCount[wChairID]; ++i) {
			proto::game::tagHandCards* pstHandCards = stOutCard.add_hand_cards();
			for (_uint8 j = 0; j < m_CardsWeaveInfo[wChairID][i].cbCardCount; ++j) {
				pstHandCards->add_card(m_CardsWeaveInfo[wChairID][i].cbCardList[j]);
			}
		}
	}
	else {
		for (_uint8 i = 0; i < m_cbBestWeaveCount[wChairID]; ++i) {
			proto::game::tagHandCards* pstHandCards = stOutCard.add_hand_cards();
			for (_uint8 j = 0; j < m_BestWeaveInfo[wChairID][i].cbCardCount; ++j) {
				pstHandCards->add_card(m_BestWeaveInfo[wChairID][i].cbCardList[j]);
			}
		}
	}
	std::vector<_uint8> vecListenCards;
	if (IsListen(wChairID, vecListenCards))
	{
		for (_uint8 i = 0; i < vecListenCards.size(); ++i)
			stOutCard.add_listen_cards(vecListenCards[i]);
	}
	_uint8 cbTotalHuXi = m_bHandsHuXiCount[wChairID] + m_bWeaveHuXiCount[wChairID];
	stOutCard.set_total_huxi(cbTotalHuXi);
	save_video_data(SERVER_PHZ_OUT_CARD_RES, stOutCard.SerializeAsString());
}

void Table::save_abandon_card_video(_tint32 wChairID)
{
	proto::game::AckUpdateAbandon stAbandon;
	stAbandon.set_seatid(wChairID);
	for (_uint8 i = 0; i < m_UserAbandonCount[wChairID]; ++i)
	{
		stAbandon.add_abandon_cards(m_UserAbandonCards[wChairID][i]);
	}
	save_video_data(SERVER_PHZ_UPDATE_ABANDON_CARD, stAbandon.SerializeAsString());
}


void Table::save_game_end_video(_tint32 cmd, std::string str)
{
	save_video_data(cmd, str);
	ostringstream oss;
	oss << cmd << ":S:" << str << ":E:";
	m_strLastEndInfo = oss.str();
}
bool Table::IsPassCard(_tint32 wChairID, _uint8 cbCardData)
{
	if (wChairID == INVALID_CHAIR || cbCardData == INVALID_CARD)
	{
		log.error("Is pass card error wChairID[%d] cbCardData[%d] .\n", wChairID, cbCardData);
		return false;
	}

	_tint32 wTempChairID = wChairID - 1;
	if (wTempChairID < 0) wTempChairID = m_GamePlayer - 1;

	for (_uint8 i = 0; i < m_UserAbandonCount[wTempChairID]; ++i)
	{
		if (m_UserAbandonCards[wTempChairID][i] == cbCardData)
		{
			log.info("Is pass card error wChairID[%d] cbCardData[%d] .\n", wChairID, cbCardData);
			return true;
		}
	}

	for (_uint8 i = 0; i < m_UserAbandonCount[wChairID]; ++i)
	{
		if (m_UserAbandonCards[wChairID][i] == cbCardData)
		{
			log.info("Is pass card error wChairID[%d] cbCardData[%d] .\n", wChairID, cbCardData);
			return true;
		}
	}

	return false;
}

//臭牌处理
bool Table::HandlerChouCard()
{
	//处理操作优先级
	_tint32 cbSortChairID = m_GamePlayer;
	_uint32	wSortCodes[GAME_PLAYER] = { 0 };
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		_tint32 wTempChairID = (m_wCurrentUser + i) % m_GamePlayer;
		if (m_bUserAction[wTempChairID] == ACK_NULL) continue;
		_uint8 cbCode = 0;
		if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_CHIHU) cbCode = ACK_CHIHU;
		else if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_PENG) cbCode = ACK_PENG;
		else if (cbCode == 0 && m_bUserAction[wTempChairID] & ACK_CHI) cbCode = ACK_CHI;
		wSortCodes[wTempChairID] = (cbCode * 10 + cbSortChairID) * 10 + wTempChairID;
		cbSortChairID--;
	}
	//排序
	for (_uint8 m = 0; m < m_GamePlayer - 1; ++m)
	{
		for (_uint8 n = m + 1; n < m_GamePlayer; ++n)
		{
			if (wSortCodes[m] < wSortCodes[n])
			{
				_uint32 cbTemp = wSortCodes[m];
				wSortCodes[m] = wSortCodes[n];
				wSortCodes[n] = cbTemp;
			}
		}
	}
	//臭牌处理
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (wSortCodes[i] == 0) continue;
		_tint32 wTempChairID = wSortCodes[i] % 10;
		if (!m_bResponse[wTempChairID]) break;
		if (m_bResponse[wTempChairID] && m_bPerformAction[wTempChairID] != ACK_NULL) break;
		if (m_bResponse[wTempChairID] && m_bPerformAction[wTempChairID] == ACK_NULL)
		{
			if (m_bUserAction[wTempChairID] & ACK_PENG)//臭偎记录
			{
				m_bChouWeiCard[wTempChairID][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;
				m_bPengAbandon[wTempChairID][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;//臭牌记录
				log.info(" Do multi operate handler chou choupeng chairid[%d] card[0x%02x] tid[%s] .\n", wTempChairID, m_cbCurrentCard, tid.c_str());
			}
			if (m_bUserAction[wTempChairID] & ACK_CHI)//臭偎记录
			{
				m_bChiAbandon[wTempChairID][m_GameLogic.SwitchToCardIndex(m_cbCurrentCard)] = true;//臭牌记录
				log.info(" Do multi operate handler chou chouchi chairid[%d] card[0x%02x] tid[%s] .\n", wTempChairID, m_cbCurrentCard, tid.c_str());
			}
		}
	}

	return true;
}

bool Table::IsPassPengCard(_tint32 wChairID, _uint8 cbCardData)
{
	if (wChairID == INVALID_CHAIR || cbCardData == INVALID_CARD)
	{
		log.error("Is pass peng card error wChairID[%d] cbCardData[%d] .\n", wChairID, cbCardData);
		return false;
	}

	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		if (i == wChairID) continue;
		for (_uint8 j = 0; j < m_bWeaveItemCount[i]; ++j)
		{
			if (m_WeaveItemArray[i][j].cbWeaveKind == ACK_CHI && m_WeaveItemArray[i][j].cbCenterCard == cbCardData)
			{
				m_bChouWeiCard[wChairID][m_GameLogic.SwitchToCardIndex(cbCardData)] = true;
				log.info("Is pass peng card wChairID[%d] uid[%d] cbCardData[%d] tid[%s] .\n", wChairID, seats[wChairID].uid, cbCardData, tid.c_str());
				return true;
			}
		}
	}

	return false;
}

void Table::save_offline_info(_tint32 wChairID, _tint32 cmd, std::string str)
{
	if (wChairID < 0 || wChairID >= m_GamePlayer) return;
	ostringstream oss;
	oss << cmd << ":S:" << str << ":E:";
	if (m_cbOfflineCount[wChairID] < MaxOfflineLen)
	{
		m_strOfflineInfo[wChairID][m_cbOfflineCount[wChairID]++] = oss.str();
		log.info("save offline info wChairID[%d] m_cbOfflineCount[%d] tid[%s] .\n", wChairID, m_cbOfflineCount[wChairID], tid.c_str());
	}
}

void Table::OnOfflineInfoReq(Player* player)
{
	if (m_strLastEndInfo.size() == 0)
	{
		Ppacket ppack;
		ppack.pack(SERVER_PHZ_OFFLINE_INFO_ERR);
		unicast(player, ppack.data);
	}
	else
	{
		proto::game::AckLastEndInfo stLastInfo;
		stLastInfo.set_info(m_strLastEndInfo);
		Ppacket ppack;
		stLastInfo.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_OFFLINE_INFO_RES);
		unicast(player, ppack.data);
	}
}

bool Table::IsListenOver(_tint32 wChairID, _uint8 cbCard)
{
	_uint8 cbNowCount = 0;
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		for (_uint8 j = 0; j < m_bWeaveItemCount[i]; ++j)
		{
			if (m_bMingXiao == false)
			{
				if ((m_WeaveItemArray[i][j].cbWeaveKind == ACK_WEI || m_WeaveItemArray[i][j].cbWeaveKind == ACK_TI) && wChairID != i) continue;
			}
			for (_uint8 k = 0; k < m_WeaveItemArray[i][j].cbCardCount; ++k)
			{
				if (m_WeaveItemArray[i][j].cbCardList[k] == cbCard)
					cbNowCount++;
			}
		}
	}
	for (_uint8 i = 0; i < m_GamePlayer; ++i)
	{
		for (_uint8 j = 0; j < m_UserAbandonCount[i]; ++j)
		{
			if (m_UserAbandonCards[i][j] == cbCard)
				cbNowCount++;
		}
	}
	for (_uint8 i = 0; i < MAX_INDEX; ++i)
	{
		if (m_cbCardIndex[wChairID][i] == 0) continue;
		if (m_GameLogic.SwitchToCardData(i) == cbCard)
		{
			cbNowCount += m_cbCardIndex[wChairID][i];
		}
	}

	if (cbNowCount >= 4)
		return true;

	return false;
}

void Table::SendChouNotify(_tint32 wChairID, _uint8 cbType)
{
	if (seats[wChairID].player != NULL)
	{
		proto::game::AckChouNotity stChouNotify;
		stChouNotify.set_type(cbType);
		Ppacket ppack;
		stChouNotify.SerializeToString(&ppack.body);
		ppack.pack(SERVER_PHZ_CHOU_NOTIFY);
		unicast(seats[wChairID].player, ppack.data);
	}
}

void Table::UpdateWeaveOutCard(_tint32 wChairID, _uint8 cbCard)
{
	if (wChairID < 0 || wChairID >= m_GamePlayer) return;
	if (m_GameLogic.IsValidCard(cbCard) == false) return;

	if (m_bWeaveItemCount[wChairID] > 0 && m_bWeaveItemCount[wChairID] <= MAX_WEAVE)
	{
		if (m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID] - 1].cbOutCard == 0)
		{
			m_WeaveItemArray[wChairID][m_bWeaveItemCount[wChairID] - 1].cbOutCard = cbCard;
		}
	}
}

void Table::OnHandDetailInfo(Player* player)
{
	if (player->seatid < 0 || player->seatid >= m_GamePlayer)
	{
		log.error("On hand detail info error uid[%d] seatid[%d] tid[%s] .\n", player->uid, player->seatid, tid.c_str());
		return;
	}

	proto::game::AckHandDetail stDetail;
	for (_uint8 c = 0; c < m_GamePlayer; ++c)
	{
		if (c == player->seatid) continue;
		Player *p = seats[c].player;
		if (p == NULL) continue;
		proto::game::tagPlayers* pstDetailPlayer = stDetail.add_players();
		pstDetailPlayer->set_seatid(c);
		pstDetailPlayer->set_total_huxi(m_bWeaveShowHuXiCount[c]);
		_uint8 cbHongCount = 0;
		_uint8 cbHeiCount = 0;
		for (_uint8 j = 0; j < m_bWeaveItemCount[c]; ++j)
		{
			proto::game::tagWeaveCards* pstWeaves = pstDetailPlayer->add_weaves();
			pstWeaves->set_weave_kind(m_WeaveItemArray[c][j].cbWeaveKind);
			pstWeaves->set_card_count(m_WeaveItemArray[c][j].cbCardCount);
			pstWeaves->set_out_card(m_WeaveItemArray[c][j].cbOutCard);
			if (m_bMingXiao || (m_bMingXiao == false && m_WeaveItemArray[c][j].cbWeaveKind != ACK_TI && m_WeaveItemArray[c][j].cbWeaveKind != ACK_WEI))
			{
				pstWeaves->set_center_card(m_WeaveItemArray[c][j].cbCenterCard);
				for (_uint8 k = 0; k < m_WeaveItemArray[c][j].cbCardCount; ++k)
				{
					pstWeaves->add_card_list(m_WeaveItemArray[c][j].cbCardList[k]);
					_uint8 cbCard = m_WeaveItemArray[c][j].cbCardList[k];
					if (CardNum(cbCard) == 0x02 || CardNum(cbCard) == 0x07 || CardNum(cbCard) == 0x0a) cbHongCount++;
					else cbHeiCount++;
				}
			}
		}
		pstDetailPlayer->set_hong_count(cbHongCount);
		pstDetailPlayer->set_hei_count(cbHeiCount);
		_uint8 cbHandCardCount = 0;
		for (_uint8 m = 0; m < MAX_INDEX; ++m) cbHandCardCount += m_cbCardIndex[c][m];
		pstDetailPlayer->set_hand_count(cbHandCardCount);
		pstDetailPlayer->set_name(p->name);
		pstDetailPlayer->set_sex(p->sex);
		pstDetailPlayer->set_avatar(p->avatar);
		pstDetailPlayer->set_money(p->money);
	}
	Ppacket ppack;
	stDetail.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_HAND_DETAIL_RES);
	unicast(player, ppack.data);
	log.debug("uid[%d] tid[%s] stDetail[%s] .\n", player->uid, tid.c_str(), stDetail.DebugString().c_str());
}

bool Table::IsWeiTiCard(_tint32 wChairID, _uint8 cbCard)
{
	if (wChairID < 0 || wChairID >= m_GamePlayer || m_GameLogic.IsValidCard(cbCard) == false) return false;
	if (m_GameLogic.IsWeiPengCard(m_cbCardIndex[wChairID], cbCard) == true
		|| m_GameLogic.IsTiPaoCard(m_cbCardIndex[wChairID], cbCard) == true) return true;
	for (_uint8 i = 0; i < m_bWeaveItemCount[wChairID]; ++i)
	{
		if (m_WeaveItemArray[wChairID][i].cbWeaveKind == ACK_WEI
			&& m_WeaveItemArray[wChairID][i].cbCardList[0] == cbCard) return true;
	}
	return false;
}

void Table::game_restart_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents)
{
	Table *table = (Table*)w->data;
	ev_timer_stop(zjh.loop, &table->game_restart_timer);
	table->game_start_req();
}

/*
 *恢复服数据恢复
 */
int Table::decompress_str(string &src, string &des)
{
	//base64编码
	static unsigned char ubuf[MaxVideoBufLen] = { 0 };
	int unoutlen = MaxVideoBufLen;
	int iRet = base64_decode((const unsigned char *)src.c_str(), src.size(), (unsigned char *)ubuf, &unoutlen);
	if (iRet != CRYPT_OK)
	{
		log.error("base64_decode video svr is error iRet:%d .\n", iRet);
		return -1;
	}
	//zlib解压
	static unsigned char uzbuf[MaxVideoBufLen] = { 0 };
	uLong unzlen = MaxVideoBufLen;
	iRet = uncompress((Bytef*)uzbuf, (uLongf *)&unzlen, (const Bytef*)ubuf, (uLong)unoutlen);
	if (iRet != Z_OK)
	{
		log.error("uncompress video error iRet:%d .\n", iRet);
		return -1;
	}
	std::string packStr((char *)uzbuf, unzlen);
	des.clear();
	des = packStr;
	return 0;
}

int Table::compress_str(string &src, string &des)
{
	//zlib压缩
	static unsigned char zbuf[MaxVideoBufLen] = { 0 };
	uLong nzlen = MaxVideoBufLen;
	int iRet = compress((Bytef*)zbuf, (uLongf *)&nzlen, (const Bytef*)src.c_str(), (uLong)src.size());
	if (iRet != Z_OK)
	{
		log.error("send video to svr compress error iRet:%d .\n", iRet);
		return -1;
	}
	//base64编码
	static unsigned char buf[MaxVideoBufLen] = { 0 };
	int noutlen = MaxVideoBufLen;
	iRet = base64_encode((const unsigned char *)zbuf, nzlen, (unsigned char *)buf, &noutlen);
	if (iRet != CRYPT_OK)
	{
		log.error("send video to svr base64_encode is error iRet:%d .\n", iRet);
		return -1;
	}
	std::string packStr((char *)buf, noutlen);
	des.clear();
	des = packStr;
	return 0;
}

//重启定时器
int Table::AgainTimer()
{
	switch (m_cbCurTimer)
	{
	case 1:
		ev_timer_again(zjh.loop, &start_timer);
		break;
	case 2:
		ev_timer_again(zjh.loop, &preready_timer);
		break;
	case 3:
		ev_timer_again(zjh.loop, &dispatch_show_timer);
		break;
	case 4:
		ev_timer_again(zjh.loop, &out_show_timer);
		break;
	case 5:
		ev_timer_again(zjh.loop, &dispatch_delay_timer);
		break;
	case 6:
		ev_timer_again(zjh.loop, &out_delay_timer);
		break;
	case 7:
		ev_timer_again(zjh.loop, &dissolve_room_timer);
		break;
	case 8:
		ev_timer_again(zjh.loop, &send_videodata_timer);
		break;
	default:
		break;
	}
	return 0;
}

int Table::SerializeTableData()
{
	time_t begin = time(NULL);
#if 1
	proto::game::AckTableInfo pTableInfo;
	pTableInfo.set_tid(tid);
	pTableInfo.set_base_money(base_money);
	//座位信息
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		Player* player = seats[i].player;
		if (player == NULL) continue;
		proto::game::tagPlayers* pstPlayer = pTableInfo.add_players();
		pstPlayer->set_uid(seats[i].uid);
		pstPlayer->set_ready(seats[i].ready);
		pstPlayer->set_tid(player->tid);
		pstPlayer->set_seatid(player->seatid);
		pstPlayer->set_offline(player->is_offline);
		pstPlayer->set_skey(player->skey);
		pstPlayer->set_name(player->name);
		pstPlayer->set_sex(player->sex);
		pstPlayer->set_avatar(player->avatar);
		pstPlayer->set_money(player->money);
		pstPlayer->set_dissolve_cnt(player->dissolve_cnt);
		pstPlayer->set_dissovle_state(player->dissovle_state);
	}
	pTableInfo.set_state(state);
	pTableInfo.set_stand_money(stand_money);
	pTableInfo.set_take_in(m_wTakeIn);
	pTableInfo.set_play_id(m_wTakeIn);
	pTableInfo.set_room_status(m_cbRoomStatus);
	pTableInfo.set_room_user_id(m_wRoomUserId);
	pTableInfo.set_dissovle_state(dissovle_state);
	pTableInfo.set_dissolve_applyer(dissolve_applyer);
	pTableInfo.set_is_dissolved(is_dissolved);

	pTableInfo.set_is_with_maohu(m_bWithMaoHu);
	pTableInfo.set_is_must_hu(m_cbMustHu);
	pTableInfo.set_is_zimo_fan(m_bZiMoFan);
	pTableInfo.set_is_hhd(m_bHHD);
	pTableInfo.set_tian_hu_3ti5kan(m_bTianHu3Ti5Kan);
	pTableInfo.set_is_tian_hu_3ti5kan(m_bIsTianHu3Ti5Kan);

	for (_uint8 i = 0; i < MAX_CARDS; ++i)
	{
		pTableInfo.add_repertory_cards(m_cbRepertoryCard[i]);
	}
	pTableInfo.set_left_card_count(m_bLeftCardCount);
	for (_uint8 i = 0; i < pTableInfo.players_size() && i < GAME_PLAYER; ++i)
	{
		proto::game::tagPlayers* stPlayer = pTableInfo.mutable_players(i);
		
		stPlayer->set_all_piao_score(m_UserAllPiaoScore[i]);
		stPlayer->set_zimo_count(m_UserZiMoCnt[i]);

		for (_uint8 j = 0; j < MAX_INDEX; ++j)
		{
			stPlayer->add_card_index(m_cbCardIndex[i][j]);
		}
		for (_uint8 j = 0; j < m_cbCardsWeaveCount[i]; ++j)
		{
			proto::game::tagHandCards* stCards = stPlayer->add_cards_weave();
			for (_uint8 k = 0; k < m_CardsWeaveInfo[i][j].cbCardCount; ++k)
			{
				stCards->add_card(m_CardsWeaveInfo[i][j].cbCardList[k]);
			}
		}
		for (_uint8 j = 0; j < m_cbBestWeaveCount[i]; ++j)
		{
			proto::game::tagHandCards* stCards = stPlayer->add_best_weave();
			for (_uint8 k = 0; k < m_BestWeaveInfo[i][j].cbCardCount; ++k)
			{
				stCards->add_card(m_BestWeaveInfo[i][j].cbCardList[k]);
			}
		}
		for (_uint8 j = 0; j < m_bWeaveItemCount[i]; ++j)
		{
			proto::game::tagWeaveCards* stWeave = stPlayer->add_weaves();
			stWeave->set_weave_kind(m_WeaveItemArray[i][j].cbWeaveKind);
			stWeave->set_center_card(m_WeaveItemArray[i][j].cbCenterCard);
			stWeave->set_out_card(m_WeaveItemArray[i][j].cbOutCard);
			for (_uint8 k = 0; k < m_WeaveItemArray[i][j].cbCardCount; ++k)
			{
				stWeave->add_card_list(m_WeaveItemArray[i][j].cbCardList[k]);
			}
		}
		for (_uint8 j = 0; j < m_cbUserChiCardCount[i]; ++j)
		{
			proto::game::tagChiCardInfo* stChiCardInfo = stPlayer->add_chicard_info();
			stChiCardInfo->set_chi_kind(m_UserChiCardInfo[i][j].cbChiKind);
			stChiCardInfo->set_center_card(m_UserChiCardInfo[i][j].cbCenterCard);
			for (_uint8 k = 0; k < 3; ++k)
			{
				stChiCardInfo->add_cards(m_UserChiCardInfo[i][j].cbCardData[k]);
			}
			for (_uint8 m = 0; m < m_UserChiCardInfo[i][j].cbSecondCount; ++m)
			{
				proto::game::tagSecondInfo *stSecondInfo = stChiCardInfo->add_second_info();
				stSecondInfo->set_chi_kind(m_UserChiCardInfo[i][j].SecondData[m].cbChiKind);
				stSecondInfo->set_center_card(m_UserChiCardInfo[i][j].SecondData[m].cbCenterCard);
				for (_uint8 k = 0; k < 3; ++k)
				{
					stSecondInfo->add_cards(m_UserChiCardInfo[i][j].SecondData[m].cbCardData[k]);
				}
				for (_uint8 n = 0; n < m_UserChiCardInfo[i][j].SecondData[m].cbThirdCount; ++n)
				{
					proto::game::tagThirdInfo *stThirdInfo = stSecondInfo->add_third_info();
					stThirdInfo->set_chi_kind(m_UserChiCardInfo[i][j].SecondData[m].ThirdData[n].cbChiKind);
					stThirdInfo->set_center_card(m_UserChiCardInfo[i][j].SecondData[m].ThirdData[n].cbCenterCard);
					for (_uint8 k = 0; k < 3; ++k)
					{
						stThirdInfo->add_cards(m_UserChiCardInfo[i][j].SecondData[m].ThirdData[n].cbCardData[k]);
					}
				}
			}
		}
		for (_uint8 j = 0; j < m_UserAbandonCount[i]; ++j)
		{
			stPlayer->add_abandon_cards(m_UserAbandonCards[i][j]);
		}
		for (_uint8 j = 0; j < m_UserAbandonCount[i]; ++j)
		{
			stPlayer->add_abandon_type(m_UserAbandonType[i][j]);
		}
		for (_uint8 j = 0; j < MAX_INDEX; ++j)
		{
			stPlayer->add_peng_chou(m_bPengAbandon[i][j]);
		}
		for (_uint8 j = 0; j < MAX_INDEX; ++j)
		{
			stPlayer->add_chi_chou(m_bChiAbandon[i][j]);
		}
		for (_uint8 j = 0; j < MAX_INDEX; ++j)
		{
			stPlayer->add_chou_wei(m_bChouWeiCard[i][j]);
		}
		stPlayer->set_niao(m_UserNiaoCount[i]);
		stPlayer->set_piao(m_UserPiaoScore[i]);
		stPlayer->set_hu_count(m_cbHuPaiCount[i]);
		stPlayer->set_dian_pao_count(m_cbDianPaoCount[i]);
		stPlayer->set_best_huxi(m_wBestHuXi[i]);
		stPlayer->set_best_score(m_wBestScore[i]);
		stPlayer->set_total_score(m_wTotalScore[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_weave_huxi(m_bWeaveHuXiCount[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_show_huxi(m_bWeaveShowHuXiCount[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_hands_huxi(m_bHandsHuXiCount[i]);
	}
	//操作
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_user_responses(m_bResponse[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_user_actions(m_bUserAction[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_user_prerform(m_bPerformAction[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_chi_kind(m_cbUserChiKind[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_second_chi_kind(m_cbUserSecondChiKind[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_third_chi_kind(m_cbUserThirdChiKind[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_xiang_gong(m_bXiangGong[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_user_dong_zhang(m_bDongZhang[i]);
	}
	for (_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		pTableInfo.add_common_best(m_bCommonOrBest[i]);
	}
	pTableInfo.set_dispatch(m_bDispatch);
	pTableInfo.set_dispatch_hu(m_bDispatchHu);
	pTableInfo.set_tiweipao_hu(m_bTiWeiPaoHu);
	pTableInfo.set_wei_hu(m_bWeiHu);
	pTableInfo.set_ti_card(m_bTiCard);
	pTableInfo.set_is_weiti_card(m_bIsWeiTiCard);
	pTableInfo.set_pao_card(m_bPaoCard);
	pTableInfo.set_first_card(m_bFirstCard);
	pTableInfo.set_card_operated(m_bCardOperated);
	pTableInfo.set_after_weave(m_bAfterWeave);
	pTableInfo.set_banker_user(m_wBankerUser);
	pTableInfo.set_banker_card(m_cbBankerCard);
	pTableInfo.set_current_user(m_wCurrentUser);
	pTableInfo.set_current_card(m_cbCurrentCard);
	pTableInfo.set_current_outcard_user(m_wOutCardUser);
	pTableInfo.set_current_outcard(m_cbOutCardData);
	pTableInfo.set_game_player(m_GamePlayer);
	pTableInfo.set_base_huxi(m_wBaseHuXi);
	pTableInfo.set_huxi_to_tun(m_wHuXiToTun);
	pTableInfo.set_is_da_niao(m_bDaNiao);
	pTableInfo.set_is_out_card(m_bOutCard);
	pTableInfo.set_tp_15_zhang(m_b3Ren15Zhang);
	for (_uint8 i = 0; i < m_vecNiaoCount.size(); ++i)
	{
		pTableInfo.add_niao_count(m_vecNiaoCount[i]);
	}
	pTableInfo.set_is_piao(m_bPiao);
	for (_uint8 i = 0; i < m_vecPiaoScore.size(); ++i)
	{
		pTableInfo.add_piao_count(m_vecPiaoScore[i]);
	}
	pTableInfo.set_is_ming_xiao(m_bMingXiao);
	pTableInfo.set_current_num(m_wCurrentNum);
	pTableInfo.set_all_num(m_wAllNum);
	pTableInfo.set_innings(m_strInnings);
	pTableInfo.set_video_str(m_videostr);
	pTableInfo.set_cur_timer(m_cbCurTimer);
	for(std::vector<tagGain>::iterator iter = m_vecGains.begin(); iter != m_vecGains.end(); ++iter)
	{
		proto::game::tagGain* pstGain = pTableInfo.add_gains();
		for(_uint8 i = 0; i < GAME_PLAYER; ++i)
		{
			pstGain->add_score((*iter).Gain[i]);
		}
	}
	//压缩保存
	std::string data_str;
	data_str.clear();
	pTableInfo.SerializeToString(&data_str);
	//解压
	_tint32 iRet = compress_str(data_str, data_str);
	if (iRet < 0)
	{
		log.error("Serialize Table Data compress fail iRet[%d].\n", iRet);
		return -1;
	}
	zjh.game->save_data_to_redis(tid, data_str);
	//log.info("Serialize Table Data succ tid[%s] compress size[%d].\n", tid.c_str(), data_str.size());
#endif
	time_t end = time(NULL);
	int total = end - begin;
	if (total >= 1)
	{
		log.error("slow cmd:[%s] second[%d]\n",__FUNCTION__, total);
	}

	return 0;
}

int Table::ParseTableData(std::string &data_str)
{
#if 1
	//解压
	_tint32 iRet = 0;
	iRet = decompress_str(data_str, data_str);
	if (iRet < 0)
	{
		log.error("Parse Table Data decompress fail iRet[%d] data size[%d].\n", iRet, data_str.size());
		return -1;
	}
	proto::game::AckTableInfo pTableInfo;
	pTableInfo.ParseFromString(data_str);
	//桌上信息
	tid = pTableInfo.tid();
	base_money = pTableInfo.base_money();
	//玩家座位信息
	for (_uint8 i = 0; i < pTableInfo.players_size() && i < GAME_PLAYER; ++i)
	{
		const proto::game::tagPlayers& stPlayer = pTableInfo.players(i);
		seats[i].uid = stPlayer.uid();
		seats[i].occupied = 1;
		seats[i].ready = stPlayer.ready();
		Player *player = NULL;
		if (players.find(stPlayer.uid()) == players.end())
		{
			player = new (nothrow) Player();
		}
		else
		{
			player = players[stPlayer.uid()];
		}
		player->InitData(stPlayer);
		seats[i].player = player;
		players[player->uid] = player;
	}
	state = (State)pTableInfo.state();
	stand_money = pTableInfo.stand_money();
	m_wTakeIn = pTableInfo.take_in();
	m_wPlayID = pTableInfo.play_id();
	m_cbRoomStatus = pTableInfo.room_status();
	m_wRoomUserId = pTableInfo.room_user_id();
	dissovle_state = pTableInfo.dissovle_state();
	dissolve_applyer = pTableInfo.dissolve_applyer();
	is_dissolved = pTableInfo.is_dissolved();

	m_bWithMaoHu = pTableInfo.is_with_maohu();
	m_bZiMoFan = pTableInfo.is_zimo_fan();
	m_cbMustHu = pTableInfo.is_must_hu();
	m_bHHD = pTableInfo.is_hhd();

	m_bTianHu3Ti5Kan = pTableInfo.tian_hu_3ti5kan();
	m_bIsTianHu3Ti5Kan = pTableInfo.is_tian_hu_3ti5kan();
	m_b3Ren15Zhang = pTableInfo.tp_15_zhang();

	for (_uint8 i = 0; i < pTableInfo.repertory_cards_size() && i < MAX_CARDS; ++i)
	{
		m_cbRepertoryCard[i] = pTableInfo.repertory_cards(i);
	}
	m_bLeftCardCount = pTableInfo.left_card_count();
	for (_uint8 i = 0; i < pTableInfo.players_size() && i < GAME_PLAYER; ++i)
	{
		const proto::game::tagPlayers& stPlayer = pTableInfo.players(i);

		m_UserAllPiaoScore[i] = stPlayer.all_piao_score();
		m_UserZiMoCnt[i] = stPlayer.zimo_count();
		
		for (_uint8 j = 0; j < stPlayer.card_index_size() && j < MAX_INDEX; ++j)
		{
			m_cbCardIndex[i][j] = stPlayer.card_index(j);
		}
		m_cbCardsWeaveCount[i] = stPlayer.cards_weave_size();
		for (_uint8 j = 0; j < stPlayer.cards_weave_size() && j < MAX_INDEX; ++j)
		{
			const proto::game::tagHandCards& stCards = stPlayer.cards_weave(j);
			m_CardsWeaveInfo[i][j].cbCardCount = stCards.card_size();
			for (_uint8 k = 0; k < stCards.card_size() && k < 4; ++k)
			{
				m_CardsWeaveInfo[i][j].cbCardList[k] = stCards.card(k);
			}
		}
		m_cbBestWeaveCount[i] = stPlayer.best_weave_size();
		for (_uint8 j = 0; j < stPlayer.best_weave_size() && j < MAX_INDEX; ++j)
		{
			const proto::game::tagHandCards& stCards = stPlayer.best_weave(j);
			m_BestWeaveInfo[i][j].cbCardCount = stCards.card_size();
			for (_uint8 k = 0; k < stCards.card_size() && k < 4; ++k)
			{
				m_BestWeaveInfo[i][j].cbCardList[k] = stCards.card(k);
			}
		}
		m_bWeaveItemCount[i] = stPlayer.weaves_size();
		for (_uint8 j = 0; j < stPlayer.weaves_size() && j < MAX_WEAVE; ++j)
		{
			const proto::game::tagWeaveCards& stWeave = stPlayer.weaves(j);
			m_WeaveItemArray[i][j].cbWeaveKind = stWeave.weave_kind();
			m_WeaveItemArray[i][j].cbCenterCard = stWeave.center_card();
			m_WeaveItemArray[i][j].cbOutCard = stWeave.out_card();
			m_WeaveItemArray[i][j].cbCardCount = stWeave.card_list_size();
			for (_uint8 k = 0; k < stWeave.card_list_size() && k < 4; ++k)
			{
				m_WeaveItemArray[i][j].cbCardList[k] = stWeave.card_list(k);
			}
		}
		m_cbUserChiCardCount[i] = stPlayer.chicard_info_size();
		for (_uint8 j = 0; j < stPlayer.chicard_info_size() && j < 6; ++j)
		{
			const proto::game::tagChiCardInfo &stChiCardInfo = stPlayer.chicard_info(j);
			m_UserChiCardInfo[i][j].cbChiKind = stChiCardInfo.chi_kind();
			m_UserChiCardInfo[i][j].cbCenterCard = stChiCardInfo.center_card();
			for (_uint8 k = 0; k < stChiCardInfo.cards_size() && k < 3; ++k)
			{
				m_UserChiCardInfo[i][j].cbCardData[k] = stChiCardInfo.cards(k);
			}
			m_UserChiCardInfo[i][j].cbSecondCount = stChiCardInfo.second_info_size();
			for (_uint8 m = 0; m < stChiCardInfo.second_info_size() && m < 6; ++m)
			{
				const proto::game::tagSecondInfo &stSecondInfo = stChiCardInfo.second_info(m);
				m_UserChiCardInfo[i][j].SecondData[m].cbChiKind = stSecondInfo.chi_kind();
				m_UserChiCardInfo[i][j].SecondData[m].cbCenterCard = stSecondInfo.center_card();
				for (_uint8 k = 0; k < stSecondInfo.cards_size() && k < 3; ++k)
				{
					m_UserChiCardInfo[i][j].SecondData[m].cbCardData[k] = stSecondInfo.cards(k);
				}
				m_UserChiCardInfo[i][j].SecondData[m].cbThirdCount = stSecondInfo.third_info_size();
				for (_uint8 n = 0; n < stSecondInfo.third_info_size() && n < 6; ++n)
				{
					const proto::game::tagThirdInfo &stThirdInfo = stSecondInfo.third_info(n);
					m_UserChiCardInfo[i][j].SecondData[m].ThirdData[n].cbChiKind = stThirdInfo.chi_kind();
					m_UserChiCardInfo[i][j].SecondData[m].ThirdData[n].cbCenterCard = stThirdInfo.center_card();
					for (_uint8 k = 0; k < stThirdInfo.cards_size() && k < 3; ++k)
					{
						m_UserChiCardInfo[i][j].SecondData[m].ThirdData[n].cbCardData[k] = stThirdInfo.cards(k);
					}
				}
			}
		}
		m_UserAbandonCount[i] = stPlayer.abandon_cards_size();
		for (_uint8 j = 0; j < stPlayer.abandon_cards_size() && j < MAX_INDEX; ++j)
		{
			m_UserAbandonCards[i][j] = stPlayer.abandon_cards(j);
		}
		for (_uint8 j = 0; j < stPlayer.abandon_type_size() && j < MAX_INDEX; ++j)
		{
			m_UserAbandonType[i][j] = stPlayer.abandon_type(j);
		}
		for (_uint8 j = 0; j < stPlayer.peng_chou_size() && j < MAX_INDEX; ++j)
		{
			m_bPengAbandon[i][j] = stPlayer.peng_chou(j);
		}
		for (_uint8 j = 0; j < stPlayer.chi_chou_size() && j < MAX_INDEX; ++j)
		{
			m_bChiAbandon[i][j] = stPlayer.chi_chou(j);
		}
		for (_uint8 j = 0; j < stPlayer.chou_wei_size() && j < MAX_INDEX; ++j)
		{
			m_bChouWeiCard[i][j] = stPlayer.chou_wei(j);
		}
		m_UserNiaoCount[i] = stPlayer.niao();
		m_UserPiaoScore[i] = stPlayer.piao();
		m_cbHuPaiCount[i] = stPlayer.hu_count();
		m_cbDianPaoCount[i] = stPlayer.dian_pao_count();
		m_wBestHuXi[i] = stPlayer.best_huxi();
		m_wBestScore[i] = stPlayer.best_score();
		m_wTotalScore[i] = stPlayer.total_score();
	}
	for (_uint8 i = 0; i < pTableInfo.weave_huxi_size() && i < GAME_PLAYER; ++i)
	{
		m_bWeaveHuXiCount[i] = pTableInfo.weave_huxi(i);
	}
	for (_uint8 i = 0; i < pTableInfo.show_huxi_size() && i < GAME_PLAYER; ++i)
	{
		m_bWeaveShowHuXiCount[i] = pTableInfo.show_huxi(i);
	}
	for (_uint8 i = 0; i < pTableInfo.hands_huxi_size() && i < GAME_PLAYER; ++i)
	{
		m_bHandsHuXiCount[i] = pTableInfo.hands_huxi(i);
	}
	//操作
	for (_uint8 i = 0; i < pTableInfo.user_responses_size() && i < GAME_PLAYER; ++i)
	{
		m_bResponse[i] = pTableInfo.user_responses(i);
	}
	for (_uint8 i = 0; i < pTableInfo.user_actions_size() && i < GAME_PLAYER; ++i)
	{
		m_bUserAction[i] = pTableInfo.user_actions(i);
	}
	for (_uint8 i = 0; i < pTableInfo.user_prerform_size() && i < GAME_PLAYER; ++i)
	{
		m_bPerformAction[i] = pTableInfo.user_prerform(i);
	}
	for (_uint8 i = 0; i < pTableInfo.chi_kind_size() && i < GAME_PLAYER; ++i)
	{
		m_cbUserChiKind[i] = pTableInfo.chi_kind(i);
	}
	for (_uint8 i = 0; i < pTableInfo.second_chi_kind_size() && i < GAME_PLAYER; ++i)
	{
		m_cbUserSecondChiKind[i] = pTableInfo.second_chi_kind(i);
	}
	for (_uint8 i = 0; i < pTableInfo.third_chi_kind_size() && i < GAME_PLAYER; ++i)
	{
		m_cbUserThirdChiKind[i] = pTableInfo.third_chi_kind(i);
	}
	for (_uint8 i = 0; i < pTableInfo.xiang_gong_size() && i < GAME_PLAYER; ++i)
	{
		m_bXiangGong[i] = pTableInfo.xiang_gong(i);
	}
	for (_uint8 i = 0; i < pTableInfo.common_best_size() && i < GAME_PLAYER; ++i)
	{
		m_bCommonOrBest[i] = pTableInfo.common_best(i);
	}
	for (_uint8 i = 0; i < pTableInfo.user_dong_zhang_size() && i < GAME_PLAYER; ++i)
	{
		m_bDongZhang[i] = pTableInfo.user_dong_zhang(i);
	}
	m_bDispatch = pTableInfo.dispatch();
	m_bDispatchHu = pTableInfo.dispatch_hu();
	m_bTiWeiPaoHu = pTableInfo.tiweipao_hu();
	m_bWeiHu = pTableInfo.wei_hu();
	m_bTiCard = pTableInfo.ti_card();
	m_bIsWeiTiCard = pTableInfo.is_weiti_card();
	m_bPaoCard = pTableInfo.pao_card();
	m_bFirstCard = pTableInfo.first_card();
	m_bCardOperated = pTableInfo.card_operated();
	m_bAfterWeave = pTableInfo.after_weave();
	m_wBankerUser = pTableInfo.banker_user();
	m_cbBankerCard = pTableInfo.banker_card();
	m_wCurrentUser = pTableInfo.current_user();
	m_cbCurrentCard = pTableInfo.current_card();
	m_wOutCardUser = pTableInfo.current_outcard_user();
	m_cbOutCardData = pTableInfo.current_outcard();
	m_GamePlayer = pTableInfo.game_player();
	m_wBaseHuXi = pTableInfo.base_huxi();
	m_wHuXiToTun = pTableInfo.huxi_to_tun();
	m_bDaNiao = pTableInfo.is_da_niao();
	m_bOutCard = pTableInfo.is_out_card();
	for (_uint8 i = 0; i < pTableInfo.niao_count_size(); ++i)
	{
		m_vecNiaoCount.push_back(pTableInfo.niao_count(i));
	}
	m_bPiao = pTableInfo.is_piao();
	for (_uint8 i = 0; i < pTableInfo.piao_count_size(); ++i)
	{
		m_vecPiaoScore.push_back(pTableInfo.piao_count(i));
	}
	m_bMingXiao = pTableInfo.is_ming_xiao();
	m_wCurrentNum = pTableInfo.current_num();
	m_wAllNum = pTableInfo.all_num();
	m_strInnings = pTableInfo.innings();
	m_videostr = pTableInfo.video_str();
	m_cbCurTimer = pTableInfo.cur_timer();	
	for(_uint8 i = 0; i < pTableInfo.gains_size(); ++i)
	{
		tagGain stGain;
		const proto::game::tagGain stProtoGain = pTableInfo.gains(i);
		for(_uint8 j = 0; j < stProtoGain.score_size(); ++j)
		{
			stGain.Gain[j] = stProtoGain.score(j);
		}
		m_vecGains.push_back(stGain);
	}
	//重启定时器
	AgainTimer();
	//log.info("Parse Table Data Parse succ tid[%s].\n", tid.c_str());
#endif
	return 0;
}


bool Table::OnUserGainsReq(Player* player)
{
	_tint32 wChairID = player->seatid;
	if(wChairID < 0 || wChairID >= GAME_PLAYER)
	{
		log.error("On user gains req error uid[%d] wChairID[%d] tid[%s] .\n", player->uid, wChairID, tid.c_str());
		return false;
	}

	proto::game::AckGains stGains;
	for(_uint8 i = 0; i < GAME_PLAYER; ++i)
	{
		if(seats[i].player == NULL) continue;
		proto::game::tagPlayers* pstPlayer = stGains.add_players();
		pstPlayer->set_seatid(seats[i].player->seatid);
		pstPlayer->set_uid(seats[i].player->uid);
		pstPlayer->set_name(seats[i].player->name);
		pstPlayer->set_avatar(seats[i].player->avatar);
		pstPlayer->set_sex(seats[i].player->sex);
		pstPlayer->set_money(seats[i].player->money);
		_tint32 wTotalGains = 0;
		std::vector<tagGain>::iterator iter = m_vecGains.begin();
		for(; iter != m_vecGains.end(); ++iter)
		{
			pstPlayer->add_gains((*iter).Gain[i]);
			wTotalGains += (*iter).Gain[i];
		}
		pstPlayer->set_total_gains(wTotalGains);
	}
	Ppacket ppack;
	stGains.SerializeToString(&ppack.body);
	ppack.pack(SERVER_PHZ_GAINS_RES);
	unicast(player, ppack.data);
	return true;
}
