/**
 * @file model.h
 * @author Dunets Andrii (andrijdunec@gmail.com)
 * @brief Model part for MVC pattern
 * @date 2023-01-08
 */
#ifndef EMAIL_SERVICE_APP_MODEL_H
#define EMAIL_SERVICE_APP_MODEL_H

#include "db_backend.h"

class model
{
private:
    db_backend db;
    std::string exception;
public:
    model() = delete;

    model(const std::string &dbName, const std::string &userName, const std::string &userPass)
            : db(dbName, userName, userPass)
    {
    }

    ~model()
    {
        db.db_disconnect();
    }

    std::vector<std::string> model_getTableNames()
    {
        return db.db_selectTablesName();
    }

    std::vector<std::vector<std::string>>
    model_selectTable(const std::string &tableName, std::string const &columns, std::string const &where = "")
    {
        return db.db_selectTable(tableName, columns, where);
    }

    void model_updateTable(const std::string &tableName, const std::string &columns, const std::string &new_,
                           const std::string &where)
    {
        try
        {
            db.db_updateTable(tableName, columns, new_, where);
        }
        catch (std::exception const &e)
        {
            throw exception;
        }
    }

    void model_delete(const std::string &tableName, const std::string &where)
    {
        db.db_delete(tableName, where);
    }

    std::string formWhere(const std::string &option, const std::string &toCmp, const int &colType)
    {
        if (colType != INT_ID && colType != TEXT_ID && colType != DATE_ID)
        {
            throw std::string{"Invalid type of statement - "+option};
        }

        if (colType == TEXT_ID)
        {
            exception = "Invalid statement input for VARCHAR type";
            return option+" LIKE "+toCmp;
        }

        std::string delim;
        if (colType == DATE_ID)
        {
            exception = "Invalid statement input for TIMESTAMP type";
            delim = ";'";
        } else
        {
            exception = "Invalid statement input for INT type";
            delim = "-'";
        }

        char text[toCmp.size()+1];
        std::string delimEnd = "'";
        if (strstr(toCmp.c_str(), ".") != nullptr)
        {
            delimEnd = "";
            sprintf(text, "%s", toCmp.c_str());
        } else
        {
            sprintf(text, "%s", toCmp.c_str()+1);
        }

        char *firstNum = strtok(text, delim.c_str());
        char *secondNum = nullptr;
        if (firstNum != nullptr)
        {
            if (firstNum[0] == '<' || firstNum[0] == '>')
            {
                if (firstNum[1] == '=')
                {
                    return option+" "+firstNum[0]+"= "+delimEnd+(firstNum+2)+delimEnd;
                }
                return option+" "+firstNum[0]+" "+delimEnd+(firstNum+1)+delimEnd;
            }
            secondNum = strtok(nullptr, "'");
            if (secondNum != nullptr)
            {
                return option+" >= "+delimEnd+firstNum+delimEnd+" AND "+option+" <= "+delimEnd+secondNum+delimEnd;
            }
            return option+" = "+delimEnd+firstNum+delimEnd;
        }

        throw exception;
    }

    void model_add(const std::string &tableName, const std::string &data)
    {
        std::vector<std::string> vectorNames = model_getColumnNames(tableName, "*");
        std::string stringNames;

        for (auto &col: vectorNames)
        {
            stringNames += "\""+col+"\",";
        }
        stringNames.pop_back();

        db.db_add(tableName, stringNames, data);
    }

    unsigned long model_columnsNumber(const std::string &tableName)
    {
        return model_getColumnNames(tableName, "*").size();
    }

    int model_getColumnType(const std::string &tableName, const std::string &column)
    {
        char text[column.size()+1];
        sprintf(text, "%s", column.c_str());
        if (strstr(column.c_str(), ".") == nullptr)
        {
            return db.db_getColumnType(tableName, column);
        }
        char *word = strtok(text, ".");
        return db.db_getColumnType(word, strtok(nullptr, "\0"));
    }

    std::vector<std::string> model_getColumnNames(const std::string &tableName, const std::string &columns)
    {
        return db.db_getColumnNames(tableName, columns);
    }
};

#endif //EMAIL_SERVICE_APP_MODEL_H
