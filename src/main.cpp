#include <iostream>
#include <dpp/dpp.h>
#include "VERSION.h"
#include "HelpCommand.h"
#include "buyCommand.h"
#include "updateCommand.h"
const std::string AUTH_TOKEN = "ODg4ODcxNzgzNzg3Mjk0NzIw.GGGoZb.iyxx_QyRmpgIaJo6IM-ww5yOucT-jMvpkGJAEY";

int main() {
    std::cout << "Welcome to JibblesBot " << VERSION << std::endl;
    /* Create bot cluster */
    dpp::cluster bot(AUTH_TOKEN);

    /* Log to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        /* Run the right command depending on what the command is */
        if (event.command.get_command_name() == "help") {
            helpCommand(event);
        }
        else if (event.command.get_command_name() == "buyskis" || event.command.get_command_name() == "buygear") {
            buyCommand(event);
        }
        else if (event.command.get_command_name() == "delentry") {
            deleteCommand(event);
        }
        else if (event.command.get_command_name() == "updatetopic") {
            updateCommand(event);
        }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            //bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
    });
    //Start the bot
    bot.start(false);
    return 0;
}
