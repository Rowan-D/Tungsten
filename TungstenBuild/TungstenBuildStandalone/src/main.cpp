#include <CLI/CLI.hpp>
#include "TungstenBuild/TungstenBuild.hpp"

int main(int argc, char** argv)
{
    CLI::App app{"TungstenBuild"};
    
    std::filesystem::path workspacePath, projectPath, engineDir, intermediateDir, buildDir;
    
    auto workspaceCommand = app.add_subcommand("workspace", "Setup workspace");
    CLI::Option* workspaceOpt = workspaceCommand->add_option("-W,--workspace", workspacePath, "Path to workspace")->required();
    CLI::Option* workspaceProjectOpt = workspaceCommand->add_option("-P,--project", projectPath, "Path to project")->required();

    auto buildCommand = app.add_subcommand("build", "Build project");
    CLI::Option* buildWorkspaceOpt = buildCommand->add_option("-W,--workspace", workspacePath, "Path to workspace");
    CLI::Option* buildProjectOpt = buildCommand->add_option("-P,--project", projectPath, "Path to project");
    CLI::Option* engineOpt = buildCommand->add_option("-E,--engine", engineDir, "Path to engine directory");
    CLI::Option* intOpt = buildCommand->add_option("-I,--intermediate", intermediateDir, "Path to intermediate directory");
    CLI::Option* buildOpt = buildCommand->add_option("-B,--build", buildDir, "final output directory");

    CLI11_PARSE(app, argc, argv);

    wBuild::TungstenBuild tungstenBuild;
    tungstenBuild.SetWorkspacePath(workspacePath);
    tungstenBuild.SetProjectPath(projectPath);
    tungstenBuild.SetEngineDir(engineDir);
    tungstenBuild.SetIntDir(intermediateDir);
    tungstenBuild.SetBuildDir(buildDir);

    if (*workspaceCommand)
    {
        return 0;
    }
    if (*buildCommand)
    {
        return !tungstenBuild.BuildProject();
    }
    std::cout << "TungstenBuildStandalone is a command line application for running TungstenBuild. Usage:\n\n";

    std::cout << "  tungstenbuild workspace -W <workspace-path> -P <project-path>\n";
    std::cout << "  tungstenbuild build -W <workspace-path>\n";
    std::cout << "  tungstenbuild build -P <project-path> -E <engine-dir> -I <int-dir> -B <build-dir>\n\n";

    std::cout << "Run 'tungstenbuild --help' For more information." << std::endl;
    return 0;
}