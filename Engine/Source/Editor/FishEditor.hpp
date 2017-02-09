#ifndef FishEditor_hpp
#define FishEditor_hpp

#include <FishEngine.hpp>
#include <functional>

namespace FishEditor
{
	class Model;
	typedef std::shared_ptr<Model> ModelPtr;

	class ModelNode;
	typedef std::shared_ptr<ModelNode> ModelNodePtr;

	class AssetImporter;
	typedef std::shared_ptr<AssetImporter> AssetImporterPtr;

	class TextureImporter;
	typedef std::shared_ptr<TextureImporter> TextureImporterPtr;

	class ModelImporter;
	typedef std::shared_ptr<ModelImporter> ModelImporterPtr;


    //typedef std::list<Actor> Action;

    class Action
    {
    public:
        typedef std::function<void()> Actor;

        void operator += (Actor const& actor)
        {
            m_actors.push_back(actor);
        }

        void operator()() const
        {
            for (auto const & a : m_actors)
            {
                a();
            }
        }

    private:
        std::list<Actor> m_actors;
    };
}

class HierarchyView;

#endif //FishEditor_hpp
