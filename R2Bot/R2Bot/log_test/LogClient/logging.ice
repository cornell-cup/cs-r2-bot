// File logging.ice

module Logging
{
	interface RemoteLog
	{
		void log(string name, string time, string msg);
	};
};