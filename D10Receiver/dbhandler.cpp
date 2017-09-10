#include "dbhandler.h"

dbhandler::dbhandler(const std::string& dbName, const std::string& tblName) : db(dbName.c_str())
{
	sDbFileName = dbName;
	sTableName = tblName;
	checkTable();	
}
dbhandler::~dbhandler()
{
	db.disconnect();
}


bool dbhandler::checkTable()
{
	std::string cmdStr("CREATE TABLE IF NOT EXISTS ");
	cmdStr += sTableName + "(id INTEGER PRIMARY KEY AUTOINCREMENT, OrderId CHAR(100) NOT NULL, Property CHAR(50), Value CHAR(50))";
	sqlite3pp::command cmd(db,cmdStr.c_str());
	return (cmd.execute() == SQLITE_OK);
}

void dbhandler::insertValues(const std::string& oid, const std::string& prop, const std::string& val)
{
	std::string cmdStr("INSERT INTO ");
	cmdStr += sTableName + " (orderID,Property,Value) VALUES (?, ?, ?)";
	sqlite3pp::command cmd(db, cmdStr.c_str());
	cmd.bind(1, oid, sqlite3pp::nocopy);
	cmd.bind(2, prop, sqlite3pp::nocopy);
	cmd.bind(3, val, sqlite3pp::nocopy);
	cmd.execute();
}