//
// Created by mk on 9/22/22.
// This function handles when a user types /help
//

#include "HelpCommand.h"
#include "VERSION.h"

void helpCommand(const dpp::slashcommand_t& event) {
    dpp::embed *help_message = new dpp::embed();
    help_message->set_color(dpp::colors::sti_blue); //Set the sidebar color
    help_message->set_title("Gearbot Help Information");
    help_message->add_field("Basic Command Help:",
                            "`/buyskis <model of ski here>` \n `/buygear <gear here>` \n Do not include the angle brackets when specifying the gear purchased."
                            "\n For example: If I purchased the ON3P Woodsman at 108mm underfoot and 187cm long, I would type `/buyskis ON3P Woodsman 108 187cm`.");
    //help_message->add_field("\u200b", "\u200b"); //Add a blank space between groups.
    help_message->add_field("Additional Information: ",
                            "Accidental entry? use `/delentry` to remove the last entry if it is yours. "
                            "\n Channel topic not up to date? use `/updatetopic` to manually update."
                            "\n Want to see who's who? Run `/search <name>` to see what they've gotten recently!"
                            "\n What about by gear model or brand? Use `/query <criteria>` to search by description!");
    help_message->add_field("\u200b", "Gearbot version " + VERSION + " made by <@224194164743471114>.");
    event.reply(dpp::message(event.command.channel_id, *help_message)); //send the message to user
    delete help_message; //Deconstruct and free memory.
    std::cout << "Help Command Ran \n";
}