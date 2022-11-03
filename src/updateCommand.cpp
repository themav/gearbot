//
// Created by mk on 9/22/22.
//

#include "updateCommand.h"
#include "DatabaseUtils.h"

/* This function runs the channel topic update on the current channel */
void updateCommand(const dpp::slashcommand_t& event, DatabaseUtils& db) {
    dpp::cluster* _cl = event.from->creator; //Get a pointer to the bot cluster via the event.
    //We use the cluster to get the channel object the event happened in, then we set the topic to the topic string from Database Utils.
    _cl->channel_edit(_cl->channel_get_sync(event.command.channel_id).set_topic(db.getTopic()));
    event.reply("Channel topic manually refreshed. No new data committed."); //Send confirmation to the user
}