# LinkedList Generic

Generic singly linked list library written in C.

This project implements a generic linked list using `void *` and callback functions to manage different types of data.

## Technologies

* C23
* CMake
* Ninja
* MSVC
* Visual Studio Code

## Features

* Generic data storage using `void *`
* Dynamic memory management
* Insertion at the front, back, and before a specific ID
* ID-based element identification
* Callback functions for copying, printing, and destroying data
* List integrity validation
* Debug and Release configurations

## Building

### Debug

Configure the project:

```bash
cmake --preset debug
```

Build the project:

```bash
cmake --build --preset debug
```

### Release

Configure the project:

```bash
cmake --preset release
```

Build the project:

```bash
cmake --build --preset release
```

The generated library can be found in the corresponding `build` directory.

## Project Structure

```text
LinkedList_Generic/
├── .github/
│   └── workflows/
├── .vscode/
├── include/
├── src/
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
├── README.md
└── .gitignore
```

## Status

The project is currently under development. Testing is being implemented to verify the behavior and error handling of the library.

## License

This project is licensed under the MIT License.
