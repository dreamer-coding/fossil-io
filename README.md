# **Fossil IO Library by Fossil Logic**

Fossil IO is an extensive and versatile library meticulously crafted to manage input, output, and error handling across a multitude of platforms. This library offers a comprehensive array of functions dedicated to secure data handling, efficient file stream management, and detailed error reporting. Developed in the C programming language, Fossil IO is engineered to support high-performance and reliable I/O operations, ensuring uniform and predictable behavior across various operating systems, including Windows, macOS, and Linux. By leveraging Fossil IO, developers can achieve consistent and secure I/O operations, making it an indispensable tool for cross-platform software development.

## Key Features

1. **Cross-Platform Compatibility**: Provides consistent I/O operations across major operating systems, including Windows, macOS, and Linux.
2. **Secure Input and Output**: Includes functions for secure data handling, preventing common vulnerabilities such as buffer overflows and format string attacks.
3. **Modular Design**: Comprises separate modules for error handling, output, input, file streams, and SOAP functions, allowing for easy integration and customization.
4. **Efficient File Handling**: Supports file stream operations with a focus on secure and reliable file I/O.
5. **Error Reporting**: Includes a comprehensive set of error codes and a lookup function to provide detailed error messages.

## Prerequisites

To get started with Fossil Io, ensure you have the following installed:

- **Meson Build System**: If you don’t have Meson installed, follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).

### Adding Fossil Io Dependency

#### Adding Fossil Io Dependency With Meson

1. **Install Meson Build System**:
   Install Meson version `1.3` or newer:

   ```sh
   python -m pip install meson           # To install Meson
   python -m pip install --upgrade meson # To upgrade Meson
   ```

2. **Create a `.wrap` File**:
   Add the `fossil-io.wrap` file in your `subprojects` directory and include the following content:

   ```ini
   # ======================
   # Git Wrap package definition
   # ======================
   [wrap-git]
   url = https://github.com/fossillogic/fossil-io.git
   revision = v0.2.2

   [provide]
   fossil-io = fossil_io_dep
   ```

3. **Integrate the Dependency**:
   In your `meson.build` file, integrate Fossil Io by adding the following line:

   ```ini
   dep = dependency('fossil-io')
   ```

---

**Note**: For the best experience, always use the latest release of Fossil Io. Visit the [Fossil Io Releases](https://github.com/fossillogic/fossil-io/releases) page for the latest versions.

## Configure Options

Fossil IO offers configurable options to tailor the build process to your needs:

- **Running Tests**: To enable testing, configure the build with `-Dwith_test=enabled`.

Example:

```sh
meson setup builddir -Dwith_test=enabled
```

## Contributing and Support

For contributions, issue reporting, or support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are appreciated.
