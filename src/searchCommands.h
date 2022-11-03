// Created by MK on 10/25/22

#ifndef JIBBLESBOT_SEARCHCOMMAND_H
#define JIBBLESBOT_SEARCHCOMMAND_H

#include <dpp/dpp.h>
#include "fmt_table.hpp"
#include "DatabaseUtils.h"

void whoisCommand(const dpp::slashcommand_t &event, DatabaseUtils& db);
void queryCommand(const dpp::slashcommand_t& event, DatabaseUtils& db);

#endif