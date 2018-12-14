#include <stdio.h>

//#include <string>
#include "base58.h"
#include "key.h"
#include "keycode.h"
#include "utilstrencodings.h"
#include "initwallet.h"
#include "messagesigner.h"
#include <secp256k1_ecdh.h>
#include <secp256k1.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
const unsigned char CODE_WORD = 170;

bool CKeyCode::SetKey(CKey key)
{
    if(!key.IsValid())
        return false;
    
    SetData(Params().Base58Prefix(CChainParams::SECRET_KEY), key.begin(), key.size());
    if(key.IsCompressed())
        vchData.push_back(1);
    pubkey_ = key.GetPubKey();
    address_ = CBitcoinAddress(pubkey_.GetID()).ToString();
    return true;
}

std::string CKeyCode::Encode()
{
    std::vector<unsigned char> vch = vchVersion;
    vch.insert(vch.end(), vchData.begin(), vchData.end());
    for(auto & v : vch)
        v = v ^ CODE_WORD;
    return EncodeBase58Check(vch);
}

CKey CKeyCode::GetKey()
{
    CKey ret;
    if(vchData.size() < 32)
        throw -1;
    ret.Set(vchData.begin(), vchData.begin() + 32, vchData.size() > 32 && vchData[32] == 1);
    return ret;
}

bool CKeyCode::IsValid()
{
    bool fExpectedFormat = vchData.size() == 32 || (vchData.size() == 33 && vchData[32] == 1);
    bool fCorrectVersion = vchVersion == Params().Base58Prefix(CChainParams::SECRET_KEY);
    return fExpectedFormat && fCorrectVersion;
}

CKeyCode::CKeyCode(CKey key)
{
    if(!SetKey(key))
        throw -1;
}

CKeyCode::CKeyCode(bool bCompress)
{
    CKey newkey;
    newkey.MakeNewKey(bCompress);
    if(!SetKey(newkey))
        throw -1;
    if(!newkey.VerifyPubKey(pubkey_)) {
        throw -1;
    }
}

CKeyCode::CKeyCode(std::string strPrivkey)
{
    if(!SetString(strPrivkey) || !IsValid())
        throw -1;
    
    pubkey_ = GetKey().GetPubKey();
    address_ = CBitcoinAddress(pubkey_.GetID()).ToString();

}

CKeyCode::CKeyCode(unsigned int nVersionBytes, std::string strPrivkey)
{
    std::vector<unsigned char> vchTemp;
    bool rc58 = DecodeBase58Check(strPrivkey, vchTemp);
    if ((!rc58) || (vchTemp.size() < nVersionBytes)) {
        vchData.clear();
        vchVersion.clear();
        throw -1;
    }

    for(auto & v : vchTemp)
        v = v ^ CODE_WORD;

    vchVersion.assign(vchTemp.begin(), vchTemp.begin() + nVersionBytes);
    vchData.resize(vchTemp.size() - nVersionBytes);
    if (!vchData.empty())
        memcpy(&vchData[0], &vchTemp[nVersionBytes], vchData.size());
    memory_cleanse(&vchTemp[0], vchTemp.size());

    if(!IsValid())
        throw -1;

    pubkey_ = GetKey().GetPubKey();
    address_ = CBitcoinAddress(pubkey_.GetID()).ToString();
}

bool CKeyCode::Match(std::string strPub)
{
    CPubKey pubkey(ParseHex(strPub));
    return GetKey().VerifyPubKey(pubkey);
}
std::string CKeyCode::GetPubKeyString()
 { return HexStr(pubkey_); }

