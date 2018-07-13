#ifndef __DATA_TABLE_USER_H__
#define __DATA_TABLE_USER_H__

#include <string>

#define DataTableUserName "table_user"

#define DataTableUserCreateSql "CREATE TABLE IF NOT EXISTS table_user("\
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "\
                "phone TEXT NOT NULL, "\
                "name TEXT NOT NULL, "\
                "token TEXT NOT NULL, "\
                "activation INTEGER NOT NULL"\
                ")"
#define DataTableUserDrapSql "DROP TABLE table_user"

class DataUser
{
public:
    int id;
    std::string phone, name, token;
    bool actication;
};

class DataTableUser
{
public:

    static DataTableUser & instance( void );

    bool insert( const DataUser & p_userInfo );
    bool insert( const std::string & p_phone, const std::string & p_name, const std::string & p_token, const bool p_activation );

    std::vector< DataUser > list( void );
    DataUser find( const int p_id );

    bool update( const DataUser & p_userInfo );
    bool remove( const int p_id );

protected:
    bool init( void );
};

#endif //__DATA_TABLE_USER_H__