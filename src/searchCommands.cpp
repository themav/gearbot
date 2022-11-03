// Created by MK on 10/25/22
// This handles the search commands

#include "searchCommands.h"


void whoisCommand(const dpp::slashcommand_t &event, DatabaseUtils& db) {
  std::string table = //Cast the user paramater to a snowflake (ID) value and get the results.
      db.getUserEntries(std::get<dpp::snowflake>(event.get_parameter("user")));
  event.reply("```\n" + table + "```"); //Output fixed-width message
}

void queryCommand(const dpp::slashcommand_t &event, DatabaseUtils& db) {
  std::string table = //Cast the criteria paramater to a string and get the results.
      db.searchIn(std::get<std::string>(event.get_parameter("criteria")));
  event.reply("```\n" + table + "```");
}