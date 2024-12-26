
# Basic Compiler from Scratch

This project is a simple **compiler** written in C++ from scratch, designed to process basic programming language constructs. The goal is to provide a foundational understanding of how a compiler works, focusing on the **lexical analysis** phase, including **tokenization**, **parsing**, and future stages such as **semantic analysis** and **code generation**.

Currently, the project focuses on recognizing core programming constructs like **function definitions**, **function calls**, and basic expressions. Future development will include advanced features such as **error handling**, **control flow structures**, and **code generation**.

This project is a work in progress and aims to evolve into a simple, extensible compiler for a small programming language.


## Features (Work in Progress)

- Tokenizes basic components of a programming language, including **keywords**, **identifiers**, **numbers**, and **operators**.
- Supports **function definitions** and **function calls** with basic syntax.
- Recognizes **tokens** for common language constructs, such as **functions**, **variables**, **operators**, and **parentheses**.
- Plans for implementing **function declarations** and **scope management**.
- **Future Plans**:
    - Implement **parsing** to handle function definitions, function calls, and variable declarations.
    - Add support for **control flow** statements (like `if`, `else`, `while`).
    - Develop **error handling** for invalid syntax and runtime errors.
    - Implement **semantic analysis** for type checking, scope resolution, and function call validation.
    - **Code generation** for an intermediate or target machine language.


## Technologies Used

- **C++17**: The primary programming language used to implement the compiler.
- **CMake**: Build system to configure and compile the project.
- **CLion**: The Integrated Development Environment (IDE) used for development.


## How to Set Up and Run

### Prerequisites

- **C++17 compatible compiler** (e.g., GCC, Clang).
- **CMake**: Build system to configure the project.


### Setup Instructions

#### 1. **Clone the repository**

Open a terminal (or command prompt) and run the following command to clone the repository:
```bash
git clone https://github.com/Satyam709/Compiler.git
cd Compiler
```

#### 2. **Create a build directory** (optional but recommended)

It's a good practice to build the project in a separate directory:
```bash
mkdir build
cd build
```

#### 3. **Run CMake to configure the project**

From within the `build` directory, run CMake to generate the necessary makefiles or project files for your chosen build system:
```bash
cmake ..
```

#### 4. **Build the project**

Now, build the project using CMake. This command will compile the source files and produce the executable:
```bash
cmake --build .
```

Alternatively, if you're using **Ninja** (if it's installed), you can build with:
```bash
cmake --build . -- -jN
```
where `N` is the number of CPU cores you want to use for the build process.

#### 5. **Run the compiled project**

After building, the executable will be located in the `build` directory. You can run the program directly from the terminal:
```bash
./Compiler
```

On **Windows**, if you're using a `cmd` or PowerShell terminal, use:
```bash
Compiler.exe
```

This will run the compiler, which will tokenize a simple arithmetic expression and print out the resulting tokens.


---

### IDE Setup Instructions

#### **Setting Up in Visual Studio Code (VS Code)**

1. **Install VS Code**: If you don’t have VS Code, [download it here](https://code.visualstudio.com/).

2. **Install C++ Extension**:
   - Open VS Code and go to the **Extensions** tab (or press `Ctrl+Shift+X`).
   - Search for the **C++ extension** by Microsoft and install it.

3. **Install CMake Tools Extension**:
   - In the Extensions tab, search for **CMake Tools** and install it. This will help with building CMake projects in VS Code.

4. **Open the project**:
   - Open the project folder in VS Code by going to **File → Open Folder** and selecting the project directory.

5. **Configure CMake**:
   - VS Code should automatically detect CMake in the project and ask you to configure the project. Click on **Configure**.
   - If not, open the **Command Palette** (`Ctrl+Shift+P`), and type `CMake: Configure`. Select it.

6. **Build the project**:
   - Once the configuration is complete, press **F7** or go to **Terminal → Run Build Task** to build the project.

7. **Run the project**:
   - To run the project, you can use the **Run** option in the **Debug** panel, or open the terminal in VS Code and run:
     ```bash
     ./Compiler
     ```


---

#### **Setting Up in CLion**

1. **Install CLion**: If you don’t have CLion, [download it here](https://www.jetbrains.com/clion/).

2. **Open the project**:
   - Open CLion and select **Open** from the welcome screen.
   - Navigate to your project directory and click **OK**.

3. **CMake Configuration**:
   - CLion automatically detects the CMake configuration in your project. It will ask you to reload the project configuration. Click **Reload** to configure the project.

4. **Build the project**:
   - After the project is configured, click the **Build** button (the hammer icon) in the top right, or use **Ctrl+F9** to build the project.

5. **Run the project**:
   - To run the project, click on the green **Run** button or use **Shift+F10**.


---

### Notes

- **VS Code** requires the installation of extensions for C++ and CMake, while **CLion** has built-in support for CMake and C++ development.
- Make sure that your compiler (e.g., GCC, Clang, or MSVC) is correctly set up in your system’s PATH or through the IDE's settings.
- The project is set up using **CMake**, so it should work seamlessly across multiple platforms (Windows, macOS, Linux) as long as CMake is properly configured.


## How to Contribute

We welcome contributions to further develop this compiler project! To contribute:

1. **Fork** the repository on GitHub.
2. **Clone** your forked repository to your local machine.
3. **Create a new branch** to make your changes.
4. **Make your changes** and test them to ensure they work.
5. **Submit a pull request** with a detailed description of your changes.

We are especially interested in contributions for adding **parsing**, **error handling**, and **code generation**.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for more details.

