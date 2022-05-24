WARNING! This project is stalled. It was never finished and is not under current development.

# Simple Robot(s) Package Manager (srpm)
Software projects are built on top of dependent projects. In addition good
software design suggests building software in modules that are easily testable.
The Simple Robots Package Manager is a lightweight and distributed package
manager to bring these dependencies into the build.

srpm is designed to be lightweight by making it build agnostic. The build
system should call the package manager to import the desired dependencies. A
config file specifies which dependencies to import and the directory to put
them into.  All dependencies, including recursive release dependencies are
merged into the target directory.  This implies that packages include a
standard structure so they can be merged correctly.  In order to handle
dependencies in a distributed way, packages are identified by a url to a
release package, which contains both the latest configuration file as well
as the latest binary release.  Each package has a name.  When a local package
exists with this name, it will be used instead of the release version.

To remain lightweight, srpm does not provide many of the features provided by
other package managers such as Maven, Gradle, Conan.io. srpm does not integrate
with any build systems, manage artifacts directly, support multiple variations
on a build, manage multiple versions of a component, or handle recursive
dependencies. In addition, it only runs on linux.

## Package Configuration
Package configuration files are written in json. (non-optimal, but flexible
enough for future features). An example configuration file that will import
jsoncpp and libcurl packages into the build/deps directory.  Additional targets
can be defined, and each will receive a copy of the dependent packages.  This
allows pulling different dependency trees for different parts of the build.
A special target, "release" specifies that the dependency is also loaded when
the package is loaded.  For example, if a c++ library depended on another c++
library, it would include both build and release dependencies to ensure that
users of the library would include both transitive dependencies.

```
{
  "name": "package-manager",
  "location": "https://github.com/simplerobot/package-manager/releases/latest/download/package.zip",
 
  "release": "build/release",
  "freshness-file": "build/dependencies.make",
  
  "targets": [
    {
      "name": "test",
      "dir": "build/test/deps",
    },
    {
      "name": "main",
      "dir": "build/main/deps",
    },
  ],
  
  "dependencies":
  [
    {
      "name": "srpm-jsoncpp",
      "location": "https://github.com/simplerobot/srpm-jsoncpp/releases/latest/download/package.zip",
      "targets": [ "test", "main" ],
    },
    {
      "name": "srpm-libcurl",
      "url": "https://github.com/simplerobot/srpm-libcurl/releases/latest/download/package.zip",
      "targets": [ "test", "main" ],
    },
    {
      "name": "test-cpp-lib",
      "url": "https://github.com/simplerobot/test-cpp-lib/releases/latest/download/package.zip",
      "targets": [ "test" ],
    },
  ],
}
```

When developing software you may want to override a package while making a
change. srpm is typically run in the top directory of a project. If the parent
directory contains a file with the name <project>.srpm that srpm file will be
used instead.  If the parent directory contains a <project> directory, then
the local release will be used instead.

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
To ensure all the loaded dependencies are visible in a Makefile, the include
needs to ensure that Make reloads after running srpm.  We do this by including
the freshness file as a makefile.
```
build/dependencies.make : FORCE
	srpm project.srpm
include build/dependencies.make
FORCE :

```
