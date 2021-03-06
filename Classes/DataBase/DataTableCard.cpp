//
//  DataTableCard.cpp
//  DadGuess-mobile
//
//  Created by 吴思 on 2018/10/30.
//

#include "DataTableCard.h"
#include <sstream>


DataCardInfo::DataCardInfo() : DataCardInfo( "", "", -1, "", "", false )
{
    
}

DataCardInfo::DataCardInfo( const std::string & p_id, const std::string & p_batchId, const int p_rfid, const std::string & p_coverFileUrl, const std::string & p_coverFileMd5, const bool p_isGift ) : id( p_id ), batchId( p_batchId ), rfid( p_rfid ), coverFileUrl( p_coverFileUrl ), coverFileMd5( p_coverFileMd5 ), isGift( p_isGift ), activation( false )
{
    
}

std::string DataCardInfo::toJson( void ) const
{
    std::stringstream t_sstr;
    
    t_sstr << "{ ";
    
    t_sstr << "\"id\": \"" << id << "\", ";
    t_sstr << "\"batchId\": \"" << batchId << "\", ";
    t_sstr << "\"rfid\": " << rfid << ", ";
    t_sstr << "\"coverFileUrl\": \"" << coverFileUrl << "\", ";
    t_sstr << "\"coverFileMd5\": \"" << coverFileMd5 << "\", ";
    t_sstr << "\"isGift\": " << ( isGift == 1 ? "true" : "false" ) << " ";
    t_sstr << "\"activation\": " << ( activation == 1 ? "true" : "false" ) << " ";
    
    t_sstr << " }";
    
    return t_sstr.str();
}


DataTableCard & DataTableCard::instance( void )
{
    static DataTableCard * sm_instance = nullptr;
    if( !sm_instance )
    {
        sm_instance = new DataTableCard();
        
        if( !sm_instance->init() )
        {
            printf( "DataTableCard init faild! \n" );
        }
    }
    
    return *sm_instance;
}

bool DataTableCard::insert( const DataCardInfo & p_cardInfo ) const
{
    std::stringstream t_ssql;
    
    t_ssql << "INSERT INTO " << DataTableCardName << "( id, batchId, rfid, coverFileUrl, coverFileMd5, isGift, activation ) VALUES( "
    << "\"" << p_cardInfo.id << "\", "
    << "\"" << p_cardInfo.batchId << "\", "
    << "\"" << p_cardInfo.rfid << "\", "
    << "\"" << p_cardInfo.coverFileUrl << "\", "
    << "\"" << p_cardInfo.coverFileMd5 << "\", "
    << ( p_cardInfo.isGift ? 1 : 0 ) << ", "
    << ( p_cardInfo.activation ? 1 : 0 ) << " "
    << ");";
    std::string t_sql = t_ssql.str();
    
    return DataBase::instance().exec( t_sql );
}

std::vector< DataCardInfo > DataTableCard::list( const std::string & p_batchId ) const
{
    std::vector< DataCardInfo > t_result;
    
    std::stringstream t_ssql;
    
    t_ssql << "SELECT * FROM " << DataTableCardName;
    if( !p_batchId.empty() )
    {
        t_ssql << " WHERE batchId = \"" << p_batchId << "\"";
    }
    
    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_ssql.str() );
    
    for( auto t_row : t_list )
    {
        t_result.push_back( dataRowToDataCardInfo( t_row ) );
    }
    DataBase::sm_mutex.unlock();
    
    return t_result;
}

DataCardInfo DataTableCard::find( const std::string & p_id ) const
{
    DataCardInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableCardName <<  " WHERE id= \"" << p_id << "\"";
    std::string t_sql = t_ssql.str();
    
    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_sql );
    
    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataCardInfo( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();
    
    return t_result;
}

DataCardInfo DataTableCard::find( const int p_rfid ) const
{
    DataCardInfo t_result;
    
    std::stringstream t_ssql;
    t_ssql << "SELECT * FROM " << DataTableCardName <<  " WHERE rfid= " << p_rfid ;
    std::string t_sql = t_ssql.str();
    
    DataBase::sm_mutex.lock();
    auto t_list = DataBase::instance().query( t_sql );
    
    if( t_list.size() == 1 )
    {
        t_result = dataRowToDataCardInfo( *t_list.begin() );
    }
    DataBase::sm_mutex.unlock();
    
    return t_result;
}

bool DataTableCard::update( const DataCardInfo & p_cardInfo ) const
{
    bool t_needUpdate = false;
    
    DataCardInfo t_oldInfo = find( p_cardInfo.id );
    if( t_oldInfo.id.length() <= 0 )
    {
        return false;
    }
    
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardName << " SET ";
    
    if( p_cardInfo.batchId != t_oldInfo.batchId )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "batchId = \"" << p_cardInfo.batchId << "\"";
    }
    
    if( p_cardInfo.rfid != t_oldInfo.rfid )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "rfid = " << p_cardInfo.rfid;
    }
    
    if( p_cardInfo.coverFileUrl != t_oldInfo.coverFileUrl )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "coverFileUrl = \"" << p_cardInfo.coverFileUrl << "\"";
    }
    
    if( p_cardInfo.coverFileMd5 != t_oldInfo.coverFileMd5 )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "coverFileMd5 = \"" << p_cardInfo.coverFileMd5 << "\"";
    }

    if( p_cardInfo.isGift != t_oldInfo.isGift )
    {
        if( t_needUpdate )
        {
            t_ssql << ", ";
        }else{
            t_needUpdate = true;
        }
        
        t_ssql << "isGift = " << ( p_cardInfo.isGift ? "1" : "0" );
    }
    
    if( !t_needUpdate )
    {
        return true;
    }
    
    t_ssql << " WHERE id=\"" << p_cardInfo.id << "\"";
    
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableCard::remove( const std::string & p_id ) const
{
    std::stringstream t_ssql;
    t_ssql << "DELETE FROM " << DataTableCardName << " WHERE id=\"" << p_id << "\"";
    std::string t_sql = t_ssql.str();
    
    return DataBase::instance().exec( t_sql );
}

bool DataTableCard::activation( const DataCardInfo & p_cardInfo ) const
{
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardName << " SET activation = 1 WHERE id=\"" << p_cardInfo.id << "\"";
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableCard::activation( const std::string & p_batchId )
{
    std::stringstream t_ssql;
    t_ssql << "UPDATE " << DataTableCardName << " SET activation = 1 WHERE batchId=\"" << p_batchId << "\"";
    return DataBase::instance().exec( t_ssql.str() );
}

bool DataTableCard::drop( void ) const
{
    return DataBase::instance().exec(  DataTableCardDrapSql );
}

bool DataTableCard::init( void ) const
{
    if( !DataBase::instance().exec( DataTableCardCreateSql ) )
    {
        return false;
    }
    return true;
}

DataCardInfo DataTableCard::dataRowToDataCardInfo( std::map<std::string, std::string> & p_dataRow ) const
{
    auto t_result = DataCardInfo( p_dataRow["id"], p_dataRow["batchId"], atoi( p_dataRow["rfid"].c_str() ), p_dataRow["coverFileUrl"], p_dataRow["coverFileMd5"], atoi( p_dataRow["isGift"].c_str() ) == 1 );
    t_result.activation = atoi( p_dataRow["activation"].c_str() ) == 1;
    return t_result;
}
