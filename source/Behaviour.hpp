#ifndef Behaviour_hpp
#define Behaviour_hpp

#include "Component.hpp"
#include "Debug.hpp"

NAMESPACE_FISHENGINE_BEGIN

// Behaviours are Components that can be enabled or disabled.
class Behaviour : public Component
{
public:

    virtual ~Behaviour() = default;

    // Enabled Behaviours are Updated, disabled Behaviours are not.
    bool enabled() const { return m_enabled; }
    void setEnabled(bool value) { m_enabled = value; }

    // Has the Behaviour had enabled called.
    bool isActiveAndEnabled() const;

private:
    bool m_enabled = true;
    
    friend class boost::serialization::access;
    template<class Archive>
    inline void serialize(Archive& ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
        ar & BOOST_SERIALIZATION_NVP(m_enabled);
    }
};


NAMESPACE_FISHENGINE_END

#endif // Behaviour_hpp
