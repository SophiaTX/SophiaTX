/**
 * @file  SQLiteC++.cpp
 * @brief SQLiteC++ is a smart and simple C++ SQLite3 wrapper.
 *
 * Copyright (c) 2012 Sebastien Rombauts (sebastien dot rombauts at gmail dot com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include "SQLiteC++.h"
#include <iostream>

namespace SQLite
{

// Open the provided database UTF-8 filename.
Database::Database(const char* apFilename, const bool abReadOnly /*= true*/, const bool abCreate /*= false*/) :
    mpSQLite(NULL),
    mFilename (apFilename)
{
    int flags = abReadOnly?SQLITE_OPEN_READONLY:SQLITE_OPEN_READWRITE;
    if (abCreate)
    {
        flags |= SQLITE_OPEN_CREATE;
    }
    
    int ret = sqlite3_open_v2(apFilename, &mpSQLite, flags, NULL);
    if (SQLITE_OK != ret)
    {
        std::string strerr = sqlite3_errmsg(mpSQLite);
        sqlite3_close(mpSQLite);
        throw std::runtime_error(strerr);
    }
}

// Close the SQLite database connection.
Database::~Database(void)
{
    // check for undestroyed statements
    std::vector<Statement*>::iterator   iStatement;
    for (iStatement  = mStatementList.begin();
         iStatement != mStatementList.end();
         iStatement++)
    {
        // TODO (*iStatement)->Finalize(); ?
        std::cout << "Unregistered statement!\n";
    }

    int ret = sqlite3_close(mpSQLite);
    if (SQLITE_OK != ret)
    {
        std::cout << sqlite3_errmsg(mpSQLite);
    }
}

// Register a Statement object (a SQLite query)
void Database::registerStatement (Statement& aStatement)
{
    mStatementList.push_back (&aStatement);
}

// Unregister a Statement object
void Database::unregisterStatement (Statement& aStatement)
{
    std::vector<Statement*>::iterator   iStatement;
    iStatement = std::find (mStatementList.begin(), mStatementList.end(), &aStatement);
    if (mStatementList.end() != iStatement)
    {
        mStatementList.erase (iStatement);
    }
}


};  // namespace SQLite

namespace SQLite
{

// Compile and register the SQL query for the provided SQLite Database Connection
Statement::Statement(Database &aDatabase, const char* apQuery) :
    mDatabase(aDatabase),
    mQuery(apQuery),
    mbDone(false)
{
    int ret = sqlite3_prepare_v2(mDatabase.mpSQLite, mQuery.c_str(), mQuery.size(), &mpStmt, NULL);
    if (SQLITE_OK != ret)
    {
        throw std::runtime_error(sqlite3_errmsg(mDatabase.mpSQLite));
    }
    mDatabase.registerStatement(*this);
}

//Finalize and unregister the SQL query from the SQLite Database Connection.
Statement::~Statement(void)
{
    int ret = sqlite3_finalize(mpStmt);
    if (SQLITE_OK != ret)
    {
        std::cout << sqlite3_errmsg(mDatabase.mpSQLite);
    }
    mDatabase.unregisterStatement(*this);
}

// Reset the statement to make it ready for a new execution
void Statement::reset (void)
{
    mbDone = false;
    int ret = sqlite3_reset(mpStmt);
    if (SQLITE_OK != ret)
    {
        throw std::runtime_error(sqlite3_errmsg(mDatabase.mpSQLite));
    }
}

// Execute a step of the query to fetch one row of results
bool Statement::executeStep (void)
{
    bool bOk = false;

    if (false == mbDone)
    {
        int ret = sqlite3_step(mpStmt);
        if (SQLITE_ROW == ret)
        {
            bOk = true;
        }
        else if (SQLITE_DONE == ret)
        {
            bOk = true;
            mbDone = true;
        }
        else
        {
            throw std::runtime_error(sqlite3_errmsg(mDatabase.mpSQLite));
        }
    }

    return bOk;
}


};  // namespace SQLite
