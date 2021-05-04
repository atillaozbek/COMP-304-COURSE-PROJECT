#pragma once

/*
The SeaShell class.
Defines the main interface for the user.
Handles Input-Output jobs for SeaShellCore class.
Also Responsible for the main looping of the program.
*/

#include <string>
#include "Command.h"
#include "SeaShellCore.h"

namespace SS
{
	class SeaShell {
	public:
		SeaShell();

		//Runs the shell in an endless loop untill the exit signal.
		void Run() noexcept;
	private:
		//Executes a command structure

		//member variables
		SeaShellCore m_shellCore;

		bool m_shouldClose;
		std::string m_currentPath;
		std::string m_currentCommand;
	};
}