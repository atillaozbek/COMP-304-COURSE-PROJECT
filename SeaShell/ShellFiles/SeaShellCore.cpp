#include "SeaShellCore.h"
#include <sstream>
#include <fstream>

#define LOG(x)	std::cout << x << std::endl;

namespace SS
{   

    SeaShellCore::SeaShellCore() : m_shortdirFile("sdd.txt")
    {
		m_historyFlag = false;

		//Setting the special command and other string values
		m_currenBinDirectory.assign("/bin/");

		SystemHandler::getExecutablePath(m_executablePath);
		m_specialCommands.push_back("cd");
		m_specialCommands.push_back("history");
		m_specialCommands.push_back("shortdir");
		m_specialCommands.push_back("highlight");
		m_specialCommands.push_back("kdiff");
		m_specialCommands.push_back("goodmorning");

        //SystemHandler::getHomeDirectory(m_currentPath);
		SystemHandler::getHomeDirectory(m_homePath);
		SystemHandler::setCurrentDirectory(m_executablePath);
		m_currentPath.assign(m_executablePath);
    }

	//Getter and setter functions 

    const std::string& SeaShellCore::getCurrentPath()
    {
        return m_currentPath;
    }

	const bool SeaShellCore::isHistoryRequested()
	{
		bool temp = m_historyFlag;
		m_historyFlag = false;
		return temp;
	}

    void SeaShellCore::execCommand(const std::unique_ptr<Command>& command)
	{   

		static std::string tempName;

        auto& name = command->getName();
		auto& args = command->getArgs();

		std::vector<const char*> arguments;
		std::vector<std::string> modifiedArgumentStrings;

		//If command starts with './' execute a file in current directory 
		
		if(name.size() > 1 && name[0] == '.' && name[1] == '/')
		{	
			m_currentPath.push_back('/');
			tempName = m_currentPath + name;
		}
		else //Command root is either ./bin/ /bin/ folder
		{
			tempName = m_currenBinDirectory + name;
		}

		//Preparing arguments
		arguments.push_back(tempName.c_str());
		std::string tempArgument;

		for(const auto& arg : args)
		{
			//If file exists, append current path to it.
			if(SystemHandler::doesFileExists(m_currentPath, arg))
			{
				modifiedArgumentStrings.emplace_back(m_currentPath + "/" + arg);
				arguments.push_back(modifiedArgumentStrings[modifiedArgumentStrings.size() -1].c_str());
			}
			else
			{
				arguments.push_back(arg.c_str());
			}
		}

		arguments.push_back(nullptr);

		//Executing commands recurisvely

		SystemHandler::forkWithWait(arguments[0], arguments.data());

		if(command->getNext())
		{
			execCommand(command->getNext());
		}
	}

    void SeaShellCore::parseAndExecute(const std::string& commandStr)
    {

		//adding line to history
		SystemHandler::appendToFile(m_executablePath + "/history.txt", commandStr);

		static std::stringstream ss;
		static std::string word;

		m_currentCommand.assign(commandStr);
		ss.clear();
		ss.str(m_currentCommand);

		ss >> word;

		//Handling specal command requests
		if(word == m_specialCommands[HIGHTLIGTH] || word == m_specialCommands[KDIFF])
		{
			m_currenBinDirectory.assign(m_executablePath + "/bin/");
		}
		else
		{
			m_currenBinDirectory.assign("/bin/");
		}

		if(word == m_specialCommands[CD])
		{
			ss >> word;
			handleCD(word);
		}
		else if(word == m_specialCommands[HISTORY])
		{
			std::ifstream histFile(m_executablePath + "/history.txt");
			std::string line;
			unsigned int index = 1;

			while(std::getline(histFile, line))
			{
				printf("%d %s\n", index++, line.c_str());
			}
		}
		else if(word == m_specialCommands[SHORT_DIR])
		{
			handleShortDir();
		}
		else if(word == m_specialCommands[GOODMORNING])
		{
			handleGoodMorning();
		}
		else
		{

			auto currentCommand = std::make_unique<Command>();

			currentCommand->parse(m_currentCommand);

			execCommand(currentCommand);
		}
    }

	//Handling Change directory request
	
	void SeaShellCore::handleCD(const std::string& target)
	{
		if(target.empty())
		{
			return;
		}

		if(target[0] == '/')
		{
			SystemHandler::setCurrentDirectory(target, m_currentPath);
		}
		else
		{
			SystemHandler::moveCurrentDirectory(target, m_currentPath);
		}	
	}

	//Handling Short Directory Request
	
	void SeaShellCore::handleShortDir()
	{
		std::stringstream ss;
		std::string word;
		std::string fileData;

		ss.str(m_currentCommand);

		//Discarding the first word of the stream

		ss	>>	word;

		//getting the shortdir commands first parameter

		ss	>>	word;

		if(word == "set")
		{
			ss >> word;

			if(!word.empty())
			{
				fileData.append(word);
				fileData.push_back(' ');
				fileData.append(m_currentPath);

				LOG(word + " is associated with path " + m_currentPath);

				SystemHandler::appendToFile(m_executablePath + '/' + m_shortdirFile, fileData);

			}
			
		}
		else if (word == "jump")
		{

			std::ifstream file;
			file.open(m_executablePath + "/" + m_shortdirFile);

			if(!file.good())
			{
				throw std::runtime_error("User did not set any shortdir.");
			}

			ss >> word;

			if(!word.empty())
			{

				std::string key;

				while (std::getline(file, fileData))
				{
					ss.clear();
					ss.str(fileData);
					
					ss >> key;

					if(key == word)
					{
						ss >> word;
						SystemHandler::setCurrentDirectory(word, m_currentPath);
					}
				}
			}

		}
		else if (word == "del")
		{
			std::ifstream file;
			file.open(m_executablePath + "/" + m_shortdirFile);

			if(!file.good())
			{
				throw std::runtime_error("User did not set any shortdir.");
			}

			ss >> word; 

			if(!word.empty())
			{
				std::string key;

				std::string newFileData;

				while (std::getline(file, fileData))
				{
					ss.clear();
					ss.str(fileData);
					
					ss >> key;

					if(key != word)
					{
						newFileData.append(fileData);
						newFileData.push_back('\n');
					}
				}

				SystemHandler::truncateToFile(m_executablePath + "/" + m_shortdirFile, newFileData);
			}
		}
		else if (word == "clear")
		{
			SystemHandler::truncateToFile(m_executablePath + "/" + m_shortdirFile, "");
		}
		else if(word == "list")
		{
			std::ifstream file;
			file.open(m_executablePath + "/" + m_shortdirFile);

			if(!file.good())
			{
				throw std::runtime_error("User did not set any shortdir.");
			}

			std::cout << file.rdbuf() << std::endl;

		}		
		else
		{
			throw std::runtime_error("No name for current directory is stated.");
		}

	}

	void SeaShellCore::handleGoodMorning()
	{
		std::stringstream ss(m_currentCommand);
		std::string word;
		std::string crontabCommand;

		static bool firstCall = true;

		ss >> word; //goodmorning

		ss >> word; //time info

		//formatting the time into crontab format
		int periodPos = word.find('.');
		if(periodPos == std::string::npos)
		{
			crontabCommand.append("00 ");
			crontabCommand.append(word);
		}
		else
		{

			for(int i = periodPos + 1; i < word.size(); i++)
			{
				crontabCommand.push_back(word[i]);
			}

			crontabCommand.push_back(' ');

			for(int i = 0; i < periodPos + 0; i++)
			{
				crontabCommand.push_back(word[i]);
			}
		}

		crontabCommand.append(" * * * ");
		crontabCommand.append("usr/bin/rhythmbox-client ");
		
		ss >> word;
		crontabCommand.append(word);

		SystemHandler::appendToFile(m_executablePath + "/crntb.txt", crontabCommand);

		parseAndExecute("crontab crntb.txt");

		SystemHandler::truncateToFile(m_executablePath + "/crntb.txt", "");
	}
}