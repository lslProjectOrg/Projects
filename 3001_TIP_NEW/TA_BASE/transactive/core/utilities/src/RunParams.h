#ifndef RUNPARAMS_H_INCLUDED
#define RUNPARAMS_H_INCLUDED

/**
  * RunParams.h
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/RunParams.h $
  * Author:  B. Fountain
  * @version: $Revision: #6 $
  *
  * Last modification: $DateTime: 2013/08/26 17:21:53 $
  * Last modified by:  $Author: hongzhi.zhang $
  * 
  * A singleton class that stores all the runtime parameters. Runtime parameters
  * can come from several different sources - command line, process manager,
  * entity configuration tables.
  *
  * Because the class is a singleton, runtime parameters can be accessed from
  * anywhere in the code instead of passing them around as parameters
  *
  */

#if defined( WIN32 )
#pragma warning( push )
#pragma warning( disable : 4290 )
#pragma warning( disable : 4786 )
#endif                                   // defined( WIN32 )

#include <map>
#include <string>
#include <vector>
#include <list>
#include "core/synchronisation/src/ReEntrantThreadLockable.h"


// Standard runtime parameter names

#define RPARAM_NAMESERV				"NameServIor"
#define RPARAM_DBCONNECTION			"DbConnection"
#define RPARAM_DBCONNECTIONFILE		"DbConnectionFile"
#define RPARAM_NOTIFYHOSTS			"NotifyHosts"
#define RPARAM_NOTIFYPORT			"NotifyPort"
#define RPARAM_NOTIFYSERVICEPORT	"NotifyServicePort"
#define RPARAM_RESOLVE_TIMEOUT_MS	"ResolveTimeoutMs"
#define RPARAM_SUCCESSBATCHINGINTERVAL  "MsgSuccessBatchingInterval"
#define RPARAM_FAILEDBATCHINGINTERVAL   "MsgFailedBatchingInterval"
#define RPARAM_BATCHSIZE			"MsgBatchSize"

#define RPARAM_DEBUGLEVEL			"DebugLevel"
#define RPARAM_DEBUGFILE			"DebugFile"
#define	RPARAM_DEBUGFILEMAXSIZE		"DebugFileMaxSize"
#define	RPARAM_DEBUGMAXFILES		"DebugMaxFiles"
#define RPARAM_DEBUGCACHEMAXSIZE	"DebugCacheMaxSize"
#define RPARAM_DEBUGPIDFILENAMES	"DebugPidFilenames"
#define RPARAM_DEBUGLEVELOFF		"DebugLevelOff"
#define RPARAM_DEBUGLEVELFILE		"DebugLevelFile"
#define RPARAM_DEBUGLOGSTACKTRACE   "DebugLogStackTrace"

#define RPARAM_MGDPROCID			"ManagedProcessId"
#define RPARAM_SESSIONID			"SessionId"
#define RPARAM_PROGNAME				"ProgName"
#define RPARAM_ENTITYNAME			"EntityName"
#define RPARAM_APPTYPE				"AppType"
#define RPARAM_MGRPORT				"MgrPort"
#define RPARAM_GETFOCUS				"GetFocus"
#define RPARAM_USERPREFERENCES		"UserPreferences"
#define RPARAM_VERSION				"Version"
#define RPARAM_CONTROLENTITY		"ControlEntity"

#define RPARAM_CORBATIMEOUT			"CorbaTimeout"

#define RPARAM_OPERATIONMODE		"OperationMode"
#define RPARAM_CONTROL				"Control"
#define RPARAM_MONITOR				"Monitor"
#define RPARAM_GROUPSOFFLINE		"GroupsOffline"

#define RPARAM_HELPFILE				"HelpFile"

#define RPARAM_EXECPLANID			"ExecPlanId"
#define RPARAM_VIEWPLANID			"ViewPlanId"
#define RPARAM_ASSOC_ALARMID		"AssocAlarmId"


#define RPARAM_APPNAME				"AppName"

#define RPARAM_LOCATIONKEY			"LocationKey"

#define RPARAM_NAMINGTTL			"NamingTtl"
#define RPARAM_SCREENNAME			"ScreenName"

#define RPARAM_SYSTEMCONTROLLERNAME "SystemControllerName"

// database related runparam stuff
#define RPARAM_DBPREFIX             "DataBase_"
#define RPARAM_DBONLINE             "DataBaseGood"
#define RPARAM_DBOFFLINE            "DataBaseBad"
#define RPARAM_DBUNKNOWN            "DataBaseUnknown"

// Generic Service
#define RPARAM_SERVICEDEBUG			"ServiceDebug"

// SCADA
#define RPARAM_SCADA_OBSWORKERS		"ScadaObserverWorkers"

#define RPRAM_KRTCDATEFORMAT        "KrtcDateFormat" // TD12474
#define RPARAM_KRTCALARMSORTING		"KrtcAlarmSorting" 
#define RPARAM_MONITOREDDATABASE    "MonitoredDatabase" // Database for monitored agents
#define RPARAM_CENTRALDATABASE		"CentralDatabase"	// Central Database

// Generic GUI
#define RPARAM_RIGHTCHECKING		"IgnoreCheckRight"

// The buffer size for the import command of archive manager
#define RPARAM_IMPBUFFERSIZE		"ImpBufferSize"

// yanrong++: CL-21230
// The hostname used by naming service
#define RPARAM_NSHOSTNAME				"NamingHostname"
// ++yanrong

#define RPARAM_ENABLESUBSCRIPTIONCHANGE     "EnableSubscriptionChange"

#define RPARAM_CORBA_NAMING_PORT	"CorbaNamingPort"
#define RPARAM_DLL_HOST_PORT        "DllHostPort"


// add by hongzhi for TDS
//#ifdef TDS_DEVELOP
#define RPARAM_TDSCE					"TdsCe"
//#endif

namespace TA_Base_Core
{
	class RunParamUser;					// see below

	class RunParams
	{
	public:

        /**
		  * getInstance
		  * 
		  * Returns a single static instance of RunParams
		  *
		  * @return The one and only RunParams instance
		  */
		static RunParams& getInstance();

		static void removeInstance();

        /**
		  * ~RunParams
		  * 
		  * Destructor
		  */
		virtual ~RunParams() {}

		/**
		  * set
		  * 
		  * Add a parameter
		  *
		  * @param name Name of parameter
          * @param value Value of parameter
          *
          * Pre: name and value are not NULL
		  */
		void set(const char* name, const char* value);

        /**
		  * get
		  * 
		  * Retrieve a parameter value
		  *
          * @return Value of parameter
          *
		  * @param name Name of parameter
          *
          * Pre: name is not NULL
          */
		std::string get(const char* name);

		/**
		  * isSet
		  * 
		  * Determine whether a parameter with the given name has been set
		  *
          * @return True (parameter set), False (parameter not set)
          *
		  * @param name Name of parameter
          *
          * Pre: name is not NULL
          */
		bool isSet(const char* name);

		struct ParamNameValue
		{
			std::string name;
			std::string value;
		};
		typedef std::vector<ParamNameValue>         ParamVector;

        /**
		  * getAll
		  * 
		  * Retrieve all the parameters
		  *
          * @param params Vector of Name/Value parameter pairs
          *
          * Pre: name is not NULL
          */
		void getAll(ParamVector& params);

        /**
          * COMMAND LINE MAPPING
          *
		  * Command line parameters are expected to conform to the gnu format -
		  * parameter names are prefixed with "--" and words within a parameter name
		  * are separated with a single hyphen. Names and values are separated with
		  * an "=". Values are optional - if there is no "=" then the parameter is a
		  * flag.
		  *
		  * eg "foo --entity-name=BAR --verbose"
		  *
		  * If the first argument does not start with "--" then it is assumed to be
		  * the program name.
		  *
		  * When we parse the command line we convert the parameter names to
		  * CamelHump notation - the leading "--" is discarded, the first letter is
		  * upshifted, subsequent hyphens are discarded and the succeeding character
		  * upshifted. The conversion only applies to the name, the value is left
		  * unmodified. The program name is handled as the value of a parameter with
		  * name RPARAM_PROGNAME. In the above example we would set parameter
		  * RPARAM_PROGNAME to "foo", parameter "EntityName" to "BAR" and
		  * parameter "Verbose" to "".

		  * The following methods parse the given command line (several command line
		  * representations) and set corresponding parameters. Return false if the
		  * command line does not conform to the expected format
          *
          */

        /**
		  * parseCmdLine
		  * 
		  * Parses the command line parameters.
          * See COMMAND LINE MAPPING comments above.
		  *
          * @return Success of parse (true: success)
          *
          * @param argc Number of parameters
          * @param argv Vector of parameters
          */
		bool parseCmdLine(int argc, char* argv[]);

        /**
		  * parseCmdLine
		  * 
		  * Parses the command line parameters.
          * See COMMAND LINE MAPPING comments above.
		  *
          * @return Success of parse (true: success)
          *
          * @param cmdline Number of parameters
          * @param argv Vector of parameters
          *
          * Pre: cmdline is not NULL
          */
		bool parseCmdLine(const char* cmdline);

        /**
		  * parseCmdLine
		  * 
		  * Parses the command line parameters.
          * See COMMAND LINE MAPPING comments above.
		  *
          * @return Success of parse (true: success)
          *
          * @param args Vector of strings consisting of 
          *             parameters and parameter values
          *
          * Pre: cmdline is not NULL
          */
		bool parseCmdLine(const std::vector<std::string>& args);

        /**
		  * parseCmdLineAndReturnIt
		  * 
		  * Parses the command line parameters. It will return these command line parameter
          * in the map passed instead of setting up the RunParams. This is a helper method that
          * can be used to convert command lines into a map.
		  *
          * @return Success of parse (true: success)
          *
          * @param args string - A command line
          * @param map<string,string> - Out parameter. This will be populated with the
          *                             parameters read from cmdLine. This map holds values
          *                             and is keyed by parameter name.
          *
          * Pre: cmdline is not NULL
          */
        bool parseCmdLineAndReturnIt(const std::string& cmdline,
                                     std::map<std::string,std::string>& parameters);
        /**
          * checkUsage
          *
		  * This method checks to see whether the parameters specified in the given
		  * usage string have been set. It ignores the leading program name and uses
		  * the values only to distinguish between parameters that take values and
		  * flag parameters. Returns true if all specified parameters have been set
		  * correctly. For instance, a usage string "foo --entity-name=BAR --verbose"
		  * would return true if parameter "EntityName" has been set and parameter
		  * "Verbose" has been set to a blank string. The error message will be set
		  * to blank if the method succeeds
		  *
		  * You can also make parameters optional by surrounding them with square
		  * brackets. eg "foo --entity-name=BAR [--verbose]". In this case failure
		  * to specify a parameter is not an error, but providing a non-blank value
		  * to the flag parameter would be
          *
          * @return True (correct usage string), False (incorrect usage string)
          *
          * @param usage The usage string
          * @param error The reason for the error
          *
          * Pre: The usage cannot be NULL
          */
		bool checkUsage(const char* usage, std::string& error);

        /**
          * registerRunParamUser
          *
		  * Register this given callback object against the parameter with the
		  * given name.
          *
          * @param user Pointer to RunParamUser object
          * @param name Name of the parameter
          *
          * Pre: The RunParamUser cannot be NULL
          *      The name cannot be NULL
          */
		void registerRunParamUser(RunParamUser* user, const char* name);

        /**
          * deregisterRunParamUser
          *
		  * Cleanup any callback registrations associated with this object
		  * Note: this used to be called Deregister().
          *
          * @param user Pointer to RunParamUser object
          *
          * Pre: The RunParamUser cannot be NULL
          */
		void deregisterRunParamUser(RunParamUser* user);

        /**
          * forwardUpdatesTo
          *
          * Forwards RunParam updates to different instances of RunParams. This may
          * sound strange as RunParams is a singleton, however in an environment
          * that uses DLLs each DLL will have its own instance of the RunParams class.
          * In order to allow these different instances to correctly receive updates
          * the 'main' RunParams instance needs the ability to forward them on.
          *
          * @param child The child to forward set requests to.
          */
        void forwardUpdatesTo( RunParams* child );

	private:

        /**
		  * getInstance
		  * 
		  * Returns a single static instance of DebugUtil
		  *
		  * @return The one and only DebugUtil instance
		  */
		RunParams() {};

        // Fix for TD15371, refer to TD2689
        static TA_Base_Core::ReEntrantThreadLockable& getLock();

        static RunParams* m_instance;

		// We store the parameter details, including the interested users, in a
		// map indexed by parameter name

		typedef std::list<RunParamUser*> UserList;
		struct ParamDetails
		{
			std::string     value;
			UserList        users;
		};
		typedef std::map<std::string, ParamDetails> ParamDetailsMap;

		ParamDetailsMap m_Params;

        typedef std::list<RunParams*> ChildList;
        ChildList m_childRunParams;

		/**
		 * protect the param details from multi-threaded access
		 *
		 * NOTE: This is re-entrant due to the unusual behaviour of runparams
		 *			(ie: Some apps will receive a onRunParamChange() and then
		 *			call getParam() within the same thread of execution!
		 */
		TA_Base_Core::ReEntrantThreadLockable m_paramDetailsLock;

		/**
		  * notify
		  * 
		  * Notify any registered users of a change to a named parameter
		  *
		  * @param name Name of the parameter
          *
          * Pre: The name cannot be NULL
		  */
		void notify(const char* name);
		
        /**
		  * convertCmdLine
		  * 
		  * Split the given cmdline string into a vector of whitespace-separated
		  * arguments
		  * TODO: handle escaped whitespace (quotes, backslash)
		  *
          * @param cmdline The command line
		  * @param args Vector of arguments
          *
          * Pre: The command line cannot be NULL
		  */
		void convertCmdLine(const char* cmdline, std::vector<std::string>& args);

		/**
		  * convertCmdLine
		  * 
		  * Convert the standard argc/argv cmdline to a vector of arguments
		  *
          * @param argc Number of arguments
		  * @param args Vector of arguments
          */
		void convertCmdLine(int argc, char* argv[], std::vector<std::string>& args);

		/**
		  * extractArg
		  * 
		  * Extract a single argument from the given string, split it into name
		  * and value components, and CamelHump the parameter name (as described
		  * above). Everything after the "=" will be assigned to the value, including
		  * whitespace. Returns true if the argument corresponds to the standard
		  * format. The input string will be advanced to the next argument (points
		  * to the end of the string if no more arguments)
		  *
          * @return True (argument corresponds to standard format), False (it doesn't)
          *
          * @param str   Single argument
		  * @param name  Parameter name
          * @param value Parameter value
          */
		bool extractArg(const char* str, std::string& name, std::string& value);

		/**
		  * extractArg
		  * 
		  * Extract an argument as above, except that we allow the argument to be
		  * surrounded by square brackets indicating that it is optional. This is
		  * called by CheckUsage(). eg "[--name=value]" "[--name]"
		  *
          * @return True (argument corresponds to standard format), False (it doesn't)
          *
          * @param str      Single argument
		  * @param name     Parameter name
          * @param value    Parameter value
          * @param optional Flag indication if parameter is optional
          */
		bool extractArg(const char* str, std::string& name, std::string& value, bool& optional);
	};


	// You must implement this interface if you want to register to be notified
	// when a runparam changes

	class RunParamUser
	{
	public:
		virtual void onRunParamChange(const std::string& name, const std::string& value) = 0;

		virtual ~RunParamUser()
		{
			RunParams::getInstance().deregisterRunParamUser(this);
		}
	};


    // Returns the unsigned integer value of the RunParam with the given name, 
    // or defaultVal if none exists.
    // TODO LPP: If we had a set of templatized string conversion functions,
    // we could write this as a template function usable for all types.
    unsigned int getRunParamValue(const std::string& name, unsigned int defaultVal);

}       // Closes namespace TA_Base_Core

#if defined( WIN32 )
#pragma warning( pop )
#endif  // defined( WIN32 ) 

#endif  // RUNPARAMS_H_INCLUDED
