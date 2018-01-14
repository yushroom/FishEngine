#include "SearchUtility.hpp"
#include "../AssetDataBase.hpp"

using namespace std;


void FishEditor::SearchUtility::RemoveUnwantedWhitespaces(std::string & searchString)
{
	boost::replace_all(searchString, ": ", ":");
}


bool FishEditor::SearchUtility::ParseSearchString(std::string const & searchText, SearchFilter & filter)
{
	bool result;
	if (searchText.empty())
	{
		result = false;
	}
	else
	{
		filter.ClearSearch();
		string text = searchText;
		SearchUtility::RemoveUnwantedWhitespaces(text);
		bool flag = false;
		int i = SearchUtility::FindFirstPositionNotOf(text, " \t,*?");
		if (i == -1)
		{
			i = 0;
		}
		while (i < text.length())
		{
			int num = text.IndexOfAny(" \t,*?".ToCharArray(), i);
			int num2 = text.find_first_of('"', i);
			int num3 = -1;
			if (num2 != -1)
			{
				num3 = text.find_first_of('"', num2 + 1);
				if (num3 != -1)
				{
					num = text.IndexOfAny(" \t,*?".ToCharArray(), num3);
				}
				else
				{
					num = -1;
				}
			}
			if (num == -1)
			{
				num = text.length();
			}
			if (num > i)
			{
				string text2 = text.substr(i, num - i);
				if (SearchUtility::CheckForKeyWords(text2, filter, num2, num3))
				{
					flag = true;
				}
				else
				{
					filter.m_NameFilter = filter.m_NameFilter + (!filter.m_NameFilter.empty() ? " " : "") + text2;
				}
			}
			i = num + 1;
		}
		result = flag;
	}
	return result;
}

bool FishEditor::SearchUtility::CheckForKeyWords(string const & searchString, SearchFilter & filter, int quote1, int quote2)
{
	bool result = false;
	int num = searchString.find_first_of("t:");
	if (num == 0)
	{
		string item = searchString.substr(num + 2);
		filter.classNames = new List<string>(filter.classNames)
		{
			item
		}.ToArray();
		result = true;
	}
	num = searchString.find_first_of("l:");
	if (num == 0)
	{
		string item2 = searchString.substr(num + 2);
		filter.assetLabels = new List<string>(filter.assetLabels)
		{
			item2
		}.ToArray();
			result = true;
	}
	num = searchString.find_first_of("v:");
	if (num >= 0)
	{
		string item3 = searchString.substr(num + 2);
		filter.versionControlStates = new List<string>(filter.versionControlStates)
		{
			item3
		}.ToArray();
			result = true;
	}
	num = searchString.find_first_of("b:");
	if (num == 0)
	{
		string item4 = searchString.substr(num + 2);
		filter.assetBundleNames = new List<string>(filter.assetBundleNames)
		{
			item4
		}.ToArray();
		result = true;
	}
	num = searchString.find_first_of("ref:");
	if (num == 0)
	{
		int num2 = 0;
		int num3 = num + 3;
		int num4 = searchString.find_first_of(':', num3 + 1);
		if (num4 >= 0)
		{
			string s = searchString.substr(num3 + 1, num4 - num3 - 1);
			int num5;
			if (int.TryParse(s, out num5))
			{
				num2 = num5;
			}
		}
		else
		{
			string assetPath;
			if (quote1 != -1 && quote2 != -1)
			{
				int num6 = quote1 + 1;
				int num7 = quote2 - quote1 - 1;
				if (num7 < 0 || quote2 == -1)
				{
					num7 = searchString.Length - num6;
				}
				assetPath = "Assets/" + searchString.Substring(num6, num7);
			}
			else
			{
				assetPath = "Assets/" + searchString.Substring(num3 + 1);
			}
			UnityEngine.Object @object = AssetDatabase::LoadMainAssetAtPath(assetPath);
			if (@object != null)
			{
				num2 = @object.GetInstanceID();
			}
		}
		filter.m_ReferencingInstanceIDs.clear();
		filter.m_ReferencingInstanceIDs.push_back(num2);
		result = true;
	}
	return result;
}

int FishEditor::SearchUtility::FindFirstPositionNotOf(std::string const & source, std::string const & chars)
{
	int result = -1;
	if (source.empty())
	{
		result = -1;
	}
	else if (chars.empty())
	{
		result = 0;
	}
	else
	{
		auto pos = source.find_first_not_of(chars);
		if (pos != std::string::npos)
			result = pos;
	}
	return result;
}
