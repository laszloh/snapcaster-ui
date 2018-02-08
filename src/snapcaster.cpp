/***
    This file was originally part of snapcast
    Modified to fit the snapcaster-ui project
    
    Copyright (C) 2014-2017  Johannes Pohl
    Copyright (C) 2018 Laszlo Hegedüs
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <sys/resource.h>

#include "controller.h"
#include "browseZeroConf/browsemDNS.h"

#ifdef HAS_DAEMON
#include "daemon.h"
#endif
#include "popl.hpp"
#include "aixlog.hpp"
#include "utils/string_utils.h"
#include "signalHandler.h"

using namespace std;
using namespace popl;
using namespace snapcaster;

volatile sig_atomic_t g_terminated = false;

int main (int argc, char **argv)
{
	int exitcode = EXIT_SUCCESS;
	try
	{
		OptionParser op("Supported options");
		auto helpSwitch = op.add<Switch>("", "help", "print this help message");
		auto debugOption = op.add<Implicit<string>, Visibility::hidden>("", "debug", "enable debug logging", "");
		auto versionSwitch = op.add<Switch>("v", "version", "show version number");
		auto hostValue = op.add<Value<string>>("h", "host", "server hostname or ip address", "");
		auto portValue = op.add<Value<size_t>>("p", "port", "server port", 1704);
		auto rotaryValue = op.add<Value<string>>("r", "rotary", "path to rotary event device", "");
		auto buttonValue = op.add<Value<string>>("b", "button", "path to button event device", "");
#ifdef HAS_DAEMON
		auto daemonOption = op.add<Value<int>>("d", "daemon", "daemonize, optional process priority [-20..19]", -3);
        auto userValue = op.add<Value<string>>("u", "user", "option to run under [user:group]", "");
#endif
        
		try
		{
			op.parse(argc, argv);
		}
		catch  (const std::invalid_argument& e)
		{
			cerr << "Exception: " << e.what() << std::endl;
			cout << "\n" << op << "\n";
			exit(EXIT_FAILURE);
		}
		
		if (versionSwitch->is_set())
		{
			cout << "snapcaster ui v" << VERSION << "\n"
					<< "Copyright (C) 2018 laszloh (github.com/laszloh).\n"
					<< "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
					<< "This is free software: you are free to change and redistribute it.\n"
					<< "There is NO WARRANTY, to the extent permitted by law.\n\n"
					<< "Written by Laszlo Hegedüs.\n"
					<< "Using code from Johannes M. Pohl (snapcast@badaix.de).\n";
			exit(EXIT_SUCCESS);
		}
		
		if (helpSwitch->is_set())
		{
			cout << op << "\n";
			exit(EXIT_SUCCESS);
		}
		
		AixLog::Log::init<AixLog::SinkNative>("snapcaster-ui", AixLog::Severity::trace, AixLog::Type::special);
		if (debugOption->is_set())
		{
			AixLog::Log::instance().add_logsink<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::all, "%Y-%m-%d %H-%M-%S.#ms [#severity] (#tag_func)");
			if (!debugOption->value().empty())
				AixLog::Log::instance().add_logsink<AixLog::SinkFile>(AixLog::Severity::trace, AixLog::Type::all, debugOption->value(), "%Y-%m-%d %H-%M-%S.#ms [#severity] (#tag_func)");
		}
		else
		{
			AixLog::Log::instance().add_logsink<AixLog::SinkCout>(AixLog::Severity::info, AixLog::Type::all, "%Y-%m-%d %H-%M-%S [#severity]");
		}
		
		signal(SIGHUP, signal_handler);
		signal(SIGTERM, signal_handler);
		signal(SIGINT, signal_handler);
		
#ifdef HAS_DAEMON
		std::unique_ptr<Daemon> daemon;
		if (daemonOption->is_set())
		{
			string pidFile = "/var/run/snapclient/pid";
			string user = "";
			string group = "";

			if (userValue->is_set())
			{
				if (userValue->value().empty())
					std::invalid_argument("user must not be empty");

				vector<string> user_group = utils::string::split(userValue->value(), ':');
				user = user_group[0];
				if (user_group.size() > 1)
					group = user_group[1];
			}
			daemon.reset(new Daemon(user, group, pidFile));
			daemon->daemonize();
            
            int processPriority = daemonOption->value();
			if (processPriority < -20)
				processPriority = -20;
			else if (processPriority > 19)
				processPriority = 19;
			if (processPriority != 0)
				setpriority(PRIO_PROCESS, 0, processPriority);
			SLOG(NOTICE) << "daemon started" << std::endl;
		}
#endif

		std::unique_ptr<Controller> controller(new Controller(hostValue->value(), portValue->value(),
											rotaryValue->value(), buttonValue->value()));
		if (!g_terminated)
		{
			LOG(INFO) << "Booting up...\n";
//			controller->start(pcmDevice, host, port, latency);
			while(!g_terminated)
				std::this_thread::sleep_for(std::chrono::seconds(1));
//			controller->stop();
		}
	}
	catch (const std::exception& e)
	{
		SLOG(ERROR) << "Exception: " << e.what() << std::endl;
		exitcode = EXIT_FAILURE;
	}
	
	SLOG(NOTICE) << "daemon terminated." << endl;
	exit(exitcode);
}
