#include <stdio.h>
#include "include/a.h"
#include "readcfg.h"
#include "cmdmsg.h"
#include <string>
#include "loopbuf.h"
#include "wallet_pdu.h"
#include "keycode.h"
#include "streams.h"
#include "keystore.h"
#include "script/sign.h"
#include "policy/policy.h"

#include "key.h"
#include "utilstrencodings.h"
#include "initwallet.h"
#include "messagesigner.h"
#include <secp256k1_ecdh.h>
#include <secp256k1.h>
#include "base58.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>  
#include <chrono>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/assign/list_of.hpp>
using namespace std;

// sign mnp message 
string EncodeHexTx(const CTransaction& tx)
{
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;
    return HexStr(ssTx.begin(), ssTx.end());
}
std::string  signmnpmessage(std::string strPrivKey , std::string strMasterKey, std::string straddr)
{

    std::string strMessage;
    std::string strError = "";


    //CNetAddr netAddr(straddr);
    //CService Ipaddr(netAddr,masterport);

    CKey         keyCollate;
    CPubKey      pubkeyCollate;

    CKey         keyMaster;
    CPubKey      pubkeyMaster;
    // std::string  privstr;
    //std::string        pubkeystr;
    CMessageSigner privSigner;
    if(!privSigner.GetKeysFromSecret(strPrivKey,  keyCollate, pubkeyCollate))
                throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid strPrivkey  . Please useing the correct Key.");

    if(!privSigner.GetKeysFromSecret(strMasterKey,  keyMaster, pubkeyMaster))
                throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid strPrivkey  . Please useing the correct Key.");


    CBitcoinSecret vchSecret;
    vchSecret.SetKey(keyMaster);
    //if(!vchSecret.SetString(strMasterKey)) return false;
    //cout<<  vchSecret.ToString()<<endl ; 

    cout << HexStr(pubkeyCollate)<<"  ==pubkey==   " << CBitcoinAddress(pubkeyCollate.GetID()).ToString() <<endl;

    //strMessage = straddr+":"+strPort + pubkeyCollate.GetID().ToString() + pubkeyMaster.GetID().ToString() +
    strMessage = straddr+ pubkeyCollate.GetID().ToString() + pubkeyMaster.GetID().ToString() +
                        boost::lexical_cast<std::string>(PROTOCOL_VERSION);
//    "10.0.0.1:5600ad69a2fff96bb922287d041c5eacd3c6fe84a35ce41967bd2996a7e37020c0afeb9c4c527f48502970206" 
    cout<< strMessage<< endl;

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strMessage;

   
    vector<unsigned char> vchSig;
    if (!keyCollate.SignCompact(ss.GetHash(), vchSig))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Sign failed");

    std::string str_err;
    if( privSigner.VerifyMessage(pubkeyCollate, vchSig, strMessage , str_err) )
    {
       cout<< " VerifyMessage  success " << endl;
    }
    else
    {
       cout<< " VerifyMessage error failed  " << HexStr(ss.GetHash())<< endl;
    }


    CHashSigner hashSigner;
    CHashWriter ss1(SER_GETHASH, 0);
    ss1 << strMessageMagic;
    ss1 << strMessage;
    
    uint256 hash1= ss1.GetHash();
    if( hashSigner.VerifyHash(hash1,  pubkeyCollate, vchSig,  str_err))
    {
       cout<< " VerifyMessage hash  success " << endl;
    }
    else
    {
       cout<< " VerifyMessage hash error failed  " << HexStr(ss.GetHash())<< endl;
    }


    CHashWriter ss2(SER_GETHASH, 0);
    std::string  strMessage2=strMessageMagic+strMessage; 
    ss2<< strMessage2 ;

    CHashWriter ss3(SER_GETHASH, 0);
    CHashWriter ss4(SER_GETHASH, PROTOCOL_VERSION);
    std::string strmsg3("this is a test ,this is a test , this is a test !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    ss3<<strmsg3;
    ss4<<strmsg3;

    return EncodeBase64(&vchSig[0], vchSig.size());

}

std::string transutaddr( std::string uoshex)
{
    std::size_t pos = uoshex.find("UOS");
    if (pos!=std::string::npos  )
        return transfromuosb58(uoshex); 

    std::vector<unsigned char> vchIn =  ParseHex(uoshex);  
    char chHeader= vchIn[0];
    size_t len=0;
    if (chHeader == 2 || chHeader == 3)
            len= 33;
    if (chHeader == 4 || chHeader == 6 || chHeader == 7)
          len=  65;
    std::vector<unsigned char> vchIn2(vchIn.begin(),vchIn.begin()+len);
    CPubKey pubkey(vchIn2);  
    std::cout<<"ut pub key "<<HexStr( pubkey)<<std::endl;    

    CKeyID vchAddress = pubkey.GetID();

    CBitcoinAddress  bitAddress;
    bitAddress.Set(pubkey.GetID());
    std::string addr = bitAddress.ToString();
    return addr;
}

std::string transfromuosb58( std::string uos58)
{
  std::size_t pos = uos58.find("UOS");
  if (pos==std::string::npos || pos!=0 )
  {
     return std::string("pubkey error");
  }

  std::string sub_str = uos58.substr(pos+3);
  std::vector<unsigned char> vchIn2;
  DecodeBase58(sub_str, vchIn2 );
  //std::cout<<"ut pub key "<<HexStr( vchIn2)<<std::endl;
  std::string hexstr= HexStr( vchIn2);
  return transutaddr( hexstr);
}

std::string  signlicmessage(std::string strPrivKey,std::string strTxid,std::string strMasterKey,int64_t idate,int txididx)
{
    std::string strError = "";
    std::string str_err = "";

    CMessageSigner privSigner;
    CPubKey pubkeyMaster; 
    CKey keyMaster;
    if(!privSigner.GetKeysFromSecret(strMasterKey,  keyMaster, pubkeyMaster))
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid strPrivkey  . Please useing the correct Key.");

    CKeyCode privkey(1,strPrivKey);
    CKey signkey = privkey.GetKey();
    
    vector<unsigned char> vchSig;

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strTxid;
    ss << txididx;
    ss << pubkeyMaster;
    ss << idate;
    ss << 1;

    uint256 hash = ss.GetHash();

    if(!signkey.SignCompact(hash, vchSig)){
       cout<< " signMessage hash  failed" << endl;
       return std::string("false");
    }
    std::string license = EncodeBase64(&vchSig[0], vchSig.size());


    CHashSigner hashSigner;
    CHashWriter ss1(SER_GETHASH, 0);
    ss1 << strMessageMagic;
    ss1 << strTxid;
    ss1 << txididx;
    ss1 << pubkeyMaster;
    ss1 << idate;
    ss1 << 1;

    CPubKey pubkeyuc(ParseHex(std::string("03e947099921ee170da47a7acf48143c624d33950af362fc39a734b1b3188ec1e3"))); 
    uint256 hash1= ss1.GetHash();
    if( hashSigner.VerifyHash(hash1, pubkeyuc, vchSig,  str_err))
    {
       cout<< " VerifyMessage hash  success " << endl;
    }
    else
    {
       cout<< " VerifyMessage hash error failed  " << HexStr(ss.GetHash())<< endl;
    }

    return license;
}

void  producekey(string  str_count )
{
    unsigned int count = boost::lexical_cast<unsigned int>(str_count);

    auto tp = std::chrono::system_clock::now();
    std::time_t cur_time = std::chrono::system_clock::to_time_t(tp);
    std::string str_time = std::ctime(&cur_time);
    std::cout<<str_time<<std::endl;

    string pubkey ,addr;

    for(int i  =0;i<count;i++)
    {
       string addr = getnewaddress("", pubkey);
    }

    cout<<"get new address addr=  "<<addr << " pubkey  " << pubkey<<endl;

    auto tp_end = std::chrono::system_clock::now();
    std::time_t cur_time_end = std::chrono::system_clock::to_time_t(tp_end);
    std::string str_time_end = std::ctime(&cur_time_end);
    std::cout<<str_time_end<<std::endl;

}

void getjsonkey(string param,vector<string>&keyid )
{
    string str_json= param;
   // cout<< str_json<<endl;
    
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    boost::property_tree::ptree::iterator root_it = root.begin();
    
    for(; root_it != root.end(); ++root_it)
    {
        string key = root_it->first;
        cout<<"key =="<< key<<endl;
        if(key==string(""))
        {
             boost::property_tree::ptree key_node = root.get_child(key);
             boost::property_tree::ptree::iterator key_node_it = key_node.begin();
             
             for(; key_node_it != key_node.end(); ++key_node_it)
             {
                cout<<"key"<<key_node_it->first << endl;
                //cout << tkt_node_it->first << ":"<< tkt_node.get<string>(tkt_node_it->first) <<endl;
             }
        } 
    }
}
//test msg
void gettx(string json_str,vector<string>&keyid,vector<string>& value )
{
    string str_json= json_str;
   // cout<< str_json<<endl;

    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    boost::property_tree::ptree::iterator root_it = root.begin();

    for(; root_it != root.end(); ++root_it)
    {
       
        string key = root_it->first;
        //cout<< key<<endl;
        if(key==string(""))
        {
             boost::property_tree::ptree tkt_node = root.get_child(key);
             boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();

             cout<<"txid find "<<tkt_node_it->first << endl;      
             //cout << tkt_node_it->first << ":"<< tkt_node.get<string>(tkt_node_it->first) <<endl;

             boost::property_tree::ptree tkt = tkt_node_it->second;
             boost::property_tree::ptree::iterator tkt_it = tkt.begin();
             for(; tkt_it != tkt.end(); ++tkt_it)
             {
                string tkt_key = tkt_it->first;
                string tkt_val = tkt.get<string>(tkt_key);
                if(tkt_key.find(string("txid"))!=string::npos)
                   keyid.push_back(tkt_val);
                else
                   value.push_back(tkt_val);
                cout << tkt_key << ":"<<tkt_val<<endl;
             }
        }
        else
        { 
          string val = root.get<string>(key);
          cout << key <<":"<< val <<endl;
          keyid.push_back(key);
          value.push_back(val);
        }
    }   
}

//
void testjson(string json_str)
{
    string str_json;//  = "{"TEST":"" , "MSG":"130",                \
                            "TKT":[{"DTYP":"T","STOT":"1","SNUM":"1",   \
                              "CPN":[{"CNBR":"1","DDAT":"090117","DTME":"1205","ADAT":"090117",          \
                                      "ATME":"1340","ORIG":"TSA","DEST":"PVG","ALC1":"FM","FLTN":"802","CLAS":"Y","FSTN":"OK","FBAS":"Y","BAGA":"20K"           \
                                    }]              \
                                  }]        \
                           }";
    str_json= json_str;
    cout<< str_json<<endl; 

    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    boost::property_tree::ptree::iterator root_it = root.begin();
    for(; root_it != root.end(); ++root_it)
    {
        string key = root_it->first;
        cout<< key<<endl;
   
        if("TKT" == key)
        {
            boost::property_tree::ptree tkt_node = root.get_child(key);
            boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
            for(; tkt_node_it != tkt_node.end(); ++tkt_node_it)
            {
                boost::property_tree::ptree tkt = tkt_node_it->second;
                boost::property_tree::ptree::iterator tkt_it = tkt.begin();
                for(; tkt_it != tkt.end(); ++tkt_it)
                {
                    string tkt_key = tkt_it->first;
                    if("CPN" == tkt_key)
                    {
                        boost::property_tree::ptree cpn_node = tkt.get_child(tkt_key);
                        boost::property_tree::ptree::iterator cpn_node_it = cpn_node.begin();
                        for(; cpn_node_it != cpn_node.end(); ++cpn_node_it)
                        {
                            boost::property_tree::ptree cpn = cpn_node_it->second;
                            boost::property_tree::ptree::iterator cpn_it = cpn.begin();
                            for(; cpn_it != cpn.end();++cpn_it)
                            {
                                string cpn_key = cpn_it->first;
                                string cpn_val = cpn.get<string>(cpn_key);
                                cout<<cpn_key<<":"<<cpn_val<<endl;
                            }
                        }
                    }
                    else
                    {
                        string tkt_val = tkt.get<string>(tkt_key);
                        cout << tkt_key << ":"<<tkt_val<<endl;
                    }
                }
            }
        }
        else
        {
            string val = root.get<string>(key);
            cout << key <<":"<< val <<endl;
        }
    }
}
//
string   createrawtx(string param1, string param2)
{
   // "createrawtransaction [{\"txid\":\"id\",\"vout\":n},...] {\"address\":amount,\"data\":\"hex\",...} ( locktime )\n"

    CMutableTransaction rawTx;
    vector<string> txids;
    vector<string> txidxs;
    vector<string> outaddrs;
    vector<string> outamts;
    gettx(param1,txids,txidxs );
    gettx(param2,outaddrs,outamts );
    for (unsigned int idx = 0; idx < txids.size(); idx++) {

        uint256 txid = uint256S(txids[idx]);
        uint32_t nSequence = (rawTx.nLockTime ? std::numeric_limits<uint32_t>::max() - 1 : std::numeric_limits<uint32_t>::max());
        unsigned int nOutput = boost::lexical_cast<unsigned int>(txidxs[idx]);
        CTxIn in(COutPoint(txid, nOutput), CScript(), nSequence);

        rawTx.vin.push_back(in);
    }

    set<CBitcoinAddress> setAddress;

    for (unsigned int oidx = 0; oidx < outaddrs.size(); oidx++) {

        CBitcoinAddress address(outaddrs[oidx]);
        if (!address.IsValid())
              return string("");

        if (setAddress.count(address))
              return string("");
        setAddress.insert(address);

        CScript scriptPubKey = GetScriptForDestination(address.Get());
        double fAmount =  boost::lexical_cast<double>(outamts[oidx]); 
        CAmount nAmount = fAmount * COIN ; 

        CTxOut out(nAmount, scriptPubKey);
        rawTx.vout.push_back(out);
    }

    return EncodeHexTx(rawTx);
}


string signrawtransaction(string strHex, string strtxdata, string paramkey)
{
    //   "signrawtransaction \"hexstring\" ( [{\"txid\":\"id\",\"vout\":n,\"scriptPubKey\":\"hex\",\"redeemScript\":\"hex\"},...] [\"privatekey1\",...] sighashtype )\n"

    vector<unsigned char> txData=  ParseHex(strHex) ;
    CDataStream ssData(txData, SER_NETWORK, PROTOCOL_VERSION);
    vector<CMutableTransaction> txVariants;
    while (!ssData.empty()) {
        try {
            CMutableTransaction tx;
            ssData >> tx;
            txVariants.push_back(tx);
        }
        catch (const std::exception&) {
            return string("error");
        }
    }

    if (txVariants.empty())
        return string("error");

    // mergedTx will end up with all the signatures; it
    // starts as a clone of the rawtx:
    CMutableTransaction mergedTx(txVariants[0]);

 
    bool fGivenKeys = false;
    CBasicKeyStore tempKeystore;
    if (paramkey!=string("")) {
        fGivenKeys = true;
        vector<string> privkey;
        getjsonkey(paramkey,privkey);
        for (unsigned int idx = 0; idx < privkey.size(); idx++) 
        {
            CBitcoinSecret vchSecret;
            bool fGood = vchSecret.SetString(privkey[idx]);
            if (!fGood)
                return string("invald private key");
            CKey key = vchSecret.GetKey();
            if (!key.IsValid())
                return string("invalid private key ");
            tempKeystore.AddKey(key);
        }
    }
    else 
        return string("param is less error");

    // Add previous txouts given in the RPC call:
    vector<string> txids;
    vector<string> txidxs;
    gettx(strtxdata,txids,txidxs );
    //only one tx
    for (unsigned int idx = 0; idx < 1; idx++) {

        uint256 txid =uint256S(txids[0]); 

        int nOut =boost::lexical_cast<int>(txidxs[0])  ;
        if (nOut < 0)
            return string("vout must be positive") ; 

        vector<unsigned char> pkData= ParseHex(txids[1]);
        CScript scriptPubKey(pkData.begin(), pkData.end());

        // if redeemScript given and not using the local wallet (private keys
        // given), add redeemScript to the tempKeystore so it can be signed:
     }

    const CKeyStore& keystore = tempKeystore;

    int nHashType = SIGHASH_ALL;

    bool fHashSingle = ((nHashType & ~SIGHASH_ANYONECANPAY) == SIGHASH_SINGLE);

    // Script verification errors

    // Sign what we can:
    for (unsigned int i = 0; i < 1 ; i++) {
        CTxIn& txin = mergedTx.vin[i];
    
        vector<unsigned char> pkData= ParseHex(txids[1]);
        CScript scriptPubKey(pkData.begin(), pkData.end());
        //const CScript& prevPubKey = coins->vout[txin.prevout.n].scriptPubKey; //????

        txin.scriptSig.clear();
        // Only sign SIGHASH_SINGLE if there's a corresponding output:
        if (!fHashSingle || (i < mergedTx.vout.size()))
            SignSignature(keystore, scriptPubKey, mergedTx, i, nHashType);
           // SignSignature(keystore, prevPubKey, mergedTx, i, nHashType);

        // ... and merge in other signatures:
        for(const CMutableTransaction& txv : txVariants) {
            txin.scriptSig = CombineSignatures(scriptPubKey, mergedTx, i, txin.scriptSig, txv.vin[i].scriptSig);
        }
        ScriptError serror = SCRIPT_ERR_OK;
        if (!VerifyScript(txin.scriptSig, scriptPubKey , STANDARD_SCRIPT_VERIFY_FLAGS, MutableTransactionSignatureChecker(&mergedTx, i), &serror)) {
            return string("error");
        }
    }

    string result = EncodeHexTx(mergedTx); 

    return result;
}

