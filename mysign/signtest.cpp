#include <stdio.h>
#include "include/a.h"
#include "readcfg.h"
#include "signtest.h"
#include <string>
#include "arith_uint256.h"
#include "key.h"
#include "utilstrencodings.h"
#include "initwallet.h"
#include "messagesigner.h"
#include <secp256k1_ecdh.h>
#include <secp256k1.h>
#include "base58.h"
#include <iostream>

//#include <secp256k1_recovery.h>

using namespace std;
//ulord test
bool testverify_ulord()
{
   std::string strmessage="test sign message ,user signed name string 2018 01 23";


   std::string strErrorRet;
   //messSign.SignMessage(strmessage, vchSigRet, key);
   std::string vchstr("20d804557358a0c1848680cd388c95d792c1c5a7a3136b2f4cf3c17e263625d96349726a8218fd9c936437c7618d42e0b03459bfd37ebf71dea0ab7ced6feec920");

   CHashWriter ss(SER_GETHASH, 0);
   ss << string("LordCoin Signed Message:\n");
   ss << strmessage;
   std::vector<unsigned char> vchSigRet(ParseHex(vchstr));

   CPubKey  pubkey(ParseHex("03e867486ebaeeadda25f1e47612cdaad3384af49fa1242c5821b424937f8ec1f5"));
   CPubKey pubkey2;
   if (!pubkey2.RecoverCompact(ss.GetHash(), vchSigRet))
        return false;

   bool retflag=  (pubkey2.GetID() == pubkey.GetID());

   if(retflag==true)
   {
       return true;
   }
   else
   {
        return false;
   }
}

double GetDifficulty(unsigned int nBits )
{
    // Floating point number that is a multiple of the minimum difficulty,
    // minimum difficulty = 1.0.

    int nShift = (nBits >> 24) & 0xff;

    double dDiff =
        (double)0x0000ffff / (double)(nBits & 0x00ffffff);

    while (nShift < 29)
    {
        dDiff *= 256.0;
        nShift++;
    }
    while (nShift > 29)
    {
        dDiff /= 256.0;
        nShift--;
    }

    return dDiff;
}

void fun()
{
    arith_uint256  a("00ffffffff000000000000000000000000000000000000000000000000000000");
    
    std::cout << "pow limit : " << GetDifficulty(a.GetCompact() ) << "   "   << hex<<  a.GetCompact() << std::endl;
    arith_uint256 a1("000000ffffffff00000000000000000000000000000000000000000000000000");
    std::cout << "pow limit : " << GetDifficulty(a1.GetCompact() ) <<"   "  << hex << a1.GetCompact() << std::endl;
    arith_uint256 a2("0000000000ffffffff0000000000000000000000000000000000000000000000");
    std::cout << "pow limit : " <<GetDifficulty(a2.GetCompact() ) << "   " << hex<<  a2.GetCompact() << std::endl;
    arith_uint256 a3("000000000000000000000000000000000000000000000000ffffffff00000000");
    std::cout << "pow limit : " <<GetDifficulty(a3.GetCompact() ) << "   " << hex<<  a3.GetCompact() <<"  "<<  std::endl;
    arith_uint256 a4("00000000000000000000000000000000000000000000000000002233ffff3f10");
    std::cout << "pow limit : " <<GetDifficulty(a4.GetCompact() ) << "   " << hex<<  a4.GetCompact() << std::endl;
}

int main(int argc, char* argv[])
{

   string  dbuser ,dbhost, dbpass;
   int startheight, endheight;
   string privkey,pubkeystr; 

   //fun();

   readcfg(startheight,endheight,dbuser,dbhost,dbpass,privkey,pubkeystr);
   string strmessage="test sign message ,user signed name string 2018 01 23";
  
   char szTmpDate[40]={0};
   char szTmpTime[20]={0}; 
   sprintf(szTmpDate,"%s",__DATE__); //"Sep 18 2010"
   sprintf(szTmpTime,"%s",__TIME__);    //"10:59:19"
   cout<<" TmpDate  "<<  szTmpDate<< " Time   " << szTmpTime << endl; 
   
   char  netparams[20]=""; 
   if( argc>2  )
   {
       cout<< "main :"  <<endl;
       strcpy(netparams,"main");
   }
   else
   {
       cout<< "test :"  <<endl;
       strcpy(netparams,"test");
   }

   Secp256k1Init ecc(netparams);


   bool testret = testverify2();   
   testret = testverify();   
  
   if(argc>1)
   {
       string newpriv, newpub ; 
       NewKeyString( newpriv, newpub);
       cout<< "privkey:" << newpriv<<endl;
       cout<< "pubkey:" << newpub <<endl;
       CBitcoinAddress  bitAddress;
       CPubKey  addrpubkey(ParseHex(newpub));
       bitAddress.Set(addrpubkey.GetID());
       cout<< "pubaddr:" << bitAddress.ToString()<<endl;
       return 0; 
   }

   CKey keyRet;
   CPubKey pubkeyRet;
   GetKeyFromString(  keyRet,  pubkeyRet,  privkey,   pubkeystr ) ;    
    
   CMessageSigner messSign;
   CMessageSigner messVery;     

   std::vector<unsigned char> vchSigRet;
   std::vector<unsigned char> vchkey;
   
   string strErrorRet;
   messSign.SignMessage(strmessage, vchSigRet, keyRet);
   cout<<"vchsigret "<< HexStr(vchSigRet)<<endl;

   CPubKey pubkeyVery;

   GetPubKeyFromString(  pubkeyVery,  pubkeystr)  ; 

   bool retflag= messVery.VerifyMessage( pubkeyVery,  vchSigRet,  strmessage,  strErrorRet);   
   if(retflag==true)
   {
       cout<<"very success"<<endl;
   }
   else
   {
       cout<<"very failed "<< strErrorRet<<endl;
   }
   char * p; 
   char szBuf[128];
   system("pause");

   return 0;
}

