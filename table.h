#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ev++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <json/json.h>
#include <map>
#include <set>
#include <algorithm>
#include <sstream> 

#include "zjh.h"
#include "log.h"
#include "jpacket.h"
#include "game.h"
#include "client.h"
#include "player.h"
#include "proto.h"
#include "base64.h"
#include "zlib.h"
#include "GameLogic.h"
#include "loginproto.pb.h"
#include "gameproto.pb.h"
#include "Common.h"

#define MaxVideoBufLen  1024 * 10
#define MaxOfflineLen	200			//离线信息最大长度

class Player;
class Client;

//每一局得分记录
struct tagGain
{
	_tint32 Gain[GAME_PLAYER];
	tagGain()
	{
		memset(Gain, 0, sizeof(Gain));
	}
};

//游戏座位定义
typedef struct
{
	int uid;	
	int occupied; 
	int ready; 
	Player *player;

	void clear(void)
	{
		uid = 0;
		occupied = 0; 
		ready = 0;
		player = NULL;
	}

	void reset(void)
	{
		ready = 0;
	}
} Seat;

//游戏状态定义
typedef enum
{
	GAME_FREE = 0,
	GAME_NIAO,
	GAME_PIAO,
	GAME_PLAYING,
	GAME_END,
} State;

//游戏桌子定义
class Table
{
	public:
		std::string					tid;		
		int             			vid;
		int							zid;
		int							type; 		
		int							base_money;			//底分
		std::map<int, Player*>		players;			//桌上和桌下玩家
		Seat						seats[GAME_PLAYER];	//桌上所有桌位
		State						state;				//游戏状态
		int							stand_money;		//玩家下桌分数（金额）线
	public:
		_uint32						m_wTakeIn;			//房间带入
		_uint32						m_wPlayID;			//玩法ID
		_uint8						m_cbRoomStatus;		//房间状态
		_uint32						m_wRoomUserId;		//房主uid
	public:
		std::map<int, Player*>		voters_players;		//解散房间投票者
		int							dissolve_applyer;	//解散房间申请者
		int							dissovle_state;		//是否处于解散房间 0--no 1--yes
		int							is_dissolved;		//此房间已被解散
	private:
		//时钟
		ev_timer                    start_timer;
		ev_tstamp                   start_timer_stamp;	

		ev_timer                    preready_timer;
		ev_tstamp                   preready_timer_stamp;	

		ev_timer                    dispatch_show_timer;								
		ev_tstamp                   dispatch_show_timer_stamp;

		ev_timer                    out_show_timer;								
		ev_tstamp                   out_show_timer_stamp;

		ev_timer                    dispatch_delay_timer;								
		ev_tstamp                   dispatch_delay_timer_stamp;

		ev_timer                    out_delay_timer;								
		ev_tstamp                   out_delay_timer_stamp;

		ev_timer					dissolve_room_timer;
		ev_tstamp					dissolve_room_tstamp;

		ev_timer                    send_videodata_timer;
		ev_tstamp                   send_videodata_timer_stamp; 

		ev_timer                    game_restart_timer;
		ev_tstamp                   game_restart_timer_stamp; 

		//时钟回调
		static void start_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void preready_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void dispatch_show_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void out_show_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void dispatch_delay_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void out_delay_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void dissolve_room_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void Send_Video_Data_Timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
		static void game_restart_timer_cb(struct ev_loop *loop, struct ev_timer *w, int revents);
	public:
		Table();
		virtual ~Table();
		//初始化房间参数
		void init_made_info(Json::Value &val);
		int split(const std::string& str, std::vector<std::string>& ret_, std::string sep);
		void check_playid(int nPlayID);
		//房间初始化
		int init( Json::Value &valCfg, Client* client);
		//广播
		int broadcast(Player *p, const std::string &packet);
		//单播
		int unicast(Player *p, const std::string &packet);
		//随机函数
		int random(int start, int end, int seed);
		//玩家操作接口
		int handler_user_operate(int cmd, Player* player);
		//坐下站起
		int sit_down(Player *player);
		void stand_up(Player *player);
		//删除玩家
		int del_player(Player *player);
		//登录和桌上信息
		int handler_login(Player *player);
		int handler_login_succ_uc(Player *player);
		int handler_login_succ_bc(Player *player);
		int handler_table_info(Player *player, int req_count = -1);
		//预准备处理
		int handler_preready();
		//退出登录
		int handler_logout(Player *player);
		//互动表情
		int handler_interaction_emotion(Player *player);
		//打鸟
		int handler_betting(Player *player);
		//打鸟失败
		void betting_error_uc(Player *player, int code);
		//申请上桌
		int handler_apply_uptable(Player *player);
		//上桌
		int handler_uptable(Player *player);
		//上桌失败
		void uptable_error_uc(Player *player, int code);
		//下桌
		int handler_downtable(Player *player);
		//玩家准备
		void handler_ready(Player* player); 
		//请求开始游戏
		int game_start_req();
		//开始游戏返回
		int game_start_res( Jpacket &packet );
		//心跳返回
		int heart_beat_res(Player* player);
		//用户断线
		void user_offline(Player * player);
		//检测房间解散结果
		void check_dissovle_result();
		//投票解散房间
		int handler_dissolve_action(Player *player);
		//请求解散房间
		int handler_dissolve_room(Player *player);
		//提前结束这局
		int advance_game_end();
		//更新结算数据到数据库
		void update_user_info_to_datasvr(int UserScore[GAME_PLAYER]);
	public:
		//初始化数据
		void InitData();
		//重置数据
		void Reset();
		//开始打鸟
		bool StartBetting();
		//游戏开始
		bool GameStart();
		//游戏操作开始
		bool OnOperateStart();
		//游戏结束
		bool GameEnd(_tint32 wChairID);
		//用户出牌
		bool OnUserOutCard(Player* player);
		//吃牌
		bool UserOperateChi( _tint32 wTargetUser );
		//碰牌
		bool UserOperatePeng( _tint32 wTargetUser );
		//操作后判断跑牌
		bool EstimatePaoCard();
		//单操作
		bool DoSingleOperate( _tint32 wTargetUser, _uint8 cbTargetAction );
		//多操作
		bool DoMultiOperate( _tint32 wTargetUser, _uint8 cbTargetAction );
		//用户预操作
		bool OnUserPreOperateCard( Player* player );
		//是否为最优玩家, 返回最优玩家
		bool IsFirstUser( _tint32 wChairID, _tint32& wFirstUser );
		//用户操作
		bool OnUserOperateCard(Player* player );
		//派发扑克
		bool DispatchCardData(_tint32 wCurrentUser);
		//出牌提示
		bool SendOutCardNotify(_tint32 wCurrentUser, bool bOutCard);
		//派发响应判断
		bool DispatchEstimateUserRespond( _tint32 wCenterUser, _uint8 cbCenterCard );
		//打牌响应判断
		bool OutEstimateUserRespond( _tint32 wCenterUser, _uint8 cbCenterCard );
		//出牌判断
		bool IsAllowOutCard(_tint32 wChairID);
		//更新胡息
		bool UpdateUserHuXiCount(_tint32 wChairID);
		//获取胡番类型和番数
		_uint32 GetHuFanType( _uint8 cbCardIndex[MAX_INDEX], _uint8 cbWeaveItemCount,  tagWeaveItem WeaveItemArray[MAX_WEAVE], tagHuCardInfo HuCardInfo, _uint8 cbHuCard, std::vector<tagHuFanInfo> &vecHuFanInfo );
		//玩家交换牌
		bool OnUserChangeCard(Player* player );
		//玩家更新牌
		bool OnUserUpdateCard( Player* player );
		//刷新普通组合
		bool UpdateCommonCardsWeave( _tint32 wChairID );
		//刷新最佳手牌
		bool UpdateBestCardsWeave( _tint32 wChairID );
		//更新手牌组合以及手牌胡息
		bool UpdateCardsWeave( _tint32 wChairID, _uint8 cbRemoveCards[4], _uint8 cbRemoveCount, _uint8 cbRow = -1, _uint8 cbCol = -1 );
		//更新弃牌
		bool UpdateAbandonCard();
		//是否是跑牌
		bool IsPaoCard( _tint32 wCenterUser, _uint8 cbCenterCard );
		//开始飘
		bool StartPiao();
		//飘失败
		void PiaoError( Player* player, int code );
		//玩家飘操作
		bool OnUserPiaoOperate(Player* player );
		//听牌信息
		bool IsListen( _tint32 wChairID, std::vector<_uint8>& vecListenCards );
		//是否可以吃碰
		bool IsAllowChiPeng( _tint32 wChairID );
		//离线通知
		void handler_offline(Player *player);
		//最终结算
		void OnFinalEndReq( Player* player );
		//是否是过牌
		bool IsPassCard( _tint32 wChairID, _uint8 cbCardData );
		//臭牌处理
		bool HandlerChouCard();
		//是否是过牌
		bool IsPassPengCard( _tint32 wChairID, _uint8 cbCardData );
		//请求离线信息
		void OnOfflineInfoReq( Player* player );
		//是否已经全部现出
		bool IsListenOver( _tint32 wChairID, _uint8 cbCard );
		//臭牌提示
		void SendChouNotify( _tint32 wChairID, _uint8 cbType );
		//更新组合后打牌
		void UpdateWeaveOutCard(_tint32 wChairID, _uint8 cbCard);
		//手牌详情
		void OnHandDetailInfo(Player* player);
		//是否是提偎牌
		bool IsWeiTiCard( _tint32 wChairID, _uint8 cbCard );

		//3提5坎
		void Do3Ti5Kan(proto::game::AckGameEnd& stGameEnd, _uint32& wTotalHuXi, _tint32 wChairID);
		//战绩请求
		bool OnUserGainsReq(Player* player);
		
		//录像数据保存
	protected:
		//保存tableinfo录像
		void save_table_info_video();
		//保存游戏开始录像
		void save_game_start_video();
		//保存操作录像
		void save_operate_video( _tint32 wChairID, _uint8 cbAckCode, _uint8 cbActionCard );
		//保存发牌录像
		void save_dispatch_card_video( _tint32 wChairID, _uint8 cbCardData, _tint32 wToSeatid = INVALID_CHAIR );
		//保存操作提示录像
		void save_operate_notify_video();
		//保存操作录像
		void save_operate_res_video( _tint32 wChairID, _uint8 cbOpCode );
		//保存打牌录像
		void save_out_card_video( _tint32 wChairID, _uint8 cbCardData );
		//保存弃牌录像
		void save_abandon_card_video( _tint32 wChairID );
		//保存游戏结束录像
		void save_game_end_video( _tint32 cmd, std::string str );
		//保存录像信息
		void save_video_data(_tint32 cmd, std::string str);
		//发送录像信息
		void send_video_to_svr();
		//保存玩家离线信息
		void save_offline_info( _tint32 wChairID, _tint32 cmd, std::string str );
		//重启定时器
		int AgainTimer();
		
		int handler_face(Player *player);

		int handler_chat(Player *player);
	public:
		//恢复数据恢复
		//解压
		int decompress_str(string &src, string &des);
		//压缩
		int compress_str(string &src, string &des);
		//序列化牌桌数据
		_tint32 SerializeTableData();
		//解析牌桌数据
		_tint32 ParseTableData(std::string &data_str);
		//游戏变量
	protected:
		_uint8							m_cbRepertoryCard[MAX_CARDS];			//库存扑克
		_uint8							m_bLeftCardCount;						//剩余数目
		_uint8							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//用户手牌
		_uint8							m_cbCardsWeaveCount[GAME_PLAYER];		//手牌组合数
		tagCardsWeave					m_CardsWeaveInfo[GAME_PLAYER][MAX_INDEX];//手牌组合
		_uint8							m_cbBestWeaveCount[GAME_PLAYER];		//手牌最优组合数
		tagCardsWeave					m_BestWeaveInfo[GAME_PLAYER][MAX_INDEX];//手牌最优组合
		_uint8							m_bWeaveHuXiCount[GAME_PLAYER];			//组合胡息数
		_uint8							m_bWeaveShowHuXiCount[GAME_PLAYER];		//组合显示胡息数
		_uint8							m_bHandsHuXiCount[GAME_PLAYER];			//手牌胡息数
		_uint8							m_bWeaveItemCount[GAME_PLAYER];			//组合数目
		tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];//组合扑克
		_uint8							m_cbUserChiCardCount[GAME_PLAYER];		//玩家吃牌组合数
		tagNewChiCardInfo				m_UserChiCardInfo[GAME_PLAYER][6];		//玩家吃牌组合
		_uint8							m_UserAbandonCount[GAME_PLAYER];		//玩家放弃的牌数
		_uint8							m_UserAbandonCards[GAME_PLAYER][MAX_INDEX];//玩家放弃的牌
		_uint8							m_UserAbandonType[GAME_PLAYER][MAX_INDEX];//玩家放弃的牌的类型 1：发的牌 2：打的牌
		_uint8							m_UserZiMoCnt[GAME_PLAYER];				//自摸次数
		_tint16							m_UserAllPiaoScore[GAME_PLAYER];		//飘所得总分
	protected:
		bool 							m_b3Ren15Zhang;							//三人玩法15张，
		bool							m_bDispatch;							//派发标志
		bool							m_bOutCard;								//出牌标志
		bool							m_bDispatchHu;							//派发胡牌标志
		bool							m_bTiWeiPaoHu;							//提偎跑胡牌标志
		bool							m_bWeiHu;								//偎胡牌标志
		bool							m_bTiCard;								//提牌标志
		bool							m_bIsWeiTiCard;							//偎提标志
		bool							m_bPaoCard;								//跑牌标志
		bool							m_bFirstCard;							//第一张牌标志
		bool							m_bCardOperated;						//此牌是否被操作过
		bool							m_bAfterWeave;							//是否是在组合后打牌
		bool							m_bCurrentMustHu;						//当前胡是不是必胡
	protected:
		_tint32							m_wBankerUser;							//庄家用户
		_uint8							m_cbBankerCard;							//庄家的最后一张牌
		_tint32							m_wCurrentUser;							//当前用户
		_uint8							m_cbCurrentCard;						//当前摸到或打出的牌
		_tint32							m_wOutCardUser;							//出牌用户的座位号
		_uint8							m_cbOutCardData;						//出牌扑克
	public:
		bool							m_bResponse[GAME_PLAYER];				//响应标志
		_uint8							m_bUserAction[GAME_PLAYER];				//用户动作
		_uint8							m_bPerformAction[GAME_PLAYER];			//执行动作
		_uint8							m_cbUserChiKind[GAME_PLAYER];			//玩家操作的吃类型
		_uint8							m_cbUserSecondChiKind[GAME_PLAYER];		//玩家操作的第二层吃类型
		_uint8							m_cbUserThirdChiKind[GAME_PLAYER];		//玩家操作的第三层吃类型
		bool							m_bPengAbandon[GAME_PLAYER][MAX_INDEX];	//碰臭牌记录
		bool							m_bChiAbandon[GAME_PLAYER][MAX_INDEX];	//吃臭牌记录
		bool							m_bChouWeiCard[GAME_PLAYER][MAX_INDEX];	//臭偎记录
		bool							m_bXiangGong[GAME_PLAYER];				//是否是相公
		bool							m_bDongZhang[GAME_PLAYER];				//是否是动张
	public:
		bool							m_bCommonOrBest[GAME_PLAYER];			//手牌是普通组合还是最优组合
		_uint32							m_wHuangZhuangCount;					//黄庄次数
		_tint32							m_UserNiaoCount[GAME_PLAYER];			//玩家打鸟个数
		_tint32							m_UserPiaoScore[GAME_PLAYER];			//玩家飘的分数
	public:
		bool							m_bIsTianHuShow;						//是否是天胡亮张
		bool							m_bIsTianHu3Ti5Kan;						//是否天胡3提5坎
		bool							m_bIsDiHuOut;							//是否是地胡亮张
	public:
		_uint8							m_cbOfflineCount[GAME_PLAYER];			//离线信息长度
		std::string						m_strOfflineInfo[GAME_PLAYER][MaxOfflineLen];//离线信息
	public:
		_uint8							m_cbHuPaiCount[GAME_PLAYER];			//胡牌次数
		_uint8							m_cbDianPaoCount[GAME_PLAYER];			//点炮次数
		_uint32							m_wBestHuXi[GAME_PLAYER];				//最高胡息
		_uint32							m_wBestFanShu[GAME_PLAYER];				//最高番数
		_uint32							m_wBestScore[GAME_PLAYER];				//最高得分
		_tint32							m_wTotalScore[GAME_PLAYER];				//总输赢
		std::vector<tagGain>			m_vecGains;								//战绩记录
		//参数配置
	public:
		_uint8							m_GamePlayer;							//房间人数
		_uint32							m_wBaseHuXi;							//起胡胡息
		_uint32							m_wHuXiToTun;							//囤数换算，多少胡息为1囤
		bool							m_bDaNiao;								//是否能打鸟
		std::vector<_uint8>				m_vecNiaoCount;							//打鸟的个数
		bool							m_bPiao;								//是否能飘
		std::vector<_uint8>				m_vecPiaoScore;							//能飘的分数
		bool							m_bMingXiao;							//偎牌明牌
		_uint8							m_cbCurTimer;							//当前定时器
	protected:
		_uint32							m_wCurrentNum;							//局数编号	
		_uint32							m_wAllNum;								//总局数	
		std::string						m_strInnings;							//房间局数唯一标识
		CGameLogic						m_GameLogic;							//游戏逻辑
		std::string						m_videostr;								//录像数据
		std::string						m_strLastEndInfo;						//上局结算信息
		bool							m_bAlreadyResp;						//游戏已经开始

		bool							m_bWithMaoHu;							//是否带有毛胡
		bool							m_bZiMoFan;								//自摸翻倍
		_uint8							m_cbMustHu;								//0:没有要求，1：点炮必胡，2：必胡
		bool							m_bHHD;									//是否红黑点
		bool							m_bTianHu3Ti5Kan;						//是否有天地胡
};

#endif
