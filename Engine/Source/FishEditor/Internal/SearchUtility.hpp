#pragma once

#include "SearchFilter.hpp"
#include <boost/algorithm/string.hpp>

namespace FishEditor
{
	class SearchUtility
	{
	private:
		static void RemoveUnwantedWhitespaces(std::string & searchString);

		static bool ParseSearchString(std::string const & searchText, SearchFilter & filter);

		static bool CheckForKeyWords(std::string const & searchString, SearchFilter & filter, int quote1, int quote2);

		static int FindFirstPositionNotOf(std::string const & source, std::string const & chars);
	};
}