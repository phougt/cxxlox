# cxxlox

Based off the book, Crafting Interpreters by Bob Nystrom, cxxlox is an interpreter for the Lox language.

Implementation of the interpreter in the book was implemented using Java, but I decided to implement it using C++ instead.

## Requirements

- CMake (version 3.20 or higher)
- A C++ compiler that supports C++20 (e.g., GCC, Clang, MSVC)

## Project Structure

```
cxxlox/
├── src/
│   ├── CMakeLists.txt
│   ├── includes/
│   │   └── ... (header files)
│   ├── components/
│   │   ├── interpreter.cpp
│   │   ├── parser.cpp
│   │   ├── scanner.cpp
│   │   └── helper.cpp
│   └── main.cpp
└── build/ (to be created)
```

## Building the Project

Follow these steps to build the jlox project:

1. **Clone the Repository**

   First, clone the repository to your local machine:

   ```bash
   git clone https://github.com/phougt/cxxlox.git
   cd cxxlox
   ```

2. **Create a Build Directory**
   
   Create a build directory at the same level as the src folder:

   ```bash
   mkdir build
   cd build
   ```

2. **Configure the Project with CMake**
   
   You can specify a generator when configuring the project. For example, to use the Ninja generator, run:

   ```bash
   cmake -G Ninja ../src
   ```
   If you want to use the default generator (usually Makefiles on Unix-like systems), you can simply run:

   ```bash
   cmake ../src
   ```

   To see a list of available generators, you can run:
   
   ```bash
   cmake --help
   ```

4. **Build the project using:**
   ```bash
   cmake --build .
   ```
5. **Run the executable**
   ```bash
   ./cxxlox
   ```
