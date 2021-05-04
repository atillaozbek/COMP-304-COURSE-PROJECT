
#include "SeaShell.h"
#include <iostream>
#include <exception>
#include <unistd.h>
#include "SystemHandler.h"

#define VERSION_NUMBER	"0.0.1"	

namespace SS
{
	SeaShell::SeaShell() {

		m_shouldClose = false;
		std::cout << "Welcome to SeaShell" <<std::endl;
		std::cout << "Version: " << VERSION_NUMBER << std::endl;
		std::cout << "Please enter your command" << std::endl;
		m_currentPath.assign(m_shellCore.getCurrentPath());

		printf("%s :$ ", m_currentPath.c_str());
	}

	void SeaShell::Run() noexcept
	{
		
		static std::string inputString;

		while(true)
		{	
			getline(std::cin, inputString);
			if(inputString == "exit")
			{
				exit(0);
			}
			
			try
			{
				m_shellCore.parseAndExecute(inputString);
			}
			catch(std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}


			//Printing history if requested
			/*
			if(m_shellCore.isHistoryRequested())
			{
				unsigned int index = 1; 

				for(const auto& elem : history)
				{
					printf("%d %s\n", index++, elem.c_str());
				}
			}
			*/

			m_currentPath = m_shellCore.getCurrentPath();
			printf("%s :$ ",m_currentPath.c_str());
		}

	}
}