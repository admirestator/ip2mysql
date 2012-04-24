/* Standard C++ headers */
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <stdexcept>
//#include <stddef.h>

/* MySQL Connector/C++ specific headers */
#include "./include/cppconn/driver.h"
#include "./include/cppconn/connection.h"
#include "./include/cppconn/statement.h"
#include "./include/cppconn/prepared_statement.h"
#include "./include/cppconn/resultset.h"
#include "./include/cppconn/metadata.h"
#include "./include/cppconn/resultset_metadata.h"
#include "./include/cppconn/exception.h"
#include "./include/cppconn/warning.h"
//#include <driver/mysql_public_iface.h>


#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWD ""
#define DATABASE "ipdb"
#define TABLE	"qqwry"

#define NUMOFFSET 100
#define COLNAME 200

/*control bat operation's number
  like read ip item of insert sql*/
#define BATNUM	200

using namespace std;
using namespace sql;


//iostream operation
ifstream& open_file (ifstream & in, const string & file);
ofstream& open_file (ofstream & out, const string & file);

typedef struct _czipstruct {
    size_t id;
    string ip1;
    string ip2;
    string country;
    string state;
    string region;
    string location;
}IpAddr;

int main(int argc, char *argv[])
{
    //ipdata in stream
    ifstream ipdatain;
    static size_t total_ip;
    register vector<IpAddr> ip_addr;
    vector<IpAddr>::iterator ip_addr_idx;
    
    if (!open_file(ipdatain, "ipdb")) {
        throw runtime_error ("open ip library failed!");
    }

    try {
        Driver * driver = get_driver_instance();

        /* Using the Driver to create a connection */
        auto_ptr<Connection > con(driver->connect(DBHOST, USER, PASSWD));
        
        /* Creating a " - "simple" = not a prepared statement */
        auto_ptr< Statement > stmt(con->createStatement());

        //open ip library in text file for read

    
        //connect to mysql ipdb
        stmt->execute("USE ipdb");
        stmt->execute("DROP TABLE IF EXISTS qqwry");

        //create ipdata table name 'ipdb'
        stringstream sqlcmd;
        sqlcmd.str("");
        sqlcmd << "CREATE TABLE qqwry(";
        sqlcmd << "id INT NOT NULL AUTO_INCREMENT,";
        sqlcmd << "ip1 CHAR(15) NOT NULL,";
        sqlcmd << "ip2 CHAR(15) NOT NULL,";
        sqlcmd << "country CHAR(64) NOT NULL,";
        sqlcmd << "state CHAR(64),";
        sqlcmd << "region CHAR(64),";
        sqlcmd << "location CHAR(64) NOT NULL, PRIMARY KEY(id));";
        stmt->execute(sqlcmd.str());
        cout << "++\t ipdata table created" << endl;

        register IpAddr tmpip;
            //read data from ip library 200 row/ one time
        string line , word;
        list<string> tmplist;
        list<string>::iterator tmpidx;
        
        //tmpidx = tmplist.end(); 
        while(getline(ipdatain,line)) {
            ++total_ip;
            istringstream stm(line);
            tmpip.id = total_ip;
            while (!stm.eof()) {
                stm >> word;
                tmplist.push_back(word);
            }
            tmpidx = tmplist.begin();
            tmpip.ip1 = *tmpidx++;
            tmpip.ip2 = *tmpidx++;
            tmpip.country = *tmpidx++;
            tmpip.location = *tmpidx++;
            
            while (++tmpidx != tmplist.end()) {
                //cout << "%" << *tmpidx << "%"<< endl;
                tmpip.location = tmpip.location + " " +  *tmpidx;
                //cout << "@" << tmpip.location << "@" << endl;
                tmpidx++;
            }
            /*
              cout << "< [" << total_ip << "] " << tmpip.ip1 << " "
                   << tmpip.ip2 << " " << tmpip.country << " "
              << tmpip.location << ">\n";
            */
            ip_addr.push_back(tmpip);
            tmpip.location = "";
            tmplist.clear();
            
            //insert ip data into mysql 200 rows one time
            if (total_ip % 200 == 0) {
                vector<IpAddr>::const_iterator
                    ip_addr_idxcst = ip_addr.begin();
                stringstream tmpcmd;

                tmpcmd.str("");
                //cout << "cc"<< endl;
                for (int i = 0; i < BATNUM; ++ip_addr_idxcst, ++i) {
                    //cout << "cd" << endl;
                    tmpcmd << "(\"" << ip_addr_idxcst->id << "\", \""
                           << ip_addr_idxcst->ip1 << "\", \""
                           << ip_addr_idxcst->ip2 << "\", \""
                           << ip_addr_idxcst->country << "\", \""
                           << "ccstate" << "\", \""
                           << "ccregion"<< "\", \""
                           << ip_addr_idxcst->location << "\"), ";
                    /*
                    cout  << "(\"" << ip_addr_idxcst->id << "\", \""
                           << ip_addr_idxcst->ip1 << "\", \""
                           << ip_addr_idxcst->ip2 << "\", \""
                           << ip_addr_idxcst->country << "\", \""
                           << "ccstate" << "\", \""
                           << "ccregion"<< "\", \""
                          << ip_addr_idxcst->location << "\"), "<< endl;
                    */
                }
                //cout << "[tmpcmd] " << tmpcmd.str()  << endl;
                //clear vector for new 200 ip info.
                tmpcmd.seekp((int)tmpcmd.tellp()-2);
                tmpcmd << " ";
                sqlcmd.str("");
                //cout << "cts" << endl;
                sqlcmd << "INSERT INTO qqwry VALUES" + tmpcmd.str();
                //cout << "id[" << ip_addr_idxcst->id << "] sqlcmd: "
                // << sqlcmd.str() << endl;
                stmt->execute(sqlcmd.str());
                ip_addr.clear();
            }
        }
        cout << "insert ip data into mysql success!" << endl;
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        cout << "not ok 1 - examples/connect.php" << endl;
        
        return EXIT_FAILURE;
    } catch (std::runtime_error &e) {
        cout << "# ERR: runtime_error in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what() << endl;
        cout << "not ok 1 - examples/connect.php" << endl;

        return EXIT_FAILURE;
    }
    
//close library
ipdatain.close();
return 0;
}

ifstream& open_file (ifstream & in, const string & file)
{
    in.close ();
    in.clear ();
    in.open (file.c_str ());
     
    return in;
}
 
ofstream& open_file (ofstream & out, const string & file)
{
    out.close ();
    out.clear ();
    out.open (file.c_str ());
    
    return out;
}
