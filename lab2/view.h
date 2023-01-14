/**
 * @file view.h
 * @author Dunets Andrii (andrijdunec@gmail.com)
 * @brief View part for MVC pattern
 * @date 2023-01-08
 */
#ifndef EMAIL_SERVICE_APP_VIEW_H
#define EMAIL_SERVICE_APP_VIEW_H

#include "iostream"
#include <iomanip>
#include <vector>

class view
{
private:

public:
    view() = default;
    ~view() = default;

    static void view_tableData(const std::vector<std::vector<std::string>> &res, const std::string &tableName, const std::vector<std::string> &tableColumns)
    {
        system("cls");
        using namespace std;
        auto colNumber = tableColumns.size();

        cout << setfill('-') << endl;
        cout << setw(20*colNumber) << "" << endl;
        cout << "Table " << tableName << " data:" << endl;
        cout << setw(20*colNumber) << "" << endl;

        cout << left << setfill(' ');
        for (auto &col: tableColumns)
        {
            cout << setw(19) << col << "|";
        }
        cout << setfill('-') << endl;
        cout << setw(20*colNumber) << "" << endl;

        cout << setfill(' ');
        for (auto &row: res)
        {
            for (auto &field: row)
            {
                cout << setw(19) << field.c_str() << "|";
            }
            cout << endl;
        }

        cout << setfill('-') << setw(20*colNumber) << "" <<endl;
    }

    static void view_showMenu() {
        using namespace std;
        system("cls");

        cout << setfill('-');
        cout << setw(20*2) << "" << endl;
        cout << "WELCOME TO THE EMAIL SERVICE APP" << endl;
        cout << setw(20*2) << "" << endl << endl;

        cout << "Choose your option:" << endl;
        cout << "\t1. Select\n\t2. Update\n\t3. Add\n\t4. Delete\n\t5. Exit" << endl;
        cout << setw(20*2) << "" << endl << endl;
    }

    void view_SelectMenu() {
        using namespace std;
        system("cls");

        cout << "SELECT SUBMENU";
        cout << setfill('-') <<setw(20*2) << "" << endl << endl;

        cout << "\tEnter table name or names: " << flush;
        view_Clean();
    }

    void view_UpdateMenu() {
        using namespace std;
        system("cls");

        cout << "UPDATE SUBMENU";
        cout << setfill('-') <<setw(20*2) << "" << endl << endl;

        cout << "\tEnter table name: " << flush;
        view_Clean();
    }

    void view_AddMenu() {
        using namespace std;
        system("cls");

        cout << "Add SUBMENU";
        cout << setfill('-') <<setw(20*2) << "" << endl << endl;

        cout << "\tEnter table name: " << flush;
        view_Clean();
    }

    void view_DeleteMenu() {
        using namespace std;
        system("cls");

        cout << "DELETE SUBMENU";
        cout << setfill('-') <<setw(20*2) << "" << endl << endl;

        cout << "\tEnter table name: " << flush;
        view_Clean();
    }

    void view_Column() {
        std::cout << "\tEnter column or columns: " << std::flush;
    }

    void view_WhereSelect() {
        std::cout << "\tDo you want to enter options for filter?" << std::endl;
        std::cout << "\t1. Yes\n\t2. No" << std::endl;
    }

    void view_WhereSelectNext() {
        std::cout << "\tEnter next option?" << std::endl;
        std::cout << "\t1. Yes\n\t2. No" << std::endl;
    }

    void view_Clean() {
        std::cin.ignore(INT_MAX, '\n');
    }

    void view_Option() {
        std::cout << "\tEnter option to compare: " << std::flush;
    }

    void view_toCmp() {
        std::cout << "\tEnter compare data: " << std::flush;
    }

    void view_newData() {
        std::cout << "\tEnter new data: " << std::flush;
    }
};

#endif //EMAIL_SERVICE_APP_VIEW_H
