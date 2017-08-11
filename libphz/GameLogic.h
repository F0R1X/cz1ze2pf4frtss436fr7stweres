#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

//#pragma once
#include "Common.h"

//数组说明
//typedef CArrayTemplate<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;
typedef std::vector<tagAnalyseItem> CAnalyseItemArray;
typedef std::vector<tagWeaveItem> CWeaveItemArray;

//游戏逻辑类
class CGameLogic
{
	//变量定义
	public:
		static const _uint8				m_cbCardDataArray[MAX_CARDS];				//扑克数据
		//函数定义
	public:
		//构造函数
		CGameLogic();
		//析构函数
		virtual ~CGameLogic();
		//控制函数
	public:
		//初始化扑克
		void initCarData(_uint8 cbCardData[]);
		//混乱扑克
		void RandCardData(_uint8 cbCardData[], _uint8 bMaxCount);
		//删除扑克
		bool RemoveCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbRemoveCard);
		//删除扑克
		bool RemoveCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbRemoveCard[], _uint8 bRemoveCount);
		//删除扑克
		bool RemoveCard(_uint8 cbCardData[], _uint8 bCardCount, _uint8 cbRemoveCard[], _uint8 bRemoveCount);
		//逻辑函数
	public:
		//提牌判断
		_uint8 GetAcitonTiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbTiCardIndex[5]);
		//畏牌判断
		_uint8 GetActionWeiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbWeiCardIndex[7]);
		//吃牌判断
		_uint8 GetActionChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, tagChiCardInfo ChiCardInfo[6]);
		//获取吃牌信息
		_uint8 GetChiCardInfo(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, tagNewChiCardInfo ChiCardInfo[6]);
		//判断函数
	public:
		//是否吃牌
		bool IsChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard);
		//是否提跑
		bool IsTiPaoCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard);
		//是否偎碰
		bool IsWeiPengCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard);
		//胡牌结果
		bool GetHuCardInfoFinal(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbWeaveItemCount, tagWeaveItem WeaveItemArray[MAX_WEAVE], tagHuCardInfo & HuCardInfo, _uint32 wBaseHuXi, std::vector<tagHuFanInfo>& vecHuFanInfo, bool bZimo, bool bDispatch, bool bHHD ,bool bHu = false, bool isWithMaoHu = false );
		//胡牌结果
		bool GetHuCardInfo(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbHuXiWeave, tagHuCardInfo & HuCardInfo, _uint32 wBaseHuXi, bool bZimo = true, bool bMingXiao = false);
		bool GetHuCardInfoOld(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbHuXiWeave, tagHuCardInfo & HuCardInfo, _uint32 wBaseHuXi, bool bZimo = true, bool bMingXiao = false);
		//辅助函数
	public:
		//有效判断
		bool IsValidCard(_uint8 cbCardData);
		//扑克数目
		_uint8 GetCardCount(_uint8 cbCardIndex[MAX_INDEX]);
		//获取胡息
		_uint8 GetWeaveHuXi(const tagWeaveItem & WeaveItem);
		//打印牌信息
		void DebugCards(_uint8 cbCardIndex[], _uint8 count);
		//计算胡息
		_uint8 GetCardsWeaveHuXi( tagCardsWeave CardsWeave[MAX_INDEX], _uint8 cbCardsWeaveCount );
		//更新手牌组合
		bool UpdateCardsWeave( tagCardsWeave CardsWeave[MAX_INDEX], _uint8 &cbCardsWeaveCount, _uint8 cbRemoveCards[4], _uint8 cbRemoveCount, _uint8 cbRow = -1, _uint8 cbCol = -1);
		//整理普通模式牌
		bool SettleCommonWeaves(_uint8 cbCardIndex[MAX_INDEX], tagCardsWeave WeaveItem[MAX_INDEX], _uint8 &cbWeaveCount );
		//整理最优模式牌
		bool SettleBestWeaves(_uint8 cbCardIndex[MAX_INDEX], tagCardsWeave WeaveItem[MAX_INDEX], _uint8 &cbWeaveCount );
		//顺序整理方案
		bool SettleOrderWeaves(_uint8 cbCardIndex[MAX_INDEX], tagCardsWeave WeaveItem[MAX_INDEX], _uint8 &cbWeaveCount);
		//转换函数
	public:
		//扑克转换
		_uint8 SwitchToCardData(_uint8 cbCardIndex);
		//扑克转换
		_uint8 SwitchToCardIndex(_uint8 cbCardData);
		//扑克转换
		_uint8 SwitchToCardData(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCardData[], _uint8 bMaxCount);
		//扑克转换
		_uint8 SwitchToCardIndex(_uint8 cbCardData[], _uint8 bCardCount, _uint8 cbCardIndex[MAX_INDEX]);
		//内部函数
	private:
		//分析扑克
		bool AnalyseCard(_uint8 cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray);
		bool AnalyseCardOld(_uint8 cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray);
		//提取吃牌
		_uint8 TakeOutChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbResultCard[3]);
		//提取吃牌
		_uint8 TakeOutChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbResultCard[3], _uint32 &wChiKinds );
		_uint32 GetHuFanType(tagAnalyseItem stAnalyseItem, std::vector<tagHuFanInfo> &vecHuFanInfo, _uint8 &cbFinalHuXiCount, bool bWithMaoHu, bool bHHD);
		//_uint32 GetHuFanType(const tagHuCardInfo &HuCardInfo, const tagAnalyseItem& stAnalyseItem, const _uint8 cbWeaveItemCount, const tagWeaveItem WeaveItemArray[MAX_WEAVE], _uint8 cbWeaveHuXi, std::vector<tagHuFanInfo> &vecHuFanInfo, bool bWithMaoHu,bool bHHD);
		//
	public:
		_uint8 GetSingleCard(_uint8 cbCardIndex[MAX_INDEX]);
		void GetSingleGroup(_uint8 cbCardIndex[MAX_INDEX], _uint8 singleCard, CWeaveItemArray &WeaveItemGroup);
		void GetDoubleGroup(_uint8 cbCardIndex[MAX_INDEX], CWeaveItemArray &WeaveItemGroup);
		bool GetAllGroup(_uint8 cbCardIndex[MAX_INDEX], CWeaveItemArray &BestItemGroup, CAnalyseItemArray & AnalyseItemArray);
public:
};

//////////////////////////////////////////////////////////////////////////

#endif
