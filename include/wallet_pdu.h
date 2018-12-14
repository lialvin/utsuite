#ifndef _WALLET_PDU_H_
#define _WALLET_PDU_H

#pragma pack(1)
typedef struct
{
    short      dataLen ;        //整个包长，包括包头和包尾8B
    short      cmdType;           //TETRA接入服务类型， 0 ping  1 biz1    
    union
    {
        char szData[4];         //正常包的数据实际长度
    }u;
}WALLET_PKG_HEAD;



enum WALLET_CMD_TYPE
{
    CMD_GET_WALLET           = 0,
    CMD_GET_PRIVATE_KEY      = 1 //扫描目录

};

typedef  struct
{
    char  pubkey[50]; // 名称 目录名或者文件名 0结尾
    char  privatekey[256]; // 名称 目录名或者文件名 0结尾
    char  filename[50];
}WALLET_PDU;

#pragma pack()

#endif
