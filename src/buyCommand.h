//
// Created by mk on 9/23/22.
//

#ifndef JIBBLESBOT_BUYCOMMAND_H
#define JIBBLESBOT_BUYCOMMAND_H

#include <dpp/dpp.h>
#include "DatabaseUtils.h"
void buyCommand(const dpp::slashcommand_t &event, DatabaseUtils& db);
void deleteCommand(const dpp::slashcommand_t &event, DatabaseUtils& db);
#endif //JIBBLESBOT_BUYCOMMAND_H
