#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "FishEngine.hpp"
#include "Debug.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
    class Meta(NonSerializable) TagManager
    {
    public:
        TagManager() = delete;

        static bool Exists(std::string const & tag)
        {
			auto it = std::find(s_tags.begin(), s_tags.end(), tag);
			return it != s_tags.end();
        }
		
//		static bool Exists(int index)
//		{
//			//if (index == -1)
//			return index >= 0 && (index < 7 || (index < s_tags.size() && !s_tags[index].empty()));
//		}
		
		static int TagToIndex(std::string const & tag)
		{
			if (tag.empty())
				return -1;
			for (int i = 0; i < s_tags.size(); ++i)
			{
				if (tag == s_tags[i])
					return i;
			}
			return -1;
		}
		
		static std::string const & IndexToTag(int index)
		{
			if (index == -1)
				return s_tags[0];
			if (index >= 0)
			{
				if (index < 7)
					return s_tags[index];
				else if (index < s_tags.size() && !s_tags[index].empty())
					return s_tags[index];
			}
			return s_tags[0];
		}
		
		static void AddTag(std::string const & tag)
		{
			if (Exists(tag)) {
				Debug::LogError("tag %s has already exists", tag.c_str());
			}
			else{
				s_tags.push_back(tag);
				s_editorFlag ++;
			}
		}
		
		static void RemoveTag(int index)
		{
			if (index > InternalTagCount && index < s_tags.size())
			{
				s_tags[index] = "";
				s_editorFlag ++;
			}
		}

    private:
		friend class ::UIGameObjectHeader;
		
		static constexpr int InternalTagCount = 7;
		
        static std::vector<std::string> s_tags;
		
		static int s_editorFlag;	// special flag for Editor use only;
    };
}
