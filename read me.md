# CPC-Shell

## Overview

This project is a custom shell implemented using C++ and the Qt framework. It provides a command-line interface with support for tabbed sessions, directory management, and basic shell commands for both Windows and Linux systems.

## Features

- Tabbed terminal interface with up to 10 simultaneous tabs.
- Supports basic shell commands like `cd`, `clear`, `copy`, `date`, `del`, and `move`.
- Custom font rendering using JetBrains Mono.
- Dynamic tab creation and management.
- Scrollable terminal output area.
- Persistent directory tracking per tab.
- Cross-platform support for Windows and Linux.

## Dependencies

- Qt Framework (Qt 5 or later)
- C++ Standard Library

## Installation

### Prerequisites

Ensure you have the following installed:

- Qt Creator or Qt Development Libraries
- C++ Compiler (g++, clang, or MSVC)

### Building the Project

1. Clone the repository:
   ```sh
   git clone <repository_url>
   cd custom_shell
   ```
2. Open the project in Qt Creator or build using the command line:
   ```sh
   qmake && make  # For Linux
   qmake && mingw32-make  # For Windows (MinGW)
   ```
3. Run the application:
   ```sh
   ./custom_shell  # On Linux
   custom_shell.exe  # On Windows
   ```

## Usage

- Launch the shell to open a tabbed terminal interface.
- Use `Ctrl+N` to open a new tab.
- Close tabs using the close button.
- Execute supported shell commands in each tab.

## File Structure

- `main.cpp`: Application entry point.
- `parentwindow.cpp/.h`: Handles tabbed interface and UI layout.
- `responsewindow.cpp/.h`: Manages command execution and shell responses.
- `ui_parentwindow.h`: UI definition for the main window.
- `ui_responsewindow.h`: UI definition for individual terminal windows.

## Roadmap

- Implement additional shell commands (`mkdir`, `rmdir`, `rename`).
- Add support for background processes (`bg`, `fg`).
- Improve UI with color themes and custom shortcuts.
- Enhance command history and autocomplete.

## License

This project is licensed under the MIT License.

## Contributors

- **Lavkush Pal** - Developer 
Developed in March 2025

 

