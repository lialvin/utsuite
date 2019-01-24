// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WALLET_CMD_MSG_H
#define WALLET_CMD_MSG_H  

#include <algorithm>
#include <exception>
#include <map>
#include <set>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>
//typedef signed long long int64_t;
#include <boost/unordered_map.hpp>
std::string  signmnpmessage(std::string strPrivKey , std::string strMasterKey, std::string straddr);
std::string  signlicmessage(std::string strPrivKey,std::string strTxid,std::string mnpubkey,int64_t idate,int txididx);
void  producekey(std::string  str_count );
void  testjson(std::string  str_json );
std::string createrawtx(std::string strjsonin,std::string strjsonout);
std::string transutaddr( std::string uoshex);
#endif
