# conan仓库及其管理

上文我们已经介绍了如何使用conan维护可执行程序项目的依赖.但有一样非常重要的问题没有解决--这些依赖怎么让conan维护管理.

我们上面介绍过[conancenter](https://conan.io/center/).它是conan官方维护的公开仓库.角色类似pypi.但它并不支持注册用户,只能在[项目工单](https://github.com/conan-io/conan-center-index/issues)中发出请求,等每周工作人员例行手工加人.这未必不是好事,现在这个仓库中库虽然维护的不算多全,但也算是整洁干净,这应该也是这种制度造成的结果.

我们如果希望自己的库可以也被conan管理,那最好的办法还是自己搭建一个仓库.

## artifactory私有仓库搭建

其实搭建conan仓库也不算多麻烦,借助docker我们可以很简单的实现.

1. 在你的数据库(比如pg)中创建一个仓库(比如叫`artifactory`)

2. 在一个文件夹(比如`/volume2/docker_deploy/devtools/artifactory/data`)下创建目录`etc/`并在其中创建配置文件`system.yaml`.其中将你的数据库信息填好

    ```yaml
    type: postgresql
    driver: org.postgresql.Driver
    url: "jdbc:postgresql://<host>:port/artifactory"
    username: <username>
    password: password
    ```

3. 使用如下docker-compose部署

    ```compose
    version: "2.4"

    x-log: &default-log
    options:
        max-size: "10m"
        max-file: "3"

    services:
    artifactory:
        image: releases-docker.jfrog.io/jfrog/artifactory-cpp-ce:latest
        mem_limit: 2g
        restart: on-failure

        ports:
            - "8081:8081"
            - "8082:8082"
        volumes: 
            - "/volume2/docker_deploy/devtools/artifactory/data:/var/opt/jfrog/artifactory"
        logging:
            <<: *default-log

    ```

上面我们开放了两个端口,`8081`端口是用于与客户端通信的,而`8082`端口则是管理页面.

这样我们就可以在路径`http://<SERVER_HOSTNAME>:8082/ui/`上访问到其管理页面了,初始用户名为`admin`,默认密码为`password`,进去后会要求修改密码.

这个服务叫[artifactory](https://www.jfrog.com/confluence/display/JFROG/JFrog+Artifactory),我们使用的是它的社区版本,这个版本基本功能都有而且有界面方便维护管理.在安装好后我们还需要对其进行一番设置

## artifactory私有仓库的配置

`artifactory`提供了三种类型的Conan仓库供不同目的使用：

+ 本地仓库`Local`:即当前 Artifactory 服务器上存储软件包的仓库(允许上传)
+ 远端仓库`Remote`:即第三方的软件仓库,在本服务器上作为 Proxy 和 Cache 运作(无法上传)
+ 虚拟仓库`Virtual`:作为一个索引中心,能将其他两类仓库整合到一个仓库名下,方便`install 操作`使用.(无法上传)

通常我们会新建一个本地仓库用于上传本地软件,一个远端仓库用来代理和缓存conancenter,然后建一个虚拟仓库用于将这两个整合在一起

## 使用远程仓库

我们可以使用`conan remote list`查看目前已经在本地注册的远程仓库有哪些,一般来说conan默认有两个仓库

+ `conancenter`: <https://center.conan.io>
+ `conan-center`: <https://conan.bintray.com>

我们自己的仓库需要额外的添加进去:

1. 使用`conan remote add <REMOTE> http://<artifactoryhost>:<artifactoryport>/artifactory/api/conan/<仓库名> [False]`命令将这个指定仓库添加到本地,注意最后一位可以填`False`意味着不进行ssl校验.
2. 在本地我们需要在使用前用`conan user -p <PASSWORD> -r <REMOTE> <USERNAME>`登陆我们自己的仓库.尤其是私有仓库很多是要用户名密码登录的

我们如果只是使用其中的包就只用添加虚拟仓库,这样下次执行`install`命令时只需要使用`-r <虚拟仓库名>`指定即可,我们就不用关系包是远程仓库里的还是本地仓库里的了;

但如果我们还要自己创建包则需要再添加我们的本地仓库到本地,因为上传只能使用本地仓库.

## 使用conan管理自己的包项目

我们可以用`conan new -t -s <包名>/<版本号>`命令创建一个自己的包项目的配置代码模板文件夹.它会创建如下内容:

+ `src`文件夹,用于放源码其中会有:
    + 一个`Cmakelists.txt`文件用于描述编译过程,通常不用管,如果有一些编译要求比如需要用c++17的特性可以加上`add_definitions("-std=c++17")`
    + `<包名>.cpp`和`<包名>.h`两个空文件,我们的包内容就写这里
+ `test_package`文件夹,用于放测试代码,其中会有:
    + 一个`Cmakelists.txt`文件用于描述编译过程,通常不用管,如果有一些编译要求比如需要用c++17的特性可以加上`add_definitions("-std=c++17")`
    + 一个`conanfile.py`文件用于描述测试可执行文件的cmake的构造,通常也不用管,如果要用到一些特殊设置,比如会有其他包依赖可以在`requires`字段加上这些依赖.
    + 一个`example.cpp`文件用于写测试.根据需要写好测试即可

+ [conanfile.py](https://docs.conan.io/en/latest/reference/conanfile.html)文件,用于描述包项目cmake的构造过程.
    其作用类似前文介绍的`conanfile.txt`,但描述更加细致.我们除了需要描述编译配置还需要描述项目的元信息.一个典型的`conanfile.py`如下:

    ```python
    from conans import ConanFile, CMake


    class <package name title>Conan(ConanFile):
        name = "<package name>"
        version = "<package version>"
        license = "MIT"
        author = "<author> <author email>"
        url = "<Package recipe repository url here, for issues about the package>"
        description = "<Description of Spdloghelper here>"
        topics = ("<keyword>", "<keyword>", "<keyword>", ...)
        settings = "os", "compiler", "build_type", "arch"
        options = {"shared": [True, False], "fPIC": [True, False]}
        default_options = {"shared": False, "fPIC": True}
        generators = "cmake"
        exports_sources = "src/*"
        requires = [
            "<requires/<version>>"
        ]

        def config_options(self):
            if self.settings.os == "Windows":
                del self.options.fPIC

        def build(self):
            cmake = CMake(self)
            cmake.configure(source_folder="src")
            cmake.build()

            # Explicit way:
            # self.run('cmake %s/hello %s'
            #          % (self.source_folder, cmake.command_line))
            # self.run("cmake --build . %s" % cmake.build_config)

        def package(self):
            self.copy("*.h", dst="include", src="src")
            self.copy("*.lib", dst="lib", keep_path=False)
            self.copy("*.dll", dst="bin", keep_path=False)
            self.copy("*.dylib*", dst="lib", keep_path=False)
            self.copy("*.so", dst="lib", keep_path=False)
            self.copy("*.a", dst="lib", keep_path=False)

        def package_info(self):
            self.cpp_info.libs = ["<package name>"]

    ```

    其中比较重要的是`build`部分,它会决定包如何编译.不过一般不用改.

然后我们可以在src文件夹下构造自己的包项目,在构造好后我们可以在项目根目录下执行`conan create . test/demo --build` 这条命令,它会全部重新下载编译`test_package`中的测试代码然后执行.

这种方式构造项目的最大缺点是有传染性,如果你的所有项目都使用conan维护那conan会很好用,但如果你的用户想不用conan,毕竟conan有python依赖,那这些项目对他来说就完全没用.conan可以兼容其他如cmake的`FetchContent`方式,但cmake没法兼容conan.因此如果你的代码并不需要开源,那上面的方式就可以了.

如果你的代码要开源一个更加推荐的方式是使用git维护包项目,用`FetchContent`方式维护依赖,然后使用下面的方式包装你的项目到conan中维护.

## 使用conan管理自己包装的外部项目

在项目根目录下使用`conan new -t <包名>/<版本号>`命令会创建一个没有`src`目录的conan包项目,和上面的区别只在根目录下的`conanfile.py`中.

这个`conanfile.py`大致如下:

```python
from conans import ConanFile, CMake, tools


class SpdloghelperConan(ConanFile):
    name = "<package name>"
    version = "<package version>"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Spdloghelper here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        self.run("git clone https://github.com/conan-io/hello.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("hello/CMakeLists.txt", "PROJECT(<package name>)",
                              '''PROJECT(<package name>)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="hello")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="<package name>")
        self.copy("*<package name>.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["<package name>"]

```

可以看到主要的区别就在于包装项目没有`exports_sources`字段,取而代之的是方法`source`

而方法`source`干的事情就是下载要包装的项目源码,加上`include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()`好让cmake使用conan生成的模块.我们一般也只要修改这个部分就可以了

和自建项目一样我们也可以在`test_package`中编写测试代码.

## `原生conan包`vs`conan包装外部包`

下面是原生conan包和conan包装外部包优劣势矩阵图,我们可以参考了根据自己的环境和项目特性选择合适的方法

| 对比项\包形式    | 原生conan包  | conan包装外部包 |
| ---------------- | ------------ | --------------- |
| 纯内网环境       | 适用         | 不适用          |
| 墙内             | 受外网影响小 | 受外网影响大    |
| 项目开放性       | 私有项目     | 开源项目        |
| 有大量老项目依赖 | 不适用       | 适用            |

## 上传项目到自托管conan

只要上面我们部署好了conan的本地仓库服务,我们就可以将两种方式构造的项目打包上传到本地仓库服务中.

```bash
conan upload <RECIPE> -r <REMOTE> --all
```

+ `<RECIPE>`就是你的`<项目名/项目版本@用户/分组>`结构,比如前面我们用`conan create`时使用的是`test/demo`那就可以使用`test`作为用户,`demo`作为分组

+ `--all`表示将全部打包内容上传

