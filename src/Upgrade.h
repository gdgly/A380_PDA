#ifndef Upgrade_H
#define Upgrade_H

#include "Common.h"

// bin�ļ��� ǰ128byte��ʵ��ʹ��69byte�������������������Ϣ
typedef struct 
{
    char    appVer[40];
    uint8   packetCnt[2];
    uint8   crc16_1st26K[2];
    uint8   crc16_2nd26K[2];
    uint8   crc16_all52K[2];
    uint8   faildReason;
    uint8   meterId[6];
    uint8   devType;
    uint8   rfParam[5];    // RF�շ�Ƶ�� 3byte���ŵ����� 1byte ����Ƶ���� 1byte
    uint8   rxdChanel;
    uint8   waterMeterVbat;
    uint8   gasMeterVbat;
    uint8   snr;
    uint8   rssi;
    uint8   upgradeStatus;
    uint8   crc16_appVer[2];
}UpgradeInfo;



#endif