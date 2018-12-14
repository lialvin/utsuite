#include <stdio.h>
#include "initwallet.h"
#include <string>

#include "base58.h"
#include "chain.h"
//#include "rpc/server.h"
#include "validation.h"
#include "script/script.h"
#include "script/standard.h"
#include "sync.h"
#include "util.h"
#include "utiltime.h"
#include "wallet.h"
#include "messagesigner.h"
#include "random.h"
#include <fstream>
#include <stdint.h>
#include "sanity.h"
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <univalue.h>
#include <boost/foreach.hpp>

using namespace std;

//bool EnsureWalletIsAvailable(bool avoidException);

std::string static EncodeDumpTime(int64_t nTime) {
    return DateTimeStrFormat("%Y-%m-%dT%H:%M:%SZ", nTime);
}

string  JSONRPCError(int code, const string& message)
{
    string error;
    error="code "+code+ message;
    return error;
}

CWallet* pwalletMain = NULL;

bool static InitError(const std::string &str)
{
    return false;
}

bool static InitWarning(const std::string &str)
{
    return true;
}

bool InitSanityCheck(void)
{
    if(!ECC_InitSanityCheck()) {
        InitError("Elliptic curve cryptography sanity check failure. Aborting.");
        return false;
    }
    if (!glibc_sanity_test() || !glibcxx_sanity_test())
        return false;

    return true;
}
// init ecc crtpt algo  main  test
bool  ecc_check(char * netparams)
{
    // Initialize fast PRNG
    SelectParams(netparams);

    // Sanity check
    if (!InitSanityCheck())
        return InitError(_("Initialization sanity check failed. Ulord Core is shutting down."));
    return true;
}


Secp256k1Init::Secp256k1Init(char * netparams)
{
     ECC_Start();
     ecc_check(netparams );
}

bool  initwallet()
{
    std::string strWarning;
    std::string strError;
    std::ostringstream strErrors;
    std::string strWalletFile = "mywallet.dat";
    int nMaxVersion=0;
    bool fFirstRun = true;
    pwalletMain = new CWallet(strWalletFile);
    DBErrors nLoadWalletRet = pwalletMain->LoadWallet(fFirstRun);
   
    if (nLoadWalletRet != DB_LOAD_OK)
    {
        if (nLoadWalletRet == DB_CORRUPT)
            strErrors << _("Error loading wallet.dat: Wallet corrupted") << "\n";
        else if (nLoadWalletRet == DB_NONCRITICAL_ERROR)
        {
            InitWarning(_("Error reading wallet.dat! All keys read correctly, but transaction data"
                           " or address book entries might be missing or incorrect."));
        }
        else if (nLoadWalletRet == DB_TOO_NEW)
            strErrors << _("Error loading wallet.dat: Wallet requires newer version of Ulord Core") << "\n";
        else if (nLoadWalletRet == DB_NEED_REWRITE)
        {
            strErrors << _("Wallet needed to be rewritten: restart Ulord Core to complete") << "\n";
            return InitError(strErrors.str());
        }
        else
            strErrors << _("Error loading wallet.dat") << "\n";
    }

    nMaxVersion = CLIENT_VERSION;
    pwalletMain->SetMinVersion(FEATURE_LATEST); // permanently upgrade the wallet immediately

    if (fFirstRun)
    {
        // Create new keyUser and set as default key
        RandAddSeedPerfmon();

       CPubKey newDefaultKey;
       if (pwalletMain->GetKeyFromPool(newDefaultKey)) {
           pwalletMain->SetDefaultKey(newDefaultKey);
           if (!pwalletMain->SetAddressBook(pwalletMain->vchDefaultKey.GetID(), "", "receive"))
               strErrors << _("Cannot write default address") << "\n";
      }

        // Try to create wallet backup right after new wallet was created
        std::string strBackupWarning;
        std::string strBackupError;
        if(!AutoBackupWallet(pwalletMain, "", strBackupWarning, strBackupError)) {
            if (!strBackupWarning.empty())
                InitWarning(strBackupWarning);
            if (!strBackupError.empty())
                return InitError(strBackupError);
        }

    }

}

void exitwallet()
{
       pwalletMain->Flush(true);
}

int64_t static DecodeDumpTime(const std::string &str) {
    static const boost::posix_time::ptime epoch = boost::posix_time::from_time_t(0);
    static const std::locale loc(std::locale::classic(),
        new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%SZ"));
    std::istringstream iss(str);
    iss.imbue(loc);
    boost::posix_time::ptime ptime(boost::date_time::not_a_date_time);
    iss >> ptime;
    if (ptime.is_not_a_date_time())
        return 0;
    return (ptime - epoch).total_seconds();
}


std::string static EncodeDumpString(const std::string &str) {
    std::stringstream ret;
    BOOST_FOREACH(unsigned char c, str) {
        if (c <= 32 || c >= 128 || c == '%') {
            ret << '%' << HexStr(&c, &c + 1);
        } else {
            ret << c;
        }
    }
    return ret.str();
}

std::string DecodeDumpString(const std::string &str) {
    std::stringstream ret;
    for (unsigned int pos = 0; pos < str.length(); pos++) {
        unsigned char c = str[pos];
        if (c == '%' && pos+2 < str.length()) {
            c = (((str[pos+1]>>6)*9+((str[pos+1]-'0')&15)) << 4) |
                ((str[pos+2]>>6)*9+((str[pos+2]-'0')&15));
            pos += 2;
        }
        ret << c;
    }
    return ret.str();
}


void EnsureWalletIsUnlocked()
{
   bool ret=   pwalletMain->IsLocked();
   return ;
}

std::string getnewaddress(std::string strAccount,std::string& strpubkey)
{
    
    LOCK2(cs_main, pwalletMain->cs_wallet);

    // Parse the account first so we don't generate a key if there's an error

    if (!pwalletMain->IsLocked(true))
        pwalletMain->TopUpKeyPool();

    // Generate a new key that is added to wallet
    CPubKey newKey;
    if (!pwalletMain->GetKeyFromPool(newKey))
        throw JSONRPCError(RPC_WALLET_KEYPOOL_RAN_OUT, "Error: Keypool ran out, please call keypoolrefill first");
    
    CKeyID keyID = newKey.GetID();
    
    strpubkey= HexStr(newKey);
    pwalletMain->SetAddressBook(keyID, strAccount, "receive");
     
    return CBitcoinAddress(keyID).ToString();
}

bool  importprivkey(const std::string& params,std::string & addr )
{
    
    //LOCK2(cs_main, pwalletMain->cs_wallet);

    EnsureWalletIsUnlocked();

    string strSecret = params;
    string strLabel = "";
 

    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(strSecret);

    if (!fGood) throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid private key encoding");

    CKey key = vchSecret.GetKey();
    if (!key.IsValid()) throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Private key outside allowed range");


    CPubKey pubkey = key.GetPubKey();
    assert(key.VerifyPubKey(pubkey));
    CKeyID vchAddress = pubkey.GetID();

    CBitcoinAddress  bitAddress;
    bitAddress.Set(pubkey.GetID());
    addr = bitAddress.ToString();

    {
        pwalletMain->MarkDirty();
        pwalletMain->SetAddressBook(vchAddress, strLabel, "receive");

        // Don't throw error in case a key is already there
        if (pwalletMain->HaveKey(vchAddress))
            return false;

        pwalletMain->mapKeyMetadata[vchAddress].nCreateTime = 1;

        if (!pwalletMain->AddKeyPubKey(key, pubkey))
            throw JSONRPCError(RPC_WALLET_ERROR, "Error adding key to wallet");

        // whenever a key is imported, we need to scan the whole chain
        pwalletMain->nTimeFirstKey = 1; // 0 would be considered 'no value'

    }

    return true;
}
void ImportAddress(const CBitcoinAddress& address, const string& strLabel);
void ImportScript(const CScript& script, const string& strLabel, bool isRedeemScript)
{
    if (!isRedeemScript && ::IsMine(*pwalletMain, script) == ISMINE_SPENDABLE)
        throw JSONRPCError(RPC_WALLET_ERROR, "The wallet already contains the private key for this address or script");

    pwalletMain->MarkDirty();

    if (!pwalletMain->HaveWatchOnly(script) && !pwalletMain->AddWatchOnly(script))
        throw JSONRPCError(RPC_WALLET_ERROR, "Error adding address to wallet");

    if (isRedeemScript) {
        if (!pwalletMain->HaveCScript(script) && !pwalletMain->AddCScript(script))
            throw JSONRPCError(RPC_WALLET_ERROR, "Error adding p2sh redeemScript to wallet");
        ImportAddress(CBitcoinAddress(CScriptID(script)), strLabel);
    }
}

void ImportAddress(const CBitcoinAddress& address, const string& strLabel)
{
    CScript script = GetScriptForDestination(address.Get());
    ImportScript(script, strLabel, false);
    // add to address book or update label
    if (address.IsValid())
        pwalletMain->SetAddressBook(address.Get(), strLabel, "receive");
}


//Adds a script (in hex) or address that can be watched as if it were in your wallet but cannot be used to spend
bool importaddress( const string  addr)
{

    string strLabel = "";
 
    // Whether to perform rescan after import


    // Whether to import a p2sh version, too

    LOCK2(cs_main, pwalletMain->cs_wallet);

    CBitcoinAddress address(addr);
    if (address.IsValid()) {
        ImportAddress(address, strLabel);
    }
    else {
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Dash address or script");
    }


    return  true; 
}
//Adds a public key (in hex) that can be watched as if it were in your wallet but cannot be used to spend.\n"
bool importpubkey(const string params,string  strlabel)
{

    string strLabel = strlabel;

    // Whether to perform rescan after import


    if (!IsHex(params.c_str()))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Pubkey must be a hex string");
    std::vector<unsigned char> data(ParseHex(params.c_str()));
    CPubKey pubKey(data.begin(), data.end());
    if (!pubKey.IsFullyValid())
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Pubkey is not a valid public key");

    LOCK2(cs_main, pwalletMain->cs_wallet);

    ImportAddress(CBitcoinAddress(pubKey.GetID()), strLabel);
    ImportScript(GetScriptForRawPubKey(pubKey), strLabel, false);


    return true;
}

//Imports keys from an Electrum wallet export file (.csv or .json)
bool  importelectrumwallet(const string& params)
{
 
    LOCK2(cs_main, pwalletMain->cs_wallet);

    EnsureWalletIsUnlocked();

    ifstream file;
    std::string strFileName = params;
    size_t nDotPos = strFileName.find_last_of(".");
    if(nDotPos == string::npos)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "File has no extension, should be .json or .csv");

    std::string strFileExt = strFileName.substr(nDotPos+1);
    if(strFileExt != "json" && strFileExt != "csv")
        throw JSONRPCError(RPC_INVALID_PARAMETER, "File has wrong extension, should be .json or .csv");

    file.open(strFileName.c_str(), std::ios::in | std::ios::ate);
    if (!file.is_open())
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Cannot open Electrum wallet export file");

    bool fGood = true;

    int64_t nFilesize = std::max((int64_t)1, (int64_t)file.tellg());
    file.seekg(0, file.beg);

    pwalletMain->ShowProgress(_("Importing..."), 0); // show progress dialog in GUI

    if(strFileExt == "csv") {
        while (file.good()) {
            pwalletMain->ShowProgress("", std::max(1, std::min(99, (int)(((double)file.tellg() / (double)nFilesize) * 100))));
            std::string line;
            std::getline(file, line);
            if (line.empty() || line == "address,private_key")
                continue;
            std::vector<std::string> vstr;
            boost::split(vstr, line, boost::is_any_of(","));
            if (vstr.size() < 2)
                continue;
            CBitcoinSecret vchSecret;
            if (!vchSecret.SetString(vstr[1]))
                continue;
            CKey key = vchSecret.GetKey();
            CPubKey pubkey = key.GetPubKey();
            assert(key.VerifyPubKey(pubkey));
            CKeyID keyid = pubkey.GetID();
            if (pwalletMain->HaveKey(keyid)) {
                LogPrintf("Skipping import of %s (key already present)\n", CBitcoinAddress(keyid).ToString());
                continue;
            }
            LogPrintf("Importing %s...\n", CBitcoinAddress(keyid).ToString());
            if (!pwalletMain->AddKeyPubKey(key, pubkey)) {
                fGood = false;
                continue;
            }
        }
    } 
    file.close();
    pwalletMain->ShowProgress("", 100); // hide progress dialog in GUI

    // Whether to perform rescan after import

    return true; 
}

//            "dumpprivkey \"dashaddress\"\n"
string  dumpprivkey(const string & params, string & strpubkey, std::string &strprivhexkey,string & strkeyidpubkey)
{
    
    LOCK2(cs_main, pwalletMain->cs_wallet);

    EnsureWalletIsUnlocked();

    string strAddress = params;
    CBitcoinAddress address;
    if (!address.SetString(strAddress))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid ulord address");
    CKeyID keyID;
    if (!address.GetKeyID(keyID))
        throw JSONRPCError(RPC_TYPE_ERROR, "Address does not refer to a key");
    CKey vchSecret;
    if (!pwalletMain->GetKey(keyID, vchSecret))
        throw JSONRPCError(RPC_WALLET_ERROR, "Private key for address " + strAddress + " is not known");
    vector<unsigned char> tempkey(vchSecret.begin(),vchSecret.end());

    strprivhexkey= HexStr(tempkey);

    CPubKey  pubkey= vchSecret.GetPubKey(); 

    CKeyID keyid = pubkey.GetID();
   
  	std::vector<unsigned char > keyidvec( keyid.begin(),keyid.end() ); 

    strkeyidpubkey = HexStr(keyidvec);

    strpubkey = HexStr(pubkey);
    return CBitcoinSecret(vchSecret).ToString();
}

/**
 * Used by addmultisigaddress / createmultisig:
 */
CScript _createmultisig_redeemScript(std::string   addr1,std::string addr2  )
{
    int nRequired = 2;

    vector<string> keys;
    keys.push_back(addr1);
    keys.push_back(addr2);

    std::vector<CPubKey> pubkeys;
    pubkeys.resize(keys.size());
    for (unsigned int i = 0; i < keys.size(); i++)
    {
        const std::string& ks = keys[i];
        // Case 1: Ulord address and we have full public key:
        CBitcoinAddress address(ks);
        if (pwalletMain && address.IsValid())
        {
            CKeyID keyID;
            if (!address.GetKeyID(keyID))
                throw runtime_error(
                    strprintf("%s does not refer to a key",ks));
            CPubKey vchPubKey;
            if (!pwalletMain->GetPubKey(keyID, vchPubKey))
                throw runtime_error(
                    strprintf("no full public key for address %s",ks));
            if (!vchPubKey.IsFullyValid())
                throw runtime_error(" Invalid public key: "+ks);
            pubkeys[i] = vchPubKey;
        }

        // Case 2: hex public key
        else
        if (IsHex(ks))
        {
            CPubKey vchPubKey(ParseHex(ks));
            if (!vchPubKey.IsFullyValid())
                throw runtime_error(" Invalid public key: "+ks);
            pubkeys[i] = vchPubKey;
        }
        else
        {
            throw runtime_error(" Invalid public key: "+ks);
        }
    }
    CScript result = GetScriptForMultisig(nRequired, pubkeys);

    if (result.size() > MAX_SCRIPT_ELEMENT_SIZE)
        throw runtime_error(
                strprintf("redeemScript exceeds size limit: %d > %d", result.size(), MAX_SCRIPT_ELEMENT_SIZE));

    return result;
}

std::string  createmultisig(std::string addr1 , std::string addr2 ,std::string & addr3 )
{

    // Construct using pay-to-script-hash:
    CScript inner = _createmultisig_redeemScript(addr1 ,addr2 );
    CScriptID innerID(inner);
	
   
    std::vector<unsigned char > keyidvec( innerID.begin(),innerID.end() ); 
	addr3= HexStr(keyidvec);
	
    CBitcoinAddress address(innerID);

    std::string hexScript=  HexStr(inner.begin(), inner.end());

    return address.ToString();
}


bool  importwallet(std::string walletfile )
{
    

    LOCK2(cs_main, pwalletMain->cs_wallet);

    EnsureWalletIsUnlocked();

    ifstream file;
    file.open(walletfile.c_str(), std::ios::in | std::ios::ate);
    if (!file.is_open())
    {
        return false;
    }


    bool fGood = true;

    int64_t nFilesize = std::max((int64_t)1, (int64_t)file.tellg());
    file.seekg(0, file.beg);

    pwalletMain->ShowProgress(_("Importing..."), 0); // show progress dialog in GUI
    while (file.good()) {
        pwalletMain->ShowProgress("", std::max(1, std::min(99, (int)(((double)file.tellg() / (double)nFilesize) * 100))));
        std::string line;
        std::getline(file, line);
        if (line.empty() || line[0] == '#')
            continue;

        std::vector<std::string> vstr;
        boost::split(vstr, line, boost::is_any_of(" "));
        if (vstr.size() < 2)
            continue;
        CBitcoinSecret vchSecret;
        if (!vchSecret.SetString(vstr[0]))
            continue;
        CKey key = vchSecret.GetKey();
        CPubKey pubkey = key.GetPubKey();
        assert(key.VerifyPubKey(pubkey));
        CKeyID keyid = pubkey.GetID();
        if (pwalletMain->HaveKey(keyid)) {
            continue;
        }
        int64_t nTime = DecodeDumpTime(vstr[1]);
        std::string strLabel;
        bool fLabel = true;
        for (unsigned int nStr = 2; nStr < vstr.size(); nStr++) {
            if (boost::algorithm::starts_with(vstr[nStr], "#"))
                break;
            if (vstr[nStr] == "change=1")
                fLabel = false;
            if (vstr[nStr] == "reserve=1")
                fLabel = false;
            if (boost::algorithm::starts_with(vstr[nStr], "label=")) {
                strLabel = DecodeDumpString(vstr[nStr].substr(6));
                fLabel = true;
            }
        }
        if (!pwalletMain->AddKeyPubKey(key, pubkey)) {
            fGood = false;
            continue;
        }
        pwalletMain->mapKeyMetadata[keyid].nCreateTime = nTime;
        if (fLabel)
            pwalletMain->SetAddressBook(keyid, strLabel, "receive");
    }
    file.close();
    pwalletMain->ShowProgress("", 100); // hide progress dialog in GUI


    //pwalletMain->MarkDirty();

    if (!fGood)
    {
        return false;
    }
    return true; 
}


bool  dumpwallet(std::string  walletfile)
{

    LOCK2(cs_main, pwalletMain->cs_wallet);

    EnsureWalletIsUnlocked();

    ofstream file;
    file.open(walletfile.c_str());
    if (!file.is_open())
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Cannot open wallet dump file");

    std::map<CKeyID, int64_t> mapKeyBirth;
    std::set<CKeyID> setKeyPool;
    pwalletMain->GetKeyBirthTimes(mapKeyBirth);
    pwalletMain->GetAllReserveKeys(setKeyPool);

    // sort time/key pairs
    std::vector<std::pair<int64_t, CKeyID> > vKeyBirth;
    for (std::map<CKeyID, int64_t>::const_iterator it = mapKeyBirth.begin(); it != mapKeyBirth.end(); it++) {
        vKeyBirth.push_back(std::make_pair(it->second, it->first));
    }
    mapKeyBirth.clear();
    std::sort(vKeyBirth.begin(), vKeyBirth.end());

    // produce output
    file << strprintf("# Wallet dump created by Ulord Core %s (%s)\n", CLIENT_BUILD, CLIENT_DATE);
    file << "\n";
    for (std::vector<std::pair<int64_t, CKeyID> >::const_iterator it = vKeyBirth.begin(); it != vKeyBirth.end(); it++) {
        const CKeyID &keyid = it->second;
        std::string strTime = EncodeDumpTime(it->first);
        std::string strAddr = CBitcoinAddress(keyid).ToString();
        CKey key;
        if (pwalletMain->GetKey(keyid, key)) {
            if (pwalletMain->mapAddressBook.count(keyid)) {
                file << strprintf("%s %s label=%s # addr=%s\n", CBitcoinSecret(key).ToString(), strTime, EncodeDumpString(pwalletMain->mapAddressBook[keyid].name), strAddr);
            } else if (setKeyPool.count(keyid)) {
                file << strprintf("%s %s reserve=1 # addr=%s\n", CBitcoinSecret(key).ToString(), strTime, strAddr);
            } else {
                file << strprintf("%s %s change=1 # addr=%s\n", CBitcoinSecret(key).ToString(), strTime, strAddr);
            }
        }
    }
    file << "\n";
    file << "# End of dump\n";
    file.close();
    return  true;
}

