#ifndef Editor_hpp
#define Editor_hpp

#include "FishEditor.hpp"
#include <MeshFilter.hpp>

NAMESPACE_FISHEDITOR_BEGIN

template<typename T>
class Editor
{
public:
    virtual ~Editor() = default;
    
    virtual void OnInspectorGUI() {
        
    }
};

typedef Editor<FishEngine::MeshFilter> MeshFilterEditor;

NAMESPACE_FISHEDITOR_END

#endif /* Editor_hpp */
