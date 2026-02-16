# uog-cpp

**Learn C++ at University of Glasgow**

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++](https://img.shields.io/badge/C++-11%2F14%2F17-00599C.svg)](https://isocpp.org/)

This repository contains comprehensive C++ learning materials developed for the University of Glasgow. The content is structured as a progressive course covering C++ fundamentals, intermediate concepts, and object-oriented programming principles.

**Acknowledgment**: This repository is developed by heavily using [www.learncpp.com](https://www.learncpp.com) as a primary reference. However, the repo does not try to compile the website into a single book.

## Table of Contents
- [Repository Structure](#repository-structure)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Course Contents](#course-contents)
  - [Section 01: C++ Basics](#section-01-c-basics)
  - [Section 02: C++ Intermediate](#section-02-c-intermediate)
- [Assignments](#assignments)
- [How to Use This Repository](#how-to-use-this-repository)
- [Contributing](#contributing)
- [License](#license)
- [Author](#author)

## Repository Structure
The repository is divided into the following sections: 

1. **Section 01**: C++ Basics - Foundational concepts
2. **Section 02**: C++ Intermediate - Advanced fundamental topics

Each section corresponds to one folder and contains multiple chapters covering various topics in C++ programming. Each chapter in a specific section is numbered sequentially. 

**Note**: Chapter numbers in Section 02 continue from the last chapter number in Section 01.

Each chapter typically includes:
- `.qmd` files - Quarto markdown source files for presentations
- `.html` files - Rendered HTML presentations
- `cpp/` directory - C++ example code and demonstrations
- Supporting files and resources

## Prerequisites

Before starting with this course material, you should have:

- **Basic programming knowledge**: Familiarity with programming concepts (variables, functions, loops)
- **C++ compiler**: GCC, Clang, or MSVC
  - GCC 7.0+ or Clang 5.0+ recommended for C++17 support
  - MSVC 2017+ on Windows
- **Text editor or IDE**: VS Code, Visual Studio, CLion, or any C++ compatible editor
- **Quarto (optional)**: For rendering presentation files
  - Install from [quarto.org](https://quarto.org)

### Installing C++ Compiler

**Linux/Ubuntu:**
```bash
sudo apt-get update
sudo apt-get install build-essential
```

**macOS:**
```bash
xcode-select --install
```

**Windows:**
- Install [MinGW-w64](https://www.mingw-w64.org/) or
- Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/)

## Getting Started

1. **Clone the repository**:
   ```bash
   git clone https://github.com/RishonGT/uog-cpp.git
   cd uog-cpp
   ```

2. **Navigate to a chapter**:
   ```bash
   cd sec-01-cpp-basics/01-structure-and-varaibles
   ```

3. **Compile and run examples**:
   ```bash
   cd cpp
   g++ -std=c++17 example_file.cpp -o example
   ./example
   ```

4. **View presentations**:
   - Open `.html` files directly in your web browser
   - Or render from `.qmd` files using Quarto:
     ```bash
     quarto render chapter_file.qmd
     ```

## Course Contents

### Educational Philosophy

**REMARK**  
It can be debatable whether the contents in the two sections "C++ Basics" and "C++ Intermediate" should be considered separate, as they are all foundational in C++. However, for educational purposes, the content is separated for two key reasons:

- **Easier learning progression**: This structure helps students follow the material systematically and allows the course Moodle to reflect the GitHub materials clearly.
- **Language comparison perspective**: The content in "C++ Intermediate" covers sophisticated topics such as pointers, references, pass by value, pass by reference, pass by address, lvalue, and rvalue. These concepts are not directly relevant to Python programming (taught in Year 2), though Python has similar concepts like "pass an immutable object" (similar to pass by value) and "pass a mutable object" (similar to pass by reference).

## Section 01: C++ Basics

- **Chapter 00** &mdash; Folder `00-intro-to-eng3091`
    - Welcome to ENG3091, course materials, marking system and how the course is run.
- **Chapter 01** &mdash; Folder `01-structure-and-variables`
    - How a basic C++ program source code looks like. 
    - Declare and initialize variables in C++. 
    -  Input and output values in C++ using `iostream`
- **Chapter 02** &mdash; Folder `02-functions-and-files`
    - User-defined functions: basic syntax and use
    - Function parameters & arguments
    - Forward declaration
    - Working with multiple files
    - Header files
- **Chapter 03** &mdash; Folder `03-control-flow`
    - `if`, `if-else` statement
    - `switch` statement
    - `for` loop
    - `while` loop
    - `do-while` loop
    - `break` and `continue` statements
- **Chapter 04** &mdash; Folder `04-constants-and-strings`
    - Define constant parameters in functions
    - Data type `std::string`

## Section 02: C++ Intermediate
- **Chapter 05** &mdash; Folder `05-function-overloading`
  - Quick intro to function overloading
  - Function overload resolution & ambigous matches
  - Default arguments
- **Chapter 06** &mdash; Folder `06-function-template`
  - Function templates
  - Function template instantiation
  - Function templates with multiple template types
- **Chapter 07** &mdash; Folder `07-compound-types-references-pointers`
  - Quick intro to compound data types
  - Value categories: **lvalues** and **rvalues**
  - lvalue references, lvalue references to const
  - pass by lvalue reference (pass by reference)
  - pass by const lvalue reference
  - Intro to pointers
  - `null` pointers
  - Pass by address

## Assignments

The `assignments/` directory contains course assignments and related materials. Each assignment includes:
- Problem descriptions
- Starter code (where applicable)
- Test cases or requirements

## How to Use This Repository

### For Students

1. **Follow sequentially**: Start with Section 01, Chapter 00 and progress through the chapters in order
2. **Practice with examples**: Each chapter includes code examples in the `cpp/` directory
3. **Compile and experiment**: Modify the example code to reinforce your understanding
4. **Review presentations**: Use the HTML presentations as lecture notes
5. **Complete assignments**: Work through assignments to apply what you've learned

### For Instructors

- The `.qmd` (Quarto) files are the source material that can be edited and re-rendered
- Each chapter is self-contained with examples and presentations
- Materials follow a progressive learning path suitable for semester-long courses
- Assignments can be customized based on course requirements

### Compiling Examples

Most examples can be compiled with:
```bash
g++ -std=c++17 -Wall -Wextra filename.cpp -o output
./output
```

Some examples may require additional flags or libraries. Check comments in the source files for specific requirements.

## Contributing

Contributions are welcome! If you find errors, have suggestions, or want to add content:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Make your changes
4. Commit your changes (`git commit -am 'Add improvement'`)
5. Push to the branch (`git push origin feature/improvement`)
6. Open a Pull Request

### Contribution Guidelines

- Follow the existing code style and organization
- Test all code examples before submitting
- Update documentation to reflect any changes
- Ensure Quarto files render correctly

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Author

**Khiem Nguyen**  
University of Glasgow

---

## Roadmap

**Planned Future Sections** (work in progress):

- **Section 03**: Object-oriented Programming in C++
  - Chapters 10-14: Classes, inheritance, polymorphism, operator overloading, and virtual functions
- **Section 04**: UML Class Diagrams
  - Visual modeling and design patterns

*Note: Sections 03 and 04 are mentioned in the roadmap but are not yet available in the repository.*

---

**Last Updated**: February 2026

For questions or feedback, please open an issue on GitHub.
