/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/data_access_interface/src/DbConnection.h $
 * @author:  R.Stagg
 * @version: $Revision: #3 $
 *
 * Last modification: $DateTime: 2012/02/10 14:06:39 $
 * Last modified by:  $Author: huirong.luo $
 * 
 * This class will return a connection string for a given data type/action
 */
#if !defined(DbConnection_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_)
#define DbConnection_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_

#include <map>
#include <string>
#include <vector>

#include "core/database/src/DbStatus.h"
#include "core/database/src/DataTypeEnums.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

namespace TA_Base_Core
{
    class DbConnection
    {

    public:
	    virtual ~DbConnection();

    public:
	    /**
	     * Returns the only instance of this class
	     */
	    static DbConnection& getInstance();

		static void removeInstance();
	    /**
	     * Fills in the conection string with the correct string for the given datatype
	     * and action. An exception will be thrown if a string can not be found.
	     * @param dataType    The type of data this connection is for.
	     * @param action    the action to be preformed on this datatype
	     * @param connectionString    The returned connection string goes here.
	     * 
	     */
	    void getConnectionString(const EDataTypes dataType, const EDataActions action, std::string& connectionString);

    private:

	    DbConnection();
	    DbConnection(const DbConnection& theDbConnection);

    private:
        typedef std::pair<EDataTypes, EDataActions> DataKey;
        typedef std::pair<std::string, std::string> DataConnection; // database name, connection string
        typedef std::map<DataKey, std::vector<DataConnection> > DatabaseMap; 
        DatabaseMap m_dbMap;
        static DbConnection* m_instance;
        static NonReEntrantThreadLockable m_instanceLock;

    };
} // namespace
#endif // !defined(DbConnection_29AA5110_185C_4781_89E1_C0663B2B94FD__INCLUDED_)