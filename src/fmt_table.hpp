/* Formatted Text Table Class. I wrote this helper class because Discord sucks.
 * Discord doesn't provide a way to tabulate data, so preformatted text is the
 * only sane way to do it. This could also be used for console output as well.
 *
 * The first row inserted should be the table's headers. Also, the total number
 * of columns is calculated off the first row. Extra columns without a header
 * will not print. Set the cell header to blank ("") if needed.
 *
 * Copyright (C) MK 2022. Licensed under the MIT license.
 */
#pragma once

#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>

class fmt_table {
public:
  typedef std::vector<std::string> table_row;

  // Add a cell to the current row
  void add_cell(const std::string &str) { _cur_row.push_back(str); }

  // copy the current row to the table; clear out for next row.
  void end_row() {
    _table.push_back(_cur_row);
    _cur_row.clear();
  }

  // Directly add a row to the table
  void add_row(const table_row &row) { _table.push_back(row); }

  // Print out the formatted table.
  // The theory of operation here is we first determine number of table columns
  // and calculate the width of each column. Then we use those calculated values
  // to create the table row separator. After that, we can iterate through each
  // row and print a separator plus the contents of the cell, padded to the
  // column width.
  // Returns: A formatted string representing the table
  std::string print_table() {
    std::stringstream output;
    std::left(output); // Left justify everything.
    if (_table.empty())
      return output.str();               // If we're empty, do nothing.
    size_t col_count = _table[0].size(); // First row is the header.
    size_t width[col_count];             // Array to store the column widths in.
    std::stringstream separator;
    for (size_t i = 0; i < col_count; i++) {
      separator << _corner;    // Set the corner
      width[i] = row_width(i); // Calculate width of each column.
      for (size_t a = 0; a < width[i]; a++)
        separator << _line; // Output the number of dashes equal to the width of
                            // the column.
    }
    separator << _corner << std::endl;
    output << separator.str(); // Output the initial line.
    for (auto &row : _table) {
      size_t row_size =
          std::min(col_count, row.size()); // Ensure we don't go out of bounds.
      for (size_t i = 0; i < row_size; i++) {
        output << _bar; // Leading Edge of cell
        output << std::setw(width[i]) << row.at(i);
      } // The above pads the cell contents with spaces to fill the width.
      output << _bar << std::endl; // Trailing edge of last cell and new line.
      output << separator.str();
    }
    return output.str();
  }

private:
  const char _corner{'+'};
  const char _bar{'|'};
  const char _line{'-'};
  std::vector<table_row> _table; // A collection of all rows.
  table_row _cur_row;            // Current working row.

  // This function determines the maximum width of a column given an index.
  size_t row_width(unsigned int row_idx) {
    size_t _width{0};
    if (_table.empty())
      return _width; // Zero if there's nothing.
    for (auto &row : _table) {
      try { // The following compares each width value and keeps the largest
        _width = _width > row.at(row_idx).length() ? _width
                                                   : row.at(row_idx).length();
      } catch (std::out_of_range &e) {
      } // Do nothing because we already have a value if the field is blank.
    }
    return _width;
  }
};
