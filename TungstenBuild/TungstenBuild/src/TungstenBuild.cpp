#include <array>
#include <span>
#include <cstdlib>

#include <ryml.hpp>
#include <ryml_std.hpp>

#include <csv.hpp>
#include <string_view>

#include "TungstenBuild/TungstenBuild.hpp"
#include "TungstenUtils/ReadFile.hpp"
#include "TungstenUtils/WriteFile.hpp"
#include "TungstenUtils/FindAndReplace.hpp"
#include "TungstenUtils/platform/ExecutablePath.hpp"
#include "TungstenBuild/Config.hpp"

namespace wBuild
{
    static constexpr std::size_t WorkspacePathVarIndex = static_cast<std::size_t>(Var::WorkspacePath);
    static constexpr std::size_t ProjectPathVarIndex = static_cast<std::size_t>(Var::ProjectPath);
    static constexpr std::size_t EngineDirVarIndex = static_cast<std::size_t>(Var::EngineDir);
    static constexpr std::size_t IntDirVarIndex = static_cast<std::size_t>(Var::IntDir);
    static constexpr std::size_t BuildDirVarIndex = static_cast<std::size_t>(Var::BuildDir);
    static constexpr std::size_t VarCount = static_cast<std::size_t>(Var::COUNT);

    static bool RenderTemplateFile(const std::filesystem::path& templateFile, const std::filesystem::path& outputPath, std::span<const std::pair<std::string_view, std::string_view>> replacements)
    {
        std::optional<std::string> templateStr = wUtils::ReadFile(templateFile);
        if (!templateStr)
        {
            return false;
        }
        for (const auto& [from, to] : replacements)
        {
            wUtils::FindAndReplaceAll(*templateStr, from, to);
        }
        return wUtils::WriteFile(outputPath, *templateStr);
    }

    TungstenBuild::TungstenBuild()
        : errorList(), m_executableDir()
    {
    }


    std::filesystem::path TungstenBuild::GetProjectFilePath(const std::filesystem::path& inputPath)
    {
        namespace fs = std::filesystem;

        try
        {
            if (fs::exists(inputPath))
            {
                if (fs::is_regular_file(inputPath))
                {
                    return inputPath;
                }

                if (!fs::is_directory(inputPath))
                {
                    W_LOG_ERROR(errorList, "Locating Project File Invalid Path Error: The provided path is neither a project file or directory!");
                    return {};
                }

                std::vector<fs::path> wProjFiles;
                for (const auto& entry : fs::directory_iterator(inputPath))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".wproj")
                    {
                        wProjFiles.push_back(entry.path());
                    }
                }

                if (wProjFiles.empty())
                {
                    W_LOG_ERROR(errorList, "Project File Not Found Error: Tungsten Project File (.wproj) not found!");
                    return {};
                }
                else if (wProjFiles.size() > 1)
                {
                    std::string wProjFilesList;
                    for (fs::path wProjFile : wProjFiles)
                    {
                        wProjFilesList += '\n' + wProjFile.filename().string();
                    }
                    W_LOG_ERROR(errorList, "Multiple Project File Error: There should be no more than one Tungsten Project File (.wproj) in the project directory. {} were found!{}", wProjFiles.size(), wProjFilesList);
                    return {};
                }
                else
                {
                    return wProjFiles.front();
                }
            }
            else
            {
                W_LOG_ERROR(errorList, "Locating Project File Invalid Path Error: project file or directory dose not exist!");
                return {};
            }
        }
        catch (const fs::filesystem_error& e)
        {
            W_LOG_ERROR(errorList, "Locating Project File Filesystem Error: {}", e.what());
            return {};
        }
        catch (const std::exception& e)
        {
            W_LOG_ERROR(errorList, "Locating Project File General Error: {}", e.what());
            return {};
        }
    }

    bool TungstenBuild::SetupWorkspace()
    {
        return false;
    }

    bool TungstenBuild::BuildProject()
    {
        namespace fs = std::filesystem;

        W_LOG_INFO(errorList, "Build called.");

        const fs::path tungstenCoreSourceDir = m_vars[EngineDirVarIndex] / "TungstenCore";
        const fs::path tungstenRuntimeSourceDir = m_vars[EngineDirVarIndex] / "TungstenRuntime";

        W_LOG_INFO(errorList, "projectPath: {}", m_vars[ProjectPathVarIndex].string());
        W_LOG_INFO(errorList, "wCoreSourceDir: {}", tungstenCoreSourceDir.string());
        W_LOG_INFO(errorList, "wRuntimeSourceDir: {}", tungstenRuntimeSourceDir.string());
        W_LOG_INFO(errorList, "intDir: {}", m_vars[IntDirVarIndex].string());
        W_LOG_INFO(errorList, "buildDir: {}", m_vars[BuildDirVarIndex].string());

        try
        {
            const fs::path projectFilePath = GetProjectFilePath(m_vars[ProjectPathVarIndex]);
            if (projectFilePath.empty())
            {
                W_LOG_ERROR(errorList, "Could not get project file path!");
                return false;
            }
            W_LOG_INFO(errorList, "projectFilePath: {}", projectFilePath.string());

            std::optional<std::string> projectFile = wUtils::ReadFile(projectFilePath, errorList);
            if (!projectFile)
            {
                W_LOG_ERROR(errorList, "Could not read project file!");
                return false;
            }
            std::string yamlStr = std::move(*projectFile);
            ryml::Tree tree = ryml::parse_in_place(ryml::to_substr(yamlStr));
            ryml::ConstNodeRef root = tree.crootref();
            
            if (!root.has_child("targetName"))
            {
                W_LOG_ERROR(errorList, "{} has no \"targetName\"", projectFilePath.string());
                return false;
            }

            const c4::csubstr targetNameVal = root["targetName"].val();
            const std::string_view targetName(targetNameVal.str, targetNameVal.len);
            const fs::path projectDir = projectFilePath.parent_path();

            std::string componentList;
            std::string componentDeclarations;
            std::string componentIncludes;

            bool first = true;
            std::string lastNamespace;
            ryml::ConstNodeRef componentListsSeq = root["componentLists"];
            for (std::size_t componentListIndex = 0; componentListIndex < componentListsSeq.num_children(); ++componentListIndex)
            {
                const c4::csubstr componentListRelitivePathVal = componentListsSeq[componentListIndex].val();
                const std::string_view componentListRelitivePathStrView(componentListRelitivePathVal.str, componentListRelitivePathVal.len);
                const fs::path componentListPath = projectDir / fs::path(componentListRelitivePathStrView);

                W_LOG_INFO(errorList, "componentListPath: {}", componentListPath.string());

                std::ifstream componentListFile(componentListPath);
                if (!componentListFile)
                {
                    W_LOG_ERROR(errorList, "Failed to open componentListFile: {}", componentListPath.string());
                    return false;
                }

                W_LOG_INFO(errorList, "Reading componentListFile");
                csv::CSVReader reader(componentListFile);
                const bool hasPageSize = reader.get_col_names().count("PageSize") > 0;
                
                for (csv::CSVRow& row : reader) {
                    std::string namespaceAndTypeName = row["TypeName"].get<std::string>();
                    if (!first)
                    {
                        componentList += ", ";
                    }
                    componentList += namespaceAndTypeName;
                    const std::size_t pos = namespaceAndTypeName.rfind("::");
                    const std::string declaration = row["Declaration"].get<std::string>();
                    if (pos == std::string_view::npos) // no namespace
                    {
                        if (!lastNamespace.empty())
                        {
                            componentDeclarations += "}\n";
                        }
                        componentDeclarations += declaration + ' ' + namespaceAndTypeName;
                        lastNamespace.clear();
                    }
                    else // namespace
                    {
                        const std::string_view qualifiedTypeName = namespaceAndTypeName;
                        const std::string_view ns = qualifiedTypeName.substr(0, pos);
                        if (ns != lastNamespace)
                        {
                            if (!lastNamespace.empty())
                            {
                                componentDeclarations += "}\n";
                            }
                            componentDeclarations += fmt::format(fmt::runtime("namespace {}\n"), ns);
                            componentDeclarations += "{\n";
                            namespaceAndTypeName.resize(pos);
                            lastNamespace = std::move(namespaceAndTypeName);
                        }
                        componentDeclarations += fmt::format(fmt::runtime("    {} {};\n"), declaration, qualifiedTypeName.substr(pos + 2));
                    }

                    componentIncludes += "#include \"";
                    componentIncludes += row["Include"].get<std::string>();
                    componentIncludes += "\"\n";

                    first = false;
                }
                W_LOG_INFO(errorList, "Read componentListFile");
            }
            if (!lastNamespace.empty())
            {
                componentDeclarations += "}\n";
            }
            
            std::string componentSettingsIncludes;
            ryml::ConstNodeRef componentSettingsIncludesSeq = root["componentSettingsIncludes"];
            for (std::size_t componentSettingsIncludeIndex = 0; componentSettingsIncludeIndex < componentSettingsIncludesSeq.num_children(); ++componentSettingsIncludeIndex)
            {
                const c4::csubstr componentSettingsIncludeVal = componentSettingsIncludesSeq[componentSettingsIncludeIndex].val();
                const std::string_view componentSettingsIncludeStrView(componentSettingsIncludeVal.str, componentSettingsIncludeVal.len);
                
                componentSettingsIncludes += "#include \"";
                componentSettingsIncludes += componentSettingsIncludeStrView;
                componentSettingsIncludes += "\"\n";
            }

            const std::string projectName = projectFilePath.stem().string();
            W_LOG_INFO(errorList, "projectName: {}", projectName);
            const std::string_view executableName = projectName;

            if (!ExecutableDir())
            {
                W_LOG_ERROR(errorList, "Could not get Executable Path");
            }

            const fs::path tungstenBuildResDir = m_executableDir / TUNGSTENBUILD_RESOURCE_PATH;

            const std::string tungstenCoreSourceDirStr = fs::weakly_canonical(tungstenCoreSourceDir).string();
            const std::string tungstenRuntimeSourceDirStr = fs::weakly_canonical(tungstenRuntimeSourceDir).string();
            const std::string projectDirStr = fs::weakly_canonical(projectDir).string();

            const std::array<std::pair<std::string_view, std::string_view>, 5> wRuntimeWorkspaceCMakeListsReplacements = {{
                { "@TUNGSTEN_CORE_SOURCE_DIR@", tungstenCoreSourceDirStr },
                { "@TUNGSTEN_PROJECT_SOURCE_DIR@", projectDirStr },
                { "@TUNGSTEN_PROJECT_TARGET_NAME@", targetName },
                { "@TUNGSTEN_RUNTIME_EXECUTABLE_NAME@", executableName },
                { "@TUNGSTEN_RUNTIME_SOURCE_DIR@", tungstenRuntimeSourceDirStr }
            }};

            const std::array<std::pair<std::string_view, std::string_view>, 3> componentDeclarationsReplacements = {{
                { "@COMPONENT_DECLORATIONS@", componentDeclarations },
                { "@COMPONENT_LIST@", componentList },
                { "@PAGED_COMPONENT_COUNT@", pagedComponentCount }
            }};

            const std::array<std::pair<std::string_view, std::string_view>, 1> componentIncludesReplacements = {{
                { "@COMPONENT_INCLUDES@", componentIncludes }
            }};

            RenderTemplateFile(tungstenBuildResDir / "wRuntimeWorkspace.CMakeLists.txt.in", m_vars[IntDirVarIndex] / "CMakeLists.txt", wRuntimeWorkspaceCMakeListsReplacements);
            fs::create_directory(m_vars[IntDirVarIndex] / "include");
            fs::create_directory(m_vars[IntDirVarIndex] / "include/generated");
            RenderTemplateFile(tungstenBuildResDir / "componentDeclarations.hpp.in", m_vars[IntDirVarIndex] / "include/generated/componentDeclarations.hpp", componentDeclarationsReplacements);
            RenderTemplateFile(tungstenBuildResDir / "componentIncludes.hpp.in", m_vars[IntDirVarIndex] / "include/generated/componentIncludes.hpp", componentIncludesReplacements);

            const fs::path buildDir = fs::absolute(m_vars[IntDirVarIndex] / "build");
            fs::create_directory(buildDir);
            const std::string buildDirStr = buildDir.string();

            W_LOG_INFO(errorList, "Configuring CMake project");
            if (std::system(fmt::format("cmake -S \"{}\" -B \"{}\"", fs::absolute(m_vars[IntDirVarIndex]).string(), buildDirStr).c_str()))
            {
                W_LOG_ERROR(errorList, "CMake configuration failed.");
                return false;
            }

            W_LOG_INFO(errorList, "Building project");
            if (std::system(fmt::format("cmake --build \"{}\"", buildDirStr).c_str()))
            {
                W_LOG_ERROR(errorList, "Build failed.");
                return false;
            }

            fs::create_directory(m_vars[BuildDirVarIndex]);
            fs::copy_file(buildDir / "TungstenRuntime" / executableName, m_vars[BuildDirVarIndex] / executableName, fs::copy_options::overwrite_existing);
            W_LOG_INFO(errorList, "Copied final binary to distribution folder.");

            return true;
        }
        catch (const fs::filesystem_error& e)
        {
            W_LOG_ERROR(errorList, "Building Project Filesystem Error: {}", e.what());
            return false;
        }
        catch (const std::exception& e)
        {
            W_LOG_ERROR(errorList, "Building Project General Error: {}", e.what());
            return false;
        }
    }

    bool TungstenBuild::ExecutableDir()
    {
        if (!m_executableDir.empty())
        {
            return true;
        }
        m_executableDir = wUtils::platform::ExecutableDir(errorList);
        return !m_executableDir.empty();
    }
}