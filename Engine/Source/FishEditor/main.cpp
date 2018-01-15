#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Widget.hpp>
#include <FishGUI/Window.hpp>
#include "MainEditor.hpp"

class IMWidget2 : public FishGUI::IMWidget
{
public:
	IMWidget2(const char* name) : FishGUI::IMWidget(name)
	{
	}
	
	virtual void DrawImpl() override
	{
		if (m_renderFunction != nullptr)
		{
			m_renderFunction();
		}
	}
	
	typedef std::function<void(void)> RenderFunction;
	
	void SetRenderFunction(RenderFunction func)
	{
		m_renderFunction = func;
	}
	
protected:
	RenderFunction	m_renderFunction;
};

//void FBO_2_PPM_file(int width, int height)
//{
//	FILE    *output_image;
//	int     output_width, output_height;
//
//	output_width = width;
//	output_height = height;
//
//	/// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
//	int             i, j, k;
//	unsigned char   *pixels = (unsigned char*)malloc(output_width*output_height*3);
//
//	/// READ THE CONTENT FROM THE FBO
//	glReadBuffer(GL_COLOR_ATTACHMENT0);
//	glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//
//	output_image = fopen("/Users/yushroom/output.ppm", "wt");
//	fprintf(output_image,"P3\n");
//	fprintf(output_image,"# Created by Ricao\n");
//	fprintf(output_image,"%d %d\n",output_width,output_height);
//	fprintf(output_image,"255\n");
//
//	k = 0;
//	for(i=0; i<output_width; i++)
//	{
//		for(j=0; j<output_height; j++)
//		{
//			fprintf(output_image,"%u %u %u ",(unsigned int)pixels[k],(unsigned int)pixels[k+1],
//					(unsigned int)pixels[k+2]);
//			k = k+3;
//		}
//		fprintf(output_image,"\n");
//	}
//	free(pixels);
//}

#include "UI/MainWindow.hpp"
#include "UI/GLWidget.hpp"
#include "UI/OpenProjectDialog.hpp"
#include "UI/HierarchyWidget.hpp"

#include <UnityToolBar.hpp>
#include <FileListWidget.hpp>
#include <DirTreeWidget.hpp>

using namespace FishGUI;
using namespace FishEngine;
using namespace FishEditor;

int main()
{
	FishGUI::Init();
	
	const char* path = "/Users/yushroom/program/FishEngine/Example/PBR/Assets";
	
	OpenProjectDialog::Set(path);
	::MainWindow mainWindow;
	
	auto win = dynamic_cast<FishGUI::MainWindow*>( WindowManager::GetInstance().GetMainWindow() );
	
	auto right = new FishGUI::TabWidget("right");
	right->SetWidth(270);
	right->SetMinSize(250, 150);
	auto inspector = new IMWidget2("Inspector");
	right->AddChild(inspector);
	
	auto bottom = new FishGUI::TabWidget("bottom");
	bottom->SetHeight(180);
	bottom->SetMinSize(150, 150);
	auto project = new Widget("Project");
	auto console = new IMWidget2("Console");
	bottom->AddChild(project);
	bottom->AddChild(console);
	
//	auto rootNode = new FileNode("/Users/yushroom/program/FishEngine/Example/UnityChan-crs/Assets");
	auto rootNode = new FileNode(path);
	auto dirs = new DirTreeWidget("Dirs", rootNode);
	dirs->SetWidth(150);
	dirs->SetMinSize(100, 100);
	auto files = new FileListWidget("Files");
	files->SetWidth(400);
	files->SetMinSize(200, 100);
	files->SetRoot(rootNode);
	dirs->GetSelectionModel()->SetSelectionChangedCallback([files](FileNode* node){
		files->SetRoot(node);
	});
	{
		auto layout = new SplitLayout(Orientation::Horizontal);
		project->SetLayout(layout);
		layout->Split(dirs, files);
	}
	
	auto left = new TabWidget("left");
	left->SetWidth(200);
	left->SetMinSize(200, 150);
	auto hierarchy = new HierarchyWidget("Hierarchy");
	left->AddChild(hierarchy);
	
	auto center = new TabWidget("center");
	center->SetWidth(500);
	center->SetMinSize(256, 256);
	auto scene = new GLWidget("Scene");
	auto game = new IMWidget2("Game");
	auto assetStore = new IMWidget2("Asset Store");
	center->AddChild(scene);
	center->AddChild(game);
	center->AddChild(assetStore);
	
	auto layout1 = new SplitLayout(Orientation::Horizontal);
	auto layout2 = new SplitLayout(Orientation::Vertical);
	auto layout3 = new SplitLayout(Orientation::Horizontal);
	
	
	layout1->Split(layout2, right);
	layout2->Split(layout3, bottom);
	layout3->Split(left, center);
	
	std::string email = "thisisaverylongmail@example.com";
	bool enabled = true;
	FishGUI::Vector3f position{0, 1, -10};
	FishGUI::Vector3f rotation{0, 0, 0};
	FishGUI::Vector3f scale{1, 1, 1};
	
	win->SetToolBar(new UnityToolBar());
	win->SetStatusBar(new StatusBar());
	
	float fov = 60;
	//	float near = 0.3f;
	//	float far = 1000.f;
	bool allowHDR = true;
	bool allowMSAA = true;
	bool allowDynamicResolution = false;
	
	auto f1 = [&](){
		FishGUI::Group("Transform");
		FishGUI::Float3("Position", position.x, position.y, position.z);
		FishGUI::Float3("Rotation", rotation.x, rotation.y, rotation.z);
		FishGUI::Float3("Scale", scale.x, scale.y, scale.z);
		FishGUI::EndGroup();
		
		FishGUI::Group("Camera");
		FishGUI::Combox("Clear Flags", "Skybox");
		FishGUI::Combox("Culling Mask", "Everything");
		FishGUI::Combox("Projection", "Perspective");
		FishGUI::Slider("Field of View", fov, 1, 179);
		auto near = std::to_string(0.30);
		auto far = std::to_string(1000);
		FishGUI::InputText("Clipping Plane Near", near);
		FishGUI::InputText("Clipping Plane Far", far);
		if (FishGUI::CheckBox("Allow HDR", allowHDR))
		{
			printf("value of [Allow HDR] is changed\n");
		}
		FishGUI::CheckBox("Allow MSAA", allowMSAA);
		FishGUI::CheckBox("Allow Dynamic Resolution", allowDynamicResolution);
		FishGUI::EndGroup();
		
		FishGUI::Button("Add Component");
//		static bool running = false;
//		if (FishGUI::Button(running ? "Stop" : "Run"))
//		{
//			if (running)
//				FishEditor::MainEditor::Stop();
//			else
//				FishEditor::MainEditor::Play();
//			running = !running;
//
//		}
	};
	
	auto f2 = [&]() {
		FishGUI::CheckBox("CheckBox", enabled);
		FishGUI::Label("Login");
		FishGUI::InputText("Email", email);
		FishGUI::Label("AaBbCcDdEeFfGg");
		if (FishGUI::Button("button 1")) {
			printf("button 1 clicked\n");
		}
		if (FishGUI::Button("button 2")) {
			printf("button 2 clicked\n");
		}
		for (int i = 1; i < 10; ++i) {
			FishGUI::Image(i, 32, 32);
		}
	};
	
//	project->SetRenderFunction(f1);
//	dirs->SetRenderFunction(f1);
//	files->SetRenderFunction(f2);
	inspector->SetRenderFunction(f1);
//	scene->SetRenderFunction(f2);
//	hierarchy->SetRenderFunction(f2);
	
#if 0
	auto l = new SimpleLayout();
	l->SetWidget(scene);
	win->SetLayout(l);
	win->SetOverlayDraw([scene](){
		scene->DrawScene();
	});
#else
	win->SetLayout(layout1);
//	win->SetPreDraw([scene](){
//		scene->DrawScene();
//	});
	win->SetOverlayDraw([scene](){
		scene->DrawScene();
	});
#endif
	
	FishGUI::Run();
	return 0;
}
