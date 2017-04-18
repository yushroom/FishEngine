#pragma once

#include <string>
#include <vector>

namespace FishEditor
{
	class SearchFilter
	{
	public:
		enum class SearchArea
		{
			AllAssets,
			SelectedFolders,
			AssetStore
		};

		enum class State
		{
			EmptySearchFilter,
			FolderBrowsing,
			SearchingInAllAssets,
			SearchingInFolders,
			SearchingInAssetStore
		};

	public:

		void ClearSearch()
		{
			this->m_NameFilter = "";
			this->m_ClassNames.clear();
			this->m_AssetLabels.clear();
			this->m_AssetBundleNames.clear();
			this->m_ReferencingInstanceIDs.clear();
			this->m_ScenePaths.clear();
			this->m_VersionControlStates.clear();
			this->m_ShowAllHits = false;
		}

		State GetState()
		{
			bool flag = !m_NameFilter.empty() || !IsNullOrEmpty(this->m_AssetLabels) || !IsNullOrEmpty(this->m_ClassNames) || !IsNullOrEmpty(this->m_AssetBundleNames) || !IsNullOrEmpty(this->m_ReferencingInstanceIDs) || !IsNullOrEmpty(this->m_VersionControlStates);
			bool flag2 = !IsNullOrEmpty(this->m_Folders);
			State result;
			if (flag)
			{
				if (this->m_SearchArea == SearchArea::AssetStore)
				{
					result = State::SearchingInAssetStore;
				}
				else if (flag2 && this->m_SearchArea == SearchArea::SelectedFolders)
				{
					result = State::SearchingInFolders;
				}
				else
				{
					result = State::SearchingInAllAssets;
				}
			}
			else if (flag2)
			{
				result = State::FolderBrowsing;
			}
			else
			{
				result = State::EmptySearchFilter;
			}
			return result;
		}

		bool IsSearching()
		{
			State state = this->GetState();
			return state == State::SearchingInAllAssets || state == State::SearchingInFolders || state == State::SearchingInAssetStore;
		}

		bool SetNewFilter(SearchFilter newFilter)
		{
			bool result = false;
			if (newFilter.m_NameFilter != this->m_NameFilter)
			{
				this->m_NameFilter = newFilter.m_NameFilter;
				result = true;
			}
			if (newFilter.m_ClassNames != this->m_ClassNames)
			{
				this->m_ClassNames = newFilter.m_ClassNames;
				result = true;
			}
			if (newFilter.m_Folders != this->m_Folders)
			{
				this->m_Folders = newFilter.m_Folders;
				result = true;
			}
			if (newFilter.m_VersionControlStates != this->m_VersionControlStates)
			{
				this->m_VersionControlStates = newFilter.m_VersionControlStates;
				result = true;
			}
			if (newFilter.m_AssetLabels != this->m_AssetLabels)
			{
				this->m_AssetLabels = newFilter.m_AssetLabels;
				result = true;
			}
			if (newFilter.m_AssetBundleNames != this->m_AssetBundleNames)
			{
				this->m_AssetBundleNames = newFilter.m_AssetBundleNames;
				result = true;
			}
			if (newFilter.m_ReferencingInstanceIDs != this->m_ReferencingInstanceIDs)
			{
				this->m_ReferencingInstanceIDs = newFilter.m_ReferencingInstanceIDs;
				result = true;
			}
			if (newFilter.m_ScenePaths != this->m_ScenePaths)
			{
				this->m_ScenePaths = newFilter.m_ScenePaths;
				result = true;
			}
			if (newFilter.m_SearchArea != this->m_SearchArea)
			{
				this->m_SearchArea = newFilter.m_SearchArea;
				result = true;
			}
			this->m_ShowAllHits = newFilter.m_ShowAllHits;
			return result;
		}

#if 0
		std::string ToString()
		{
			std::string text = "SearchFilter: ";
			//text += std::string.Format("[Area: {0}, State: {1}]", this->m_SearchArea, this->GetState());
			text += FishEngine::Format("[Area: {1}, State: {2}]", this->m_SearchArea)
			if (!string.IsNullOrEmpty(this->m_NameFilter))
			{
				text = text + "[Name: " + this->m_NameFilter + "]";
			}
			if (this->m_AssetLabels != null && this->m_AssetLabels.Length > 0)
			{
				text = text + "[Labels: " + this->m_AssetLabels[0] + "]";
			}
			if (this->m_VersionControlStates != null && this->m_VersionControlStates.Length > 0)
			{
				text = text + "[VersionStates: " + this->m_VersionControlStates[0] + "]";
			}
			if (this->m_AssetBundleNames != null && this->m_AssetBundleNames.Length > 0)
			{
				text = text + "[AssetBundleNames: " + this->m_AssetBundleNames[0] + "]";
			}
			string text2;
			if (this->m_ClassNames != null && this->m_ClassNames.Length > 0)
			{
				text2 = text;
				text = string.Concat(new object[]
				{
					text2,
					"[Types: ",
					this->m_ClassNames[0],
					" (",
					this->m_ClassNames.Length,
					")]"
				});
			}
			if (this->m_ReferencingInstanceIDs != null && this->m_ReferencingInstanceIDs.Length > 0)
			{
				text2 = text;
				text = string.Concat(new object[]
				{
					text2,
					"[RefIDs: ",
					this->m_ReferencingInstanceIDs[0],
					"]"
				});
			}
			if (this->m_Folders != null && this->m_Folders.Length > 0)
			{
				text = text + "[Folders: " + this->m_Folders[0] + "]";
			}
			text2 = text;
			return string.Concat(new object[]
			{
				text2,
				"[ShowAllHits: ",
				this->showAllHits,
				"]"
			});
		}
#endif

		static std::vector<std::string> Split(std::string const & text)
		{
			std::vector<std::string> result;
			if (text.empty())
			{
				result = {};
			}
			else
			{
				List<string> list = new List<string>();
				IEnumerator enumerator = Regex.Matches(text, "\".+?\"|\\S+").GetEnumerator();
				try
				{
					while (enumerator.MoveNext())
					{
						Match match = (Match)enumerator.Current;
						list.Add(match.Value.Replace("\"", ""));
					}
				}
				finally
				{
					IDisposable disposable;
					if ((disposable = (enumerator as IDisposable)) != null)
					{
						disposable.Dispose();
					}
				}
				result = list.ToArray();
			}
			return result;
		}

	private:

		template<class T>
		inline bool IsNullOrEmpty(std::vector<T> list)
		{
			return list.empty();
		}

		std::string FilterToSearchFieldString()
		{
			std::string text = "";
			if (!std::string.IsNullOrEmpty(this->m_NameFilter))
			{
				text += this->m_NameFilter;
			}
			this->AddToString<std::string>("t:", this->m_ClassNames, ref text);
			this->AddToString<std::string>("l:", this->m_AssetLabels, ref text);
			this->AddToString<std::string>("v:", this->m_VersionControlStates, ref text);
			this->AddToString<std::string>("b:", this->m_AssetBundleNames, ref text);
			return text;
		}

		void AddToString<T>(std::string prefix, T[] list, ref string result)
		{
			if (list != null)
			{
				if (result == null)
				{
					result = "";
				}
				for (int i = 0; i < list.Length; i++)
				{
					T t = list[i];
					if (!string.IsNullOrEmpty(result))
					{
						result += " ";
					}
					result = result + prefix + t;
				}
			}
		}

		void SearchFieldStringToFilter(std::string const & searchString)
		{
			this->ClearSearch();
			if (searchString.empty())
			{
				SearchUtility::ParseSearchString(searchString, *this);
			}
		}

		static SearchFilter CreateSearchFilterFromString(std::string const & searchText)
		{
			SearchFilter searchFilter;
			SearchUtility::ParseSearchString(searchText, searchFilter);
			return searchFilter;
		}

	public:
		std::string m_NameFilter = "";
		std::vector<std::string> m_ClassNames;
		std::vector<std::string> m_AssetLabels;
		std::vector<std::string> m_AssetBundleNames;
		std::vector<std::string> m_VersionControlStates;
		std::vector<int> m_ReferencingInstanceIDs;
		std::vector<std::string> m_ScenePaths;
		bool m_ShowAllHits = false;

		SearchArea m_SearchArea = SearchArea::AllAssets;

		std::vector<std::string> m_Folders;
	};
}