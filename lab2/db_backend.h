/**
 * @file db_backend.h
 * @author Dunets Andrii (andrijdunec@gmail.com)
 * @brief Persistence PostgreSQL layer for MVC pattern
 * @date 2023-01-08
 */
#ifndef EMAIL_SERVICE_APP_DB_BACKEND_H
#define EMAIL_SERVICE_APP_DB_BACKEND_H

#include "pqxx/pqxx"
#include "pqxx/except"

#define INT_ID (23)
#define TEXT_ID (1043)
#define DATE_ID (1114)

using db_result = pqxx::result;

class db_backend
{
private:
    pqxx::connection con;
public:
    db_backend() = delete;

    db_backend(const std::string &dbName, const std::string &userName, const std::string &userPass) try
            : con(std::string{
            "dbname = "+dbName+" user = "+userName+" password = "+userPass+" hostaddr = 127.0.0.1 port = 5432"})
    {
        std::cout << "Opened database successfully: " << con.dbname() << std::endl;
    }
    catch (std::exception const &e)
    {
        throw std::string{"Can't open database "+dbName};
    }

    void db_disconnect()
    {
        con.disconnect();
    }

    std::vector<std::string> db_selectTablesName()
    {
        std::vector<std::string> names;
        pqxx::work query(con);
        db_result res = query.exec("SELECT tablename FROM pg_tables\n"
                                   "WHERE schemaname = 'public';");

        names.reserve(res.size());
        for (auto row: res)
        {
            names.emplace_back(row.front().c_str());
        }
        return names;
    }

    std::vector<std::vector<std::string>> db_selectTable(const std::string &tableName,
                                                         const std::string &columns, const std::string &where = "")
    {
        pqxx::work query(con);
        try
        {
            std::string text{"SELECT "+columns+" FROM "+tableName+" "+where};
            db_result res = query.exec(text);

            std::vector<std::vector<std::string>> data;
            data.resize(res.size());

            for (auto i = 0; i < data.size(); ++i)
            {
                data[i].resize(res[i].size());
                for (auto j = 0; j < data[i].size(); ++j)
                {
                    data[i][j] = res[i][j].c_str();
                }
            }
            return data;
        } catch (pqxx::undefined_table const &e)
        {
            throw std::string{"Invalid table names - "+tableName};
        } catch (pqxx::undefined_column const &e)
        {
            throw std::string{"Invalid columns - "+columns+" for tables - "+tableName};
        }
    }

    int db_getColumnType(const std::string &tableName, const std::string &column)
    {
        pqxx::work query(con);
        try
        {
            return query.exec("SELECT "+column+" FROM "+tableName).column_type(column);
        } catch (pqxx::undefined_table const &e)
        {
            throw std::string{"Invalid table name - "+tableName};
        } catch (pqxx::undefined_column const &e)
        {
            throw std::string{"Invalid column - "+column+" for table - "+tableName};
        }
    }

    void db_updateTable(const std::string &tableName, const std::string &columns, const std::string &new_,
                        const std::string &where)
    {
        pqxx::work query(con);

        try
        {
            query.exec("UPDATE "+tableName+" SET "+columns+" = "+new_+" "+where);
            query.commit();
        } catch (pqxx::restrict_violation const &e)
        {
            throw std::string{"Restrict violation for column - "+columns+" in table - "+tableName};
        } catch (pqxx::unique_violation const &e)
        {
            throw std::string{
                    "Unique violation for column - "+columns+" in table - "+tableName+"\nValue is not unique: "+new_};
        } catch (pqxx::foreign_key_violation const &e)
        {
            throw std::string{
                    "Foreign key violation for column - "+columns+" in table - "+tableName+"\nValue already exist: "+
                    new_};
        }
    }

    void db_delete(const std::string &tableName, const std::string &where)
    {
        pqxx::work query(con);

        try
        {
            query.exec("DELETE FROM "+tableName+" "+where+";");
            query.commit();
        } catch (pqxx::foreign_key_violation const &e)
        {
            throw std::string{"Foreign key violation in table - "+tableName+"\nRaw cannot be deleted"};
        }
    }

    void db_add(const std::string &tableName, const std::string &columns, const std::string &data)
    {
        pqxx::work query(con);

        try
        {
            query.exec("INSERT INTO "+tableName+" ("+columns+") VALUES ("+data+");");
            query.commit();
        } catch (pqxx::sql_error const &e)
        {
            throw std::string{"Invalid amount or data of parameters\nQuery: "+e.query()};
        }
    }

    std::vector<std::string> db_getColumnNames(const std::string &tableName, const std::string &columns)
    {
        try
        {
            pqxx::work query(con);
            db_result res = query.exec("SELECT "+columns+" FROM "+tableName);


            std::vector<std::string> result;
            result.reserve(res.columns());

            for (auto i = 0; i < result.capacity(); ++i)
            {
                result.emplace_back(res.column_name(i));
            }
            return result;
        } catch (pqxx::undefined_table const &e)
        {
            throw std::string{"Invalid table name - "+tableName};
        } catch (pqxx::undefined_column const &e)
        {
            throw std::string{"Invalid columns - "+columns+" for table - "+tableName};
        }
    }
};

#endif //EMAIL_SERVICE_APP_DB_BACKEND_H
