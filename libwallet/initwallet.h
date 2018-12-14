// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef LIB_INIT_WALLET__MAIN_H
#define LIB_INIT_WALLET__MAIN_H 

#include <algorithm>
#include <exception>
#include <map>
#include <set>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>
#include "key.h"
#include <boost/unordered_map.hpp>

//extern CWallet* pwalletMain;
enum RPCErrorCode
{
    //! Standard JSON-RPC 2.0 errors
    RPC_INVALID_REQUEST  = -32600,
    RPC_METHOD_NOT_FOUND = -32601,
    RPC_INVALID_PARAMS   = -32602,
    RPC_INTERNAL_ERROR   = -32603,
    RPC_PARSE_ERROR      = -32700,

    //! General application defined errors
    RPC_MISC_ERROR                  = -1,  //! std::exception thrown in command handling
    RPC_FORBIDDEN_BY_SAFE_MODE      = -2,  //! Server is in safe mode, and command is not allowed in safe mode
    RPC_TYPE_ERROR                  = -3,  //! Unexpected type was passed as parameter
    RPC_INVALID_ADDRESS_OR_KEY      = -5,  //! Invalid address or key
    RPC_OUT_OF_MEMORY               = -7,  //! Ran out of memory during operation
    RPC_INVALID_PARAMETER           = -8,  //! Invalid, missing or duplicate parameter
    RPC_DATABASE_ERROR              = -20, //! Database error
    RPC_DESERIALIZATION_ERROR       = -22, //! Error parsing or validating structure in raw format
    RPC_VERIFY_ERROR                = -25, //! General error during transaction or block submission
    RPC_VERIFY_REJECTED             = -26, //! Transaction or block was rejected by network rules
    RPC_VERIFY_ALREADY_IN_CHAIN     = -27, //! Transaction already in chain
    RPC_IN_WARMUP                   = -28, //! Client still warming up

    //! Aliases for backward compatibility
    RPC_TRANSACTION_ERROR           = RPC_VERIFY_ERROR,
    RPC_TRANSACTION_REJECTED        = RPC_VERIFY_REJECTED,
    RPC_TRANSACTION_ALREADY_IN_CHAIN= RPC_VERIFY_ALREADY_IN_CHAIN,

    //! P2P client errors
    RPC_CLIENT_NOT_CONNECTED        = -9,  //! Ulord Core is not connected
    RPC_CLIENT_IN_INITIAL_DOWNLOAD  = -10, //! Still downloading initial blocks
    RPC_CLIENT_NODE_ALREADY_ADDED   = -23, //! Node is already added
    RPC_CLIENT_NODE_NOT_ADDED       = -24, //! Node has not been added before
    RPC_CLIENT_NODE_NOT_CONNECTED   = -29, //! Node to disconnect not found in connected nodes
    RPC_CLIENT_INVALID_IP_OR_SUULORD = -30, //! Invalid IP/Suulord

    //! Wallet errors
    RPC_WALLET_ERROR                = -4,  //! Unspecified problem with wallet (key not found etc.)
    RPC_WALLET_INSUFFICIENT_FUNDS   = -6,  //! Not enough funds in wallet or account
    RPC_WALLET_INVALID_ACCOUNT_NAME = -11, //! Invalid account name
    RPC_WALLET_KEYPOOL_RAN_OUT      = -12, //! Keypool ran out, call keypoolrefill first
    RPC_WALLET_UNLOCK_NEEDED        = -13, //! Enter the wallet passphrase with walletpassphrase first
    RPC_WALLET_PASSPHRASE_INCORRECT = -14, //! The wallet passphrase entered was incorrect
    RPC_WALLET_WRONG_ENC_STATE      = -15, //! Command given in wrong wallet encryption state (encrypting an encrypted wallet etc.)
    RPC_WALLET_ENCRYPTION_FAILED    = -16, //! Failed to encrypt the wallet
    RPC_WALLET_ALREADY_UNLOCKED     = -17, //! Wallet is already unlocked
};

class Secp256k1Init
{
    ECCVerifyHandle globalVerifyHandle;

public:
    Secp256k1Init(char * );
    ~Secp256k1Init() {
        ECC_Stop();
    }
};
bool initwallet();
void exitwallet();

std::string  JSONRPCError(int code, const std::string& message);
bool  dumpwallet(std::string  walletfile);
bool  importwallet(std::string walletfile);
std::string getnewaddress(std::string strAccount,std::string & strpubkey);
std::string dumpprivkey(const std::string & straddr, std::string & pubkey,std::string &strprivhexkey,std::string & strkeyidpubkey);
bool  importprivkey(const std::string& params, std::string & addr);
std::string  createmultisig(std::string addr1 , std::string addr2 ,std::string & addr3 );
//std::string  signmnpmessage(std::string strPrivKey , std::string strMasterKey, std::string straddr,std::string strPort);

#endif
