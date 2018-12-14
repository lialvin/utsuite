#ifndef BT_STAT__BITCOIN__H
#define BT_STAT__BITCOIN__H
#include <string>

void AddAddrMyDbIndex(const char * dest, long int  nAmount, unsigned int txIdx ,unsigned int  vIdx, int height,int type );
void ReadBtcoinRecToWrite(int height   );
int  my_sql_init(std::string ,std::string  ,std::string);
void mysql_close();
void UpdateAddrMyDb(const int  height );
#endif
