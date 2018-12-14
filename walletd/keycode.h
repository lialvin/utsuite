#ifndef UTKEYCODE__H
#define UTKEYCODE__H 
#include "base58.h"

class CKeyCode : public CBase58Data
{
private:
    CPubKey pubkey_;
    std::string address_;
    bool SetKey(CKey key);
public:
    CKeyCode(CKey key);
    CKeyCode(bool bCompress);
    CKeyCode(std::string strPrivkey);
	CKeyCode(unsigned int nVersionBytes, std::string strPrivkey);
    CKey GetKey();
    bool IsValid();
    CPubKey GetPubKey() { return pubkey_; }
    std::string GetPubKeyString(); 
    std::string GetAddress() { return address_; }
    bool Match(std::string strPub);
	std::string Encode();
};

#endif // 

