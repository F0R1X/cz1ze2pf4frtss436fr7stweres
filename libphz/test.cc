#include "Common.h"
#include "GameLogic.h"
#include <iostream>

int main(int argc, char *argv[])
{
	//吃
	CGameLogic m_GameLogic;
	/*_uint8 cbCardData[] = {0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x17, 0x17};
	_uint8 cbCardIndex[MAX_INDEX];
	memset( cbCardIndex, 0, sizeof(cbCardIndex) );
	for( _uint8 i = 0; i < sizeof(cbCardData); ++i )
	{
		cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData[i])]++;
	}

	tagNewChiCardInfo ChiCardInfo[6];
	memset( ChiCardInfo, 0, sizeof(ChiCardInfo) );
	_uint8 cbCurrentCard = 0x07;
	_uint8 cbChiCardCount = m_GameLogic.GetChiCardInfo( cbCardIndex, cbCurrentCard, ChiCardInfo );
	printf( "main cbChiCardCount[%d] .\n", cbChiCardCount );
	printf( "----------------------------------------------------------------------------------------------------.\n");
	
	for( _uint8 i = 0; i < cbChiCardCount; ++i )
	{
		printf( "One cbChiKind[0x%02x] cbCenterCard[0x%02x] cbCardData0[0x%02x] cbCardData1[0x%02x] cbCardData2[0x%02x] cbSecondCount[%d].\n",
					ChiCardInfo[i].cbChiKind, ChiCardInfo[i].cbCenterCard, ChiCardInfo[i].cbCardData[0],
					ChiCardInfo[i].cbCardData[1], ChiCardInfo[i].cbCardData[2], ChiCardInfo[i].cbSecondCount );
		for( _uint8 j = 0; j < ChiCardInfo[i].cbSecondCount; ++j )
		{
			printf( "Two cbChiKind[0x%02x] cbCenterCard[0x%02x] cbCardData0[0x%02x] cbCardData1[0x%02x] cbCardData2[0x%02x] cbThirdCount[%d].\n",
					ChiCardInfo[i].SecondData[j].cbChiKind, ChiCardInfo[i].SecondData[j].cbCenterCard, ChiCardInfo[i].SecondData[j].cbCardData[0],
					ChiCardInfo[i].SecondData[j].cbCardData[1], ChiCardInfo[i].SecondData[j].cbCardData[2], ChiCardInfo[i].SecondData[j].cbThirdCount );
			for( _uint8 k = 0; k < ChiCardInfo[i].SecondData[j].cbThirdCount; ++k )
			{
				printf( "Three cbChiKind[0x%02x] cbCenterCard[0x%02x] cbCardData0[0x%02x] cbCardData1[0x%02x] cbCardData2[0x%02x] .\n",
					ChiCardInfo[i].SecondData[j].ThirdData[k].cbChiKind, ChiCardInfo[i].SecondData[j].ThirdData[k].cbCenterCard,
					ChiCardInfo[i].SecondData[j].ThirdData[k].cbCardData[0], ChiCardInfo[i].SecondData[j].ThirdData[k].cbCardData[1],
					ChiCardInfo[i].SecondData[j].ThirdData[k].cbCardData[2] );
			}
		}

		printf( "----------------------------------------------------------------------------------------------------.\n");
	}*/

	tagCardsWeave CardsWeave[MAX_INDEX];
	_uint8 cbCardsWeaveCount = 8;
	_uint8 cbRemoveCards[4] = {18,18,18,0};
	_uint8 cbRemoveCount = 2;

	CardsWeave[0].cbCardCount = 2;
	CardsWeave[0].cbCardList[0] = 4;
	CardsWeave[0].cbCardList[1] = 4;

	CardsWeave[1].cbCardCount = 2;
	CardsWeave[1].cbCardList[0] = 6;
	CardsWeave[1].cbCardList[1] = 6;

	CardsWeave[2].cbCardCount = 2;
	CardsWeave[2].cbCardList[0] = 8;
	CardsWeave[2].cbCardList[1] = 8;

	CardsWeave[3].cbCardCount = 2;
	CardsWeave[3].cbCardList[0] = 10;
	CardsWeave[3].cbCardList[1] = 10;

	CardsWeave[4].cbCardCount = 3;
	CardsWeave[4].cbCardList[0] = 18;
	CardsWeave[4].cbCardList[1] = 20;
	CardsWeave[4].cbCardList[2] = 17;

	CardsWeave[5].cbCardCount = 1;
	CardsWeave[5].cbCardList[0] = 21;

	CardsWeave[6].cbCardCount = 2;
	CardsWeave[6].cbCardList[0] = 22;
	CardsWeave[6].cbCardList[1] = 22;

	CardsWeave[7].cbCardCount = 3;
	CardsWeave[7].cbCardList[0] = 18;
	CardsWeave[7].cbCardList[1] = 26;
	CardsWeave[7].cbCardList[2] = 23;

	printf( "cbCardsWeaveCount[%d] .\n", cbCardsWeaveCount );
	for( _uint8 i = 0; i < cbCardsWeaveCount; ++i )
	{
		printf("cbCardCount[%d].\n", CardsWeave[i].cbCardCount );
		for( _uint8 j = 0; j < CardsWeave[i].cbCardCount; ++j )
		{
			printf("cards[%d] .\n", CardsWeave[i].cbCardList[j] );
		}
	}

	printf("---------------------------------------------------------.\n");
	m_GameLogic.UpdateCardsWeave( CardsWeave, cbCardsWeaveCount, cbRemoveCards, cbRemoveCount );

	printf( "cbCardsWeaveCount[%d] .\n", cbCardsWeaveCount );
	for( _uint8 i = 0; i < cbCardsWeaveCount; ++i )
	{
		printf("cbCardCount[%d].\n", CardsWeave[i].cbCardCount );
		for( _uint8 j = 0; j < CardsWeave[i].cbCardCount; ++j )
		{
			printf("cards[%d] .\n", CardsWeave[i].cbCardList[j] );
		}
	}

	cbRemoveCards[0] = 22;
	cbRemoveCards[1] = 22;
	cbRemoveCount = 2;

	printf("---------------------------------------------------------.\n");
	m_GameLogic.UpdateCardsWeave( CardsWeave, cbCardsWeaveCount, cbRemoveCards, cbRemoveCount );

	printf( "cbCardsWeaveCount[%d] .\n", cbCardsWeaveCount );
	for( _uint8 i = 0; i < cbCardsWeaveCount; ++i )
	{
		printf("cbCardCount[%d].\n", CardsWeave[i].cbCardCount );
		for( _uint8 j = 0; j < CardsWeave[i].cbCardCount; ++j )
		{
			printf("cards[%d] .\n", CardsWeave[i].cbCardList[j] );
		}
	}
	return 0;
}
