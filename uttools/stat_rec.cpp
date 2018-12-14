#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <regex> 

using namespace std;
using namespace boost; 

int  stat_str(string & srcstr, string  cmpstr ) ;
void  stat_ip(string filename,int count);
int test_regex();
string  getip(string ipline);
void    find_ip(string ipsrc,string ipdst);
void finddstfile(string it1, string ipdst);

void printhelp()
{
       cout<<"1  help  :"  <<endl;
       cout<<"2  statip ip.txt  2 "<<endl;
       cout<<"3  findip ipsrc.txt  ipdst.txt "<<endl;

}



int main(int argc, char* argv[])
{

    if(argc<2) // help 
    {
       printhelp();
       return 0;
    }

    //test_regex();

    string stropt;
    if(argc>1)
    {
        stropt = string(argv[1]);
    }

    if(stropt==string("statip") )
    {
         int count =1;
        if(argc<2)
        {
           cout<< " args error please see help"<<endl;
           return 0;
        }
         if(argc>3)
             count= boost::lexical_cast<int>(argv[3]) ;
         
         string  filestr = string(argv[2]);
         stat_ip(filestr,count );     
    }
    else if(stropt==string("findip"))
    {
        if(argc<4)
        {
           cout<< " args error please see help"<<endl;
           return 0;
        }
        string  ipsrc = string(argv[2]);
        string  ipdst = string(argv[3]);
        find_ip(ipsrc,ipdst);

    }
    else
    {
       printhelp();
    }
    return 0;
}

void    find_ip(string ipsrc,string ipdst)
{
     vector<string> ipLine;
     vector<string> dstLine;
     string stringLine;    
     string stringip;
     ifstream infile;    
     ifstream dstfile;    
       // ofstream fout( "stat_result.txt", ios::app);    
     infile.open (ipsrc);
     while( !infile.eof() ) // To get you all the lines.    
     {        
        getline(infile,stringLine); // Saves the line in stringLine.        
        // stringLine= trim(stringLine);
        stringip = getip(stringLine);
        if(stringip!=string(""))
        {
           ipLine.push_back(stringip); 
        }
     } 
     infile.close();

     dstfile.open (ipdst);
     while( !dstfile.eof() ) // To get you all the lines.    
     {        
        getline(dstfile,stringLine); // Saves the line in stringLine.        
        // stringLine= trim(stringLine);
        stringip = getip(stringLine);
        if(stringip!=string(""))
        {
           dstLine.push_back(stringLine); 
        }
     } 
     dstfile.close();

 
     for(auto it1: ipLine) 
     {
        for(auto it2: dstLine)
        {
           stringip = getip(it2);
           if(it1==stringip)
             cout<<it2<<endl;
        }
       // finddstfile(it1, ipdst);    
     }    
}

void finddstfile(string it1, string ipdst)
{
     vector<string> ipLine;
     string stringLine;
     string stringip;
     ifstream infile;    
       // ofstream fout( "stat_result.txt", ios::app);    
     infile.open (ipdst); 
     while( !infile.eof() ) // To get you all the lines.    
     {        
        getline(infile,stringLine); // Saves the line in stringLine.        
        // stringLine= trim(stringLine);
        stringip = getip(stringLine);
        if(it1==stringip)
           cout<<stringLine<<endl;
     } 
     infile.close();
  
}

string& trim(string &s) 
{
    if (s.empty()) 
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void  stat_ip(string filestr,int count)
{
    string stringbuf; 
    string stringLine;    
    ifstream infile;    
    vector<string> tempLine;    
    vector<string> vectLine;    
    vector<string> serchLine;    
    vector<string> resultLine;    
   // ofstream fout( "stat_result.txt", ios::app);    
    infile.open (filestr);    
    while( !infile.eof() ) // To get you all the lines.    
    {        
        getline(infile,stringLine); // Saves the line in stringLine.        
        // stringLine= trim(stringLine);
        stringLine = getip(stringLine);
        vectLine.push_back(stringLine); 
        serchLine.push_back(stringLine); 
        stringbuf.append(stringLine);

        char_separator<char> sep(" ");        
        tokenizer<char_separator<char>> tok(stringLine, sep);         
        for(tokenizer<char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end(); ++beg)        
        {            
            tempLine.push_back( *beg );        
        }        
    }

    infile.close();
    int num; 
    for(auto ipstr: tempLine)
    { 
        num=0; 
        for(auto it1: serchLine)
        {
            if(ipstr==it1)
                num++;
        }        
        if( std::count(serchLine.begin(), serchLine.end(), ipstr) !=num)
        {
           cout<<"erro  "<< ipstr<<endl;
        } 
        //if( stat_str( stringbuf, ipstr)>=count)
        if( num>=count)
        {
             int nRet = std::count(resultLine.begin(), resultLine.end(), ipstr);
             if(nRet==0)
                resultLine.push_back(ipstr );
        }
    }
   
    for(auto ipstr: resultLine)
    {   
       cout<<ipstr<<"   " <<endl;
    }
 
    return ;
}
 

int  stat_str(string & srcstr, string  cmpstr ) 
{
    int n=0,sum=0;
     
    string temp=srcstr;
    while( 1 )
    {  
        //循环：查找到子串就把子串删去
        n=temp.find(cmpstr); 
        //返回子串的位置
        if (n!=-1)
        {//n=-1表示未找到子串
            temp=temp.substr(n+cmpstr.length(),temp.length()-cmpstr.length());//开一个新的字符串变量temp存储删去子串后的字符串
            sum++;  //出现次数
        }
        else 
            break;
    }
    {
    //    cout<<cmpstr <<"   " <<sum<<endl;
    }
    return sum;
}

string  getip(string ipline)
{
    std::string result;

    std::string regString("(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)");    
    std::smatch ms;    
    std::regex_constants::syntax_option_type fl = std::regex_constants::icase;    
    std::regex regExpress(regString, fl);    

     // 查找     
    if(std::regex_search(ipline, ms, regExpress))
    {
        result= ms[0];
        return result;
    } 
    return  string("");
    
}

int test_regex()
{  
    std::string text("我的IP地址是:109.168.0.1.");    
    std::string newIP("127.0.0.1");    
    std::string regString("(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)");    
     
    // 表达式选项 - 忽略大小写     
    std::regex_constants::syntax_option_type fl = std::regex_constants::icase;    
         
    // 编译一个正则表达式语句     
    std::regex regExpress(regString, fl);    
     
    // 保存查找的结果     
    std::smatch ms;    
     
    // 判断是否全行匹配     
    if(std::regex_match(text, ms, regExpress))    
    {    
        std::cout<<("正则表达式:")<<regString<<("匹配:")<<text<<("成功.")<<std::endl;    
    }    
    else    
    {    
        std::cout<<("正则表达式:")<<regString<<("匹配:")<<text<<("失败.")<<std::endl;    
    }    
     
    // 查找     
    if(std::regex_search(text, ms, regExpress))    
    {    
        std::cout<<("正则表达式:")<<regString<<("查找:")<<text<<("成功.")<<std::endl;    
        for(size_t i= 0; i < ms.size(); ++i)    
        {    
            std::cout<<("第")<<i<<("个结果:\"")<<ms.str(i)<<("\" - ");    
            std::cout<<("起始位置:")<<ms.position(i)<<("长度")<<ms.length(i)<<std::endl;    
        }    
        std::cout<<std::endl;    
     
        // 替换1     
        text = text.replace(ms[0].first, ms[0].second, newIP);    
        std::cout<<("替换1后的文本:")<<text<<std::endl;    
    }    
    else    
    {    
        std::cout<<("正则表达式:")<<regString<<("查找:")<<text<<("失败.")<<std::endl;    
    }    
     
    // 替换2     
    newIP = "255.255.0.0";    
    std::string newText = std::regex_replace( text, regExpress, newIP);    
    std::cout<<("替换2后的文本:")<<newText<<std::endl;    
     
    // 结束     
    std::cout<<("按回车键结束...");    
    std::cin.get();    
       
    return 0;  
}
