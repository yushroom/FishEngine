#ifndef FishEditor_hpp
#define FishEditor_hpp

#include <FishEngine.hpp>
#include <functional>

namespace FishEditor
{

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
