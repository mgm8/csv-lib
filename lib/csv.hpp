/*
 * csv.hpp
 * 
 * Copyright (C) 2018, Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * This file is part of CSV-Lib.
 * 
 * CSV-Lib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * CSV-Lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with CSV-Lib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file csv.hpp
 * 
 * \brief CSV library main class.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 28/04/2018
 * 
 * \defgroup csv CSV
 * \ingroup csvlib
 * \{
 */

#ifndef CSV_H_
#define CSV_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * \class CSV
 *
 * \brief CSV library main class.
 */
template <class TCell>
class CSV
{
    private:
        std::vector<std::vector<TCell> > cells;     /**< CSV table buffer. */
    public:
        /**
         * \brief Class constructor without initiliazation.
         *
         * \return None
         */
        CSV()
        {

        }

        /**
         * \brief Class constructor with table's size initialization.
         *
         * \param cols is the number of columns of the CSV table.
         * \param rows is the number of rows of the CSV table.
         *
         * \return None
         */
        CSV(unsigned int cols, unsigned int rows)
        {
            std::vector<TCell> row;

            for(unsigned int i=0; i<cols; i++)
            {
                row.push_back(TCell());
            }

            for(unsigned int j=0; j<rows; j++)
            {
                this->AppendRow(row);
            }
        }

        /**
         * \brief Class constructor ().
         *
         * \param in_file
         *
         * \return None
         */
        CSV(const char *in_file)
        {
            this->Read(in_file);
        }

        /**
         * \brief Class destructor.
         *
         * \return None
         */
        ~CSV()
        {
            this->Clear();
        }

        /**
         * \brief Reads a CSV table from a CSV file.
         *
         * \param file is the CSV file to read.
         *
         * \return None
         */
        void Read(const char *file)
        {
            // Open a file as an input file
            std::fstream input(file, std::fstream::in);

            if (input.is_open())
            {
                this->Clear();

                char c;
                std::string cell_buf;
                std::vector<TCell> row;

                while(input.get(c))
                {
                    std::stringstream convert(cell_buf);
                    TCell val;

                    switch(c)
                    {
                        case ',':
                            convert >> val;
                            row.push_back(val);
                            cell_buf = "";
                            break;
                        case '\n':
                            convert >> val;
                            row.push_back(val);
                            cell_buf = "";
                            this->AppendRow(row);
                            row.clear();
                            break;
                        default:
                            cell_buf += c;
                    }
                }

                input.close();
            }
            else
            {
                throw std::runtime_error("Error openning the input file!");
            }
        }

        /**
         * \brief Write the CSV table to a file.
         *
         * \param file is the file to write the CSV table.
         *
         * \return None
         */
        void Write(const char *file)
        {
            // Open a file as an output file
            std::fstream output(file, std::fstream::out);

            if (output.is_open())
            {
                for(unsigned int i=0; i<this->cells.size(); i++)
                {
                    for(unsigned int j=0; j<this->cells[i].size(); j++)
                    {
                        output << this->cells[i][j];

                        if (j < this->cells[i].size()-1)    // Check if it is in the last column
                        {
                            output << ",";
                        }
                    }

                    // End of column, add a new line
                    output << std::endl;
                }

                output.close();
            }
            else
            {
                throw std::runtime_error("Error creating the output file!");
            }
        }

        /**
         * \brief Clears the CSV table (delete all its contents and set the size to zero).
         *
         * \return None
         */
        void Clear()
        {
            this->cells.clear();
        }

        /**
         * \brief Returns the number of columns of the current CSV table.
         *
         * \return The number of columns of the current CSV table.
         */
        unsigned int GetColumns()
        {
            return this->cells[0].size();
        }

        /**
         * \brief Returns the number of rows of the current CSV table.
         *
         * \return The number of rows of the current CSV table.
         */
        unsigned int GetRows()
        {
            return this->cells.size();
        }

        /**
         * \brief Reads a cell from the CSV table.
         *
         * \param col is the column of the cell.
         * \param row is the row of the cell.
         *
         * \return The given cell content.
         */
        TCell ReadCell(unsigned int col, unsigned int row)
        {
            if ((col < this->cells[0].size()) and (row < this->cells.size()))
            {
                return this->cells[row][col];
            }
            else
            {
                throw std::range_error("This cell does not exist!");
            }
        }

        /**
         * \brief Writes to a cell from the CSV table.
         *
         * \param col is the columns of the cell.
         * \param row is the row of the cell.
         * \parma val is the data to write into the cell.
         *
         * \return None
         */
        void WriteCell(unsigned int col, unsigned int row, TCell val)
        {
            if ((col < this->cells[0].size()) and (row < this->cells.size()))
            {
                this->cells[row][col] = val;
            }
            else
            {
                throw std::range_error("This cell does not exist");
            }
        }

        /**
         * \brief Reads of a column from the CSV table.
         *
         * \param col is the column number to read
         *
         * \return The required column as a vector.
         */
        std::vector<TCell> ReadColumn(unsigned int col)
        {
            if (col < this->cells[0].size())
            {
                std::vector<TCell> column;

                for(unsigned int i=0; i<this->cells.size();i++)
                {
                    column.push_back(this->cells[i][col]);
                }

                return column;
            }
            else
            {
                throw std::range_error("This column does not exist!");
            }
        }

        /**
         * \brief Reads a row from the CSV table.
         *
         * \param row is the row number to read.
         *
         * \return The required row as a vector.
         */
        std::vector<TCell> ReadRow(unsigned int row)
        {
            if (row < this->cells.size())
            {
                return this->cells[row];
            }
            else
            {
                throw std::range_error("This row does not exist!");
            }
        }

        /**
         * \brief Writes data to a column from the CSV table.
         *
         * \parma pos is the column number to write.
         * \param col is the column data to write.
         *
         * \return None
         */
        void WriteColumn(unsigned int pos, std::vector<TCell> col)
        {
            if (pos < this->cells[0].size())
            {
                for(unsigned int i=0; i<this->cells.size(); i++)
                {
                    this->cells[i][pos] = col[i];
                }
            }
            else
            {
                throw std::range_error("This column does not exist!");
            }
        }

        /**
         * \brief Writes data to a row from the CSV table.
         *
         * \param pos is the row number to write.
         * \param row is the row data to write.
         *
         * \return None
         */
        void WriteRow(unsigned int pos, std::vector<TCell> row)
        {
            if (pos < this->cells.size())
            {
                this->cells[pos] = row;
            }
            else
            {
                throw std::range_error("This row does not exist!");
            }
        }

        /**
         * \brief Appends a column to the end of the CSV table.
         *
         * \param col is the column to append.
         *
         * \return None
         */
        void AppendColumn(std::vector<TCell> col)
        {
            for(unsigned int i=0; i<this->cells.size(); i++)
            {
                this->cells[i].push_back(col[i]);
            }
        }

        /**
         * \brief Appends a row to the end of the CSV table.
         *
         * \param row is the row to append.
         *
         * \return None
         */
        void AppendRow(std::vector<TCell> row)
        {
            this->cells.push_back(row);
        }

        /**
         * \brief cout overload.
         *
         * \param os is the ostream object.
         * \param csv is the CSV object.
         *
         * \return An ostream object.
         */
        friend std::ostream& operator<<(std::ostream& os, const CSV<TCell>& csv)
        {
            for(unsigned int i=0; i<csv.cells.size(); i++)
            {
                for(unsigned int j=0; j<csv.cells[i].size(); j++)
                {
                    os << csv.cells[i][j];

                    if (j < csv.cells[i].size()-1)    // Check if it is in the last column
                    {
                        os << "\t";
                    }
                }

                os << std::endl;
            }

            return os;
        }
};

#endif // CSV_H_

//! \} End of csv group
