#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include "readcfg.h"

using namespace std;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

int testlog( );
void initlog()
{
//    boost::log::add_file_log("dbmgr.log", keywords::auto_flush = true);  
    auto file_sink = logging::add_file_log  
        (  
        keywords::file_name = "dbmgr_%N.log",                                        /*< file name pattern >*/  
        keywords::auto_flush = true,
        keywords::rotation_size = 10 * 1024 * 1024,                                   /*< rotate files every 10 MiB... >*/  
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/  
        keywords::format = "[%TimeStamp%]: %Message%"                                 /*< log record format >*/  
        ); 

//    logging::core::get()->set_filter(logging::trivial::severity>=logging::trivial::trace);
    file_sink->locked_backend()->set_file_collector(sinks::file::make_collector(  
        keywords::target = "logs",        //folder name.  
        keywords::max_size = 50 * 1024 * 1024,    //The maximum amount of space of the folder.  
        keywords::min_free_space = 100 * 1024 * 1024  //Reserved disk space minimum.  
        ));  
  
    file_sink->locked_backend()->scan_for_files();  
      
    logging::core::get()->set_filter  
    (  
        logging::trivial::severity >= logging::trivial::info  
    );  
    logging::add_common_attributes();

}


int readcfg(int & startheight, int  &endheight, string & dbuser, string &dbhost,string &dbpass)
{
    boost::property_tree::ptree m_pt, tag_setting;
    read_ini("config.ini", m_pt);
    tag_setting = m_pt.get_child("config");
    startheight = tag_setting.get<int>("startheight", 10000);   // tcp listen port
    endheight   = tag_setting.get<int>("endheight", 20000);   // tcp listen port
    dbuser  = tag_setting.get<string>("dbuser", "");   // tcp listen port
    dbhost   = tag_setting.get<string>("dbhost", "");   // tcp listen port
    dbpass   = tag_setting.get<string>("dbpasswd", "");   // tcp listen port

    initlog();
    testlog();
    return 0;
}




int testlog( )
{
    BOOST_LOG_TRIVIAL(trace)<<"A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An from informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A from warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An from error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

 /*/   using namespace logging::trivial;  
    src::severity_logger< severity_level > lg;  
  
    BOOST_LOG_SEV(lg, trace) << "A trace severity message (测试中文能否正确输出（GBK编码）)";  
    BOOST_LOG_SEV(lg, debug) << "A debug severity message";  
    BOOST_LOG_SEV(lg, info) << "An informational severity message";  
    BOOST_LOG_SEV(lg, warning) << "A to warning severity message";  
    BOOST_LOG_SEV(lg, error) << "An to error severity message";  
    BOOST_LOG_SEV(lg, fatal) << "A to fatal severity message";    
*/
    system("pause");
    return 0;
}
