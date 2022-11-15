#include "DatabaseUtils.h"
#include "HelpCommand.h"
#include "VERSION.h"
#include "buyCommand.h"
#include "config_store.h"
#include "resyncCommands.h"
#include "searchCommands.h"
#include "updateCommand.h"
#include <csignal>
#include <dpp/dpp.h>
#include <iostream>
#include <stdexcept>


void signal_handler(int signal);
dpp::cluster *bot;

using namespace dpp;
int main() {
  std::cout << "Welcome to JibblesBot " << VERSION << std::endl;
  config_store config{"config.txt"};

  /* Create bot cluster */
  bot = new cluster(config.get_value("AUTH_TOKEN"));

  
  /* Log to stdout */
  bot->on_log(dpp::utility::cout_logger());

  /* Handle slash command */
  bot->on_slashcommand([](const dpp::slashcommand_t &event) {
    /* Instantiate Database Utilities*/
  DatabaseUtils db(config.get_value("MARIADB_HOSTNAME"),
                   config.get_value("MARIADB_USERNAME"),
                   config.get_value("MARIADB_PASSWORD"), bot);
    /* Run the right command depending on what the command is */
    if (event.command.get_command_name() == "help") {
      helpCommand(event);
    } else if (event.command.get_command_name() == "buygear" ||
               event.command.get_command_name() == "buyskis") {
      buyCommand(event, db);
    } else if (event.command.get_command_name() == "delentry") {
      deleteCommand(event, db);
    } else if (event.command.get_command_name() == "updatetopic") {
      updateCommand(event, db);
    } else if (event.command.get_command_name() == "query") {
      queryCommand(event, db);
    } else if (event.command.get_command_name() == "whois") {
      whoisCommand(event, db);
    }
  });

  try {
    if (config.get_value("RESYNC_COMMANDS") == "true") {
      bot->log(loglevel::ll_warning, "Re-syncing commands with Discord.");
      /* Register slash command here in on_ready */
      bot->on_ready([](const dpp::ready_t &event) {
        /* Wrap command registration in run_once to make sure it doesnt run on
         * every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
          resyncCommands(*bot);
        }
      });
    }
  } catch (std::out_of_range &e) {
    bot->log(loglevel::ll_info,
             "RESYNC_COMMANDS not specified in config file.");
  }
  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);
  // Start the bot
  bot->start(true);
  // Lock this thread until the bot is terminating.
  std::mutex thread_mutex;
  std::unique_lock<std::mutex> thread_lock(thread_mutex);
  bot->terminating.wait(thread_lock);
  // Cleanup Program
  delete bot;
  delete db;
  return 0;
}
// Catch a signal so we can gracefully shutdown the bot.
void signal_handler(int signal) {
  bot->log(loglevel::ll_warning,
           "Caught SIGTERM/SIGINT (Ctrl-C). Shutting down Discord Bot.");
  bot->shutdown();
}