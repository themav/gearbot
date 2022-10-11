//
// Created by mk on 9/25/22.
//

#ifndef JIBBLESBOT_DATABASEUTILS_H
#define JIBBLESBOT_DATABASEUTILS_H

#include <ctime>
#include <string>
#include <dpp/dpp.h>
#include <mariadb/mysql.h>

/*
 * The DatabaseUtils class represents an object to wrap database related functions in my discord bot.
 */
class DatabaseUtils {
private:
    MYSQL *conn;
    std::string hostname;
    dpp::cluster* _cluster;
    std::string getUserName(const std::string& _user);
public:
    void insertPurchase(const std::string& purchaser, bool& is_skis, const std::string& desc);
    bool deleteUserEntry(const dpp::snowflake& purchaser);
    std::string getTopic();
    DatabaseUtils(const std::string& _hostname, dpp::cluster* _cl) {
        conn = mysql_init(0);
        hostname = _hostname;
        mysql_real_connect(conn, //open connection
                           hostname.c_str(), //using hostname provided
                           "jibbles", //Hardcoded username
                           "PASSWORD", //Hardcoded password
                           "jibblesbot", //Hardcoded database name
                           3306, NULL, 0);
        _cluster = _cl;
    }
    ~DatabaseUtils() {
        mysql_close(conn); //Close connection and free memory.
    }

};
/* This function converts the MySQL/MariaDB date time into a c time_t value */

time_t string2time_t(const std::string& mysql_date_time);
uint time_tDaysAgo(const time_t& _time); //How many days have elapsed
#endif //JIBBLESBOT_DATABASEUTILS_H
