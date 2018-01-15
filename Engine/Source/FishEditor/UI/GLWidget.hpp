#pragma once

#include <FishGUI/Widget.hpp>

//namespace FishEditor
//{
	class GLWidget : public FishGUI::IMWidget
	{
	public:
		GLWidget(const char* name);
		virtual void DrawImpl() override;
		void DrawScene();
		void Resize(int width, int height);
	};
//}

