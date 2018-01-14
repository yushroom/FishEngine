#ifndef MAINEDITOR_HPP
#define MAINEDITOR_HPP

#include <memory>

#include "FishEditor.hpp"
#include <FishEngine/ReflectClass.hpp>

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

		static void Play();
		static void Stop();

		static void Resize(int width, int height);

		static void NewScene();

		static Action OnInitialized;

	private:

	public:
		static std::unique_ptr<SceneViewEditor>  m_mainSceneViewEditor;
	};
}

#endif // MAINEDITOR_HPP
