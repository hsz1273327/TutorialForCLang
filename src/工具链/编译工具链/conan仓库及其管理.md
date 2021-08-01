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

### 本地配置私有仓库

在本地我们需要在使用前用`conan user -p <PASSWORD> -r <REMOTE> <USERNAME>`先登陆我们自己的仓库,
然后可以使用`conan remote add <REMOTE> http://<artifactoryhost>:<artifactoryport>/artifactory/api/conan/<仓库名>`命令将这个指定仓库添加到本地.

我们如果只是使用其中的包就只用添加虚拟仓库,这样下次执行`install`命令时只需要使用`-r <虚拟仓库名>`指定即可,我们就不用关系包是远程仓库里的还是本地仓库里的了;

但如果我们还要自己创建包则需要再添加我们的本地仓库到本地,因为上传只能使用本地仓库


## 使用私有仓库管理自己的包项目
