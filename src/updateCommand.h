//
// Created by mk on 9/22/22.
//

#ifndef JIBBLESBOT_UPDATECOMMAND_H
#define JIBBLESBOT_UPDATECOMMAND_H
#include "DatabaseUtils.h"
#include <dpp/dpp.h>
void updateCommand(const dpp::slashcommand_t& event, DatabaseUtils& db);

#endif //JIBBLESBOT_UPDATECOMMAND_H
