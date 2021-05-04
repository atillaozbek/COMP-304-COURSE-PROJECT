#pragma once

/*
*The Command Class
*It parses a command string and stores ALL relevant information 
*/
#include <string>
#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <memory>

namespace SS
{
	enum class IORedirect : int
	{
		OutputTruncate = 0,
		OutputAppend = 1,
		InputRead = 2
	};

	class Command
	{
	public:
		Command();

		void print() const;
		void parse(const std::string& rawCommand);

		bool isGood() const;
		bool hasNext() const;

		//the getter functions

		const std::string& getName() const;
		const bool getBackground() const;
		const bool getAutoComplete() const;
		const std::vector<std::string>& getArgs() const;
		const std::array<std::string, 3>& getRedirects() const;
		const std::unique_ptr<Command>& getNext() const;
		

	private:

		//Command Integrity Check
		//false if parsing failed.
		bool m_isInitialized{false};		
		
		//Member variables of the command structure
		//arg_count is not used since vector covers it
		std::string m_name;
		bool m_background;
		bool m_autoComplete;

		std::vector<std::string> m_args;
		std::array<std::string, 3> m_redirects;

		//Using smart pointers for automatic memory management
		std::unique_ptr<Command> m_next;
	};

}