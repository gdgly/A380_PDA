#ifndef DbQueryOptimize_H
#define DbQueryOptimize_H

#include "Common.h"
#include "MeterDocDBF.h"

// ��ڵ�
typedef struct _Mnode
{
    struct _Mnode * Next;

    uint32 DbIdx;       // dbf ��¼����
}Mnode;

// ¥���ڵ�
typedef struct _Bnode
{
    struct _Bnode * Next;
    struct _Mnode * Mhead;

    char BuildNum[12];  // ¥�����
}Bnode;


// С���ڵ�
typedef struct _Cnode
{
    struct _Bnode * BHead;

    char CellNum[12];   // С�����
}Cnode;

// �Ż���ѯ�ṹ
typedef struct _DbQuerySt
{
    Cnode Cells[District_Max];  // С���б�
    uint16 CellCnt;             // С������
    uint16 CellIdx;     // С������
    uint16 BuildIdx;    // ¥������
    uint16 MeterIdx;    // �������

    uint32	dbCurrIdx;	// ���ݿ⵱ǰλ��
	uint32	reqMaxCnt;	// ���������
	uint32	resultCnt;	// ��ѯ�Ľ����¼��
	uint16	errorCode;	// 0 - ok,  ���� - ����
}DbQuerySt;



extern void QueryDistrictList(DistrictListSt *districts, DbQuerySt *query);
extern void QueryBuildingList(BuildingListSt *buildings, DbQuerySt *query);
extern void QueryMeterList(MeterListSt *meters, DbQuerySt *query);

#endif