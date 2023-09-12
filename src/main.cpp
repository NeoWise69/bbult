#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <memory>
#include <variant>
#include <any>
#include <functional>
#include <filesystem>
#include <chrono>
#include <cstdarg>
#include <fstream>

#define C_RESET // color
#define C_BLACK // color
#define C_RED // color
#define C_GREEN // color
#define C_YELLOW // color
#define C_BLUE // color
#define C_MAGENTA // color
#define C_CYAN // color
#define C_WHITE // color
#define C_BOLDBLACK // color
#define C_BOLDRED // color
#define C_BOLDGREEN // color
#define C_BOLDYELLOW // color
#define C_BOLDBLUE // color
#define C_BOLDMAGENTA // color
#define C_BOLDCYAN // color
#define C_BOLDWHITE // color

namespace fs = std::filesystem;
using namespace std::chrono_literals;

fs::path executable_appendix = 
#ifdef _WIN32
    ".exe"
#else
    ".out"
#endif
;

constexpr std::string_view cexpr_minimal_bbult = {
    "\nbuild!\n"
};

constexpr std::string_view cexpr_minimal_cpp = {
    "#include <iostream>\n\n"
    "auto main(int argc, char** argv) -> int {\n"
    "\treturn 0;\n"
    "}\n"
};

fs::path workspace_dir = fs::current_path();
fs::path output_dirname = "bin";
fs::path output_dir = workspace_dir / output_dirname;
fs::path output_name = "a";
fs::path output_path = output_dir / (output_name.string() + executable_appendix.string());
fs::path source_dirname = "src";
fs::path source_dir = workspace_dir / source_dirname;
fs::path include_dirname = "include";
fs::path include_dir = workspace_dir / include_dirname;

fs::path cpp_find_suffix = "cpp";
fs::path cxx_find_suffix = "cxx";
fs::path cc_find_suffix = "cc";
fs::path c_find_suffix = "c";

static std::map<std::string, std::pair<fs::path&, std::string>> variables = {
    { "workspace_dir    ",      { workspace_dir, "current project's working directory" } },
    { "output_dirname   ",     { output_dirname, "name of the output directory where to binary files will be emmited" } },
    { "output_dir       ",         { output_dir, "full path to output directory" } },
    { "output_name      ",        { output_name, "name of the output name (executable or library name)" } },
    { "output_path      ",        { output_path, "full path to output binary file, with extension" } },
    { "source_dirname   ",     { source_dirname, "name of the directory where source files will be searched" } },
    { "source_dir       ",         { source_dir, "full path to source directory" } },
    { "include_dirname  ",    { include_dirname, "name of directory where to search for include files" } },
    { "include_dir      ",        { include_dir, "full path for include directory" } },
    { "cpp_find_suffix  ",    { cpp_find_suffix, "one of extensions for c++ sources" } },
    { "cxx_find_suffix  ",    { cxx_find_suffix, "one of extensions for c++ sources" } },
    { "cc_find_suffix   ",     { cc_find_suffix, "one of extensions for c++ sources" } },
    { "c_find_suffix    ",      { c_find_suffix, "one of extensions for c/c++ sources" } },
};

static void log(const char* fmt, ...) {
    fprintf(stdout, C_CYAN "[ ] LOG: " C_RESET);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);

    fprintf(stdout, "\n");
}

static void warn(const char* fmt, ...) {
    fprintf(stdout, "[ ] WARN: ");

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);

    fprintf(stdout, "\n");
}

static void verbose(const char* fmt, ...) {
    fprintf(stdout, "[ ] VERB: ");

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);

    fprintf(stdout, "\n");
}

void command_build();

auto main(int argc, char** argv) -> int {

    std::vector<std::string> args(argv, argv + argc);
    auto check_bool_flag = [&args](const std::string& f) -> bool { 
        return std::find(args.begin(), args.end(), f) != args.end();
    };

    const auto is_verbose = check_bool_flag("--verbose");
    const auto setup_minimal_cpp = check_bool_flag("create_minimal");
    const auto show_variables = check_bool_flag("show_variables");
    const auto show_variables_short = check_bool_flag("sv");

    if (show_variables_short) {
        log("default variables:");
        for (auto& [var, val] : variables) {
            fprintf(stdout, "%s = %s\n", var.c_str(), val.first.string().c_str());
        }
        return 0;
    }

    if (show_variables) {
        log("default variables:");
        for (auto& [var, val] : variables) {
            fprintf(stdout, "%s: %s\n", var.c_str(), val.second.c_str());
        }
    }

    if (setup_minimal_cpp) {
        fs::create_directory(output_dir);
        fs::create_directory(source_dir);
        fs::create_directory(include_dir);
        std::ofstream("bbult")
            .write(cexpr_minimal_bbult.data(), cexpr_minimal_bbult.size());
        std::ofstream(source_dir / "main.cpp")
            .write(cexpr_minimal_cpp.data(), cexpr_minimal_cpp.size());

        log("minimal c++ project created!");
        return 0;
    }

    if (is_verbose) {
        // print state
        std::cout <<
            "cwd=          " << workspace_dir.string() << std::endl <<
            "output_path   " << output_path.string() << std::endl <<
            "source_dir    " << source_dir.string() << std::endl <<
            "include_dir   " << include_dir.string() << std::endl <<
            std::endl;
    }

    if (not fs::exists(output_dir)) {
        
        if (is_verbose) {
            verbose("creating output directory '%s'..", output_dirname.string().c_str());
        }

        fs::create_directory(output_dir);
    }


    std::ifstream bbult(workspace_dir / "bbult");
    if (bbult.is_open()) {
        std::string line = {};
        auto is_token_found = [&line](const std::string& token) -> bool {
            return line.find(token) != line.npos;
        };
        while (std::getline(bbult, line)) {
            if (line.empty()) continue; // skip whitespace lines

            if (is_token_found("!")) {
                // ok, this is command (all commands ends with an exclamation mark)
                if (is_token_found("build")) {
                    command_build();
                }
                else if (is_token_found("clean")) {
                    if (fs::exists(output_path)) {
                        fs::copy_file(output_path, workspace_dir / ".temp" / (output_name.string() + executable_appendix.string()));
                        fs::create_directory(workspace_dir / ".temp");
                    }
                    fs::remove_all(output_dir);
                    fs::create_directory(output_dir);
                    if (fs::exists(workspace_dir / ".temp")) {
                        fs::copy_file(workspace_dir / ".temp" / (output_name.string() + executable_appendix.string()), output_path);
                    }
                    log("'%s' was cleaned up!", output_dirname.string().c_str());
                }
            }
            else if (is_token_found("@") || is_token_found("set")) {
                // set variable
            }
            else {
                warn("unrecognized token at line:\n\t%s", line.c_str());
            }
        }
    }
    else {
        warn("'bbult' script wasn't found! Building with default settings...");
        command_build();
    }

    return 0;
}

void command_build() {
    log("BUILD!");
}
