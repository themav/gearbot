//
// Created by mk on 9/25/22.
//

#include "DatabaseUtils.h"
#include <mariadb/mariadb_com.h>
#include <mariadb/mysql.h>
#include "fmt_table.hpp"

/* This function takes the ID of the purchase user, whether it's skis, and a short description */
void DatabaseUtils::insertPurchase(const std::string& purchaser, bool& is_skis, const std::string& desc) {
    MYSQL_STMT *statement = mysql_stmt_init(conn); //Create a prepared statement
    mysql_stmt_prepare(statement, "INSERT INTO purchases_test(purchase_date, purchase_user, is_skis, description) VALUES (NOW(), ?, ?, ?);", -1); //Create the statement with the SQL
    MYSQL_BIND bind[3]; memset(bind, 0, sizeof(bind)); //Create the data binding array and clear it out;
    bind[0].buffer_type = MYSQL_TYPE_STRING; bind[0].buffer = (void*)purchaser.c_str(); bind[0].buffer_length = purchaser.length(); //Bind the purchaser string value, length is required to it knows when to stop reading the buffer
    bind[1].buffer_type = MYSQL_TYPE_TINY; bind[1].buffer = &is_skis; //Bind the boolean value
    bind[2].buffer_type = MYSQL_TYPE_STRING; bind[2].buffer = (void*)desc.c_str(); bind[2].buffer_length = desc.length(); //Bind description. Length is still important.
    std::cout << purchaser.c_str() << " " << desc.c_str() << '\n'; //Debug output
    mysql_stmt_bind_param(statement, bind); //Attach binding array to prepared statement.
    mysql_stmt_execute(statement); //Execute Statement
    mysql_stmt_close(statement); //Free memory
}

/* This function uses the database connection to calculate what the channel topic should be changed to. */
std::string DatabaseUtils::getTopic() {
    MYSQL_RES *result; //Create a pointer to store a result set
    mysql_query(conn, "SELECT purchase_user,purchase_date from purchases where is_skis = 1 ORDER BY purchase_date DESC limit 1;"); //Query for the first one.
    result = mysql_store_result(conn); MYSQL_ROW skis = mysql_fetch_row(result); //Get result set and first row.
    std::string ski_buyer = getUserName(skis[0]); //Store data in variables
    uint ski_days = time_tDaysAgo(string2time_t(skis[1])); //Parse DATETIME and calculate no. of days
    mysql_free_result(result); //Free result

    mysql_query(conn, "SELECT purchase_user,purchase_date from purchases where is_skis = 0 ORDER BY purchase_date DESC limit 1;");
    result = mysql_store_result(conn); MYSQL_ROW gear = mysql_fetch_row(result);
    std::string gear_buyer = getUserName(gear[0]);
    uint gear_days = time_tDaysAgo(string2time_t(gear[1]));
    mysql_free_result(result);//Free memory

    std::string topic = "Skis: " + ski_buyer + ", " + std::to_string(ski_days) + " day(s). Gear: " + gear_buyer
                        + ", " + std::to_string(gear_days) + " day(s).";
    std::cout << topic << '\n';
    return topic;
}

/* This function will delete the most recent entries on the "stack" if it belongs to the user who calls the function
 * Returns: true if rows were deleted. false if there was an error or no rows were deleted. */
bool DatabaseUtils::deleteUserEntry(const dpp::snowflake &purchaser) {
    bool completed = false;
    std::string user_id = std::to_string(purchaser); //Set purchaser to string
    MYSQL_STMT *statement = mysql_stmt_init(conn);
    //Look I know this SQL is fugly however it's better than implementing the logic on the client end
    mysql_stmt_prepare(statement, "DELETE FROM purchases_test WHERE id IN("
                                  "WITH temp_purchase AS ("
                                  " (SELECT id,purchase_user FROM purchases_test where is_skis=0 ORDER BY purchase_date DESC limit 1) UNION (SELECT id,purchase_user FROM purchases_test where is_skis=1 ORDER BY purchase_date DESC limit 1)"
                                  ") SELECT id FROM temp_purchase WHERE purchase_user = ?);", -1);
    MYSQL_BIND bind[1]; memset(bind, 0, sizeof(bind));
    bind[0].buffer_type = MYSQL_TYPE_STRING; bind[0].buffer = (void *)user_id.c_str(); bind[0].buffer_length = user_id.length(); //Create the parameter binding to the string
    mysql_stmt_bind_param(statement, bind); //Bind parameters
    mysql_stmt_execute(statement); //Run DELETE statement
    if(mysql_stmt_affected_rows(statement) > 0) //If there was an error (-1) or if there were no rows to delete, return false.
        completed = true;
    mysql_stmt_close(statement);//Free memory
    return completed;
}

/* This function will return a formatted text table representing the user's
 * purchases Returns: a string containing a formatted fixed-width table */
std::string DatabaseUtils::getUserEntries(const dpp::snowflake &_user) {
  std::string query{
      "SELECT is_skis,description FROM purchases WHERE purchase_user = \'" +
      std::to_string(_user) + "\'"};
  mysql_query(conn, query.c_str());
  MYSQL_RES *result = mysql_store_result(conn);
  fmt_table my_table;
  my_table.add_cell("Type");
  my_table.add_cell("Description");
  my_table.end_row();
  while (auto row = mysql_fetch_row(result)) {
    for (unsigned int i = 0; i < mysql_num_fields(result); i++) {
      if (row[i] == NULL)
        my_table.add_cell("");
      else
        my_table.add_cell(row[i]);
    }
    my_table.end_row();
  }
  mysql_free_result(result);
  return my_table.print_table();
}

/* This function searches using _criteria in the description field of
 * the purchases table.
 * Returns: A string with a fixed-width formatted table containing the results*/
std::string DatabaseUtils::searchIn(const std::string &_criteria) {
  std::string query{"SELECT purchase_user,description FROM purchases "
                    "WHERE description LIKE \'%" +
                    _criteria + "%\'"};
  // Check for SQL injection by making sure ; is NOT found
  if (query.find(";") != std::string::npos) // Not equal to not found
    return "Nice try at SQL Injection.";
  mysql_query(conn, query.c_str());
  MYSQL_RES *result = mysql_store_result(conn);
  fmt_table my_table;
  my_table.add_cell("User");
  my_table.add_cell("Description");
  my_table.end_row();
  while (auto row = mysql_fetch_row(result)) {
    for (unsigned int i = 0; i < mysql_num_fields(result); i++) {
      if (row[i] == NULL)
        my_table.add_cell("");
      else
        my_table.add_cell((i == 0) ? this->getUserName(row[i]) : row[i]);
    }
    my_table.end_row();
  }
  mysql_free_result(result);
  return my_table.print_table();
}

/* This helper function gets the username of the user */
std::string DatabaseUtils::getUserName(const std::string& _user) {
    return _cluster->user_get_sync(_user).username;
}

/* This function converts the MySQL/MariaDB date time into a c time_t value */
time_t string2time_t(const std::string& mysql_date_time) {
    tm time{}; //Create tm structure
    strptime(mysql_date_time.c_str(), "%F %T", &time); //Parse out time string using the format
    return mktime(&time); //Return time_t from the time structure
}

/* This function tells you how many days ago the event happened from today's date.
 * Returns: 0 if the date is in the future.
 * Otherwise: the number of days ago the event happened*/
uint time_tDaysAgo(const time_t& _time) {
    if(_time > time(nullptr)) //Return 0 if the date is in the future.
        return 0;
    //Difference (in ms) between times. Then divide by a magic number get days.
    return (uint)std::round(std::difftime(time(nullptr), _time) / (60*60*24));
}
