//
// Created by mk on 9/23/22.
//

#include "buyCommand.h"

void buyCommand(const dpp::slashcommand_t& event) {
    bool is_skis = event.command.get_command_name() == "buyskis"; //if it's buy skis then it's true
    dpp::cluster* _cl = event.from->creator; //Get a pointer to the bot cluster via the event.
    DatabaseUtils* db = new DatabaseUtils("192.168.1.116", _cl);
    const std::string desc = std::get<std::string>(event.get_parameter(is_skis ? "skis" : "gear")); //Get the input and use a ternary expression to select the right parameter.
    db->insertPurchase(std::to_string(event.command.get_issuing_user().id), is_skis, desc); //Convert snowflake (user ID) into string and insert it.
    event.reply("<:spendit:758054760200012018> " + event.command.get_issuing_user().username + " has purchased some " + (is_skis ? "skis" : "gear") + "!\n" "`" + desc + "`");
    _cl->channel_edit(_cl->channel_get_sync(event.command.channel_id).set_topic(db->getTopic())); //Update channel topic after inserting.
    delete db; //Reply to the user (above), and free memory.
}

void deleteCommand(const dpp::slashcommand_t& event) {
    DatabaseUtils db{"192.168.1.116", nullptr}; //Instantiate  my database utils helper class
    //If true from the delete user entry function of the database utils class, then change which message we send.
    event.reply(db.deleteUserEntry(event.command.get_issuing_user().id) ? "Your most recent entry was deleted."
                                                                         : "No records deleted. You either did not make the most recent entry, or another error occurred.");
}