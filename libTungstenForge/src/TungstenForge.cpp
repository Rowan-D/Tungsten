#include "wForgePCH.hpp"
#include "TungsrenForge.hpp"
#include "TungstenCore.hpp"

namespace wForge
{
    //struct ProjectSettings
    //{
    //    std::string name;
    //    std::vector<std::filesystem::path> sceneFiles;
    //};

    BuildError::BuildError(Sevarity type, const std::string& message, bool logError)
        : type(type), message(message)
    {
        if (logError)
        {
            Log();
        }
    }

    BuildError::BuildError(Sevarity type, std::string&& message, bool logError)
        : type(type), message(message)
    {
        if (logError)
        {
            Log();
        }
    }

    void BuildError::Log() const
    {
        switch (type)
        {
        case Sevarity::Error:
            W_LOG_ERROR_NO_LOCATION(message);
            break;
        case Sevarity::Warning:
            W_LOG_WARNING_NO_LOCATION(message);
            break;
        default:
            W_ASSERT(false, "Unknown ErrorType!");
            break;
        }
    }

    bool IsValedProjectPath(const std::filesystem::path& projectPath, std::string& errorMessage)
    {
        return true;
    }

    bool IsValedOutputPath(const std::filesystem::path& outputDir, std::string& errorMessage)
    {
        return true;
    }

    static std::optional<std::filesystem::path> GetProjectPath(std::filesystem::path inputPath, std::vector<BuildError>& errorList)
    {
        try
        {
            if (std::filesystem::exists(inputPath))
            {
                std::filesystem::path projectDirectoryPath;

                if (std::filesystem::is_regular_file(inputPath))
                {
                    projectDirectoryPath = inputPath.parent_path();
                }
                else if (std::filesystem::is_directory(inputPath))
                {
                    projectDirectoryPath = inputPath;
                }
                else
                {
                    errorList.push_back(BuildError(BuildError::Sevarity::Error, "Locating Project File Invalid Path Error: The provided path is neither a project file or directory!", true));
                    return {};
                }

                std::vector<std::filesystem::path> wProjFiles;
                for (const auto& entry : std::filesystem::directory_iterator(projectDirectoryPath))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".wproj")
                    {
                        wProjFiles.push_back(entry.path());
                    }
                }

                if (wProjFiles.empty())
                {
                    errorList.push_back(BuildError(BuildError::Sevarity::Error, "Project File Not Found Error: Tungsten Project File (.wproj) not found!", true));
                    return {};
                }
                else if (wProjFiles.size() > 1)
                {
                    std::string wProjFilesList;
                    for (std::filesystem::path wProjFile : wProjFiles)
                    {
                        wProjFilesList += '\n' + wProjFile.filename().string();
                    }
                    errorList.push_back(BuildError(BuildError::Sevarity::Error, fmt::format("Multiple Project File Error: There should be no more than one Tungsten Project File (.wproj) in the project directory. {} were found!{}", wProjFiles.size(), wProjFilesList), true));
                    return {};
                }
                else
                {
                    return wProjFiles.front();
                }
            }
            else
            {
                errorList.push_back(BuildError(BuildError::Sevarity::Error, "Locating Project File Invalid Path Error: project file or directory dose not exist!", true));
                return {};
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            errorList.push_back(BuildError(BuildError::Sevarity::Error, fmt::format("Locating Project File Filesystem Error: {}", e.what()), true));
            return {};
        }
        catch (const std::exception& e)
        {
            errorList.push_back(BuildError(BuildError::Sevarity::Error, fmt::format("Locating Project File General Error: {}", e.what()), true));
            return {};
        }
    }

    //static std::optional<std::vector<uint8_t>> SceneToBytes(const std::filesystem::path& scenePath, std::vector<BuildError>& errorList)
    //{
    //    W_LOG_INFO("{}", scenePath.string());
    //    std::ifstream sceneFile(scenePath);
    //
    //    if (!sceneFile.is_open())
    //    {
    //
    //        return {};
    //    }
    //
    //    std::string line;
    //    while (std::getline(sceneFile, line))
    //    {
    //        W_LOG_INFO_NO_LOCATION(line);
    //    }
    //
    //    sceneFile.close();
    //    return true;
    //}

    bool BuildProject(const std::filesystem::path& projectPath, std::filesystem::path outputDir, std::vector<BuildError>& errorList)
    {
        W_LOG_DEBUG("Build called. Asset path: {} Output path: {}", projectPath.string(), outputDir.string());

        std::optional<std::filesystem::path> projectFilePath = GetProjectPath(projectPath, errorList);

        if (!projectFilePath.has_value())
        {
            errorList.push_back(BuildError(BuildError::Sevarity::Error, "Project File Not Found Error: Tungsten Project File (.wproj) not found!", true));
            return false;
        }

        // TODO Check if .parent_path() can fail
        std::filesystem::path projectDirectoryPath = projectFilePath.value().parent_path();

        YAML::Node projectFile = YAML::LoadFile(projectFilePath.value().string());

        std::string projectName = projectFile["projectName"].as<std::string>();

        if (projectName != projectFilePath.value().stem().string())
        {
            errorList.push_back(BuildError(BuildError::Sevarity::Warning, fmt::format("Naming mismatch Warning: Project file name \"{}\" should match project name \"{}\"", projectFilePath.value().stem().string(), projectName), true));
        }

        if (projectName != projectDirectoryPath.filename().string())
        {
            errorList.push_back(BuildError(BuildError::Sevarity::Warning, fmt::format("Naming mismatch Warning: Project directory name \"{}\" should match project name \"{}\"", projectFilePath.value().parent_path().filename().string(), projectName), true));
        }

        std::vector<std::string> sceneList = projectFile["scenes"].as<std::vector<std::string>>();
        W_LOG_INFO("scene Count: {}", sceneList.size());
        for (const std::string& scenePathStr : sceneList)
        {
            // Normalize the combined path (resolve any `.` or `..`)
            std::filesystem::path scenePath = (projectDirectoryPath / scenePathStr).lexically_normal();
            //BuildScene(scenePath);
        }

        outputDir /= projectName + "-Build";
        std::filesystem::path assetDataDir = outputDir / "data";

        std::error_code ec;
        if (std::filesystem::create_directories(assetDataDir, ec)) {
            //W_LOG_INFO("Directories created successfully!");
        }
        else if (ec) {
            W_LOG_ERROR("Failed to create directories: {}", ec.message());
        }

        std::ofstream outFile(assetDataDir / "data0.wdat");

        // Check if the file is open (which also means it was created successfully)
        if (outFile.is_open())
        {
            // Write to the file
            outFile << "Hello, world!\n";
            outFile << "This is a file creation example in C++.\n";

            // Close the file
            outFile.close();

            //W_LOG_INFO("File created and written to successfully!");
        }
        else
        {
            W_LOG_ERROR("Failed to create the file.");
        }
    }
}