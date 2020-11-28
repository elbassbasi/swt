## SWT: The Standard Widget Toolkit
SWT is an open source widget toolkit for C/C++ designed to provide efficient, portable access to the user-interface facilities of the operating systems on which it is implemented.

https://elbassbasi.github.io/swt/index.html

## Usage
```c++
#include <swt.h>
int main() {
	WApp app;
	WShell shell;
	shell.Create(W_SHELL_TRIM | W_DISPOSE_ON_CLOSE);
	shell.Open();
	return app.Run();
}
```

## Building swt on Linux
To build swt on your computer you will need these packages:

 - The GTK development packages: often called libgtk-3-dev or gtk3-devel, or similar
 - pkg-config (which usually comes with the GTK dev package)
 - The build-essential package (or the relevant bit of it: G++, Make etc)
 - CMake
 - Git.

On Ubuntu / Debian you can install all of the above by typing:

    sudo apt-get install libgtk-3-dev pkg-config build-essential git cmake

Git clone the sources:

    git clone https://github.com/elbassbasi/swt.git

Run CMake and build swt:

  ```bash
  cd swt
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j 4
  sudo cmake --build . --target install
  ```
## Building swt on Windows
To build swt on your computer you will need these packages:

 - The build-essential package (or the relevant bit of it: G++, Make etc)
 - CMake
 - Git.

Git clone the sources:

    git clone https://github.com/elbassbasi/swt.git
    
Run CMake and build swt:

  ```bash
  cd swt
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j 4
  ```
## Building swt on Mac OSX
To build swt on your computer you will need these packages:

 - The build-essential package (or the relevant bit of it: G++, Make etc)
 - CMake
 - Git.

Git clone the sources:

    git clone https://github.com/elbassbasi/swt.git
    
Run CMake and build swt:

  ```bash
  cd swt
  mkdir build-release
  cd build-release
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j 4
  ```