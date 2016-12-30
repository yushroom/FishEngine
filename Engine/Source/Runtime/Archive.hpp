#pragma once
#include <string>
#include <vector>
#include <list>

#include "FishEngine.hpp"

namespace FishEngine
{
    class Archive
    {
    public:
        Archive();
        Archive(const Archive&);
        Archive& operator=(const Archive& ArchiveToCopy);
        virtual ~Archive();
        virtual void StartArray(const std::string& name) = 0;
        virtual void EndArray(const std::string& name) = 0;
        virtual void AddObject(ObjectPtr object) = 0;
    };
    
    class TextArchive : public Archive
    {
    public:
        virtual void StartArray(const std::string& name) override;
        virtual void EndArray(const std::string& name) override;
        virtual void AddObject(ObjectPtr object) override;
    };

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, const T& value);

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, std::shared_ptr<T> value)
    {
        Serialize(archive, name, value->m_uuid);
    }

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, std::weak_ptr<T> value)
    {
        Serialize(archive, name, value->m_uuid);
    }

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, const std::vector<T>& value)
    {
        archive.StartArray(name);
        for (auto& x : value)
        {
            Serizlize(archive, "", x);
        }
        archive.EndArray(name);
    }

    template<typename T>
    void Serialize(Archive& archive, const std::string& name, const std::list<T>& value)
    {
        archive.StartArray(name);
        for (auto& x : value)
        {
            Serizlize(archive, "", x);
        }
        archive.EndArray(name);
    }

    void Serialize(Archive& archive, const std::string& name, bool value);
    void Serialize(Archive& archive, const std::string& name, short value);
    void Serialize(Archive& archive, const std::string& name, unsigned short value);
    void Serialize(Archive& archive, const std::string& name, int  value);
    void Serialize(Archive& archive, const std::string& name, unsigned int value);
    void Serialize(Archive& archive, const std::string& name, long value);
}
