#include <string>
#include <vector>

namespace FishEditor
{
    class EditorUtility
    {
    public:
        EditorUtility() = delete;

        // Displays the "open file" dialog and returns the selected path name.
        static std::string OpenFilePanel(const std::string& title, const std::string& directory, const std::string& extension);

        // Displays the "open file" dialog and returns the selected path name.
        static std::string OpenFilePanelWithFilters(const std::string& title, const std::string& directory, const std::vector<std::string>& filters);

        static std::string SaveFilePanel(const std::string& title, const std::string& directory, const std::string& defaultName, const std::string& extension);
    };
}