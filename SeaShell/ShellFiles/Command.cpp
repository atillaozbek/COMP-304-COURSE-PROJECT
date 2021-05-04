#include "Command.h"
#include <sstream>

namespace SS
{
	Command::Command() : m_autoComplete(false), m_background(false)
	{
	}
	
	bool Command::isGood() const
	{
		return m_isInitialized;
	}

	bool Command::hasNext() const
	{
		if(m_next) //If next is not empty
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const std::string& Command::getName() const
	{
		return m_name;
	}

	const bool Command::getAutoComplete() const
	{
		return m_autoComplete;
	}

	const bool Command::getBackground() const{
		return m_background;
	}

	const std::vector<std::string>& Command::getArgs() const
	{
		return m_args;
	}

	const std::array<std::string, 3>& Command::getRedirects() const
	{
		return m_redirects;
	}

	const std::unique_ptr<Command>& Command::getNext() const
	{
		return m_next;
	}

	void Command::parse(const std::string& rawCommand)
	{
		//Creating a brand new string 
		std::string commandStr(rawCommand);

		//trimming the whitespace from the end of the commandStr.
		size_t lastCharIndex = commandStr.size() - 1;
		for (; commandStr[lastCharIndex] == ' '; lastCharIndex--);

		//commandStr.erase(commandStr.begin() + lastCharIndex + 1, commandStr.end());

		//checking if the last element is special (?, &)

		if (commandStr[lastCharIndex] == '?')
		{
			m_autoComplete = true;
		}
		else if (commandStr[lastCharIndex] == '&')
		{
			m_background = true;
		}

		//Checking for pipes
		//If found,creating a new command with the substring following the pipe.
		//erasing piped command from the current command.

		size_t pipeIndex = commandStr.find('|', 0);

		if (pipeIndex != std::string::npos)
		{	
			//Recursively parsing a Command object
			m_next = std::make_unique<Command>();
			m_next->parse(commandStr.substr(pipeIndex + 1, commandStr.size() - pipeIndex + 1));

			commandStr.erase(pipeIndex);
		}
		else
		{
			m_next = nullptr;
		}


		std::string word;
		std::stringstream ss(commandStr); //Using stringstream to extract words from string.

		//Finally adding each word to arguments of the command

		ss >> m_name;

		while (ss >> word)
		{
			if (word[0] == '&' || word[0] == '?')  //If they are special symbols do not add.
			{
				continue;
			}
			else if (!word.compare("<")) // Input redirection, next word is the file name.
			{
				ss >> word;
				if(word.empty() || !word.compare(">") || !word.compare("<") || !word.compare(">>") || !word.compare("&") || !word.compare("?"))
				{
					throw std::runtime_error("No file name is specified after <");
				}
				else
				{
					m_redirects[static_cast<int>(IORedirect::InputRead)] = word;
				}
			}
			else if (!word.compare(">")) // Input redirection, next word is the file name.
			{
				ss >> word;
				if (word.empty() || !word.compare(">") || !word.compare("<") || !word.compare(">>") || !word.compare("&") || !word.compare("?"))
				{
					throw std::runtime_error("No file name is specified after >");

				}
				else
				{
					m_redirects[static_cast<int>(IORedirect::OutputTruncate)] = word;
				}
			}
			else if (!word.compare(">>")) // Input redirection, next word is the file name.
			{
				ss >> word;
				if (word.empty() || !word.compare(">") || !word.compare("<") || !word.compare(">>") || !word.compare("&") || !word.compare("?"))
				{
					throw std::runtime_error(" No file name is specified after >>");
				}
				else
				{
					m_redirects[static_cast<int>(IORedirect::OutputAppend)] = word;
				}
			}
			else	//word is pure argument, add it to the vector.
			{
				m_args.emplace_back(word);
			}
		}

		m_isInitialized = true;

	}

	void Command::print() const
	{
		printf("Command: <%s>\n", m_name.c_str());

		if (!m_args.empty()) {
			printf("arguments:\n");
			for (const auto& arg : m_args)
			{
				printf("%s\n", arg.c_str());
			}
		}

		printf("Is backgorund: %s\n", m_background ? "yes" : "no");
		printf("Needs Auto-complete: %s\n", m_autoComplete ? "yes" : "no");
		
		printf("Redirects:\n");
		
		for (auto& red : m_redirects)
		{
			printf("%s\n", red.empty()? "N/A":red.c_str());
		}
		
		if (m_next)
		{
			printf("piped to\n");
			printf("-------------------------------------\n");
			m_next->print();
		}

	}
}