# Simple Robot(s) Package Manager (srpm)
Software projects are built on top of dependent projects. In addition good software design suggests building software in modules that are easily testable. The Simple Robots Package Manager is a lightweight and distributed package manager to bring these dependencies into the build.

srpm is designed to be lightweight by making it build agnostic. The build system should call the package manager to import the desired dependencies. A config file specifies which dependencies to import and where to put them in the build. In order to handle dependencies in a distributed way, packages are identified by a url to a configuration file for the dependent package. Packages are also given a name. When a local package exists, it's artifacts are overwritten by this local build.

To remain lightweight, srpm does not provide many of the features provided by other package managers such as Maven, Gradle, Conan.io. srpm does not integrate with any build systems, manage artifacts directly, support multiple variations on a build, manage multiple versions of a component, or handle recursive dependencies. In addition, it only runs on linux.

## Project Configuration
Project configuration files are written in json. (non-optimal, but flexible enough for future development). An example configuration file that will import jsoncpp and libcurl packages into the build directory build/dependencies.

```
{
  "release": "build/release",
  "dependencies": [
    {
      "url": "https://github.com/simplerobot/package-config/parsers/json/jsoncpp/jsoncpp.srpm",
      "name": "jsoncpp",
      "dir": "build/dependencies/jsoncpp",
    },
    {
      "url": "https://github.com/simplerobot/package-config/network-clients/libcurl/libcurl.srpm",
      "name": "libcurl",
      "dir": "build/dependencies/libcurl", 
    },
  ],
}
```

This configuration can be simplified as:

```
{
  "dir": "build/dependencies",
  "release": "build/release",
  "dependencies": [
    "https://github.com/simplerobot/package-config/parsers/json/jsoncpp/jsoncpp.srpm",
    "https://github.com/simplerobot/package-config/network-clients/libcurl/libcurl.srpm",
  ],
}
```
When developing software you may want to override a package while making a change. srpm is typically run in the top directory of a project. If the parent directory contained a file with the name <project>.srpm or a directory <project>, then that would be used instead.

## Running

Command line options:
```
srpm --help                                Prints help
srpm <project-file> [--load]               Loads dependencies
srpm <project-file> --flush-all            Flushes all cached dependencies
srpm <project-file> --flush <dependency>   Flushes a specific dependency
```
## Configuration
Files:
```
~/.srpm/config       configuration file
~/.srpm/cache/       cached dependencies
```
Configuration File:
```
{
  "cache-dir": "~/.srpm/cache/",
  "cache-valid-seconds": "300",
  "client-cert": "~/cert.pem",
  "client-key": "~/key.pem",
}
```
## Integrating with make
To ensure all the loaded dependencies are visible in a Makefile, the include needs to ensure that Make reloads after running srpm.
```
ifndef MAKE_RESTARTS
build/dependencies.make : FORCE
    srpm project.srpm
    touch build/dependencies.make
endif
include build/dependencies.make
FORCE :
```
