#include <string>
#include <boost/log/trivial.hpp>  
#include <mysql/mysql.h>
#include <iostream>
#include <boost/lexical_cast.hpp>  
#include <boost/algorithm/string.hpp>
#include "btstat.h"
#include <iostream>  
#include <fstream>  

using namespace std;
using namespace boost;

  
//    std::cin.get();  

MYSQL g_conn;
bool  g_bSqlClose=false;
int   g_sqlblockheight = 0;

int my_sql_init(string dbuser,string dbhost,string dbpasswd)
{
    if (mysql_library_init(0, NULL, NULL))
    {
        BOOST_LOG_TRIVIAL(info)<<"mysql_library_init  "<< __func__ ;
	cout << "mysql_library_init" << endl;
        exit(1);
    }


    mysql_init(&g_conn);

    MYSQL*  ret = mysql_real_connect(&g_conn, dbhost.c_str(), dbuser.c_str(), dbpasswd.c_str(), "mysql", 0, NULL, 0);
    if (!ret)
    {
        BOOST_LOG_TRIVIAL(info)<<"Failed to connect to  database  "<<mysql_error(&g_conn);
	cout << "Failed to connect to from database" << endl;
        exit(1);
    }

    g_bSqlClose=false;

    if (mysql_query(&g_conn,"select *from btcoin_height"))
    {
        BOOST_LOG_TRIVIAL(info)<<"query btcoin_height failed "<<mysql_error(&g_conn);
        cout << "query btcoin_height failed" << mysql_error(&g_conn)<< endl;
        exit(0);
    }

    {
	MYSQL_RES *result=mysql_store_result(&g_conn);

	if (result)
        {
	    //获取离线结果，数据存放到程序内存中
         	MYSQL_ROW row;
		row=mysql_fetch_row(result);//获取一行数据的结果；
		g_sqlblockheight=atoi(row[0]);//第0列的值
                cout << "query btcoin_hight g_sqlblockheight " <<g_sqlblockheight  << endl;
		mysql_free_result(result); //释放掉内存
	}
	else
	{
	        BOOST_LOG_TRIVIAL(info)<<"sql query","query btcoin_hight failed  ";
		cout << "query btcoin_hight g_sqlblockheight "   << endl;
		exit(0);
	}
    }

    return 0;
}
// Failed to extract multiaddr hei 166451 type 4  tx 8 vdx 0 amount 10000000 addr  18am8jUnBqru2jtQpQbE4LCywBWUPUooP1 
// fetch file into get height;
int   readfiletodb(const char *pfile  )
{
       char buffer[500];  
       ifstream in(pfile);  
       if (! in.is_open())  
       {    
           cout << "Error opening file"; exit (1); 
       }  
       int  height, txid,type;
       long int nAmount;
       string token_compress_on;
       while (!in.eof() )  
       {  
           in.getline(buffer,500);  
           
           vector<string> SplitVec; // #2: Search for tokens
           split(SplitVec, buffer, is_any_of(","));
           if(SplitVec.size()==5)
           { 
                nAmount= lexical_cast<long int>(SplitVec[1]);
                trim(SplitVec[0]);
                height= lexical_cast<int>(SplitVec[2]);
                txid = lexical_cast<int>(SplitVec[3]);
                type = lexical_cast<int>(SplitVec[4]);

                if(height==g_sqlblockheight+1)
                {
                   AddAddrMyDbIndex(SplitVec[0].c_str(), nAmount, txid ,0, height,type );
                } 
                else  if (height== g_sqlblockheight+2)
                {
                   g_sqlblockheight = height-1 ; 
                   UpdateAddrMyDb(  height-1 );
                   AddAddrMyDbIndex(SplitVec[0].c_str(), nAmount, txid ,0, height,type );
                }                    
                else
                {
                       BOOST_LOG_TRIVIAL(info)<<"sqldb  update addr height "<<g_sqlblockheight ;
	               cout << "update height height"<<g_sqlblockheight << endl;
	               exit(1);
                }
                // addr | amount     | height | tdidx | type
           }    
           else
           {
                 BOOST_LOG_TRIVIAL(info)<<"file field size error "<<g_sqlblockheight ;
	         cout << "update height height"<<g_sqlblockheight << endl;
	        //       exit(1);
          
           }
           
       }  
       in.close();
       UpdateAddrMyDb(  height );

       return 0;  
}

void mysql_close()
{
    return;
    g_bSqlClose=true;
    mysql_close(&g_conn);
    cout << "mysql_close  database" << endl;
}

int my_insert(MYSQL *conn,const char * pAddr , long int  amount,int nHeight,int txIdx,int type)
{
    char sqlBuf[500] = {""};
    sprintf(sqlBuf ,"insert into btcoin_addr(addr,amount) values('%s',%ld) on duplicate key update amount=amount+%ld",
          pAddr ,amount,amount);

    int ret = mysql_query(conn, sqlBuf);
    if (ret != 0)
    {
        BOOST_LOG_TRIVIAL(info)<<"query failed  "<<mysql_error(conn);
	cout << "query failed" << mysql_error(conn)<< endl;
        exit(1);
    }

    return 0;
}


void AddAddrMyDbIndex(const char * dest, long int  nAmount, unsigned int txIdx ,unsigned int  vIdx, int height,int type )
{
    if(g_bSqlClose==true) return;
    if(height!=0 &&height <= g_sqlblockheight)
    {
        BOOST_LOG_TRIVIAL(info)<<"sqldb update addr height "<<g_sqlblockheight ;
	cout << "update height height"<<g_sqlblockheight << endl;
        return;
    }
    int nRequired;
    int  nCount;

    my_insert(&g_conn , dest , nAmount,height,txIdx,type);
}

void UpdateAddrMyDb(const int  height )
{
    MYSQL  * conn=&g_conn;

    if(g_bSqlClose==true) return;
    if(height!=0 &&height <= g_sqlblockheight)
    {
        BOOST_LOG_TRIVIAL(info)<<"sqldb update addr height "<<g_sqlblockheight ;
	cout << "update height height"<<g_sqlblockheight << endl;

        return;
    }
    if(height!=g_sqlblockheight+1)
    {
        BOOST_LOG_TRIVIAL(info)<<"sqldb  update addr height "<<g_sqlblockheight ;
	 cout << "update height height"<<g_sqlblockheight << endl;
	 exit(1);
     }
     g_sqlblockheight=height;

     char sqlBuf[500] = {""};
     sprintf(sqlBuf ,"update btcoin_height set height=%d ",height);

    int ret = mysql_query(conn, sqlBuf);
    if (ret != 0)
    {
        BOOST_LOG_TRIVIAL(info)<<" update addr height  "<<mysql_error(conn);
	cout << "update addr height" << mysql_error(conn)<< endl;
        exit(1);
    }

    //cout << "new height " << height  <<endl;
    return ;
}

// from this->height insert dest table 

