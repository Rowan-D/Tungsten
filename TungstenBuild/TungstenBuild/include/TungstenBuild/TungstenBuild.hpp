#ifndef TUNGSTEN_BUILD_TUNGSTEN_BUILD_HPP
#define TUNGSTEN_BUILD_TUNGSTEN_BUILD_HPP

#include "TungstenUtils/TungstenUtils.hpp"
#include <filesystem>

namespace wBuild
{
    enum class Var
    {
        WorkspacePath,
        ProjectPath,
        EngineDir,
        IntDir,
        BuildDir,
        COUNT
    };

    class TungstenBuild
    {
    public:
        TungstenBuild();

        [[nodiscard]] inline bool GetIsVarSet(Var type) const noexcept { W_ASSERT(type != Var::COUNT, "Var type cannot be set to Var::COUNT!"); return !m_vars[static_cast<std::size_t>(type)].empty(); }
        [[nodiscard]] inline const std::filesystem::path GetVar(Var type) const { W_ASSERT(type != Var::COUNT, "Var type cannot be set to Var::COUNT"); return m_vars[static_cast<std::size_t>(type)]; }
        inline void SetVar(Var type, const std::filesystem::path& value) { W_ASSERT(type != Var::COUNT, "Var type cannot be set to Var::COUNT"); m_vars[static_cast<std::size_t>(type)] = value; }
        inline void ClearVar(Var type) noexcept { W_ASSERT(type != Var::COUNT, "Var type cannot be set to Var::COUNT!"); m_vars[static_cast<std::size_t>(type)].clear(); }

        inline bool GetIsWorkspacePathSet() const { return GetIsVarSet(Var::WorkspacePath); }
        inline std::filesystem::path GetWorkspacePath() const { return GetVar(Var::WorkspacePath); }
        inline void SetWorkspacePath(const std::filesystem::path& workspacePath) { SetVar(Var::WorkspacePath, workspacePath); }
        inline void ClearWorkspacePath() { ClearVar(Var::WorkspacePath); }

        inline bool GetIsProjectPathSet() const { return GetIsVarSet(Var::ProjectPath); }
        inline std::filesystem::path GetProjectPath() const { return GetVar(Var::ProjectPath); }
        inline void SetProjectPath(const std::filesystem::path& projectPath) { SetVar(Var::ProjectPath, projectPath); }
        inline void ClearProjectPath() { ClearVar(Var::ProjectPath); }

        inline bool GetIsEngineDirSet() const { return GetIsVarSet(Var::EngineDir); }
        inline std::filesystem::path GetEngineDir() const { return GetVar(Var::EngineDir); }
        inline void SetEngineDir(const std::filesystem::path& engineDir) { SetVar(Var::EngineDir, engineDir); }
        inline void ClearEngineDir() { ClearVar(Var::EngineDir); }

        inline bool GetIsIntDirSet() const { return GetIsVarSet(Var::IntDir); }
        inline std::filesystem::path GetIntDir() const { return GetVar(Var::IntDir); }
        inline void SetIntDir(const std::filesystem::path& intDir) { SetVar(Var::IntDir, intDir); }
        inline void ClearIntDir() { ClearVar(Var::IntDir); }

        inline bool GetIsBuildDirSet() const { return GetIsVarSet(Var::BuildDir); }
        inline std::filesystem::path GetBuildDir() const { return GetVar(Var::BuildDir); }
        inline void SetBuildDir(const std::filesystem::path& buildDir) { SetVar(Var::BuildDir, buildDir); }
        inline void ClearBuildDir() { ClearVar(Var::BuildDir); }

        [[nodiscard]] std::filesystem::path GetProjectFilePath(const std::filesystem::path& inputPath);
        bool SetupWorkspace();
        bool BuildProject();

        wUtils::TungstenLogger errorList;

    private:
        [[nodiscard]] bool ExecutableDir();
        std::filesystem::path m_executableDir;
        std::filesystem::path m_vars[static_cast<std::size_t>(Var::COUNT)]{};
    };
}

#endif