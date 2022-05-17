# Simple Robot(s) Package Manager (srpm)
Software projects are built on top of dependent projects. In addition good software design suggests building software in modules that are easily testable. The Simple Robots Package Manager is a lightweight and distributed package manager to bring these dependencies into the build.

srpm is designed to be lightweight by making it build agnostic. The build system should call the package manager to import the desired dependencies. A config file specifies which dependencies to import and where to put them in the build. In order to handle dependencies in a distributed way, packages are identified by a url to a configuration file for the dependent package. Packages are also given a name. When a local package exists, it's artifacts are overwritten by this local build.

To remain lightweight, srpm does not provide many of the features provided by other package managers such as Maven, Gradle, Conan.io. srpm does not integrate with any build systems, manage artifacts directly, support multiple variations on a build, manage multiple versions of a component, or handle recursive dependencies. In addition, it only runs on linux.

## Package Configuration
Package configuration files are written in json. (non-optimal, but flexible enough for future features). An example configuration file that will import jsoncpp and libcurl packages into the build directory build/dependencies.

```
{
  "name": "package-manager",
  "config-url": "https://github.com/simplerobot/package-manager/releases/latest/download/package.srpm",
  "binary-url": "https://github.com/simplerobot/package-manager/releases/latest/download/package.tgz",
  "release-dir": "build/release",

  "dependencies":
  [
    {
      "name": "jsoncpp",
      "config-url": "https://github.com/simplerobot/package-config/parsers/json/jsoncpp/jsoncpp.srpm",
      "import-dir": "build/dependencies/jsoncpp",
    },
    {
      "name": "libcurl",
      "config-url": "https://github.com/simplerobot/package-config/network-clients/libcurl/libcurl.srpm",
      "import-dir": "build/dependencies/libcurl", 
    },
  ],
}
```

When developing software you may want to override a package while making a change. srpm is typically run in the top directory of a project. If the parent directory contains a file with the name <project>.srpm that srpm file will be used instead.  If the parent directory contains a <project> directory, then that code will be used instead.

## Running

Command line options:
```
srpm --help                           Prints help
srpm [<project-file>] [--load]        Loads dependencies
```
## Configuration
Files:
```
package.srpm         default package file name
~/.srpm/config       configuration file
~/.srpm/cache/       cached dependencies
```
Configuration File:
```
{
  "cache-dir": "~/.srpm/cache/",
  "cache-valid-seconds": "300",
  "servers" : [
    {
      "host": "git.simplerobots.com",
      "client-cert": "~/.srpm/simplerobots.cert.pem",
      "client-key": "~/.srpm/simplerobots.key.pem",
    },
  ],
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
