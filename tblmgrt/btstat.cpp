#include <string>
#include <boost/log/trivial.hpp>  
#include <mysql/mysql.h>
#include <iostream>
#include <boost/lexical_cast.hpp>  
#include <boost/algorithm/string.hpp>
#include "btstat.h"

using namespace std;
using namespace boost;

  
//    std::cin.get();  

MYSQL g_conn;
MYSQL g_fromconn;
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

    MYSQL *ret = mysql_real_connect(&g_conn, "127.0.0.1", "root", "123456", "mysql", 0, NULL, 0);
    if (!ret)
    {
        BOOST_LOG_TRIVIAL(info)<<"Failed to connect to database  "<<mysql_error(&g_conn);
	cout << "Failed to connect to database" << endl;
        exit(1);
    }

    ret = mysql_real_connect(&g_fromconn, dbhost.c_str(), dbuser.c_str(), dbpasswd.c_str(), "mysql", 0, NULL, 0);
    if (!ret)
    {
        BOOST_LOG_TRIVIAL(info)<<"Failed to connect to from database  "<<mysql_error(&g_conn);
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

void ReadBtcoinRecToWrite(int height   )
{
    char sqlBuf[500] = {""};
    if(height!=g_sqlblockheight+1)
    {
        BOOST_LOG_TRIVIAL(info)<<"sqldb  update addr height "<<g_sqlblockheight ;
	 cout << "update height height"<<g_sqlblockheight << endl;
	 exit(1);
    }
    sprintf(sqlBuf ,"select *from btcoin_all where height =%d",
          height);
    long int nAmount; 
    BOOST_LOG_TRIVIAL(info)<<"query btcoin_all height "<<height << "  ";
    if (mysql_query(&g_fromconn,sqlBuf))
    {   
        BOOST_LOG_TRIVIAL(info)<<"query btcoin_all failed "<<mysql_error(&g_fromconn);
        cout << "query btcoin_all failed" << mysql_error(&g_fromconn)<< endl;
        exit(0);
    
    }

    {   
        MYSQL_RES *result=mysql_store_result(&g_fromconn);
        
        if (result)
        {   
            //获取离线结果，数据存放到程序内存中
                int     num_fields = mysql_num_fields(result);
                MYSQL_ROW row;
                //row=mysql_fetch_row(result);//获取一行数据的结果；
                while ((row = mysql_fetch_row(result)))
                {
                      unsigned long *lengths;
                      lengths = mysql_fetch_lengths(result);
                      
                      // addr | amount     | height | tdidx | type
                      nAmount= lexical_cast<long int>(row[1]); 
                      string strAddr(row[0]);       
                      trim(strAddr);
                      AddAddrMyDbIndex(strAddr.c_str(), nAmount, atoi(row[3]) ,0, height,atoi(row[4]) );
                }
                                
                //cout << "query btcoin_hight g_sqlblockheight " <<g_sqlblockheight  << endl;
                
                mysql_free_result(result); //释放掉内存
        }
        else    
        {       
                BOOST_LOG_TRIVIAL(info)<<"sql query","query btcoin_hight failed  ";
                cout << "query btcoin_hight g_sqlblockheight "   << endl;
                //exit(0);
        }
    }
 

}

// Failed to extract multiaddr hei 166451 type 4  tx 8 vdx 0 amount 10000000 addr  18am8jUnBqru2jtQpQbE4LCywBWUPUooP1 
void  readfiletodb()
{

}

void mysql_close()
{
    return;
    g_bSqlClose=true;
    mysql_close(&g_conn);
    mysql_close(&g_fromconn);
    cout << "mysql_close  database" << endl;
    cout << "mysql_close  database" << endl;
}

int my_insert(MYSQL *conn,const char * pAddr , long int  amount,int nHeight,int txIdx,int type)
{
    char sqlBuf[500] = {""};
    char sqlBuf1[500] = {""};
    sprintf(sqlBuf ,"insert into btcoin_addr(addr,amount) values('%s',%ld) on duplicate key update amount=amount+%ld",
          pAddr ,amount,amount);
    sprintf(sqlBuf1 ,"insert into btcoin_all(addr,amount,height,tdidx) values('%s',%ld,%d,%d,%d) ",
          pAddr ,amount,nHeight,txIdx,type);
    if(amount >1021759540016 )
    {
		//cout << "too large money " << amount << pAddr << endl;
    }
    int ret = mysql_query(conn, sqlBuf);
    if (ret != 0)
    {
        BOOST_LOG_TRIVIAL(info)<<"query failed  "<<mysql_error(conn);
	cout << "query failed" << mysql_error(conn)<< endl;
        exit(1);
    }

    my_ulonglong affected_row = mysql_affected_rows(conn);

    ret = mysql_query(conn, sqlBuf1);
    if (ret != 0)
    {
        BOOST_LOG_TRIVIAL(info)<<"query failed  "<<mysql_error(conn);
	cout << "query failed" << mysql_error(conn)<< endl;
        exit(1);
    }

    //cout << "rows affected " << (int)affected_row <<endl;
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

