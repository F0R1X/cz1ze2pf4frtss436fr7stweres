//#include "StdAfx.h"
#include "GameLogic.h"

//扑克数据
_uint8 const CGameLogic::m_cbCardDataArray[MAX_CARDS] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,					//小写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,					//大写
};

/*_uint8 const CGameLogic::m_cbCardDataArray[80]=
  {
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x07,					//小写
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x08,					//小写
  0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x09,					//小写
  0x01,0x02,0x03,0x04,0x05,0x06,0x0A,0x0A,0x0A,0x0A,					//小写
  0x11,0x12,0x13,0x14,0x15,0x13,0x17,0x18,0x19,0x17,					//大写
  0x11,0x12,0x13,0x14,0x15,0x14,0x17,0x18,0x19,0x18,					//大写
  0x11,0x12,0x13,0x14,0x15,0x15,0x17,0x18,0x19,0x19,					//大写
  0x11,0x12,0x16,0x16,0x16,0x16,0x1A,0x1A,0x1A,0x1A,					//大写
  };*/

  //////////////////////////////////////////////////////////////////////////

  //构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//初始化扑克
void CGameLogic::initCarData(_uint8 cbCardData[])
{
	memcpy(cbCardData, m_cbCardDataArray, sizeof(m_cbCardDataArray));
	return;
}
//混乱扑克
void CGameLogic::RandCardData(_uint8 cbCardData[], _uint8 bMaxCount)
{
	//混乱准备
	_uint8 cbCardDataTemp[MAX_CARDS] = { 0 };
	memcpy(cbCardDataTemp, m_cbCardDataArray, sizeof(m_cbCardDataArray));
	srand(time(NULL));

	//混乱扑克
	_uint8 bRandCount = 0, bPosition = 0;
	do
	{
		bPosition = rand() % (bMaxCount - bRandCount);
		cbCardData[bRandCount++] = cbCardDataTemp[bPosition];
		cbCardDataTemp[bPosition] = cbCardDataTemp[bMaxCount - bRandCount];
	} while (bRandCount < bMaxCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbRemoveCard)
{
	//效验扑克
	//assert(IsValidCard(cbRemoveCard));
	//assert(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	_uint8 cbRemoveIndex = SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex] > 0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	//assert(false);

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbRemoveCard[], _uint8 bRemoveCount)
{
	//删除扑克
	for (_uint8 i = 0; i < bRemoveCount; i++)
	{
		//效验扑克
		//assert(IsValidCard(cbRemoveCard[i]));
		//assert(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);
		_uint8 cbRemoveIndex = SwitchToCardIndex(cbRemoveCard[i]);
		//删除扑克
		if (cbCardIndex[cbRemoveIndex] == 0)
		{
			//错误断言
			//assert(false);

			//还原删除
			for (_uint8 j = 0; j < i; j++)
			{
				//assert(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(_uint8 cbCardData[], _uint8 bCardCount, _uint8 cbRemoveCard[], _uint8 bRemoveCount)
{
	//检验数据
	//assert(bCardCount<=MAX_COUNT);
	//assert(bRemoveCount<=bCardCount);

	//定义变量
	_uint8 bDeleteCount = 0, cbTempCardData[MAX_COUNT];
	//if (bCardCount>CountArray(cbTempCardData)) return false;
	//if (bCardCount>sizeof(cbTempCardData)) return false;
	memcpy(cbTempCardData, cbCardData, bCardCount * sizeof(cbCardData[0]));

	//置零扑克
	for (_uint8 i = 0; i < bRemoveCount; i++)
	{
		for (_uint8 j = 0; j < bCardCount; j++)
		{
			if (cbRemoveCard[i] == cbTempCardData[j])
			{
				bDeleteCount++;
				cbTempCardData[j] = 0;
				break;
			}
		}
	}

	//成功判断
	if (bDeleteCount != bRemoveCount)
	{
		//assert(false);
		return false;
	}

	//清理扑克
	_uint8 bCardPos = 0;
	for (_uint8 i = 0; i < bCardCount; i++)
	{
		if (cbTempCardData[i] != 0) cbCardData[bCardPos++] = cbTempCardData[i];
	}

	return true;
}

//提牌判断
_uint8 CGameLogic::GetAcitonTiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbTiCardIndex[5])
{
	//提牌搜索
	_uint8 cbTiCardCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] == 4) cbTiCardIndex[cbTiCardCount++] = i;
	}

	return cbTiCardCount;
}

//畏牌判断
_uint8 CGameLogic::GetActionWeiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbWeiCardIndex[7])
{
	//畏牌搜索
	_uint8 cbWeiCardCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndex[i] == 3) cbWeiCardIndex[cbWeiCardCount++] = i;
	}

	return cbWeiCardCount;
}

//提取吃牌
_uint8 CGameLogic::GetActionChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, tagChiCardInfo ChiCardInfo[6])
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return 0;

	//变量定义
	_uint8 cbChiCardCount = 0;
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//三牌判断
	if (cbCardIndex[cbCurrentIndex] >= 3) return cbChiCardCount;

	//大小搭吃
	_uint8 cbReverseIndex = (cbCurrentIndex + 10) % MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex] >= 1) && (cbCardIndex[cbReverseIndex] >= 1) && (cbCardIndex[cbReverseIndex] != 3))
	{
		//构造扑克
		_uint8 cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbCurrentIndex]--;
		cbCardIndexTemp[cbReverseIndex]--;

		//提取判断 
		_uint8 cbResultCount = 1;
		while (cbCardIndexTemp[cbCurrentIndex] > 0)
		{
			_uint8* pcbResult = ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
			if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, pcbResult) != CK_NULL) cbResultCount++;
			else break;
		}

		//设置结果
		if (cbCardIndexTemp[cbCurrentIndex] == 0)
		{
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][2] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
		}
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex] == 2)
	{
		//构造扑克
		_uint8 cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbReverseIndex] -= 2;

		//提取判断
		_uint8 cbResultCount = 1;
		while (cbCardIndexTemp[cbCurrentIndex] > 0)
		{
			_uint8* pcbResult = ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
			if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, pcbResult) != CK_NULL) cbResultCount++;
			else break;
		}

		//设置结果
		if (cbCardIndexTemp[cbCurrentIndex] == 0)
		{
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
			ChiCardInfo[cbChiCardCount].cbCardData[0][0] = cbCurrentCard;
			ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount].cbCardData[0][2] = SwitchToCardData(cbReverseIndex);
			ChiCardInfo[cbChiCardCount++].cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
		}
	}

	//二七十吃
	_uint8 bCardValue = cbCurrentCard&MASK_VALUE;
	if ((bCardValue == 0x02) || (bCardValue == 0x07) || (bCardValue == 0x0A))
	{
		//变量定义
		_uint8 cbExcursion[] = { 1,6,9 };
		_uint8 cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//类型判断
		//for (_uint8 i=0;i<CountArray(cbExcursion);i++)
		_uint8 i;
		for (i = 0; i < 3; i++)
		{
			_uint8 cbIndex = cbInceptIndex + cbExcursion[i];
			if ((cbIndex != cbCurrentIndex) && ((cbCardIndex[cbIndex] == 0) || (cbCardIndex[cbIndex] == 3))) break;
		}

		//提取判断
		//if (i==CountArray(cbExcursion))
		if (i == 3)
		{
			//构造扑克
			_uint8 cbCardIndexTemp[MAX_INDEX];
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

			//删除扑克
			//for (_uint8 j=0;j<CountArray(cbExcursion);j++)
			for (_uint8 j = 0; j < 3; j++)
			{
				_uint8 cbIndex = cbInceptIndex + cbExcursion[j];
				if (cbIndex != cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
			}

			//提取判断
			_uint8 cbResultCount = 1;
			while (cbCardIndexTemp[cbCurrentIndex] > 0)
			{
				_uint8* pcbResult = ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
				if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, pcbResult) != CK_NULL) cbResultCount++;
				else break;
			}

			//设置结果
			if (cbCardIndexTemp[cbCurrentIndex] == 0)
			{
				ChiCardInfo[cbChiCardCount].cbChiKind = CK_EQS;
				ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
				ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
				ChiCardInfo[cbChiCardCount].cbCardData[0][0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
				ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
				ChiCardInfo[cbChiCardCount++].cbCardData[0][2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);
			}
		}
	}

	//顺子类型
	_uint8 cbExcursion[3] = { 0,1,2 };
	// for (_uint8 i=0;i<CountArray(cbExcursion);i++)
	for (_uint8 i = 0; i < 3; i++)
	{
		_uint8 cbValueIndex = cbCurrentIndex % 10;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 7))
		{
			//索引定义
			_uint8 cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//吃牌判断
			_uint8 j;
			for (j = 0; j < 3; j++)
			{
				_uint8 cbIndex = cbFirstIndex + j;
				if ((cbIndex != cbCurrentIndex) && ((cbCardIndex[cbIndex] == 0) || (cbCardIndex[cbIndex] == 3))) break;
			}

			//提取判断
			//if (j==CountArray(cbExcursion))
			if (j == 3)
			{
				//构造扑克
				_uint8 cbCardIndexTemp[MAX_INDEX];
				memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

				//删除扑克
				for (_uint8 j = 0; j < 3; j++)
				{
					_uint8 cbIndex = cbFirstIndex + j;
					if (cbIndex != cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
				}

				//提取判断
				_uint8 cbResultCount = 1;
				while (cbCardIndexTemp[cbCurrentIndex] > 0)
				{
					_uint8* pcbResult = ChiCardInfo[cbChiCardCount].cbCardData[cbResultCount];
					if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, pcbResult) != CK_NULL) cbResultCount++;
					else break;
				}

				//设置结果
				if (cbCardIndexTemp[cbCurrentIndex] == 0)
				{
					_uint8 cbChiKind[3] = { CK_LEFT,CK_CENTER,CK_RIGHT };
					ChiCardInfo[cbChiCardCount].cbChiKind = cbChiKind[i];
					ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
					ChiCardInfo[cbChiCardCount].cbResultCount = cbResultCount;
					ChiCardInfo[cbChiCardCount].cbCardData[0][0] = SwitchToCardData(cbFirstIndex);
					ChiCardInfo[cbChiCardCount].cbCardData[0][1] = SwitchToCardData(cbFirstIndex + 1);
					ChiCardInfo[cbChiCardCount++].cbCardData[0][2] = SwitchToCardData(cbFirstIndex + 2);
				}
			}
		}
	}

	return cbChiCardCount;
}

//是否吃牌
bool CGameLogic::IsChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard)
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return false;

	//构造扑克
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	//插入扑克
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//提取判断
	while (cbCardIndexTemp[cbCurrentIndex] > 0)
	{
		_uint8 cbResult[3];
		if (TakeOutChiCard(cbCardIndexTemp, cbCurrentCard, cbResult) == CK_NULL) break;
	}

	return (cbCardIndexTemp[cbCurrentIndex] == 0);
}

//是否提跑
bool CGameLogic::IsTiPaoCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard)
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return false;

	//转换索引
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//碰牌判断
	return (cbCardIndex[cbCurrentIndex] == 3) ? true : false;
}

//是否偎碰
bool CGameLogic::IsWeiPengCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard)
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return false;

	//转换索引
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//跑偎判断
	return (cbCardIndex[cbCurrentIndex] == 2) ? true : false;
}

//获取番类型和番数
//获取番类型和番数
_uint32 CGameLogic::GetHuFanType(tagAnalyseItem stAnalyseItem, std::vector<tagHuFanInfo> &vecHuFanInfo, _uint8 &cbFinalHuXiCount,bool bWithMaoHu,bool bHHD)
{
	_uint32 wTotalFanNum = 1;
	_uint32 wTotalHuXi = stAnalyseItem.cbHuXiCount; //总胡息的引用

	_uint8 cbTotalCardCount = 0;
	_uint8 cbHongZiCount = 0;
	_uint8 cbHeiZiCount = 0;
	if (bWithMaoHu && (0 == wTotalHuXi))
	{
		tagHuFanInfo HuFanInfo;
		wTotalHuXi = 15;
		HuFanInfo.wHuFanType = MT_MAO_HU;
		HuFanInfo.wHuFanNum = wTotalHuXi;
		HuFanInfo.wHuFanXi = wTotalHuXi;
		vecHuFanInfo.push_back(HuFanInfo);
	}
	if (bHHD)
	{
		//红黑字统计
		for (_uint8 i = 0; i < stAnalyseItem.cbWeaveCount; ++i)
		{
			cbTotalCardCount += stAnalyseItem.WeaveItemArray[i].cbCardCount;
			for (_uint8 j = 0; j < stAnalyseItem.WeaveItemArray[i].cbCardCount && j < 4; ++j)
			{
				_uint8 cbCardData = stAnalyseItem.WeaveItemArray[i].cbCardList[j];
				if (cbCardData == INVALID_CARD)
				{
					cbTotalCardCount--;
					continue;
				}
				if (CardNum(cbCardData) == 2 || CardNum(cbCardData) == 7 || CardNum(cbCardData) == 10)
				{
					cbHongZiCount++;
				}
				else cbHeiZiCount++;
			}
		}
		//牌眼统计
		if (stAnalyseItem.cbCardEye != 0 && stAnalyseItem.cbCardEye != INVALID_CARD)
		{
			cbTotalCardCount += 2;
			_uint8 cbCardData = stAnalyseItem.cbCardEye;
			if (CardNum(cbCardData) == 0x02 || CardNum(cbCardData) == 0x07 || CardNum(cbCardData) == 0x0a)
			{
				cbHongZiCount += 2;
			}
			else cbHeiZiCount += 2;
		}

		//判断类型
		// 大红
		if (cbHongZiCount >= 13)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_HONG_HU;
			HuFanInfo.wHuFanNum = 5;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
		// 小红
		if (10 <= cbHongZiCount &&  cbHongZiCount < 13)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_XIAO_HONG_HU;
			HuFanInfo.wHuFanNum = 2;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
		// 乌胡
		if (cbHeiZiCount == cbTotalCardCount)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_HEI_HU;
			HuFanInfo.wHuFanNum = 5;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
		// 一点红
		if (/*m_bBuDaiYiDianHong == false && */cbHongZiCount == 1)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_DIAN_HU;
			HuFanInfo.wHuFanNum = 5;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
	}
	cbFinalHuXiCount = wTotalHuXi;
	return wTotalHuXi*wTotalFanNum;
}
#if 0
_uint32 CGameLogic::GetHuFanType(const tagHuCardInfo &HuCardInfo, const tagAnalyseItem& stAnalyseItem,const _uint8 cbWeaveItemCount,const tagWeaveItem WeaveItemArray[MAX_WEAVE], _uint8 cbWeaveHuXi, std::vector<tagHuFanInfo> &vecHuFanInfo, bool bWithMaoHu,bool bHHD)
{
	_uint32 wTotalFanNum = 1;
	_uint8 cbTotalCardCount = 0;
	_uint8 cbHongZiCount = 0;
	_uint8 cbHeiZiCount = 0;
	_uint32 wTotalHuXi = stAnalyseItem.cbHuXiCount + cbWeaveHuXi + HuCardInfo.cbHuXiCount; //总胡息的引用
	vecHuFanInfo.clear();

	if (bWithMaoHu && (0 == wTotalHuXi))
	{
		tagHuFanInfo HuFanInfo;
		wTotalHuXi = 15;
		HuFanInfo.wHuFanType = MT_MAO_HU;
		HuFanInfo.wHuFanNum = wTotalHuXi;
		HuFanInfo.wHuFanXi = wTotalHuXi;
		vecHuFanInfo.push_back(HuFanInfo);
	}
	if (bHHD)
	{
		//红黑字统计
		for (_uint8 i = 0; i < HuCardInfo.cbWeaveCount; i++)
		{
			cbTotalCardCount += HuCardInfo.WeaveItemArray[i].cbCardCount;
			for (_uint8 j = 0; j < HuCardInfo.WeaveItemArray[i].cbCardCount; j++)
			{
				if (INVALID_CARD == HuCardInfo.WeaveItemArray[i].cbCardList[j])
				{
					cbTotalCardCount--;
					continue;
				}
				if (CardNum(HuCardInfo.WeaveItemArray[i].cbCardList[j]) == 2 || CardNum(HuCardInfo.WeaveItemArray[i].cbCardList[j]) == 7 || CardNum(HuCardInfo.WeaveItemArray[i].cbCardList[j]) == 10)
				{
					cbHongZiCount++;
				}
				else
				{
					cbHeiZiCount++;
				}
			}
		}
		for (_uint8 j = 0; j < cbWeaveItemCount; ++j)
		{
			cbTotalCardCount += WeaveItemArray[j].cbCardCount;
			for (_uint8 i = 0; i < WeaveItemArray[j].cbCardCount; i++)
			{
				if (INVALID_CARD == WeaveItemArray[j].cbCardList[i])
				{
					cbTotalCardCount--;
					continue;
				}
				if (CardNum(WeaveItemArray[j].cbCardList[i]) == 2 || CardNum(WeaveItemArray[j].cbCardList[i]) == 7 || CardNum(WeaveItemArray[j].cbCardList[i]) == 10)
				{
					cbHongZiCount++;
				}
				else
				{
					cbHeiZiCount++;
				}
			}
		}
		for (_uint8 i = 0; i < stAnalyseItem.cbWeaveCount; ++i)
		{
			cbTotalCardCount += stAnalyseItem.WeaveItemArray[i].cbCardCount;
			for (_uint8 j = 0; j < stAnalyseItem.WeaveItemArray[i].cbCardCount && j < 4; ++j)
			{
				_uint8 cbCardData = stAnalyseItem.WeaveItemArray[i].cbCardList[j];
				if (cbCardData == INVALID_CARD)
				{
					cbTotalCardCount--;
					continue;
				}
				if (CardNum(cbCardData) == 2 || CardNum(cbCardData) == 7 || CardNum(cbCardData) == 10)
				{
					cbHongZiCount++;
				}
				else cbHeiZiCount++;
			}
		}
		//牌眼统计
		if (stAnalyseItem.cbCardEye != 0 && stAnalyseItem.cbCardEye != INVALID_CARD)
		{
			cbTotalCardCount += 2;
			_uint8 cbCardData = stAnalyseItem.cbCardEye;
			if (CardNum(cbCardData) == 0x02 || CardNum(cbCardData) == 0x07 || CardNum(cbCardData) == 0x0a)
			{
				cbHongZiCount += 2;
			}
			else cbHeiZiCount += 2;
		}
		
		//判断类型
		// 大红
		if (cbHongZiCount >= 13)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_HONG_HU;
			HuFanInfo.wHuFanNum = 5;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
		// 小红
		if (10 <= cbHongZiCount &&  cbHongZiCount < 13)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_XIAO_HONG_HU;
			HuFanInfo.wHuFanNum = 2;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
		// 乌胡
		if (cbHeiZiCount == cbTotalCardCount)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_HEI_HU;
			HuFanInfo.wHuFanNum = 5;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
		if (cbHongZiCount == 1)
		{
			tagHuFanInfo HuFanInfo;
			HuFanInfo.wHuFanType = MT_DIAN_HU;
			HuFanInfo.wHuFanNum = 5;
			HuFanInfo.wHuFanXi = 0;
			wTotalFanNum *= HuFanInfo.wHuFanNum;
			vecHuFanInfo.push_back(HuFanInfo);
		}
	}
	return wTotalHuXi*wTotalFanNum;

}
#endif
//胡牌结果
bool CGameLogic::GetHuCardInfoFinal(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbWeaveItemCount, tagWeaveItem WeaveItemArray[MAX_WEAVE], tagHuCardInfo &HuCardInfo, _uint32 wBaseHuXi,std::vector<tagHuFanInfo>& vecHuFanInfo, bool bZimo, bool bDispatch, bool bHHD, bool bHu, bool isWithMaoHu)
{
	//组合总数
	static CAnalyseItemArray AnalyseItemArray;
	AnalyseItemArray.clear();
	CAnalyseItemArray HuItemArray;
	HuItemArray.clear();
	//构造扑克
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	_uint8 cbWeaveItemCountTemp = cbWeaveItemCount;
	tagWeaveItem WeaveItemArrayTemp[MAX_WEAVE];
	memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
	//是否是提跑牌
	if (cbCurrentCard != INVALID_CARD && cbCurrentCard != 0 && IsTiPaoCard(cbCardIndexTemp, cbCurrentCard) == true)
	{
		//组合总数
		std::vector<tagWeaveItem> vecWeaveItems;
		vecWeaveItems.clear();
		//构造跑牌
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)] = 0;
		tagWeaveItem stTempItem;
		stTempItem.cbCardCount = 4;
		stTempItem.cbWeaveKind = ACK_PAO;
		stTempItem.cbCenterCard = cbCurrentCard;
		for (_uint32 c = 0; c < 4; ++c) stTempItem.cbCardList[c] = cbCurrentCard;
		vecWeaveItems.push_back(stTempItem);
		//提取三牌
		for (_uint32 i = 0; i<MAX_INDEX; i++)
		{
			if (cbCardIndexTemp[i] == 3)
			{
				cbCardIndexTemp[i] = 0;
				_uint8 cbCardData = SwitchToCardData(i);
				tagWeaveItem stTempItem;
				stTempItem.cbCardCount = 3;
				stTempItem.cbWeaveKind = ACK_WEI;
				stTempItem.cbCenterCard = cbCardData;
				for (_uint32 c = 0; c < 3; ++c) stTempItem.cbCardList[c] = cbCardData;
				vecWeaveItems.push_back(stTempItem);
			}
		}
		//分析扑克
		AnalyseItemArray.clear();
		if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
		{
			if (AnalyseItemArray.size() == 0)
			{
				tagAnalyseItem stAnalyseItem;
				for (_uint32 i = 0; i < vecWeaveItems.size(); ++i)
				{
					stAnalyseItem.cbHuXiCount += GetWeaveHuXi(vecWeaveItems[i]);
					stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = vecWeaveItems[i];
				}
				for (_uint32 i = 0; i < cbWeaveItemCountTemp; ++i)
				{
					stAnalyseItem.cbHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
					stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = WeaveItemArrayTemp[i];
				}
				AnalyseItemArray.push_back(stAnalyseItem);
			}
			else
			{
				//加入组合
				for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
				{
					tagAnalyseItem &stAnalyseItem = AnalyseItemArray[i];
					for (_uint32 j = 0; j < vecWeaveItems.size(); ++j)
					{
						tagWeaveItem &stTempItem = vecWeaveItems[j];
						stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
						stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
					}
					for (_uint32 j = 0; j < cbWeaveItemCountTemp; ++j)
					{
						tagWeaveItem &stTempItem = WeaveItemArrayTemp[j];
						stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
						stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
					}
				}
			}
			//筛选出满足胡息的组合
			for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
			{
				if (AnalyseItemArray[i].cbHuXiCount >= wBaseHuXi ||(isWithMaoHu && AnalyseItemArray[i].cbHuXiCount == 0))
				{
					HuItemArray.push_back(AnalyseItemArray[i]);
				}
			}
		}
	}
	//跑牌转换
	if (cbCurrentCard != INVALID_CARD && cbCurrentCard != 0)
	{
		//构造扑克
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
		cbWeaveItemCountTemp = cbWeaveItemCount;
		memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
		_uint32 j = 0;
		for (; j < cbWeaveItemCountTemp; ++j)
		{
			if ((WeaveItemArrayTemp[j].cbWeaveKind == ACK_WEI && !bZimo && WeaveItemArrayTemp[j].cbCardList[0] == cbCurrentCard)
				|| (WeaveItemArrayTemp[j].cbWeaveKind == ACK_PENG && bDispatch && WeaveItemArrayTemp[j].cbCardList[0] == cbCurrentCard))
				break;
		}
		if (j != cbWeaveItemCountTemp)
		{
			WeaveItemArrayTemp[j].cbWeaveKind = ACK_PAO;
			WeaveItemArrayTemp[j].cbCardCount = 4;
			WeaveItemArrayTemp[j].cbCardList[3] = cbCurrentCard;
			//组合总数
			std::vector<tagWeaveItem> vecWeaveItems;
			vecWeaveItems.clear();
			//提取三牌
			for (_uint32 i = 0; i<MAX_INDEX; i++)
			{
				if (cbCardIndexTemp[i] == 3)
				{
					cbCardIndexTemp[i] = 0;
					_uint8 cbCardData = SwitchToCardData(i);
					tagWeaveItem stTempItem;
					stTempItem.cbCardCount = 3;
					stTempItem.cbWeaveKind = ACK_WEI;
					stTempItem.cbCenterCard = cbCardData;
					for (_uint32 c = 0; c < 3; ++c) stTempItem.cbCardList[c] = cbCardData;
					vecWeaveItems.push_back(stTempItem);
				}
			}
			//分析扑克
			AnalyseItemArray.clear();
			if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
			{
				if (AnalyseItemArray.size() == 0)
				{
					tagAnalyseItem stAnalyseItem;
					for (_uint32 i = 0; i < vecWeaveItems.size(); ++i)
					{
						stAnalyseItem.cbHuXiCount += GetWeaveHuXi(vecWeaveItems[i]);
						stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = vecWeaveItems[i];
					}
					for (_uint32 i = 0; i < cbWeaveItemCountTemp; ++i)
					{
						stAnalyseItem.cbHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
						stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = WeaveItemArrayTemp[i];
					}
					AnalyseItemArray.push_back(stAnalyseItem);
				}
				else
				{
					//加入组合
					for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
					{
						tagAnalyseItem &stAnalyseItem = AnalyseItemArray[i];
						for (_uint32 j = 0; j < vecWeaveItems.size(); ++j)
						{
							tagWeaveItem &stTempItem = vecWeaveItems[j];
							stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
							stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
						}
						for (_uint32 j = 0; j < cbWeaveItemCountTemp; ++j)
						{
							tagWeaveItem &stTempItem = WeaveItemArrayTemp[j];
							stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
							stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
						}
					}
				}
				//筛选出满足胡息的组合
				for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
				{
					if (AnalyseItemArray[i].cbHuXiCount >= wBaseHuXi || (isWithMaoHu && AnalyseItemArray[i].cbHuXiCount == 0))
					{
						HuItemArray.push_back(AnalyseItemArray[i]);
					}
				}
			}
		}
	}
	//是偎碰牌
	if (cbCurrentCard != INVALID_CARD && cbCurrentCard != 0)
	{
		//构造扑克
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
		cbWeaveItemCountTemp = cbWeaveItemCount;
		memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
		//组合总数
		std::vector<tagWeaveItem> vecWeaveItems;
		vecWeaveItems.clear();
		//是偎碰牌
		if (cbCurrentCard != INVALID_CARD && cbCurrentCard != 0 && IsWeiPengCard(cbCardIndexTemp, cbCurrentCard) == true)
		{
			cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
			cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)] = 0;
			tagWeaveItem stTempItem;
			stTempItem.cbCardCount = 3;
			stTempItem.cbWeaveKind = (bZimo == true) ? ACK_WEI : ACK_PENG;
			stTempItem.cbCenterCard = cbCurrentCard;
			for (_uint32 c = 0; c < 3; ++c) stTempItem.cbCardList[c] = cbCurrentCard;
			vecWeaveItems.push_back(stTempItem);
			//提取三牌
			for (_uint32 i = 0; i<MAX_INDEX; i++)
			{
				if (cbCardIndexTemp[i] == 3)
				{
					cbCardIndexTemp[i] = 0;
					_uint8 cbCardData = SwitchToCardData(i);
					tagWeaveItem stTempItem;
					stTempItem.cbCardCount = 3;
					stTempItem.cbWeaveKind = ACK_WEI;
					stTempItem.cbCenterCard = cbCardData;
					for (_uint32 c = 0; c < 3; ++c) stTempItem.cbCardList[c] = cbCardData;
					vecWeaveItems.push_back(stTempItem);
				}
			}
			//分析扑克
			AnalyseItemArray.clear();
			if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
			{
				if (AnalyseItemArray.size() == 0)
				{
					tagAnalyseItem stAnalyseItem;
					for (_uint32 i = 0; i < vecWeaveItems.size(); ++i)
					{
						stAnalyseItem.cbHuXiCount += GetWeaveHuXi(vecWeaveItems[i]);
						stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = vecWeaveItems[i];
					}
					for (_uint32 i = 0; i < cbWeaveItemCountTemp; ++i)
					{
						stAnalyseItem.cbHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
						stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = WeaveItemArrayTemp[i];
					}
					AnalyseItemArray.push_back(stAnalyseItem);
				}
				else
				{
					//加入组合
					for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
					{
						tagAnalyseItem &stAnalyseItem = AnalyseItemArray[i];
						for (_uint32 j = 0; j < vecWeaveItems.size(); ++j)
						{
							tagWeaveItem &stTempItem = vecWeaveItems[j];
							stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
							stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
						}
						for (_uint32 j = 0; j < cbWeaveItemCountTemp; ++j)
						{
							tagWeaveItem &stTempItem = WeaveItemArrayTemp[j];
							stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
							stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
						}
					}
				}
				//筛选出满足胡息的组合
				for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
				{
					if (AnalyseItemArray[i].cbHuXiCount >= wBaseHuXi || (isWithMaoHu && AnalyseItemArray[i].cbHuXiCount == 0))
					{
						HuItemArray.push_back(AnalyseItemArray[i]);
					}
				}
			}
		}
	}
	//构造扑克
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	cbWeaveItemCountTemp = cbWeaveItemCount;
	memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
	//组合总数
	std::vector<tagWeaveItem> vecWeaveItems;
	vecWeaveItems.clear();
	//提取三牌
	for (_uint32 i = 0; i<MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 3)
		{
			cbCardIndexTemp[i] = 0;
			_uint8 cbCardData = SwitchToCardData(i);
			tagWeaveItem stTempItem;
			stTempItem.cbCardCount = 3;
			stTempItem.cbWeaveKind = ACK_WEI;
			stTempItem.cbCenterCard = cbCardData;
			for (_uint32 c = 0; c < 3; ++c) stTempItem.cbCardList[c] = cbCardData;
			vecWeaveItems.push_back(stTempItem);
		}
	}
	//加入手牌
	if (cbCurrentCard != INVALID_CARD && cbCurrentCard != 0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	//分析扑克
	AnalyseItemArray.clear();
	if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
	{
		if (AnalyseItemArray.size() == 0)
		{
			tagAnalyseItem stAnalyseItem;
			for (_uint32 i = 0; i < vecWeaveItems.size(); ++i)
			{
				stAnalyseItem.cbHuXiCount += GetWeaveHuXi(vecWeaveItems[i]);
				stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = vecWeaveItems[i];
			}
			for (_uint32 i = 0; i < cbWeaveItemCountTemp; ++i)
			{
				stAnalyseItem.cbHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
				stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = WeaveItemArrayTemp[i];
			}
			AnalyseItemArray.push_back(stAnalyseItem);
		}
		else
		{
			//加入组合
			for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
			{
				tagAnalyseItem &stAnalyseItem = AnalyseItemArray[i];
				for (_uint32 j = 0; j < vecWeaveItems.size(); ++j)
				{
					tagWeaveItem &stTempItem = vecWeaveItems[j];
					stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
					stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
				}
				for (_uint32 j = 0; j < cbWeaveItemCountTemp; ++j)
				{
					tagWeaveItem &stTempItem = WeaveItemArrayTemp[j];
					stAnalyseItem.cbHuXiCount += GetWeaveHuXi(stTempItem);
					stAnalyseItem.WeaveItemArray[stAnalyseItem.cbWeaveCount++] = stTempItem;
				}
			}
		}
		//筛选出满足胡息的组合
		for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i)
		{
			if (AnalyseItemArray[i].cbHuXiCount >= wBaseHuXi || (isWithMaoHu && AnalyseItemArray[i].cbHuXiCount == 0)) //6胡息起胡
			{
				HuItemArray.push_back(AnalyseItemArray[i]);
			}
		}
	}

	if (HuItemArray.size() > 0)
	{
		//寻找最优
		_tint32 nBestItem = -1;
		_uint32 wBestHuXi = 0;
		_uint8 cbFinalHuXiCount = 0;
		for (_uint32 i = 0; i < HuItemArray.size(); ++i)
		{
			std::vector<tagHuFanInfo> vecHuFanInfoTemp;
			_uint8 cbFinalHuXiTemp = 0;
			_uint32 wTempHuXi = GetHuFanType(HuItemArray[i], vecHuFanInfoTemp, cbFinalHuXiTemp, isWithMaoHu, bHHD);
			if (wTempHuXi > wBestHuXi)
			{
				wBestHuXi = wTempHuXi;
				nBestItem = i;
				cbFinalHuXiCount = cbFinalHuXiTemp;
				vecHuFanInfo.clear();
				vecHuFanInfo = vecHuFanInfoTemp;
			}
		}
		if (nBestItem >= 0)
		{
			//获取子项
			tagAnalyseItem * pAnalyseItem = &HuItemArray[nBestItem];
			//设置变量
			HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;
			HuCardInfo.cbHuXiCount = pAnalyseItem->cbHuXiCount;
			HuCardInfo.cbFinalHuXiCount = cbFinalHuXiCount;
			//设置组合
			for (_uint32 i = 0; i<pAnalyseItem->cbWeaveCount; i++)
			{
				_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
				HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
			}
		}
	}

	return HuItemArray.size() > 0;

#if 0
	//变量定义
	static CAnalyseItemArray AnalyseItemArray;
	//构造扑克
	memset(&HuCardInfo, 0, sizeof(HuCardInfo));
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	_uint8 cbWeaveItemCountTemp = cbWeaveItemCount;
	tagWeaveItem WeaveItemArrayTemp[MAX_WEAVE];
	memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
	//是否是提跑牌
	if (cbCurrentCard != INVALID_CARD && IsTiPaoCard(cbCardIndexTemp, cbCurrentCard) == true)
	{
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		//构造组合
		_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 4;
		HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_PAO;
		HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[3] = cbCurrentCard;
		//删除扑克
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)] = 0;
		//设置胡息
		HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		//提取三牌
		for (_uint8 i = 0; i < MAX_INDEX; i++)
		{
			if (cbCardIndexTemp[i] == 3)
			{
				//设置扑克
				cbCardIndexTemp[i] = 0;
				//设置组合
				_uint8 cbCardData = SwitchToCardData(i);
				_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
				HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_WEI;
				HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
				//设置胡息
				HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
			}
		}
		//分析扑克
		AnalyseItemArray.clear();
		if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
		{
			//寻找最优
			_uint8 cbHuXiCard = 0;
			int nBestItem = -1;
			_uint8 tmpIndexHuxi = 0;//手牌分析后的胡息
			for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
			{
				/*
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];
				//胡息分析
				if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = pAnalyseItem->cbHuXiCount;
				}
				*/

				tagAnalyseItem& pAnalyseItem = AnalyseItemArray[i];
				std::vector<tagHuFanInfo> vecTempHuFanInfo;
				_uint8 tempHuxi = GetHuFanType(HuCardInfo, pAnalyseItem, cbWeaveItemCount, WeaveItemArray, 0, vecTempHuFanInfo, isWithMaoHu, bHHD);
				//胡息分析
				if (tempHuxi > cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = tempHuxi;
					tmpIndexHuxi = pAnalyseItem.cbHuXiCount;
					vecHuFanInfo.clear();
					vecHuFanInfo = vecTempHuFanInfo;
				}
			}
			HuCardInfo.cbHuXiCount += tmpIndexHuxi;
			//设置结果
			if (nBestItem >= 0)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];
				//设置变量
				HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;
				//设置组合
				for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
				{
					_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
				}
			}
			//胡息计算
			_uint8 cbUserHuXiCount = 0;
			for (_uint8 i = 0; i < cbWeaveItemCountTemp; i++)
			{
				cbUserHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
			}
			if ((HuCardInfo.cbHuXiCount + cbUserHuXiCount) >= wBaseHuXi) return true;
		}
	}
	//构造扑克
	memset(&HuCardInfo, 0, sizeof(HuCardInfo));
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	cbWeaveItemCountTemp = cbWeaveItemCount;
	memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
	//跑牌转换
	_uint8 j = 0;
	for (; j < cbWeaveItemCountTemp; ++j)
	{
		if ((WeaveItemArrayTemp[j].cbWeaveKind == ACK_WEI && !bZimo && WeaveItemArrayTemp[j].cbCardList[0] == cbCurrentCard)
			|| (WeaveItemArrayTemp[j].cbWeaveKind == ACK_PENG && bDispatch && WeaveItemArrayTemp[j].cbCardList[0] == cbCurrentCard)) break;
	}
	if (j != cbWeaveItemCountTemp)
	{
		WeaveItemArrayTemp[j].cbWeaveKind = ACK_PAO;
		WeaveItemArrayTemp[j].cbCardCount = 4;
		WeaveItemArrayTemp[j].cbCardList[3] = cbCurrentCard;
		//提取三牌
		for (_uint8 i = 0; i < MAX_INDEX; i++)
		{
			if (cbCardIndexTemp[i] == 3)
			{
				//设置扑克
				cbCardIndexTemp[i] = 0;
				//设置组合
				_uint8 cbCardData = SwitchToCardData(i);
				_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
				HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_WEI;
				HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
				//设置胡息
				HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
			}
		}
		//分析扑克
		AnalyseItemArray.clear();
		if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
		{
			//寻找最优
			_uint8 cbHuXiCard = 0;
			int nBestItem = -1;
			_uint8 tmpIndexHuxi = 0;//手牌分析后的胡息
			for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
			{/*
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];
				//胡息分析
				if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = pAnalyseItem->cbHuXiCount;
				}*/
				tagAnalyseItem& pAnalyseItem = AnalyseItemArray[i];
				std::vector<tagHuFanInfo> vecTempHuFanInfo;
				_uint8 tempHuxi = GetHuFanType(HuCardInfo, pAnalyseItem, cbWeaveItemCount, WeaveItemArray, 0, vecTempHuFanInfo, isWithMaoHu, bHHD);
				//胡息分析
				if (tempHuxi > cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = tempHuxi;
					tmpIndexHuxi = pAnalyseItem.cbHuXiCount;
					vecHuFanInfo.clear();
					vecHuFanInfo = vecTempHuFanInfo;
				}
			}
			HuCardInfo.cbHuXiCount += tmpIndexHuxi;
			//设置结果
			if (nBestItem >= 0)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];
				//设置变量
				HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;
				//设置组合
				for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
				{
					_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
				}
			}
			//胡息计算
			_uint8 cbUserHuXiCount = 0;
			for (_uint8 i = 0; i < cbWeaveItemCountTemp; i++)
			{
				cbUserHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
			}
			if ((HuCardInfo.cbHuXiCount + cbUserHuXiCount) >= wBaseHuXi)
			{
				if (bHu)
				{
					for (_uint8 i = 0; i < cbWeaveItemCount; ++i)
					{
						if ((WeaveItemArray[i].cbWeaveKind == ACK_WEI && !bZimo && WeaveItemArray[i].cbCardList[0] == cbCurrentCard)
							|| (WeaveItemArray[i].cbWeaveKind == ACK_PENG && bDispatch && WeaveItemArray[i].cbCardList[0] == cbCurrentCard))
						{
							WeaveItemArray[i].cbWeaveKind = ACK_PAO;
							WeaveItemArray[i].cbCardCount = 4;
							WeaveItemArray[i].cbCardList[3] = cbCurrentCard;
							break;
						}
					}
				}
				return true;
			}
		}
	}
	//构造扑克
	memset(&HuCardInfo, 0, sizeof(HuCardInfo));
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	cbWeaveItemCountTemp = cbWeaveItemCount;
	memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
	//提取碰牌
	if ((cbCurrentCard != INVALID_CARD) && (IsWeiPengCard(cbCardIndexTemp, cbCurrentCard) == true))
	{
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		//构造组合
		_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
		HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = (bZimo == true) ? ACK_WEI : ACK_PENG;
		HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCurrentCard;
		//删除扑克
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)] = 0;
		//设置胡息
		HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		//提取三牌
		for (_uint8 i = 0; i < MAX_INDEX; i++)
		{
			if (cbCardIndexTemp[i] == 3)
			{
				//设置扑克
				cbCardIndexTemp[i] = 0;
				//设置组合
				_uint8 cbCardData = SwitchToCardData(i);
				_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
				HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_WEI;
				HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
				HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
				//设置胡息
				HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
			}
		}
		//判断胡牌	
		AnalyseItemArray.clear();
		if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
		{
			//寻找最优
			_uint8 cbHuXiCard = 0;
			int nBestItem = -1; 
			_uint8 tmpIndexHuxi = 0;//手牌分析后的胡息
			for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
			{
				/*
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];

				//胡息分析
				if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = pAnalyseItem->cbHuXiCount;
				}*/

				tagAnalyseItem& pAnalyseItem = AnalyseItemArray[i];
				std::vector<tagHuFanInfo> vecTempHuFanInfo;
				_uint8 tempHuxi = GetHuFanType(HuCardInfo, pAnalyseItem, cbWeaveItemCount, WeaveItemArray, 0, vecTempHuFanInfo, isWithMaoHu, bHHD);
				//胡息分析
				if (tempHuxi > cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = tempHuxi;
					tmpIndexHuxi = pAnalyseItem.cbHuXiCount;
					vecHuFanInfo.clear();
					vecHuFanInfo = vecTempHuFanInfo;
				}

			}
			HuCardInfo.cbHuXiCount += tmpIndexHuxi;
			//设置结果
			if (nBestItem >= 0)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];
				//设置变量
				HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;
				//设置组合
				for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
				{
					_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
				}
			}
			//胡息计算
			_uint8 cbUserHuXiCount = 0;
			for (_uint8 i = 0; i < cbWeaveItemCountTemp; i++)
			{
				cbUserHuXiCount += GetWeaveHuXi(WeaveItemArrayTemp[i]);
			}
			if ((HuCardInfo.cbHuXiCount + cbUserHuXiCount) >= wBaseHuXi) return true;
		}
	}

	//构造扑克
	memset(&HuCardInfo, 0, sizeof(HuCardInfo));
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	cbWeaveItemCountTemp = cbWeaveItemCount;
	memcpy(WeaveItemArrayTemp, WeaveItemArray, sizeof(WeaveItemArrayTemp));
	//不是碰跑牌
	_uint8 cbHuXiWeave = 0;
	for (_uint8 i = 0; i < cbWeaveItemCount; i++)
	{
		cbHuXiWeave += GetWeaveHuXi(WeaveItemArray[i]);
	}
	//提取三牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 3)
		{
			//设置扑克
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_WEI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
			//设置胡息
			HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
	}
	if (cbCurrentCard != INVALID_CARD) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	//分析扑克
	AnalyseItemArray.clear();
	if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == false) return false;
	//寻找最优
	_uint8 cbHuXiCard = 0;
	int nBestItem = -1;
	_uint8 tmpIndexHuxi = 0;//手牌分析后的胡息
	for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
	{
		//获取子项
		tagAnalyseItem& pAnalyseItem = AnalyseItemArray[i];
		std::vector<tagHuFanInfo> vecTempHuFanInfo;

		_uint8 tempHuxi = GetHuFanType(HuCardInfo, pAnalyseItem, cbWeaveItemCount, WeaveItemArray, cbHuXiWeave, vecTempHuFanInfo, isWithMaoHu, bHHD);
		//胡息分析
		if (tempHuxi > cbHuXiCard)
		{
			nBestItem = i;
			cbHuXiCard = tempHuxi;
			tmpIndexHuxi = pAnalyseItem.cbHuXiCount;
			vecHuFanInfo.clear(); 
			vecHuFanInfo = vecTempHuFanInfo;
		}
	}
	HuCardInfo.cbHuXiCount += tmpIndexHuxi;
	//设置结果
	if (nBestItem >= 0)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];
		//设置变量
		HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;
		//设置组合
		for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
		{
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
		}
	}

	if (((HuCardInfo.cbHuXiCount + cbHuXiWeave) >= wBaseHuXi) || (isWithMaoHu && (HuCardInfo.cbHuXiCount + cbHuXiWeave) == 0))
		return true;
	return false;
#endif
}

//胡牌结果
bool CGameLogic::GetHuCardInfo(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbHuXiWeave, tagHuCardInfo & HuCardInfo, _uint32 wBaseHuXi, bool bZimo, bool bWithMaoHu)
{
	//变量定义
	static CAnalyseItemArray AnalyseItemArray;

	//构造扑克
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	//设置结果
	memset(&HuCardInfo, 0, sizeof(HuCardInfo));
	//提取三牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 3)
		{
			//设置扑克
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_WEI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
			//设置胡息
			HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
		else if (cbCardIndexTemp[i] == 4)
		{
			//设置扑克
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_TI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[3] = cbCardData;
			//设置胡息
			HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
	}
	//提取碰牌
	if ((cbCurrentCard != INVALID_CARD) && (IsWeiPengCard(cbCardIndexTemp, cbCurrentCard) == true))
	{
		//判断胡牌	
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		AnalyseItemArray.clear();
		tagHuCardInfo tempHuCardInfo = HuCardInfo;
		if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == true)
		{
			//寻找最优
			_uint8 cbHuXiCard = 0;
			int nBestItem = -1;
			for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];

				//胡息分析
				if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = pAnalyseItem->cbHuXiCount;
				}
			}
			HuCardInfo.cbHuXiCount += cbHuXiCard;
			//设置结果
			if (nBestItem >= 0)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];

				//设置变量
				HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;

				//设置组合
				for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
				{
					_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
				}
			}
			if ((HuCardInfo.cbHuXiCount + cbHuXiWeave) >= wBaseHuXi)
				return true;
		}
		//构造组合
		HuCardInfo = tempHuCardInfo;
		_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
		HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = (bZimo == true) ? ACK_WEI : ACK_PENG;
		HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCurrentCard;
		//删除扑克
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)] = 0;
		//设置胡息
		HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
	}
	else if (cbCurrentCard != INVALID_CARD) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//分析扑克
	AnalyseItemArray.clear();
	if (AnalyseCard(cbCardIndexTemp, AnalyseItemArray) == false) return false;

	//寻找最优
	_uint8 cbHuXiCard = 0;
	int nBestItem = -1;
	for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];

		//胡息分析
		if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
		{
			nBestItem = i;
			cbHuXiCard = pAnalyseItem->cbHuXiCount;
		}
	}
	HuCardInfo.cbHuXiCount += cbHuXiCard;

	//设置结果
	if (nBestItem >= 0)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];

		//设置变量
		HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;

		//设置组合
		for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
		{
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
		}
	}

	if (((HuCardInfo.cbHuXiCount + cbHuXiWeave) >= wBaseHuXi) || (bWithMaoHu && (HuCardInfo.cbHuXiCount + cbHuXiWeave) == 0))
		return true;
	return false;
}


//胡牌结果
bool CGameLogic::GetHuCardInfoOld(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbHuXiWeave, tagHuCardInfo & HuCardInfo, _uint32 wBaseHuXi, bool bZimo, bool bWithMaoHu)
{
	//变量定义
	static CAnalyseItemArray AnalyseItemArray;

	//构造扑克
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
	//设置结果
	memset(&HuCardInfo, 0, sizeof(HuCardInfo));
	//提取三牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 3)
		{
			//设置扑克
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_WEI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
			//设置胡息
			HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
		else if (cbCardIndexTemp[i] == 4)
		{
			//设置扑克
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 4;
			HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = ACK_TI;
			HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCardData;
			HuCardInfo.WeaveItemArray[cbIndex].cbCardList[3] = cbCardData;
			//设置胡息
			HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
		}
	}
	//提取碰牌
	if ((cbCurrentCard != INVALID_CARD) && (IsWeiPengCard(cbCardIndexTemp, cbCurrentCard) == true))
	{
		//判断胡牌	
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
		AnalyseItemArray.clear();
		tagHuCardInfo tempHuCardInfo = HuCardInfo;
		if (AnalyseCardOld(cbCardIndexTemp, AnalyseItemArray) == true)
		{
			//寻找最优
			_uint8 cbHuXiCard = 0;
			int nBestItem = -1;
			for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];

				//胡息分析
				if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
				{
					nBestItem = i;
					cbHuXiCard = pAnalyseItem->cbHuXiCount;
				}
			}
			HuCardInfo.cbHuXiCount += cbHuXiCard;
			//设置结果
			if (nBestItem >= 0)
			{
				//获取子项
				tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];

				//设置变量
				HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;

				//设置组合
				for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
				{
					_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
					HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
				}
			}
			if ((HuCardInfo.cbHuXiCount + cbHuXiWeave) >= wBaseHuXi)
				return true;
		}
		//构造组合
		HuCardInfo = tempHuCardInfo;
		_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardCount = 3;
		HuCardInfo.WeaveItemArray[cbIndex].cbWeaveKind = (bZimo == true) ? ACK_WEI : ACK_PENG;
		HuCardInfo.WeaveItemArray[cbIndex].cbCenterCard = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[0] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[1] = cbCurrentCard;
		HuCardInfo.WeaveItemArray[cbIndex].cbCardList[2] = cbCurrentCard;
		//删除扑克
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)] = 0;
		//设置胡息
		HuCardInfo.cbHuXiCount += GetWeaveHuXi(HuCardInfo.WeaveItemArray[cbIndex]);
	}
	else if (cbCurrentCard != INVALID_CARD) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//分析扑克
	AnalyseItemArray.clear();
	if (AnalyseCardOld(cbCardIndexTemp, AnalyseItemArray) == false) return false;

	//寻找最优
	_uint8 cbHuXiCard = 0;
	int nBestItem = -1;
	for (_uint32 i = 0; i < AnalyseItemArray.size(); i++)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[i];

		//胡息分析
		if (pAnalyseItem->cbHuXiCount >= cbHuXiCard)
		{
			nBestItem = i;
			cbHuXiCard = pAnalyseItem->cbHuXiCount;
		}
	}
	HuCardInfo.cbHuXiCount += cbHuXiCard;

	//设置结果
	if (nBestItem >= 0)
	{
		//获取子项
		tagAnalyseItem * pAnalyseItem = &AnalyseItemArray[nBestItem];

		//设置变量
		HuCardInfo.cbCardEye = pAnalyseItem->cbCardEye;

		//设置组合
		for (_uint32 i = 0; i < pAnalyseItem->cbWeaveCount; i++)
		{
			_uint8 cbIndex = HuCardInfo.cbWeaveCount++;
			HuCardInfo.WeaveItemArray[cbIndex] = pAnalyseItem->WeaveItemArray[i];
		}
	}

	if (((HuCardInfo.cbHuXiCount + cbHuXiWeave) >= wBaseHuXi) || (bWithMaoHu && (HuCardInfo.cbHuXiCount + cbHuXiWeave) == 0))
		return true;
	return false;
}

//有效判断
bool CGameLogic::IsValidCard(_uint8 cbCardData)
{
	_uint8 cbValue = (cbCardData&MASK_VALUE);
	_uint8 cbColor = (cbCardData&MASK_COLOR) >> 4;
	return ((cbValue >= 1) && (cbValue <= 10) && (cbColor <= 1));
}

//扑克数目
_uint8 CGameLogic::GetCardCount(_uint8 cbCardIndex[MAX_INDEX])
{
	//数目统计
	_uint8 cbCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; i++) cbCount += cbCardIndex[i];

	return cbCount;
}

//获取胡息
_uint8 CGameLogic::GetWeaveHuXi(const tagWeaveItem & WeaveItem)
{
	//计算胡息
	switch (WeaveItem.cbWeaveKind)
	{
	case ACK_TI:	//提
	{
		return ((WeaveItem.cbCardList[0] & MASK_COLOR) == 0x10) ? 12 : 9;
	}
	case ACK_PAO:	//跑
	{
		return ((WeaveItem.cbCardList[0] & MASK_COLOR) == 0x10) ? 9 : 6;
	}
	case ACK_WEI:	//偎
	{
		return ((WeaveItem.cbCardList[0] & MASK_COLOR) == 0x10) ? 6 : 3;
	}
	case ACK_PENG:	//碰
	{
		return ((WeaveItem.cbCardList[0] & MASK_COLOR) == 0x10) ? 3 : 1;
	}
	case ACK_CHI:	//吃
	{
		//获取数值
		_uint8 cbValue[3];
		cbValue[0] = WeaveItem.cbCardList[0] & MASK_VALUE;
		cbValue[1] = WeaveItem.cbCardList[1] & MASK_VALUE;
		cbValue[2] = WeaveItem.cbCardList[2] & MASK_VALUE;

		// 三张牌按从小到大排序
		sort3n(cbValue);

		//一二三吃
		if ((cbValue[0]==1)&&(cbValue[1]==2)&&(cbValue[2]==3)) 
			return ((WeaveItem.cbCardList[0]&MASK_COLOR)==0x10)?6:3;

		//二七十吃
		if ((cbValue[0] == 2) && (cbValue[1] == 7) && (cbValue[2] == 10))
			return ((WeaveItem.cbCardList[0] & MASK_COLOR) == 0x10) ? 6 : 3;


		return 0;
	}
	}

	return 0;
}

//打印牌信息
void CGameLogic::DebugCards(_uint8 cbCardIndex[], _uint8 count)
{
	char s[300];
	int slen = 0;
	for (_uint8 i = 0; i < count; i++)
	{
		//slen += sprintf_s(s + slen, 300 - slen, "%d  ", cbCardIndex[i]);//windows才有
		slen += snprintf(s + slen, 300 - slen, "%.2X ", cbCardIndex[i]);
		if ((i + 1) % 10 == 0)
		{
			//slen += sprintf_s(s + slen, 300 - slen, "\n");
			slen += snprintf(s + slen, 300 - slen, "\n");
		}
	}
	printf("cards print. cards:\n%s\n", s);
}
#if 1
//分析扑克
bool CGameLogic::AnalyseCardOld(_uint8 cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray)
{
	//变量定义
	_uint8 cbWeaveItemCount = 0;
	tagWeaveItem WeaveItem[76];
	memset(WeaveItem, 0, sizeof(WeaveItem));

	//数目统计
	_uint8 cbCardCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; i++) cbCardCount += cbCardIndex[i];

	//数目判断
	if (cbCardCount == 0) return true;
	if ((cbCardCount % 3 != 0) && ((cbCardCount + 1) % 3 != 0)) return false;

	//需求计算
	_uint8 bLessWeavItem = cbCardCount / 3;
	bool bNeedCardEye = ((cbCardCount + 1) % 3 == 0);

	//单吊判断
	if ((bLessWeavItem == 0) && (bNeedCardEye == true))
	{
		//牌眼判断
		for (_uint8 i = 0; i < MAX_INDEX; i++)
		{
			if (cbCardIndex[i] == 2)
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				memset(&AnalyseItem, 0, sizeof(AnalyseItem));

				//设置结果
				AnalyseItem.cbHuXiCount = 0;
				AnalyseItem.cbWeaveCount = 0;
				AnalyseItem.cbCardEye = SwitchToCardData(i);

				//插入结果
				AnalyseItemArray.push_back(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//拆分处理
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		//分析过虑
		if (cbCardIndex[i] == 0) continue;

		//变量定义
		_uint8 cbCardData = SwitchToCardData(i);

		//大小搭吃
		//if ((cbCardIndex[i]==2)&&(cbCardIndex[(i+10)%MAX_INDEX]>=1))
		if ((cbCardIndex[i] >= 2) && (cbCardIndex[(i + 10) % MAX_INDEX] >= 1))
		{
			WeaveItem[cbWeaveItemCount].cbCardCount = 3;
			WeaveItem[cbWeaveItemCount].cbWeaveKind = ACK_CHI;
			WeaveItem[cbWeaveItemCount].cbCenterCard = cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[1] = cbCardData;
			WeaveItem[cbWeaveItemCount++].cbCardList[2] = (cbCardData + 16) % 32;
		}

		//大小搭吃
		//if ((cbCardIndex[i]>=1)&&(cbCardIndex[(i+10)%MAX_INDEX]==2))
		if ((cbCardIndex[i] >= 1) && (cbCardIndex[(i + 10) % MAX_INDEX] >= 2))
		{
			WeaveItem[cbWeaveItemCount].cbCardCount = 3;
			WeaveItem[cbWeaveItemCount].cbWeaveKind = ACK_CHI;
			WeaveItem[cbWeaveItemCount].cbCenterCard = cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveItemCount].cbCardList[1] = (cbCardData + 16) % 32;
			WeaveItem[cbWeaveItemCount++].cbCardList[2] = (cbCardData + 16) % 32;
		}

		//二七十吃
		if ((cbCardData&MASK_VALUE) == 0x02)
		{
			for (_uint8 j = 1; j <= cbCardIndex[i]; j++)
			{
				if ((cbCardIndex[i + 5] >= j) && (cbCardIndex[i + 8] >= j))
				{
					WeaveItem[cbWeaveItemCount].cbCardCount = 3;
					WeaveItem[cbWeaveItemCount].cbWeaveKind = ACK_CHI;
					WeaveItem[cbWeaveItemCount].cbCenterCard = cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[0] = cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[1] = cbCardData + 5;
					WeaveItem[cbWeaveItemCount++].cbCardList[2] = cbCardData + 8;
				}
			}
		}

		//顺子判断
		if ((i < (MAX_INDEX - 2)) && (cbCardIndex[i] > 0) && ((i % 10) <= 7))
		{
			for (_uint8 j = 1; j <= cbCardIndex[i]; j++)
			{
				if ((cbCardIndex[i + 1] >= j) && (cbCardIndex[i + 2] >= j))
				{
					WeaveItem[cbWeaveItemCount].cbCardCount = 3;
					WeaveItem[cbWeaveItemCount].cbWeaveKind = ACK_CHI;
					WeaveItem[cbWeaveItemCount].cbCenterCard = cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[0] = cbCardData;
					WeaveItem[cbWeaveItemCount].cbCardList[1] = cbCardData + 1;
					WeaveItem[cbWeaveItemCount++].cbCardList[2] = cbCardData + 2;
				}
			}
		}
	}

	//组合分析
	if (cbWeaveItemCount >= bLessWeavItem)
	{
		//变量定义
		_uint8 cbCardIndexTemp[MAX_INDEX];
		memset(cbCardIndexTemp, 0, sizeof(cbCardIndexTemp));

		//变量定义
		_uint8 cbIndex[7] = { 0,1,2,3,4,5,6 };
		// tagWeaveItem * pWeaveItem[CountArray(cbIndex)];
		tagWeaveItem * pWeaveItem[7];

		//开始组合
		do{
			//设置变量
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));
			for (_uint8 i = 0; i < bLessWeavItem; i++) pWeaveItem[i] = &WeaveItem[cbIndex[i]];
			//数量判断
			bool bEnoughCard = true;
			for (_uint8 i = 0; i < bLessWeavItem * 3; i++){
				//存在判断
				_uint8 cbIndex = SwitchToCardIndex(pWeaveItem[i / 3]->cbCardList[i % 3]);
				if (cbCardIndexTemp[cbIndex] == 0){
					bEnoughCard = false;
					break;
				}
				else cbCardIndexTemp[cbIndex]--;
			}
			//胡牌判断
			if (bEnoughCard == true){
				//牌眼判断
				_uint8 cbCardEye = 0;
				if (bNeedCardEye == true){
					for (_uint8 i = 0; i < MAX_INDEX; i++){
						if (cbCardIndexTemp[i] == 2){
							cbCardEye = SwitchToCardData(i);
							break;
						}
					}
				}
				//组合类型
				if ((bNeedCardEye == false) || (cbCardEye != 0)){
					//变量定义
					tagAnalyseItem AnalyseItem;
					memset(&AnalyseItem, 0, sizeof(AnalyseItem));
					//设置结果
					AnalyseItem.cbHuXiCount = 0;
					AnalyseItem.cbCardEye = cbCardEye;
					AnalyseItem.cbWeaveCount = bLessWeavItem;
					//设置组合
					for (_uint8 i = 0; i < bLessWeavItem; i++){
						AnalyseItem.WeaveItemArray[i] = *pWeaveItem[i];
						AnalyseItem.cbHuXiCount += GetWeaveHuXi(AnalyseItem.WeaveItemArray[i]);
					}
					//插入结果
					AnalyseItemArray.push_back(AnalyseItem);
				}
			}
			//设置索引
			if (cbIndex[bLessWeavItem - 1] == (cbWeaveItemCount - 1)){
				_uint8 i;
				for (i = bLessWeavItem - 1; i > 0; i--){
					if ((cbIndex[i - 1] + 1) != cbIndex[i]){
						_uint8 cbNewIndex = cbIndex[i - 1];
						for (_uint8 j = (i - 1); j < bLessWeavItem; j++) cbIndex[j] = cbNewIndex + j - i + 2;
						break;
					}
				}
				if (i == 0) break;
			}
			else cbIndex[bLessWeavItem - 1]++;
		} while (true);
		return (AnalyseItemArray.size() > 0);
	}
	return false;
}
#endif
//获取单牌
_uint8 CGameLogic::GetSingleCard(_uint8 cbCardIndex[MAX_INDEX])
{
	for (_uint8 i = 0; i < MAX_INDEX; ++i){
		if(cbCardIndex[i] == 1){
			return SwitchToCardData(i);
		}
	}
	return 0;
}
//获取单牌可以组成的组合
void CGameLogic::GetSingleGroup(_uint8 cbCardIndex[MAX_INDEX], _uint8 singleCard, CWeaveItemArray &WeaveItemGroup)
{
	WeaveItemGroup.clear();
	if(singleCard == 0)return;
	_uint8 cbIndex = SwitchToCardIndex(singleCard);//索引
	_uint8 cbCard = singleCard & MASK_VALUE;//点数
	_uint8 cbRIndex = (cbIndex + 10) % 20;
	_uint8 cbRCard = SwitchToCardData((cbIndex + 10) % 20);//搭吃牌
	tagWeaveItem WeaveItem;
	WeaveItem.cbCardCount = 3;
	WeaveItem.cbWeaveKind = ACK_CHI;
	WeaveItem.cbCenterCard = singleCard;
	WeaveItem.cbCardList[0] = singleCard;
	if(cbCardIndex[cbRIndex] >= 2){
		WeaveItem.cbCardList[1] = cbRCard;
		WeaveItem.cbCardList[2] = cbRCard;
		WeaveItemGroup.push_back(WeaveItem);
	}
	if(cbCard <= 0x08){
		if(cbCardIndex[cbIndex + 1] > 0 && cbCardIndex[cbIndex + 2] > 0){
			WeaveItem.cbCardList[1] = singleCard + 1;
			WeaveItem.cbCardList[2] = singleCard + 2;
			WeaveItemGroup.push_back(WeaveItem);
		}
	}
	if(cbCard >= 0x03){
		if(cbCardIndex[cbIndex - 1] > 0 && cbCardIndex[cbIndex - 2] > 0){
			WeaveItem.cbCardList[1] = singleCard - 1;
			WeaveItem.cbCardList[2] = singleCard - 2;
			WeaveItemGroup.push_back(WeaveItem);
		}
	}
	if(cbCard > 0x01 && cbCard < 0x0A){
		if(cbCardIndex[cbIndex - 1] > 0 && cbCardIndex[cbIndex + 1] > 0){
			WeaveItem.cbCardList[1] = singleCard - 1;
			WeaveItem.cbCardList[2] = singleCard + 1;
			WeaveItemGroup.push_back(WeaveItem);
		}
	}
	if(cbCard == 0x02 && cbCardIndex[cbIndex + 5] > 0 && cbCardIndex[cbIndex + 8] > 0){
		WeaveItem.cbCardList[1] = singleCard + 5;
		WeaveItem.cbCardList[2] = singleCard + 8;
		WeaveItemGroup.push_back(WeaveItem);
	}
	if(cbCard == 0x07 && cbCardIndex[cbIndex - 5] > 0 && cbCardIndex[cbIndex + 3] > 0){
		WeaveItem.cbCardList[1] = singleCard - 5;
		WeaveItem.cbCardList[2] = singleCard + 3;
		WeaveItemGroup.push_back(WeaveItem);
	}
	if(cbCard == 0x0A && cbCardIndex[cbIndex - 3] > 0 && cbCardIndex[cbIndex - 8] > 0){
		WeaveItem.cbCardList[1] = singleCard - 3;
		WeaveItem.cbCardList[2] = singleCard - 8;
		WeaveItemGroup.push_back(WeaveItem);
	}
	//std::cout<<"GetSingleGroup WeaveItemGroup:"<<WeaveItemGroup.size() << std::endl;
}

//如果手上全是对子或者三张 则组合形式固定
void CGameLogic::GetDoubleGroup(_uint8 cbCardIndex[MAX_INDEX], CWeaveItemArray &WeaveItemGroup)
{
	for (_uint8 i = 0; i < MAX_INDEX; ++i){
		if(cbCardIndex[i] >= 2){
			_uint8 cbCardData = SwitchToCardData(i);
			_uint8 cbCard = cbCardData & MASK_VALUE;
			_uint8 cbRCard = SwitchToCardData((i + 10) % 20);
			tagWeaveItem WeaveItem;
			WeaveItem.cbWeaveKind = ACK_CHI;
			WeaveItem.cbCardCount = 3;
			WeaveItem.cbCenterCard = cbCardData;
			WeaveItem.cbCardList[0] = cbCardData;
			/*if(cbCardIndex[i] == 3){
				WeaveItem.cbWeaveKind = ACK_WEI;
				WeaveItem.cbCardList[1] = cbCardData;
				WeaveItem.cbCardList[2] = cbCardData;
				WeaveItemGroup.push_back(WeaveItem);
			}*/
			//如果手上全是大牌 则找到的第一个只能组成顺子且点数最大只能为7
			if(i > 9){
				if(cbCard <= 8){
					if(cbCardIndex[i + 1] > 0 && cbCardIndex[i + 2] > 0){
						WeaveItem.cbCardList[1] = cbCardData + 1;
						WeaveItem.cbCardList[2] = cbCardData + 2;
						WeaveItemGroup.push_back(WeaveItem);
					}
					if(cbCardData == 0x12 && cbCardIndex[16] > 0 && cbCardIndex[19] > 0){
						WeaveItem.cbCardList[1] = 0x17;
						WeaveItem.cbCardList[2] = 0x1A;
						WeaveItemGroup.push_back(WeaveItem);
					}
				}
			}
			else{
				//小牌可以搭吃 则多的一张大牌只能组成顺子或者2 7 10
				if(i < 10 && cbCardIndex[i + 10] >= 2){
					WeaveItem.cbCardList[1] = cbCardData;
					WeaveItem.cbCardList[2] = cbRCard;
					WeaveItemGroup.push_back(WeaveItem);
					WeaveItem.cbCardList[1] = cbRCard;
					WeaveItem.cbCardList[2] = cbRCard;
					WeaveItemGroup.push_back(WeaveItem);
				}
				if(i < 8 && cbCardIndex[i + 1] > 0 && cbCardIndex[i + 2] > 0){
					WeaveItem.cbCardList[1] = cbCardData + 1;
					WeaveItem.cbCardList[2] = cbCardData + 2;
					WeaveItemGroup.push_back(WeaveItem);
				}
				if(i == 1 && cbCardIndex[i + 5] > 0 && cbCardIndex[i + 8] > 0){
					WeaveItem.cbCardList[1] = cbCardData + 5;
					WeaveItem.cbCardList[2] = cbCardData + 8;
					WeaveItemGroup.push_back(WeaveItem);
				}
			}
			//std::cout<<"GetDoubleGroup WeaveItemGroup:"<<WeaveItemGroup.size() << std::endl;
			return;
		}
	}
}

bool CGameLogic::GetAllGroup(_uint8 cbCardIndex[MAX_INDEX], CWeaveItemArray &BestItemGroup, CAnalyseItemArray & AnalyseItemArray)
{
	//数目统计
	_uint8 cbCardCount = 0;
	bool has_over = false;
	_uint8 cbCardEye = 0;
	for (_uint8 i = 0; i < MAX_INDEX; ++i) cbCardCount += cbCardIndex[i];
	//搜索完毕将组合拷贝
	if(cbCardCount == 0){
		has_over = true;
	}
	else if(cbCardCount == 2){
		for (_uint8 i = 0; i < MAX_INDEX; i++){
			if (cbCardIndex[i] == 2){
				has_over = true;
				cbCardEye = SwitchToCardData(i);
				break;
			}
		}
		if(!has_over){
			BestItemGroup.pop_back();
			return false;
		}
	}
	if(has_over){
		//std::cout<<"BestItemGroup :"<<BestItemGroup.size()<<std::endl;
		if(!BestItemGroup.empty()){
			tagAnalyseItem AnalyseItem;
			memset(&AnalyseItem, 0, sizeof(AnalyseItem));
			AnalyseItem.cbHuXiCount = 0;
			AnalyseItem.cbCardEye = cbCardEye;
			AnalyseItem.cbWeaveCount = BestItemGroup.size();
			//设置组合
			for (_uint8 i = 0; i < AnalyseItem.cbWeaveCount; i++){
				AnalyseItem.WeaveItemArray[i] = BestItemGroup[i];
				AnalyseItem.cbHuXiCount += GetWeaveHuXi(AnalyseItem.WeaveItemArray[i]);
			}
			//插入结果
			AnalyseItemArray.push_back(AnalyseItem);
			BestItemGroup.pop_back();
			//std::cout<<"AnalyseItemArray :"<<AnalyseItemArray.size()<<std::endl;
		}
		
		return true;
	}
	_uint8 singleCard = GetSingleCard(cbCardIndex);
	CWeaveItemArray WeaveItemGroup;
	WeaveItemGroup.clear();
	if(singleCard != 0){
		//std::cout<<"singleCard:"<<(int)singleCard<<std::endl;
		GetSingleGroup(cbCardIndex, singleCard, WeaveItemGroup);
	}
	else{
		GetDoubleGroup(cbCardIndex, WeaveItemGroup);
	}
	if(!WeaveItemGroup.empty()){
		_uint8 cbCardTemp[MAX_INDEX];
		tagWeaveItem WeaveItem;
		for(_uint8 i = 0; i < (_uint8)WeaveItemGroup.size(); ++i){
			WeaveItem = WeaveItemGroup[i];
			BestItemGroup.push_back(WeaveItem);
			memset(cbCardTemp, 0, MAX_INDEX);
			memcpy(cbCardTemp, cbCardIndex, MAX_INDEX);
			RemoveCard(cbCardTemp, WeaveItem.cbCardList, WeaveItem.cbCardCount);
			GetAllGroup(cbCardTemp, BestItemGroup, AnalyseItemArray);
		}
	}
	BestItemGroup.pop_back();
	return false;
}

//分析扑克
bool CGameLogic::AnalyseCard(_uint8 cbCardIndex[MAX_INDEX], CAnalyseItemArray & AnalyseItemArray)
{
	_uint8 cbCardCount = 0;
	for (_uint8 i = 0; i < MAX_INDEX; i++){
		cbCardCount += cbCardIndex[i];
	}
	if (cbCardCount == 0) return true;
	if ((cbCardCount % 3 != 0) && ((cbCardCount + 1) % 3 != 0)) return false;

	//单吊判断
	if (cbCardCount == 2)
	{
		//牌眼判断
		for (_uint8 i = 0; i < MAX_INDEX; i++)
		{
			if (cbCardIndex[i] == 2)
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				memset(&AnalyseItem, 0, sizeof(AnalyseItem));

				//设置结果
				AnalyseItem.cbHuXiCount = 0;
				AnalyseItem.cbWeaveCount = 0;
				AnalyseItem.cbCardEye = SwitchToCardData(i);

				//插入结果
				AnalyseItemArray.push_back(AnalyseItem);

				return true;
			}
		}

		return false;
	}
	if((cbCardCount + 1) % 3 == 0){
		_uint8 cbCardIndexTemp[MAX_INDEX];
		_uint32 begin = 0;
		for(_uint8 i = 0; i < MAX_INDEX; ++i){
			memcpy(cbCardIndexTemp, cbCardIndex, MAX_INDEX);
			if(cbCardIndex[i] >= 2){
				cbCardIndexTemp[i] -= 2;
				CWeaveItemArray BestItemGroup;
				GetAllGroup(cbCardIndexTemp, BestItemGroup, AnalyseItemArray);
				//如果不为空说明此牌可以作为牌眼
				if(AnalyseItemArray.size() - begin > 0){
					for(_uint32 n = begin; n < AnalyseItemArray.size(); ++n){
						AnalyseItemArray[n].cbCardEye = SwitchToCardData(i);
					}
					begin += AnalyseItemArray.size();
				}
			}
		}
	}
	else if(cbCardCount % 3 == 0){
		CWeaveItemArray BestItemGroup;
		GetAllGroup(cbCardIndex, BestItemGroup, AnalyseItemArray);
	}
	
	#if 0
	for (_uint32 i = 0; i < AnalyseItemArray.size(); ++i){
		tagAnalyseItem pAnalyseItem = AnalyseItemArray[i];
		fprintf(stdout, "AnalyseItemArray[%d]\n", i);
		for(_uint8 j = 0; j < pAnalyseItem.cbWeaveCount; ++j){
			tagWeaveItem WeaveItem = pAnalyseItem.WeaveItemArray[j];
			fprintf(stdout, "WeaveItem[%d][[ ", j);
			for(_uint8 k = 0; k < WeaveItem.cbCardCount; ++k){
				fprintf(stdout, "%d ", WeaveItem.cbCardList[k]);
			}
			fprintf(stdout, "]]\n");
			
		}
	}
	#endif
	return !AnalyseItemArray.empty();
}


//提取吃牌
_uint8 CGameLogic::TakeOutChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbResultCard[3])
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return 0;

	//变量定义
	_uint8 cbFirstIndex = 0;
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//大小搭吃
	_uint8 cbReverseIndex = (cbCurrentIndex + 10) % MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex] >= 2) && (cbCardIndex[cbReverseIndex] >= 1) && (cbCardIndex[cbReverseIndex] != 3))
	{
		//删除扑克
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex]--;

		//设置结果
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = cbCurrentCard;
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex] == 2)
	{
		//删除扑克
		cbCardIndex[cbCurrentIndex]--;
		cbCardIndex[cbReverseIndex] -= 2;

		//设置结果
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = SwitchToCardData(cbReverseIndex);
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		return ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
	}

	//二七十吃
	_uint8 bCardValue = (cbCurrentCard&MASK_VALUE);
	if ((bCardValue == 0x02) || (bCardValue == 0x07) || (bCardValue == 0x0A))
	{
		//变量定义
		_uint8 cbExcursion[] = { 1,6,9 };
		_uint8 cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//类型判断
		//for (_uint8 i=0;i<CountArray(cbExcursion);i++)
		_uint8 i = 0;
		for (; i < 3; i++)
		{
			_uint8 cbIndex = cbInceptIndex + cbExcursion[i];
			if ((cbCardIndex[cbIndex] == 0) || (cbCardIndex[cbIndex] == 3)) break;
		}

		//成功判断
		//if (i==CountArray(cbExcursion))
		if (i == 3)
		{
			//删除扑克
			cbCardIndex[cbInceptIndex + cbExcursion[0]]--;
			cbCardIndex[cbInceptIndex + cbExcursion[1]]--;
			cbCardIndex[cbInceptIndex + cbExcursion[2]]--;

			//设置结果
			cbResultCard[0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
			cbResultCard[1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
			cbResultCard[2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);

			return CK_EQS;
		}
	}

	//顺子判断
	_uint8 cbExcursion[3] = { 0,1,2 };
	//for (_uint8 i=0;i<CountArray(cbExcursion);i++)
	for (_uint8 i = 0; i < 3; i++)
	{
		_uint8 cbValueIndex = cbCurrentIndex % 10;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 7))
		{
			//吃牌判断
			cbFirstIndex = cbCurrentIndex - cbExcursion[i];
			if ((cbCardIndex[cbFirstIndex] == 0) || (cbCardIndex[cbFirstIndex] == 3)) continue;
			if ((cbCardIndex[cbFirstIndex + 1] == 0) || (cbCardIndex[cbFirstIndex + 1] == 3)) continue;
			if ((cbCardIndex[cbFirstIndex + 2] == 0) || (cbCardIndex[cbFirstIndex + 2] == 3)) continue;

			//删除扑克
			cbCardIndex[cbFirstIndex]--;
			cbCardIndex[cbFirstIndex + 1]--;
			cbCardIndex[cbFirstIndex + 2]--;

			//设置结果
			cbResultCard[0] = SwitchToCardData(cbFirstIndex);
			cbResultCard[1] = SwitchToCardData(cbFirstIndex + 1);
			cbResultCard[2] = SwitchToCardData(cbFirstIndex + 2);

			_uint8 cbChiKind[3] = { CK_LEFT,CK_CENTER,CK_RIGHT };
			return cbChiKind[i];
		}
	}

	return CK_NULL;
}

//扑克转换
_uint8 CGameLogic::SwitchToCardData(_uint8 cbCardIndex)
{
	//assert(cbCardIndex<MAX_INDEX);
	return ((cbCardIndex / 10) << 4) | (cbCardIndex % 10 + 1);
}

//扑克转换
_uint8 CGameLogic::SwitchToCardIndex(_uint8 cbCardData)
{
	//assert(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR) >> 4) * 10 + (cbCardData&MASK_VALUE) - 1;
}

//扑克转换
_uint8 CGameLogic::SwitchToCardData(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCardData[], _uint8 bMaxCount)
{
	//转换扑克
	_uint8 bPosition = 0;
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		_uint8 cbIndex = (i % 2) * 10 + i / 2;
		if (cbCardIndex[cbIndex] != 0)
		{
			for (_uint8 j = 0; j < cbCardIndex[cbIndex]; j++)
			{
				//assert(bPosition<bMaxCount);
				cbCardData[bPosition++] = SwitchToCardData(cbIndex);
			}
		}
	}

	return bPosition;
}

//扑克转换
_uint8 CGameLogic::SwitchToCardIndex(_uint8 cbCardData[], _uint8 bCardCount, _uint8 cbCardIndex[MAX_INDEX])
{
	//设置变量
	memset(cbCardIndex, 0, sizeof(_uint8)*MAX_INDEX);

	//转换扑克
	for (_uint8 i = 0; i < bCardCount; i++)
	{
		//assert(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}
	return bCardCount;
}

//提取吃牌
_uint8 CGameLogic::TakeOutChiCard(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, _uint8 cbResultCard[3], _uint32 &wChiKinds)
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return 0;

	//变量定义
	_uint8 cbFirstIndex = 0;
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//大小搭吃
	_uint8 cbReverseIndex = (cbCurrentIndex + 10) % MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex] >= 2) && (cbCardIndex[cbReverseIndex] >= 1) && (cbCardIndex[cbReverseIndex] != 3))
	{
		//设置结果
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = cbCurrentCard;
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		_uint8 cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;
		if ((wChiKinds & cbChiKind) == 0) return cbChiKind;
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex] == 2)
	{
		//设置结果
		cbResultCard[0] = cbCurrentCard;
		cbResultCard[1] = SwitchToCardData(cbReverseIndex);
		cbResultCard[2] = SwitchToCardData(cbReverseIndex);

		_uint8 cbChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;
		if ((wChiKinds & cbChiKind) == 0) return cbChiKind;
	}

	//二七十吃
	_uint8 bCardValue = (cbCurrentCard&MASK_VALUE);
	if ((bCardValue == 0x02) || (bCardValue == 0x07) || (bCardValue == 0x0A))
	{
		//变量定义
		_uint8 cbExcursion[] = { 1,6,9 };
		_uint8 cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//类型判断
		_uint8 i = 0;
		for (; i < 3; i++)
		{
			_uint8 cbIndex = cbInceptIndex + cbExcursion[i];
			if ((cbCardIndex[cbIndex] == 0) || (cbCardIndex[cbIndex] == 3)) break;
		}

		//成功判断
		if (i == 3)
		{
			//设置结果
			cbResultCard[0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
			cbResultCard[1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
			cbResultCard[2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);

			if ((wChiKinds & CK_EQS) == 0) return CK_EQS;
		}
	}

	//顺子判断
	_uint8 cbExcursion[3] = { 0,1,2 };
	//for (_uint8 i=0;i<CountArray(cbExcursion);i++)
	for (_uint8 i = 0; i < 3; i++)
	{
		_uint8 cbValueIndex = cbCurrentIndex % 10;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 7))
		{
			//吃牌判断
			cbFirstIndex = cbCurrentIndex - cbExcursion[i];
			if ((cbCardIndex[cbFirstIndex] == 0) || (cbCardIndex[cbFirstIndex] == 3)) continue;
			if ((cbCardIndex[cbFirstIndex + 1] == 0) || (cbCardIndex[cbFirstIndex + 1] == 3)) continue;
			if ((cbCardIndex[cbFirstIndex + 2] == 0) || (cbCardIndex[cbFirstIndex + 2] == 3)) continue;

			//设置结果
			cbResultCard[0] = SwitchToCardData(cbFirstIndex);
			cbResultCard[1] = SwitchToCardData(cbFirstIndex + 1);
			cbResultCard[2] = SwitchToCardData(cbFirstIndex + 2);

			_uint8 cbChiKind[3] = { CK_LEFT,CK_CENTER,CK_RIGHT };
			if ((wChiKinds & cbChiKind[i]) == 0) return cbChiKind[i];
		}
	}

	return CK_NULL;
}

//获取吃牌信息
_uint8 CGameLogic::GetChiCardInfo(_uint8 cbCardIndex[MAX_INDEX], _uint8 cbCurrentCard, tagNewChiCardInfo ChiCardInfo[6])
{
	//效验扑克
	//assert(cbCurrentCard!=0);
	if (cbCurrentCard == 0) return 0;

	//变量定义
	_uint8 cbChiCardCount = 0;
	_uint8 cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	//三牌判断
	if (cbCardIndex[cbCurrentIndex] >= 3) return cbChiCardCount;

	//大小搭吃
	_uint8 cbReverseIndex = (cbCurrentIndex + 10) % MAX_INDEX;
	if ((cbCardIndex[cbCurrentIndex] >= 1) && (cbCardIndex[cbReverseIndex] >= 1) && (cbCardIndex[cbReverseIndex] != 3))
	{
		//构造扑克
		_uint8 cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbCurrentIndex]--;
		cbCardIndexTemp[cbReverseIndex]--;
		//设置结果
		_uint8 cbFirstResult[3] = { 0 };
		cbFirstResult[0] = cbCurrentCard;
		cbFirstResult[1] = cbCurrentCard;
		cbFirstResult[2] = SwitchToCardData(cbReverseIndex);
		_uint8 cbFirstChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XXD : CK_XDD;

		if (cbCardIndexTemp[cbCurrentIndex] == 0)
		{
			//设置结果
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
			ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;
			cbChiCardCount++;
		}
		else
		{
			//第二层
			_uint8 cbSecondCount = 0;
			_uint32	wSecondChiKinds = 0;
			while (cbCardIndexTemp[cbCurrentIndex] > 0)
			{
				_uint8 cbSecondIndexTemp[MAX_INDEX];
				memcpy(cbSecondIndexTemp, cbCardIndexTemp, sizeof(cbSecondIndexTemp));
				_uint8 cbSecondResult[3] = { 0 };
				_uint8 cbSecondChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbSecondResult, wSecondChiKinds);
				if (cbSecondChiKind != CK_NULL)
				{
					wSecondChiKinds |= cbSecondChiKind;
					for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbSecondIndexTemp, cbSecondResult[k]);
					if (cbSecondIndexTemp[cbCurrentIndex] == 0)
					{
						//设置结果
						ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
						memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
						ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

						ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
						ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
						memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));
						cbSecondCount++;
					}
					else
					{
						//第三层
						_uint8 cbThirdCount = 0;
						_uint32	wThirdChiKinds = 0;
						while (cbSecondIndexTemp[cbCurrentIndex] > 0)
						{
							_uint8 cbThirdIndexTemp[MAX_INDEX];
							memcpy(cbThirdIndexTemp, cbSecondIndexTemp, sizeof(cbThirdIndexTemp));
							_uint8 cbThirdResult[3] = { 0 };
							_uint8 cbThirdChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbThirdResult, wThirdChiKinds);
							if (cbThirdChiKind != CK_NULL)
							{
								wThirdChiKinds |= cbThirdChiKind;
								for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbThirdIndexTemp, cbThirdResult[k]);
								if (cbThirdIndexTemp[cbCurrentIndex] == 0)
								{
									//设置结果
									ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
									memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
									ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
									memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));

									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbChiKind = cbThirdChiKind;
									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCenterCard = cbCurrentCard;
									memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData, cbThirdResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData));
									cbThirdCount++;
								}
							}
							else
							{
								ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbThirdCount = cbThirdCount;
								if (cbThirdCount > 0) cbSecondCount++;
								break;
							}
						}
					}
				}
				else
				{
					ChiCardInfo[cbChiCardCount].cbSecondCount = cbSecondCount;
					if (cbSecondCount > 0) cbChiCardCount++;
					break;
				}
			}
		}
	}

	//大小搭吃
	if (cbCardIndex[cbReverseIndex] == 2)
	{
		//构造扑克
		_uint8 cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

		//删除扑克
		cbCardIndexTemp[cbReverseIndex] -= 2;
		//设置结果
		_uint8 cbFirstResult[3] = { 0 };
		cbFirstResult[0] = cbCurrentCard;
		cbFirstResult[1] = SwitchToCardData(cbReverseIndex);
		cbFirstResult[2] = SwitchToCardData(cbReverseIndex);
		_uint8 cbFirstChiKind = ((cbCurrentCard&MASK_COLOR) == 0x00) ? CK_XDD : CK_XXD;

		if (cbCardIndexTemp[cbCurrentIndex] == 0)
		{
			//设置结果
			ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
			memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
			ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;
			cbChiCardCount++;
		}
		else
		{
			//第二层
			_uint8 cbSecondCount = 0;
			_uint32	wSecondChiKinds = 0;
			while (cbCardIndexTemp[cbCurrentIndex] > 0)
			{
				_uint8 cbSecondIndexTemp[MAX_INDEX];
				memcpy(cbSecondIndexTemp, cbCardIndexTemp, sizeof(cbSecondIndexTemp));
				_uint8 cbSecondResult[3] = { 0 };
				_uint8 cbSecondChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbSecondResult, wSecondChiKinds);
				if (cbSecondChiKind != CK_NULL)
				{
					wSecondChiKinds |= cbSecondChiKind;
					for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbSecondIndexTemp, cbSecondResult[k]);
					if (cbSecondIndexTemp[cbCurrentIndex] == 0)
					{
						//设置结果
						ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
						memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
						ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

						ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
						ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
						memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));
						cbSecondCount++;
					}
					else
					{
						//第三层
						_uint8 cbThirdCount = 0;
						_uint32	wThirdChiKinds = 0;
						while (cbSecondIndexTemp[cbCurrentIndex] > 0)
						{
							_uint8 cbThirdIndexTemp[MAX_INDEX];
							memcpy(cbThirdIndexTemp, cbSecondIndexTemp, sizeof(cbThirdIndexTemp));
							_uint8 cbThirdResult[3] = { 0 };
							_uint8 cbThirdChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbThirdResult, wThirdChiKinds);
							if (cbThirdChiKind != CK_NULL)
							{
								wThirdChiKinds |= cbThirdChiKind;
								for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbThirdIndexTemp, cbThirdResult[k]);
								if (cbThirdIndexTemp[cbCurrentIndex] == 0)
								{
									//设置结果
									ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
									memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
									ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
									memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));

									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbChiKind = cbThirdChiKind;
									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCenterCard = cbCurrentCard;
									memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData, cbThirdResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData));
									cbThirdCount++;
								}
							}
							else
							{
								ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbThirdCount = cbThirdCount;
								if (cbThirdCount > 0) cbSecondCount++;
								break;
							}
						}
					}
				}
				else
				{
					ChiCardInfo[cbChiCardCount].cbSecondCount = cbSecondCount;
					if (cbSecondCount > 0) cbChiCardCount++;
					break;
				}
			}
		}
	}

	//二七十吃
	_uint8 bCardValue = cbCurrentCard&MASK_VALUE;
	if ((bCardValue == 0x02) || (bCardValue == 0x07) || (bCardValue == 0x0A))
	{
		//变量定义
		_uint8 cbExcursion[] = { 1,6,9 };
		_uint8 cbInceptIndex = ((cbCurrentCard&MASK_COLOR) == 0x00) ? 0 : 10;

		//类型判断
		_uint8 i;
		for (i = 0; i < 3; i++)
		{
			_uint8 cbIndex = cbInceptIndex + cbExcursion[i];
			if ((cbIndex != cbCurrentIndex) && ((cbCardIndex[cbIndex] == 0) || (cbCardIndex[cbIndex] == 3))) break;
		}

		//提取判断
		if (i == 3)
		{
			//构造扑克
			_uint8 cbCardIndexTemp[MAX_INDEX];
			memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

			//删除扑克
			for (_uint8 j = 0; j < 3; j++)
			{
				_uint8 cbIndex = cbInceptIndex + cbExcursion[j];
				if (cbIndex != cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
			}

			//设置结果
			_uint8 cbFirstResult[3] = { 0 };
			cbFirstResult[0] = SwitchToCardData(cbInceptIndex + cbExcursion[0]);
			cbFirstResult[1] = SwitchToCardData(cbInceptIndex + cbExcursion[1]);
			cbFirstResult[2] = SwitchToCardData(cbInceptIndex + cbExcursion[2]);
			_uint8 cbFirstChiKind = CK_EQS;

			if (cbCardIndexTemp[cbCurrentIndex] == 0)
			{
				//设置结果
				ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
				memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
				ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;
				cbChiCardCount++;
			}
			else
			{
				//第二层
				_uint8 cbSecondCount = 0;
				_uint32	wSecondChiKinds = 0;
				while (cbCardIndexTemp[cbCurrentIndex] > 0)
				{
					_uint8 cbSecondIndexTemp[MAX_INDEX];
					memcpy(cbSecondIndexTemp, cbCardIndexTemp, sizeof(cbSecondIndexTemp));
					_uint8 cbSecondResult[3] = { 0 };
					_uint8 cbSecondChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbSecondResult, wSecondChiKinds);
					if (cbSecondChiKind != CK_NULL)
					{
						wSecondChiKinds |= cbSecondChiKind;
						for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbSecondIndexTemp, cbSecondResult[k]);
						if (cbSecondIndexTemp[cbCurrentIndex] == 0)
						{
							//设置结果
							ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
							memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
							ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

							ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
							ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
							memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));
							cbSecondCount++;
						}
						else
						{
							//第三层
							_uint8 cbThirdCount = 0;
							_uint32	wThirdChiKinds = 0;
							while (cbSecondIndexTemp[cbCurrentIndex] > 0)
							{
								_uint8 cbThirdIndexTemp[MAX_INDEX];
								memcpy(cbThirdIndexTemp, cbSecondIndexTemp, sizeof(cbThirdIndexTemp));
								_uint8 cbThirdResult[3] = { 0 };
								_uint8 cbThirdChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbThirdResult, wThirdChiKinds);
								if (cbThirdChiKind != CK_NULL)
								{
									wThirdChiKinds |= cbThirdChiKind;
									for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbThirdIndexTemp, cbThirdResult[k]);
									if (cbThirdIndexTemp[cbCurrentIndex] == 0)
									{
										//设置结果
										ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
										memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
										ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

										ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
										ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
										memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));

										ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbChiKind = cbThirdChiKind;
										ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCenterCard = cbCurrentCard;
										memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData, cbThirdResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData));
										cbThirdCount++;
									}
								}
								else
								{
									ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbThirdCount = cbThirdCount;
									if (cbThirdCount > 0) cbSecondCount++;
									break;
								}
							}
						}
					}
					else
					{
						ChiCardInfo[cbChiCardCount].cbSecondCount = cbSecondCount;
						if (cbSecondCount > 0) cbChiCardCount++;
						break;
					}
				}
			}
		}
	}

	//顺子类型
	_uint8 cbExcursion[3] = { 0,1,2 };
	for (_uint8 i = 0; i < 3; i++)
	{
		_uint8 cbValueIndex = cbCurrentIndex % 10;
		if ((cbValueIndex >= cbExcursion[i]) && ((cbValueIndex - cbExcursion[i]) <= 7))
		{
			//索引定义
			_uint8 cbFirstIndex = cbCurrentIndex - cbExcursion[i];

			//吃牌判断
			_uint8 j;
			for (j = 0; j < 3; j++)
			{
				_uint8 cbIndex = cbFirstIndex + j;
				if ((cbIndex != cbCurrentIndex) && ((cbCardIndex[cbIndex] == 0) || (cbCardIndex[cbIndex] == 3))) break;
			}

			//提取判断
			if (j == 3)
			{
				//构造扑克
				_uint8 cbCardIndexTemp[MAX_INDEX];
				memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

				//删除扑克
				for (_uint8 j = 0; j < 3; j++)
				{
					_uint8 cbIndex = cbFirstIndex + j;
					if (cbIndex != cbCurrentIndex) cbCardIndexTemp[cbIndex]--;
				}

				//设置结果
				_uint8 cbChiKind[3] = { CK_LEFT,CK_CENTER,CK_RIGHT };
				_uint8 cbFirstResult[3] = { 0 };
				cbFirstResult[0] = SwitchToCardData(cbFirstIndex);
				cbFirstResult[1] = SwitchToCardData(cbFirstIndex + 1);
				cbFirstResult[2] = SwitchToCardData(cbFirstIndex + 2);
				_uint8 cbFirstChiKind = cbChiKind[i];

				if (cbCardIndexTemp[cbCurrentIndex] == 0)
				{
					//设置结果
					ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
					memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
					ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;
					cbChiCardCount++;
				}
				else
				{
					//第二层
					_uint8 cbSecondCount = 0;
					_uint32	wSecondChiKinds = 0;
					while (cbCardIndexTemp[cbCurrentIndex] > 0)
					{
						_uint8 cbSecondIndexTemp[MAX_INDEX];
						memcpy(cbSecondIndexTemp, cbCardIndexTemp, sizeof(cbSecondIndexTemp));
						_uint8 cbSecondResult[3] = { 0 };
						_uint8 cbSecondChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbSecondResult, wSecondChiKinds);
						if (cbSecondChiKind != CK_NULL)
						{
							wSecondChiKinds |= cbSecondChiKind;
							for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbSecondIndexTemp, cbSecondResult[k]);
							if (cbSecondIndexTemp[cbCurrentIndex] == 0)
							{
								//设置结果
								ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
								memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
								ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

								ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
								ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
								memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));
								cbSecondCount++;
							}
							else
							{
								//第三层
								_uint8 cbThirdCount = 0;
								_uint32	wThirdChiKinds = 0;
								while (cbSecondIndexTemp[cbCurrentIndex] > 0)
								{
									_uint8 cbThirdIndexTemp[MAX_INDEX];
									memcpy(cbThirdIndexTemp, cbSecondIndexTemp, sizeof(cbThirdIndexTemp));
									_uint8 cbThirdResult[3] = { 0 };
									_uint8 cbThirdChiKind = TakeOutChiCard(cbSecondIndexTemp, cbCurrentCard, cbThirdResult, wThirdChiKinds);
									if (cbThirdChiKind != CK_NULL)
									{
										wThirdChiKinds |= cbThirdChiKind;
										for (_uint8 k = 0; k < 3; ++k) RemoveCard(cbThirdIndexTemp, cbThirdResult[k]);
										if (cbThirdIndexTemp[cbCurrentIndex] == 0)
										{
											//设置结果
											ChiCardInfo[cbChiCardCount].cbCenterCard = cbCurrentCard;
											memcpy(ChiCardInfo[cbChiCardCount].cbCardData, cbFirstResult, sizeof(ChiCardInfo[cbChiCardCount].cbCardData));
											ChiCardInfo[cbChiCardCount].cbChiKind = cbFirstChiKind;

											ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbChiKind = cbSecondChiKind;
											ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCenterCard = cbCurrentCard;
											memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData, cbSecondResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbCardData));

											ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbChiKind = cbThirdChiKind;
											ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCenterCard = cbCurrentCard;
											memcpy(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData, cbThirdResult, sizeof(ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].ThirdData[cbThirdCount].cbCardData));
											cbThirdCount++;
										}
									}
									else
									{
										ChiCardInfo[cbChiCardCount].SecondData[cbSecondCount].cbThirdCount = cbThirdCount;
										if (cbThirdCount > 0) cbSecondCount++;
										break;
									}
								}
							}
						}
						else
						{
							ChiCardInfo[cbChiCardCount].cbSecondCount = cbSecondCount;
							if (cbSecondCount > 0) cbChiCardCount++;
							break;
						}
					}
				}
			}
		}
	}
	return cbChiCardCount;
}

_uint8 CGameLogic::GetCardsWeaveHuXi(tagCardsWeave CardsWeave[MAX_INDEX], _uint8 cbCardsWeaveCount)
{
	_uint8 cbTotalHuXi = 0;
	if (cbCardsWeaveCount == 0) return cbTotalHuXi;

	for (_uint8 i = 0; i < cbCardsWeaveCount; ++i)
	{
		if (CardsWeave[i].cbCardCount != 3) continue;
		_uint8 cbCardList[3];
		memcpy(cbCardList, CardsWeave[i].cbCardList, sizeof(cbCardList));
		if (!IsValidCard(cbCardList[0]) || !IsValidCard(cbCardList[1]) || !IsValidCard(cbCardList[2])) continue;
		//排序
		for (_uint8 m = 0; m < 3 - 1; ++m)
		{
			for (_uint8 n = m + 1; n < 3; ++n)
			{
				if (cbCardList[m] > cbCardList[n])
				{
					_uint8 cbTemp = cbCardList[m];
					cbCardList[m] = cbCardList[n];
					cbCardList[n] = cbTemp;
				}
			}
		}
		//坎
		if (cbCardList[0] == cbCardList[1] && cbCardList[1] == cbCardList[2])
		{
			cbTotalHuXi += (CardType(cbCardList[0]) == 0) ? 3 : 6;
		}

		//二七十
		if (CardType(cbCardList[0]) == CardType(cbCardList[1]) && CardType(cbCardList[1]) == CardType(cbCardList[2])
			&& CardNum(cbCardList[0]) == 0x02 && CardNum(cbCardList[1]) == 0x07 && CardNum(cbCardList[2]) == 0x0a)
		{
			cbTotalHuXi += (CardType(cbCardList[0]) == 0) ? 3 : 6;
		}

		//一二三
		if (CardType(cbCardList[0]) == CardType(cbCardList[1]) && CardType(cbCardList[1]) == CardType(cbCardList[2])
			&& CardNum(cbCardList[0]) == 0x01 && CardNum(cbCardList[1]) == 0x02 && CardNum(cbCardList[2]) == 0x03)
		{
			cbTotalHuXi += (CardType(cbCardList[0]) == 0) ? 3 : 6;
		}
	}

	return cbTotalHuXi;
}

bool CGameLogic::UpdateCardsWeave(tagCardsWeave CardsWeave[MAX_INDEX], _uint8 &cbCardsWeaveCount, _uint8 cbRemoveCards[4], _uint8 cbRemoveCount, _uint8 cbRow, _uint8 cbCol)
{
	if (cbCol >= 0 && cbCol < cbCardsWeaveCount && cbRow >= 0 && cbRow < CardsWeave[cbCol].cbCardCount && cbRemoveCount == 1)
	{
		if (CardsWeave[cbCol].cbCardList[cbRow] == cbRemoveCards[0])
		{
			CardsWeave[cbCol].cbCardList[cbRow] = 0;
			for (_uint8 n = cbRow + 1; n < CardsWeave[cbCol].cbCardCount; ++n)
			{
				CardsWeave[cbCol].cbCardList[cbRow] = CardsWeave[cbCol].cbCardList[n];
				cbRow++;
			}
			CardsWeave[cbCol].cbCardCount--;
			if (CardsWeave[cbCol].cbCardCount == 0)
			{
				for (_uint8 m = cbCol + 1; m < cbCardsWeaveCount; ++m)
				{
					CardsWeave[cbCol].cbCardCount = CardsWeave[m].cbCardCount;
					memcpy(CardsWeave[cbCol].cbCardList, CardsWeave[m].cbCardList, sizeof(CardsWeave[cbCol].cbCardList));
					cbCol++;
				}
				cbCardsWeaveCount--;
			}
			return true;
		}
	}

	/*for( _uint8 i = 0; i < cbRemoveCount && i < 4; ++i )
	{
		for( _uint8 j = 0; j < cbCardsWeaveCount; ++j )
		{
			bool bFind = false;
			for( _uint8 k = 0; k < CardsWeave[j].cbCardCount; ++k )
			{
				if( CardsWeave[j].cbCardList[k] == cbRemoveCards[i] )
				{
					CardsWeave[j].cbCardList[k] = 0;
					for( _uint8 n = k+1; n < CardsWeave[j].cbCardCount; ++n )
					{
						CardsWeave[j].cbCardList[k] = CardsWeave[j].cbCardList[n];
						k++;
					}
					CardsWeave[j].cbCardCount--;
					bFind = true;
					break;
				}
			}

			if( bFind )
			{
				if( CardsWeave[j].cbCardCount == 0 )
				{
					for( _uint8 m = j+1; m < cbCardsWeaveCount; ++m )
					{
						CardsWeave[j].cbCardCount = CardsWeave[m].cbCardCount;
						memcpy( CardsWeave[j].cbCardList, CardsWeave[m].cbCardList, sizeof(CardsWeave[j].cbCardList) );
						j++;
					}
					cbCardsWeaveCount--;
				}
				break;
			}
		}
	}*/
	///////////////////////////////////////////////////////////
	for (_uint8 i = 0; i < cbRemoveCount && i < 4; ++i)
	{
		for (_tint16 j = cbCardsWeaveCount - 1; j >= 0; --j)
		{
			bool bFind = false;
			for (_uint8 k = 0; k < CardsWeave[j].cbCardCount; ++k)
			{
				if (CardsWeave[j].cbCardList[k] == cbRemoveCards[i])
				{
					CardsWeave[j].cbCardList[k] = 0;
					for (_uint8 n = k + 1; n < CardsWeave[j].cbCardCount; ++n)
					{
						CardsWeave[j].cbCardList[k] = CardsWeave[j].cbCardList[n];
						k++;
					}
					CardsWeave[j].cbCardCount--;
					bFind = true;
					break;
				}
			}

			if (bFind)
			{
				_tint16 n = j;
				if (CardsWeave[n].cbCardCount == 0)
				{
					for (_uint8 m = n + 1; m < cbCardsWeaveCount; ++m)
					{
						CardsWeave[n].cbCardCount = CardsWeave[m].cbCardCount;
						memcpy(CardsWeave[n].cbCardList, CardsWeave[m].cbCardList, sizeof(CardsWeave[n].cbCardList));
						n++;
					}
					cbCardsWeaveCount--;
				}
				break;
			}
		}
	}

	return true;
}

//整理普通模式牌
bool CGameLogic::SettleCommonWeaves(_uint8 cbCardIndex[MAX_INDEX], tagCardsWeave WeaveItem[MAX_INDEX], _uint8 &cbWeaveCount)
{
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	cbWeaveCount = 0;
	memset(WeaveItem, 0, sizeof(tagCardsWeave)*MAX_INDEX);

	//提取四牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 4)
		{
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 4;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[2] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[3] = cbCardData;
		}
	}
	//提取三牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 3)
		{
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 3;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[2] = cbCardData;
		}
	}
	//提取两牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 2)
		{
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData;
		}
	}
	//顺子判断
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if ((i < (MAX_INDEX - 2)) && (cbCardIndexTemp[i] > 0) && ((i % 10) <= 7))
		{
			for (_uint8 j = 1; j <= cbCardIndexTemp[i]; j++)
			{
				if ((cbCardIndexTemp[i + 1] >= j) && (cbCardIndexTemp[i + 2] >= j))
				{
					cbCardIndexTemp[i]--;
					cbCardIndexTemp[i + 1]--;
					cbCardIndexTemp[i + 2]--;
					WeaveItem[cbWeaveCount].cbCardCount = 3;
					WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
					WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
					WeaveItem[cbWeaveCount++].cbCardList[2] = cbCardData + 2;
				}
			}
		}
	}
	//绞牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		bool bFind = false;
		for (_uint8 j = 0; j < cbWeaveCount; ++j)
		{
			if ((WeaveItem[j].cbCardCount == 2) && (WeaveItem[j].cbCardList[0] == WeaveItem[j].cbCardList[1]))
			{
				_uint8 cbCardIndex = SwitchToCardIndex(WeaveItem[j].cbCardList[0]);
				if (i == ((cbCardIndex + 10) % MAX_INDEX))
				{
					WeaveItem[j].cbCardCount = 3;
					WeaveItem[j].cbCardList[2] = cbCardData;
					cbCardIndexTemp[i]--;
					bFind = true;
					break;
				}
			}
		}
	}
	//两个组合
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if (cbCardIndexTemp[i] >= 1 && cbCardIndexTemp[(i + 10) % MAX_INDEX] >= 1)
		{
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[(i + 10) % MAX_INDEX]--;
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = SwitchToCardData((i + 10) % MAX_INDEX);
		}
		if (cbCardIndexTemp[i] >= 1 && ((i % 10) <= 8) && cbCardIndexTemp[i + 1] >= 1)
		{
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i + 1]--;
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 1;
		}
		if (cbCardIndexTemp[i] >= 1 && CardNum(SwitchToCardData(i)) == 0x02 && (cbCardIndexTemp[i + 5] >= 1 || cbCardIndexTemp[i + 8] >= 1))
		{
			if (cbCardIndexTemp[i + 5] >= 1)
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i + 5]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 5;
			}
			else
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i + 8]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 8;
			}
		}
		if (cbCardIndexTemp[i] >= 1 && CardNum(SwitchToCardData(i)) == 0x07 && (cbCardIndexTemp[i - 5] >= 1 || cbCardIndexTemp[i + 3] >= 1))
		{
			if (cbCardIndexTemp[i - 5] >= 1)
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i - 5]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData - 5;
			}
			else
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i + 3]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 3;
			}
		}
		if (cbCardIndexTemp[i] >= 1 && CardNum(SwitchToCardData(i)) == 0x0a && (cbCardIndexTemp[i - 3] >= 1 || cbCardIndexTemp[i - 8] >= 1))
		{
			if (cbCardIndexTemp[i - 3] >= 1)
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i - 3]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData - 3;
			}
			else
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i - 8]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData - 8;
			}
		}
	}
	//两个组合
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if (((i % 10) <= 8) && cbCardIndexTemp[i + 1] >= 1)
		{
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i + 1]--;
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 1;
		}
	}
	//如果还有剩余俩俩叠加
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		bool bFind = false;
		for (_uint8 j = 0; j < cbWeaveCount; ++j)
		{
			if (WeaveItem[j].cbCardCount == 1)
			{
				WeaveItem[j].cbCardCount = 2;
				WeaveItem[j].cbCardList[1] = cbCardData;
				cbCardIndexTemp[i]--;
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			cbCardIndexTemp[i]--;
			WeaveItem[cbWeaveCount].cbCardCount = 1;
			WeaveItem[cbWeaveCount++].cbCardList[0] = cbCardData;
		}
	}

	return true;
}

//整理最优模式牌
bool CGameLogic::SettleBestWeaves(_uint8 cbCardIndex[MAX_INDEX], tagCardsWeave WeaveItem[MAX_INDEX], _uint8 &cbWeaveCount)
{
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	cbWeaveCount = 0;
	memset(WeaveItem, 0, sizeof(tagCardsWeave)*MAX_INDEX);

	//提取四牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 4)
		{
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 4;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[2] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[3] = cbCardData;
		}
	}
	//提取三牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 3)
		{
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 3;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[2] = cbCardData;
		}
	}
	//二七十吃
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if ((cbCardData&MASK_VALUE) == 0x02)
		{
			for (_uint8 j = 0; j <= cbCardIndexTemp[i]; j++)
			{
				if ((cbCardIndexTemp[i + 5] >= 1) && (cbCardIndexTemp[i + 8] >= 1))
				{
					cbCardIndexTemp[i]--;
					cbCardIndexTemp[i + 5]--;
					cbCardIndexTemp[i + 8]--;
					WeaveItem[cbWeaveCount].cbCardCount = 3;
					WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
					WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 5;
					WeaveItem[cbWeaveCount++].cbCardList[2] = cbCardData + 8;
				}
			}
		}
	}
	//一二三吃
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if ((cbCardData&MASK_VALUE) == 0x01)
		{
			for (_uint8 j = 0; j <= cbCardIndexTemp[i]; j++)
			{
				if ((cbCardIndexTemp[i + 1] >= 1) && (cbCardIndexTemp[i + 2] >= 1))
				{
					cbCardIndexTemp[i]--;
					cbCardIndexTemp[i + 1]--;
					cbCardIndexTemp[i + 2]--;
					WeaveItem[cbWeaveCount].cbCardCount = 3;
					WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
					WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
					WeaveItem[cbWeaveCount++].cbCardList[2] = cbCardData + 2;
				}
			}
		}
	}
	//提取两牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 2)
		{
			cbCardIndexTemp[i] = 0;
			//设置组合
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData;
		}
	}
	//顺子判断
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if ((i < (MAX_INDEX - 2)) && (cbCardIndexTemp[i] > 0) && ((i % 10) <= 7))
		{
			for (_uint8 j = 1; j <= cbCardIndexTemp[i]; j++)
			{
				if ((cbCardIndexTemp[i + 1] >= j) && (cbCardIndexTemp[i + 2] >= j))
				{
					cbCardIndexTemp[i]--;
					cbCardIndexTemp[i + 1]--;
					cbCardIndexTemp[i + 2]--;
					WeaveItem[cbWeaveCount].cbCardCount = 3;
					WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
					WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
					WeaveItem[cbWeaveCount++].cbCardList[2] = cbCardData + 2;
				}
			}
		}
	}
	//绞牌
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		bool bFind = false;
		for (_uint8 j = 0; j < cbWeaveCount; ++j)
		{
			if ((WeaveItem[j].cbCardCount == 2) && (WeaveItem[j].cbCardList[0] == WeaveItem[j].cbCardList[1]))
			{
				_uint8 cbCardIndex = SwitchToCardIndex(WeaveItem[j].cbCardList[0]);
				if (i == ((cbCardIndex + 10) % MAX_INDEX))
				{
					WeaveItem[j].cbCardCount = 3;
					WeaveItem[j].cbCardList[2] = cbCardData;
					cbCardIndexTemp[i]--;
					bFind = true;
					break;
				}
			}
		}
	}
	//两个组合
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		if (cbCardIndexTemp[i] >= 1 && cbCardIndexTemp[(i + 10) % MAX_INDEX] >= 1)
		{
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[(i + 10) % MAX_INDEX]--;
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = SwitchToCardData((i + 10) % MAX_INDEX);
		}
		if (cbCardIndexTemp[i] >= 1 && ((i % 10) <= 8) && cbCardIndexTemp[i + 1] >= 1)
		{
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i + 1]--;
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 1;
		}
		if (cbCardIndexTemp[i] >= 1 && CardNum(SwitchToCardData(i)) == 0x02 && (cbCardIndexTemp[i + 5] >= 1 || cbCardIndexTemp[i + 8] >= 1))
		{
			if (cbCardIndexTemp[i + 5] >= 1)
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i + 5]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 5;
			}
			else
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i + 8]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 8;
			}
		}
		if (cbCardIndexTemp[i] >= 1 && CardNum(SwitchToCardData(i)) == 0x07 && (cbCardIndexTemp[i - 5] >= 1 || cbCardIndexTemp[i + 3] >= 1))
		{
			if (cbCardIndexTemp[i - 5] >= 1)
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i - 5]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData - 5;
			}
			else
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i + 3]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData + 3;
			}
		}
		if (cbCardIndexTemp[i] >= 1 && CardNum(SwitchToCardData(i)) == 0x0a && (cbCardIndexTemp[i - 3] >= 1 || cbCardIndexTemp[i - 8] >= 1))
		{
			if (cbCardIndexTemp[i - 3] >= 1)
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i - 3]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData - 3;
			}
			else
			{
				cbCardIndexTemp[i]--;
				cbCardIndexTemp[i - 8]--;
				WeaveItem[cbWeaveCount].cbCardCount = 2;
				WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
				WeaveItem[cbWeaveCount++].cbCardList[1] = cbCardData - 8;
			}
		}
	}
	//拼第三张
	for (_uint8 i = 0; i < cbWeaveCount; ++i)
	{
		if (WeaveItem[i].cbCardCount > 0 && WeaveItem[i].cbCardCount < 3)
		{
			_uint8 cbThirdIndex = INVALID_CARD;
			for (_uint8 j = 0; j < WeaveItem[i].cbCardCount; ++j)
			{
				_uint8 cbTempIndex = SwitchToCardIndex(WeaveItem[i].cbCardList[j]);
				if (cbCardIndexTemp[(cbTempIndex + 10) % MAX_INDEX] >= 1)
				{
					cbThirdIndex = (cbTempIndex + 10) % MAX_INDEX;
					break;
				}
				else if ((cbTempIndex % 10) <= 8 && cbCardIndexTemp[cbTempIndex + 1] >= 1)
				{
					cbThirdIndex = cbTempIndex + 1;
					break;
				}
				else if ((cbTempIndex % 10) >= 1 && cbCardIndexTemp[cbTempIndex - 1] >= 1)
				{
					cbThirdIndex = cbTempIndex - 1;
					break;
				}
				else if (CardNum(SwitchToCardData(cbTempIndex)) == 0x02 && cbCardIndexTemp[cbTempIndex + 5] >= 1) //二七十
				{
					cbThirdIndex = cbTempIndex + 5;
					break;
				}
				else if (CardNum(SwitchToCardData(cbTempIndex)) == 0x02 && cbCardIndexTemp[cbTempIndex + 8] >= 1) //二七十
				{
					cbThirdIndex = cbTempIndex + 8;
					break;
				}
				else if (CardNum(SwitchToCardData(cbTempIndex)) == 0x07 && cbCardIndexTemp[cbTempIndex - 5] >= 1) //二七十
				{
					cbThirdIndex = cbTempIndex - 5;
					break;
				}
				else if (CardNum(SwitchToCardData(cbTempIndex)) == 0x07 && cbCardIndexTemp[cbTempIndex + 3] >= 1) //二七十
				{
					cbThirdIndex = cbTempIndex + 3;
					break;
				}
				else if (CardNum(SwitchToCardData(cbTempIndex)) == 0x0a && cbCardIndexTemp[cbTempIndex - 3] >= 1) //二七十
				{
					cbThirdIndex = cbTempIndex - 3;
					break;
				}
				else if (CardNum(SwitchToCardData(cbTempIndex)) == 0x0a && cbCardIndexTemp[cbTempIndex - 8] >= 1) //二七十
				{
					cbThirdIndex = cbTempIndex - 8;
					break;
				}
			}
			if (cbThirdIndex != INVALID_CARD)
			{
				WeaveItem[i].cbCardList[WeaveItem[i].cbCardCount++] = SwitchToCardData(cbThirdIndex);
				cbCardIndexTemp[cbThirdIndex]--;
			}
		}
	}
	//拼第三张
	for (_uint8 i = 0; i < cbWeaveCount; ++i)
	{
		if (WeaveItem[i].cbCardCount > 0 && WeaveItem[i].cbCardCount < 3)
		{
			_uint8 cbThirdIndex = INVALID_CARD;
			for (_uint8 j = 0; j < WeaveItem[i].cbCardCount; ++j)
			{
				_uint8 cbTempIndex = SwitchToCardIndex(WeaveItem[i].cbCardList[j]);
				if ((cbTempIndex % 10) <= 7 && cbCardIndexTemp[cbTempIndex + 2] >= 1)
				{
					cbThirdIndex = cbTempIndex + 2;
					break;
				}
				else if ((cbTempIndex % 10) >= 2 && cbCardIndexTemp[cbTempIndex - 2] >= 1)
				{
					cbThirdIndex = cbTempIndex - 2;
					break;
				}
			}
			if (cbThirdIndex != INVALID_CARD)
			{
				WeaveItem[i].cbCardList[WeaveItem[i].cbCardCount++] = SwitchToCardData(cbThirdIndex);
				cbCardIndexTemp[cbThirdIndex]--;
			}
		}
	}
	//如果还有剩余俩俩叠加
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		bool bFind = false;
		for (_uint8 j = 0; j < cbWeaveCount; ++j)
		{
			if (WeaveItem[j].cbCardCount == 1)
			{
				WeaveItem[j].cbCardList[WeaveItem[j].cbCardCount++] = cbCardData;
				cbCardIndexTemp[i]--;
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			cbCardIndexTemp[i]--;
			WeaveItem[cbWeaveCount].cbCardCount = 1;
			WeaveItem[cbWeaveCount++].cbCardList[0] = cbCardData;
		}
	}

	return true;
}

//顺序整理方案
bool CGameLogic::SettleOrderWeaves(_uint8 cbCardIndex[MAX_INDEX], tagCardsWeave WeaveItem[MAX_INDEX], _uint8 &cbWeaveCount)
{
	_uint8 cbCardIndexTemp[MAX_INDEX];
	memcpy(cbCardIndexTemp, cbCardIndex, sizeof(cbCardIndexTemp));

	cbWeaveCount = 0;
	memset(WeaveItem, 0, sizeof(tagCardsWeave)*MAX_INDEX);

	for (_uint8 i = 0; i < MAX_INDEX / 2; ++i)
	{
		if (cbCardIndexTemp[i] == 4 || cbCardIndexTemp[i] == 3)//三牌四牌 
		{
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = cbCardIndexTemp[i];
			for (_uint8 c = 0; c < cbCardIndexTemp[i]; ++c)
			{
				WeaveItem[cbWeaveCount].cbCardList[c] = cbCardData;
			}
			cbWeaveCount++;
			cbCardIndexTemp[i] = 0;
		}
		else if (i < (MAX_INDEX / 2 - 2) && cbCardIndexTemp[i] == 1 && cbCardIndexTemp[i + 1] == 1 && cbCardIndexTemp[i + 2] == 1) //顺子
		{
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 3;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
			WeaveItem[cbWeaveCount].cbCardList[2] = cbCardData + 2;
			cbWeaveCount++;
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i + 1]--;
			cbCardIndexTemp[i + 2]--;
		}
		else if (((cbCardIndexTemp[i] == 1) && (cbCardIndexTemp[(i + 10) % MAX_INDEX] == 2)))//绞牌
		{
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 3;
			WeaveItem[cbWeaveCount].cbCardList[0] = SwitchToCardData((i + 10) % MAX_INDEX);
			WeaveItem[cbWeaveCount].cbCardList[1] = SwitchToCardData((i + 10) % MAX_INDEX);
			WeaveItem[cbWeaveCount].cbCardList[2] = cbCardData;
			cbWeaveCount++;
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[(i + 10) % MAX_INDEX] -= 2;
		}
		else if (((cbCardIndexTemp[i] == 2) && (cbCardIndexTemp[(i + 10) % MAX_INDEX]) == 1))//绞牌
		{
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 3;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[2] = SwitchToCardData((i + 10) % MAX_INDEX);
			cbWeaveCount++;
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[(i + 10) % MAX_INDEX]--;
		}
		else if (i < (MAX_INDEX / 2 - 1) && cbCardIndexTemp[i] == 1 && cbCardIndexTemp[i + 1] == 1)//顺子两个
		{
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
			cbWeaveCount++;
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i + 1]--;
		}
		else if (cbCardIndexTemp[i] == 1 && cbCardIndexTemp[(i + 10) % MAX_INDEX] == 1)
		{         
		 	bool bWeave=true;
            _uint8 cbMoreIndex=(i+10)%MAX_INDEX;
            if((cbMoreIndex%10)<=8&&cbCardIndexTemp[cbMoreIndex+1]==1) bWeave=false;
            if((cbMoreIndex%10)>=1&&cbCardIndexTemp[cbMoreIndex-1]==1) bWeave=false;
            if(bWeave)
            {    
                _uint8 cbCardData = SwitchToCardData(i);
                WeaveItem[cbWeaveCount].cbCardCount = 2; 
                WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
                WeaveItem[cbWeaveCount].cbCardList[1] = SwitchToCardData(cbMoreIndex);
                cbWeaveCount++;
                cbCardIndexTemp[i]--;
                cbCardIndexTemp[cbMoreIndex]--;
            }    
		}
		
		if (cbCardIndexTemp[i] > 0)//剩余牌
		{
			_uint8 cbCardData = SwitchToCardData(i);
			WeaveItem[cbWeaveCount].cbCardCount = cbCardIndexTemp[i];
			for (_uint8 j = 0; j < cbCardIndexTemp[i]; ++j)
			{
				WeaveItem[cbWeaveCount].cbCardList[j] = cbCardData;
			}
			cbWeaveCount++;
			cbCardIndexTemp[i] = 0;
		}

		_uint8 cbMoreIndex = (i + 10) % MAX_INDEX;
		if (cbCardIndexTemp[cbMoreIndex] == 4 || cbCardIndexTemp[cbMoreIndex] == 3)//三四牌
		{
			_uint8 cbCardData = SwitchToCardData(cbMoreIndex);
			WeaveItem[cbWeaveCount].cbCardCount = cbCardIndexTemp[cbMoreIndex];
			for (_uint8 c = 0; c < cbCardIndexTemp[cbMoreIndex]; ++c) WeaveItem[cbWeaveCount].cbCardList[c] = cbCardData;
			cbWeaveCount++;
			cbCardIndexTemp[cbMoreIndex] = 0;
		}
		else if (cbMoreIndex < (MAX_INDEX - 2) && cbCardIndexTemp[cbMoreIndex] == 1 && cbCardIndexTemp[cbMoreIndex + 1] == 1 && cbCardIndexTemp[cbMoreIndex + 2] == 1)//顺子
		{
			_uint8 cbCardData = SwitchToCardData(cbMoreIndex);
			WeaveItem[cbWeaveCount].cbCardCount = 3;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
			WeaveItem[cbWeaveCount].cbCardList[2] = cbCardData + 2;
			cbWeaveCount++;
			cbCardIndexTemp[cbMoreIndex]--;
			cbCardIndexTemp[cbMoreIndex + 1]--;
			cbCardIndexTemp[cbMoreIndex + 2]--;
		}
		else if (cbMoreIndex < (MAX_INDEX - 1) && cbCardIndexTemp[cbMoreIndex] == 1 && cbCardIndexTemp[cbMoreIndex + 1] == 1)//顺子两个
		{
			_uint8 cbCardData = SwitchToCardData(cbMoreIndex);
			WeaveItem[cbWeaveCount].cbCardCount = 2;
			WeaveItem[cbWeaveCount].cbCardList[0] = cbCardData;
			WeaveItem[cbWeaveCount].cbCardList[1] = cbCardData + 1;
			cbWeaveCount++;
			cbCardIndexTemp[cbMoreIndex]--;
			cbCardIndexTemp[cbMoreIndex + 1]--;
		}
		
		if (cbCardIndexTemp[cbMoreIndex] > 0)//剩余牌
		{
			_uint8 cbCardData = SwitchToCardData(cbMoreIndex);
			WeaveItem[cbWeaveCount].cbCardCount = cbCardIndexTemp[cbMoreIndex];
			for (_uint8 j = 0; j < cbCardIndexTemp[cbMoreIndex]; ++j)
			{
				WeaveItem[cbWeaveCount].cbCardList[j] = cbCardData;
			}
			cbWeaveCount++;
			cbCardIndexTemp[cbMoreIndex] = 0;
		}
	}

	//如果还有剩余俩俩叠加
	for (_uint8 i = 0; i < MAX_INDEX; i++)
	{
		if (cbCardIndexTemp[i] == 0) continue;
		_uint8 cbCardData = SwitchToCardData(i);
		bool bFind = false;
		for (_uint8 j = 0; j < cbWeaveCount; ++j)
		{
			if (WeaveItem[j].cbCardCount == 1)
			{
				WeaveItem[j].cbCardCount = 2;
				WeaveItem[j].cbCardList[1] = cbCardData;
				cbCardIndexTemp[i]--;
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			cbCardIndexTemp[i]--;
			WeaveItem[cbWeaveCount].cbCardCount = 1;
			WeaveItem[cbWeaveCount++].cbCardList[0] = cbCardData;
		}
	}

	return true;
}

