/**
 * @file controller.h
 * @author Dunets Andrii (andrijdunec@gmail.com)
 * @brief Controller part for MVC pattern
 * @date 2023-01-08
 */
#ifndef EMAIL_SERVICE_APP_CONTROLLER_H
#define EMAIL_SERVICE_APP_CONTROLLER_H

#include "view.h"
#include "model.h"
#include <memory>
#include <regex>

class controller
{
private:
    std::unique_ptr<model> modObj;
    std::unique_ptr<view> viewObj;

    static std::string fixStr(const std::string &str, const char &quotes = '"')
    {
        std::regex line(R"(([a-zA-Z0-9_%:;>!<=-]+( *\.? *[a-zA-Z0-9_%:;>!<=-]+)*)|(^\*$))", std::regex::icase);
        std::sregex_iterator wordMatch(str.begin(), str.end(), line);
        std::sregex_iterator endMatch{};

        std::string res;
        std::smatch tmpMatch;
        while (wordMatch != endMatch)
        {
            tmpMatch = *wordMatch;
            std::string tmpWord = tmpMatch.str();
            std::smatch smallW;
            if (std::regex_match(tmpWord, smallW,
                                 std::regex(R"((?:[a-zA-Z0-9_%]+(?: *\. *[a-zA-Z0-9_%]+)))", std::regex::icase)))
            {
                std::string tmp = smallW.str();
                std::regex rex(R"((?:\w+(?: *\w+)+))");
                std::sregex_iterator ww(tmp.begin(), tmp.end(), rex);
                while (ww != endMatch)
                {
                    tmpMatch = *ww;
                    res += "\""+tmpMatch.str()+"\".";

                    *ww++;
                }
                if (res.empty())
                {
                    throw std::string{"Invalid string input"};
                }
                res.pop_back();
                res.push_back(',');
            } else
            {
                if (tmpMatch.str() == "*")
                {
                    return tmpMatch.str();
                } else
                {
                    res += quotes+tmpMatch.str()+quotes+',';
                }
            }
            *wordMatch++;
        }

        if (res.empty())
        {
            throw std::string{"Invalid string input"};
        }
        res.pop_back();
        return res;
    }

public:
    controller() = delete;

    controller(std::unique_ptr<model> mod_, std::unique_ptr<view> view_)
            : modObj(std::move(mod_)), viewObj(std::move(view_))
    {
    }

    void contr_selectAllTables()
    {
        std::vector<std::string> tableNames = modObj->model_getTableNames();
        for (auto &row: tableNames)
        {
            contr_selectTables(fixStr(row));
        }
    }

    void
    contr_selectTables(const std::string &tableName, const std::string &options = "*", const std::string &where = "")
    {
        viewObj->view_tableData(modObj->model_selectTable(fixStr(tableName), fixStr(options), where),
                                fixStr(tableName), modObj->model_getColumnNames(fixStr(tableName), fixStr(options)));
    }

    void contr_updateTables(const std::string &tableName, const std::string &columns, const std::string &new_,
                            const std::string &where)
    {

        modObj->model_updateTable(fixStr(tableName), fixStr(columns), fixStr(new_, '\''), where);
        contr_selectTables(fixStr(tableName));
    }

    void contr_delete(const std::string &tableName, const std::string &where)
    {
        modObj->model_delete(fixStr(tableName), where);
        contr_selectTables(fixStr(tableName));
    }

    void contr_add(const std::string &tableName, const std::string &data)
    {
        modObj->model_add(fixStr(tableName), fixStr(data, '\''));
        contr_selectTables(fixStr(tableName));
    }

    void contr_startMenu()
    {
        while (true)
        {
            try
            {
//                contr_selectAllTables();
//        con.contr_selectTables("User");
//        con.contr_selectTables("Folder", "Folder.Name, folder_id, Colour");
//        con.contr_updateTables("Send", "Send_time", "2024-02-10", "Send_time", "2023-01-01;2024-01-02");
//con.contr_delete("User", "Name", "Olha%");
//        con.contr_delete("Send", "send_id", "5");
//        con.contr_add("User", "aaaa, bbbb");
//con.contr_delete("User",
//                contr_selectTables("Send, User", "email_id, Name, Send_time", formWhereGroup("Send, User","Send.letter_id", "<=3", "User.email_id", "Send.sender_email"));

                static std::string tableName;
                static std::string tableColumn;
                static std::string where;
                static std::string data;

                viewObj->view_showMenu();
                int option = 0;
                std::cin >> option;

                switch (option)
                {
                    case 1:     /** @brief Select */
                        viewObj->view_SelectMenu();
                        std::getline(std::cin, tableName, '\n');
                        viewObj->view_Column();
                        std::getline(std::cin, tableColumn, '\n');

                        where = contr_WhereSelect(tableName, 0);
                        contr_selectTables(tableName, tableColumn, where);
                        getchar();
                        break;
                    case 2:     /** @brief Update */
                        viewObj->view_UpdateMenu();
                        std::getline(std::cin, tableName, '\n');
                        viewObj->view_Column();
                        std::getline(std::cin, tableColumn, '\n');
                        viewObj->view_newData();
                        std::getline(std::cin, data, '\n');

                        where = contr_WhereSelect(tableName);
                        contr_updateTables(tableName, tableColumn, data, where);
                        getchar();
                        break;
                    case 3:     /** @brief Add */
                        viewObj->view_AddMenu();
                        std::getline(std::cin, tableName, '\n');
                        viewObj->view_newData();
                        std::getline(std::cin, data, '\n');

                        contr_add(tableName, data);
                        getchar();
                        break;
                    case 4:     /** @brief Delete */
                        viewObj->view_DeleteMenu();
                        std::getline(std::cin, tableName, '\n');

                        where = contr_WhereSelect(tableName);
                        contr_delete(tableName, where);
                        getchar();
                        break;
                    case 5:     /** @brief Exit */
                        return;
                    default:
                        continue;

                }
                system("cls");
            } catch (std::string const &e)
            {
                system("cls");
                std::cerr << e << std::endl;
                viewObj->view_Clean();
            }
            catch (std::exception const &e)
            {
                system("cls");
                std::cerr << e.what() << std::endl;
                viewObj->view_Clean();
            }
        }
    }

    std::string contr_WhereSelect(const std::string &tableName, int isMust = 1)
    {
        std::string where;
        std::string option;
        std::string toCmp;
        std::string input;

        int isFirst = 1;
        char opt = 0;

        if (!isMust)
        {
            viewObj->view_WhereSelect();
            std::getline(std::cin, input, '\n');
            opt = fixStr(input, ' ')[1];
        }
        while (true)
        {
            if (!isFirst)
            {
                viewObj->view_WhereSelectNext();
                std::getline(std::cin, input, '\n');
                opt = fixStr(input, ' ')[1];
            }

            if (isMust | (opt == '1'))
            {
                viewObj->view_Option();
                std::getline(std::cin, option, '\n');
                viewObj->view_toCmp();
                std::getline(std::cin, toCmp, '\n');

                if (isFirst)
                {
                    isFirst = 0;
                    isMust = 0;
                    where = "WHERE ";
                } else
                {
                    where += " AND ";
                }
                where += modObj->formWhere(fixStr(option), fixStr(toCmp, '\''),
                                           modObj->model_getColumnType(fixStr(tableName), fixStr(option)));
            } else if (opt == '2')
            {
                return where;
            } else
            {
                return "";
            }
        }
    }
};

#endif //EMAIL_SERVICE_APP_CONTROLLER_H
