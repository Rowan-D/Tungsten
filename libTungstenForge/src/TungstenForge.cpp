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

    TungsrenForge::TungsrenForge()
        : errorList()
    {
    }

    bool TungsrenForge::BuildProject(const std::filesystem::path& projectPath, std::filesystem::path outputDir)
    {
        W_LOG_DEBUG("Build called. Asset path: {} Output path: {}", projectPath.string(), outputDir.string());

        std::optional<std::filesystem::path> projectFilePath = GetProjectPath(projectPath);

        if (!projectFilePath.has_value())
        {
            errorList.Log(Severity::Error, ERROR_CODE_TODO, "Project File Not Found Error: Tungsten Project File (.wproj) not found!");
            return false;
        }

        // TODO Check if .parent_path() can fail
        std::filesystem::path projectDirectoryPath = projectFilePath.value().parent_path();

        YAML::Node projectFile = YAML::LoadFile(projectFilePath.value().string());

        std::string projectName = projectFile["projectName"].as<std::string>();

        if (projectName != projectFilePath.value().stem().string())
        {
            errorList.Log(Severity::Warning, ERROR_CODE_TODO, fmt::format("Naming mismatch Warning: Project file name \"{}\" should match project name \"{}\"", projectFilePath.value().stem().string(), projectName));
        }

        if (projectName != projectDirectoryPath.filename().string())
        {
            errorList.Log(Severity::Warning, ERROR_CODE_TODO, fmt::format("Naming mismatch Warning: Project directory name \"{}\" should match project name \"{}\"", projectFilePath.value().parent_path().filename().string(), projectName));
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

    bool TungsrenForge::IsValedProjectPath(const std::filesystem::path& projectPath, std::string& errorMessage)
    {
        return true;
    }

    bool TungsrenForge::IsValedOutputPath(const std::filesystem::path& outputDir, std::string& errorMessage)
    {
        return true;
    }

    std::optional<std::filesystem::path> TungsrenForge::GetProjectPath(std::filesystem::path inputPath)
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
                    errorList.Log(Severity::Error, ERROR_CODE_TODO, "Locating Project File Invalid Path Error: The provided path is neither a project file or directory!");
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
                    errorList.Log(Severity::Error, ERROR_CODE_TODO, "Project File Not Found Error: Tungsten Project File (.wproj) not found!");
                    return {};
                }
                else if (wProjFiles.size() > 1)
                {
                    std::string wProjFilesList;
                    for (std::filesystem::path wProjFile : wProjFiles)
                    {
                        wProjFilesList += '\n' + wProjFile.filename().string();
                    }
                    errorList.Log(Severity::Error, ERROR_CODE_TODO, fmt::format("Multiple Project File Error: There should be no more than one Tungsten Project File (.wproj) in the project directory. {} were found!{}", wProjFiles.size(), wProjFilesList));
                    return {};
                }
                else
                {
                    return wProjFiles.front();
                }
            }
            else
            {
                errorList.Log(Severity::Error, ERROR_CODE_TODO, "Locating Project File Invalid Path Error: project file or directory dose not exist!");
                return {};
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            errorList.Log(Severity::Error, ERROR_CODE_TODO, fmt::format("Locating Project File Filesystem Error: {}", e.what()));
            return {};
        }
        catch (const std::exception& e)
        {
            errorList.Log(Severity::Error, ERROR_CODE_TODO, fmt::format("Locating Project File General Error: {}", e.what()));
            return {};
        }
    }

    std::optional<std::vector<uint8_t>> TungsrenForge::SceneToBytes(const std::filesystem::path& scenePath)
    {
        W_LOG_INFO("{}", scenePath.string());
        std::ifstream sceneFile(scenePath);

        if (!sceneFile.is_open())
        {

            return {};
        }

        std::string line;
        while (std::getline(sceneFile, line))
        {
            W_LOG_INFO_NO_LOCATION(line);
        }

        sceneFile.close();
        return std::vector<uint8_t>();
    }
}