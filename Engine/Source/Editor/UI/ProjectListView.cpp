#include "ProjectListView.hpp"

#include <QDropEvent>
#include <QMimeData>

#include <Debug.hpp>

ProjectListView::ProjectListView(QWidget *parent /*= 0*/)
	: QListView(parent)
{
    //setAcceptDrops(true);
	//setDragEnabled(true);
	setDragDropMode(QAbstractItemView::DragDrop);
}

ProjectListView::~ProjectListView()
{

}

//void ProjectListView::dragEnterEvent(QDragEnterEvent *e)
//{
//    FishEngine::Debug::Log("%s", "ProjectListView::dragEnterEvent");
//    e->acceptProposedAction();
//}
//
//void ProjectListView::dragMoveEvent(QDragMoveEvent *e)
//{
//    FishEngine::Debug::Log("%s", "ProjectListView::dragMoveEvent");
//    e->acceptProposedAction();
//}
//
//void ProjectListView::dropEvent(QDropEvent *e)
//{
//	FishEngine::Debug::Log("%s", "ProjectListView::dropEvent");
//	auto mimeData = e->mimeData();
//
//	if (mimeData->hasUrls())
//	{
//		QStringList pathList;
//		for (auto const & url : mimeData->urls())
//		{
//			pathList.append(url.toLocalFile());
//		}
//
//	}
//}
