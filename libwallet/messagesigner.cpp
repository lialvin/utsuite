// Copyright (c) 2014-2017 The Ulord Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "base58.h"
#include "hash.h"
#include "validation.h" // For strMessageMagic
#include "messagesigner.h"
#include "tinyformat.h"
#include "utilstrencodings.h"


//new generate key pair and return;
void NewKeyString(std::string & privstr, std::string & pubkeystr)
{
	CKey  key;
	
	key.MakeNewKey(true );
	
	CBitcoinSecret	newkey;
	newkey.SetKey(key);
        CPubKey  pubkey= key.GetPubKey();
	
	CBitcoinAddress  bitAddress;
	bitAddress.Set(pubkey.GetID());
	
/*	cout<< "privkey:" << newkey.ToSting()<<endl;
	cout<< "pubaddr:" << bitAddress.ToString()<<endl;
	cout<< "pubkey:" << HexStr(pubkey) <<endl;
*/
	privstr = newkey.ToString();
	pubkeystr = HexStr(pubkey);
	
}


bool testverify2()
{
   std::string strmessage="test sign message ,user signed name string 2018 01 23";

    CKey  key;
    key.MakeNewKey(true );

    CBitcoinSecret  newkey;
    newkey.SetKey(key);
    CPubKey  pubkey= key.GetPubKey();

   //CMessageSigner messSign;
   //CMessageSigner messVery;

   std::vector<unsigned char> vchSigRet;
   std::vector<unsigned char> vchkey;

   std::string strErrorRet;
   //messSign.SignMessage(strmessage, vchSigRet, key);
   CHashWriter ss(SER_GETHASH, 0);
   ss << strMessageMagic;
   ss << strmessage;

   if (!key.SignCompact(ss.GetHash(), vchSigRet))
   {  
      strErrorRet ="sign err";
      return  false;
   }

   CHashWriter ss1(SER_GETHASH, 0);
   ss1 << strMessageMagic;
   ss1 << strmessage;

   CPubKey pubkey2;
   if (!pubkey2.RecoverCompact(ss1.GetHash(), vchSigRet))
      return false;

   bool retflag=  (pubkey2.GetID() == pubkey.GetID());

   if(retflag==true)
   {
      return true;
   }
   return false;
}

bool  testverify()
{
   std::string strmessage="test sign message ,user signed name string 2018 01 23";

    CKey  key;
        
    key.MakeNewKey(true );
       
    CBitcoinSecret  newkey;
    newkey.SetKey(key);
    CPubKey  pubkey= key.GetPubKey();


   CMessageSigner messSign;
   CMessageSigner messVery;

   std::vector<unsigned char> vchSigRet;
   std::vector<unsigned char> vchkey;

   std::string strErrorRet;
   messSign.SignMessage(strmessage, vchSigRet, key);

   bool retflag= messVery.VerifyMessage( pubkey,  vchSigRet,  strmessage,  strErrorRet);
   if(retflag==true)
   {
      return true; 
   }
   else
   {
      return false;
   }
}

//get key  from 
void GetKeyFromString( CKey& keyRet, CPubKey& pubkeyRet,std::string  privstr, std::string  pubkeystr )
{
    CMessageSigner stemp;

    stemp.GetKeysFromSecret(privstr,  keyRet, pubkeyRet);
}

void GetPubKeyFromString(  CPubKey& pubkeyRet, std::string  pubkeystr )
{
	
   CPubKey  pubkey(ParseHex(pubkeystr));
   pubkeyRet = pubkey;
	
}

bool CMessageSigner::GetKeysFromSecret(const std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet)
{
    CBitcoinSecret vchSecret;

    if(!vchSecret.SetString(strSecret)) return false;

    keyRet = vchSecret.GetKey();
    pubkeyRet = keyRet.GetPubKey();

    return true;
}

bool CMessageSigner::SignMessage(const std::string strMessage, std::vector<unsigned char>& vchSigRet, const CKey key)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strMessage;

    return CHashSigner::SignHash(ss.GetHash(), key, vchSigRet);
}

bool CMessageSigner::SignMessage(const std::string strMessage, const int64_t timestamps, std::vector<unsigned char>& vchSigRet, const CKey key)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strMessage;
    ss << timestamps;

    return CHashSigner::SignHash(ss.GetHash(), key, vchSigRet);
}
bool CMessageSigner::VerifyMessage(const CPubKey pubkey, const std::vector<unsigned char>& vchSig, const std::string strMessage, std::string& strErrorRet)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strMessage;

    return CHashSigner::VerifyHash(ss.GetHash(), pubkey, vchSig, strErrorRet);
}

bool CHashSigner::SignHash(const uint256& hash, const CKey key, std::vector<unsigned char>& vchSigRet)
{
    return key.SignCompact(hash, vchSigRet);
}

bool CHashSigner::VerifyHash(const uint256& hash, const CPubKey pubkey, const std::vector<unsigned char>& vchSig, std::string& strErrorRet)
{
    CPubKey pubkeyFromSig;
    if(!pubkeyFromSig.RecoverCompact(hash, vchSig)) {
        strErrorRet = "Error recovering public key.";
        return false;
    }

    if(pubkeyFromSig.GetID() != pubkey.GetID()) {
        strErrorRet = strprintf("Keys don't match: pubkey=%s, pubkeyFromSig=%s, hash=%s, vchSig=%s",
                    pubkey.GetID().ToString(), pubkeyFromSig.GetID().ToString(), hash.ToString(),
                    EncodeBase64(&vchSig[0], vchSig.size()));
        return false;
    }

    return true;
}
