#pragma once

#include "FishEditor.hpp"
#include <FishEngine/Path.hpp>

namespace FishEditor
{
	class EditorUtility
	{
	public:
		static void RevealInFinder(FishEngine::Path const & path);

		//static bool DisplayDialog(string title, string message, string ok, [DefaultValue("\"\"")] string cancel);

		/// <summary>
		///   <para>Displays the "open file" dialog and returns the selected path name.</para>
		/// </summary>
		/// <param name="title"></param>
		/// <param name="directory"></param>
		/// <param name="extension"></param>
		static std::string OpenFilePanel(std::string const & title, std::string const & directory, std::string const & extension);

		/// <summary>
		///   <para>Displays the "open file" dialog and returns the selected path name.</para>
		/// </summary>
		/// <param name="title">Title for dialog.</param>
		/// <param name="directory">Default directory.</param>
		/// <param name="filters">File extensions in form { "Image files", "png,jpg,jpeg", "All files", "*" }.</param>
		static std::string OpenFilePanelWithFilters(std::string const & title, std::string const & directory, std::vector<std::string> const & filters);

		/// <summary>
		///   <para>Displays the "save file" dialog and returns the selected path name.</para>
		/// </summary>
		/// <param name="title"></param>
		/// <param name="directory"></param>
		/// <param name="defaultName"></param>
		/// <param name="extension"></param>
		static std::string SaveFilePanel(std::string const & title, std::string const & directory, std::string const & defaultName, std::string const & extension);

		/// <summary>
		///   <para>Displays the "open folder" dialog and returns the selected path name.</para>
		/// </summary>
		/// <param name="title"></param>
		/// <param name="folder"></param>
		/// <param name="defaultName"></param>
		static std::string OpenFolderPanel(std::string const & title, std::string const & folder, std::string const & defaultName);

		/// <summary>
		///   <para>Displays the "save folder" dialog and returns the selected path name.</para>
		/// </summary>
		/// <param name="title"></param>
		/// <param name="folder"></param>
		/// <param name="defaultName"></param>
		static std::string SaveFolderPanel(std::string const & title, std::string const & folder, std::string const & defaultName);

		/// <summary>
		///   <para>Displays the "save file" dialog in the Assets folder of the project and returns the selected path name.</para>
		/// </summary>
		/// <param name="title"></param>
		/// <param name="defaultName"></param>
		/// <param name="extension"></param>
		/// <param name="message"></param>
		static std::string SaveFilePanelInProject(std::string const & title, std::string const & defaultName, std::string const & extension, std::string const & message)
		{
			return EditorUtility::Internal_SaveFilePanelInProject(title, defaultName, extension, message, "Assets");
		}

		static std::string SaveFilePanelInProject(std::string const & title, std::string const & defaultName, std::string const & extension, std::string const & message, std::string const & path)
		{
			return EditorUtility::Internal_SaveFilePanelInProject(title, defaultName, extension, message, path);
		}

		/// <summary>
		///   <para>Translates an instance ID to a reference to an object.</para>
		/// </summary>
		/// <param name="instanceID"></param>
		static FishEngine::ObjectPtr InstanceIDToObject(int instanceID);

		/// <summary>
		///   <para>Copy all settings of a Unity Object.</para>
		/// </summary>
		/// <param name="source"></param>
		/// <param name="dest"></param>
		static void CopySerialized(FishEngine::ObjectPtr source, FishEngine::ObjectPtr dest);

	private:
		static std::string Internal_SaveFilePanelInProject(std::string const & title, std::string const & defaultName, std::string const & extension, std::string const & message, std::string const & path);
	};
}
