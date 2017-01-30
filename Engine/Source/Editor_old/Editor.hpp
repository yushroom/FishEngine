#ifndef Editor_hpp
#define Editor_hpp

#include "FishEditor.hpp"
#include <MeshFilter.hpp>

namespace FishEditor
{
    template<typename T>
    class Editor
    {
    public:
        virtual ~Editor() = default;
        
        virtual void OnInspectorGUI() {
            
        }
    };
    
    typedef Editor<FishEngine::MeshFilter> MeshFilterEditor;
}

#endif /* Editor_hpp */
