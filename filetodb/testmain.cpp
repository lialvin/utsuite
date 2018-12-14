#include <stdio.h>
#include "include/a.h"
#include "mystr.h"
#include "btstat.h"
#include "readcfg.h"
#include "testmain.h"
#include <boost/format.hpp>


int main(int argc, char* argv[])
{

   MyString abc=MyString("abc");
   
   int64_t  COIN=100000000;   
   uint64_t  infCOIN=-1;   
   int64_t  MAX_MONEY = int64_t(1000) * 21000000 * COIN;
 
   string  dbuser ,dbhost, dbpass;
   int startheight, endheight;
   readcfg(startheight,endheight,dbuser,dbhost,dbpass);
    
   my_sql_init(dbuser,dbhost,dbpass);
 
   for(int i=startheight; i<endheight;i++)
   {
        std::string s; 
        s= str(boost::format("%s%d.txt")%"/home/mycoin/mydata/dbdata"%i);
        readfiletodb(s.c_str() );
   } 
   mysql_close();  
    
   double b=25.0;
   double a=0.0;
   a=get_sqrt(b);
   char szBuf[128];
   sprintf(szBuf  ,"a is %lf,b is %lf\n" , a,b);
   return 0;
}
