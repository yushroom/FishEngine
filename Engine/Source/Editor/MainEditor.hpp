#ifndef MAINEDITOR_HPP
#define MAINEDITOR_HPP

#include <memory>

#include "FishEditor.hpp"
#include <ReflectClass.hpp>
//#include <boost/filesystem.hpp>

namespace FishEditor
{
    class SceneViewEditor;

    class Meta(NonSerializable) MainEditor
    {
    public:
        MainEditor() = delete;

        static void Init();
        static void Run();
        static void Clean();

//        static void AddApp(AppPtr app)
//        {
//            m_apps.push_back(app);
//        }

        static void Play();
        static void Stop();
//        static bool InPlayMode()
//        {
//            return m_inPlayMode;
//        }

        static void Resize(int width, int height);

        static Action OnInitialized;

    private:
        //static bool m_inPlayMode;

    public:
        static std::unique_ptr<SceneViewEditor>  m_mainSceneViewEditor;
    };
}

#endif // MAINEDITOR_HPP
