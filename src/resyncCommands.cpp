#include <dpp/dpp.h>
#include "resyncCommands.h"

using namespace dpp;
void resyncCommands(dpp::cluster &bot) {
    auto whois_command =
          new slashcommand("whois", "List a user's entries!", bot.me.id);
      whois_command->add_option(command_option(
          command_option_type::co_user, "user", "User", true));
      bot.global_command_create(*whois_command);
      delete whois_command;

      auto query_command =
          new slashcommand("query", "Search for a description!", bot.me.id);
      query_command->add_option(
          command_option(command_option_type::co_string, "criteria",
                         "The criteria to search by.", true));
      bot.global_command_create(*query_command);
      delete query_command;
}