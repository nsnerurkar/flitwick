#ifndef _DBHANDLER_H_
#define _DBHANDLER_H_

#include "common.h"
#include "sqlite3pp.h"

class dbhandler
{
public:
	dbhandler(const std::string& dbName, const std::string& tblName);
	~dbhandler();

	bool checkTable();

	void insertValues(const std::string& oid, const std::string& prop, const std::string& val);

private:
	std::string sDbFileName;
	std::string sTableName;

	sqlite3pp::database db;
};




#endif //_DBHANDLER_H_