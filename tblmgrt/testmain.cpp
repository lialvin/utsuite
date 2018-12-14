#include <stdio.h>
#include "include/a.h"
#include "mystr.h"
#include "btstat.h"
#include "readcfg.h"
#include "testmain.h"

int main(int argc, char* argv[])
{

   MyString abc=MyString("abc");
   
    
   string  dbuser ,dbhost, dbpass;
   int startheight, endheight;
   readcfg(startheight,endheight,dbuser,dbhost,dbpass);
    
   my_sql_init(dbuser,dbhost,dbpass);
   for(int i=startheight; i<endheight;i++)
   {
      ReadBtcoinRecToWrite(i  );
      UpdateAddrMyDb(i);   
   } 
   mysql_close();  
    
   double b=25.0;
   double a=0.0;
   a=get_sqrt(b);
   char szBuf[128];
   sprintf(szBuf  ,"a is %lf,b is %lf\n" , a,b);
   return 0;
}
