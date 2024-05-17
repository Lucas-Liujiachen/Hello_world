# Yocto下载指北

## 下载前设置

1. 请确保虚拟机被分配了至少600G的存储空间。[VMware更改虚拟机存储空间](./Problem/More_disk_space.md)
2. 由于bitbake涉及下载内容，请确保虚拟机已经打开代理，从而加快bitbake。
   (加了代理都要下载将近一天，不加下到下辈子吧！)[如何为虚拟机的bash设置代理](./Problem/Bash_Proxy.md)

## 一、构建poky步骤（可以用来测试下载情况如何）

### 构建主机包

你需要在你的主机上下载必要的主机包，下面的指令是Ubuntu的指令：
> ```$ sudo apt install gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 python3-subunit zstd liblz4-tool file locales libacl1```  
> ```$ sudo locale-gen en_US.UTF-8```

### Git克隆Poky

> ```$ git clone git://git.yoctoproject.org/poky```  
> ```Cloning into 'poky'...```  
> ```remote: Counting```  
> ```objects: 432160, done. remote: Compressing objects: 100%```  
> ```(102056/102056), done. remote: Total 432160 (delta 323116), reused```  
> ```432037 (delta 323000) Receiving objects: 100% (432160/432160), 153.81 MiB | 8.54 MiB/s, done.```  
> ```Resolving deltas: 100% (323116/323116), done.```  
> ```Checking connectivity... done.```

### 搭建镜像

**初始化构建环境：** 从Poky目录中运行oe-init-build-env环境设置脚本以在主机上定义Yocto项目的构建环境。

> ```$ cd poky```  
> ```$ source oe-init-build-env```  
> ```You had no conf/local.conf file. This configuration file has therefore been```  
> ```created for you with some default values. You may wish to edit it to, for```  
> ```example, select a different MACHINE (target hardware). See conf/local.conf```  
> ```for more information as common configuration options are commented.```
>
> ```You had no conf/bblayers.conf file. This configuration file has therefore```  
> ```been created for you with some default values. To add additional metadata```  
> ```layers into your configuration please add entries to conf/bblayers.conf.```
>
> ```The Yocto Project has extensive documentation about OE including a reference```  
> ```manual which can be found at:```  
> ```https://docs.yoctoproject.org```
>
> ```For more information about OpenEmbedded see their website:```  
> ```https://www.openembedded.org/```  
>
> ```### Shell environment set up for builds. ###```
>
> ```You can now run 'bitbake <target>'```
>
> ```Common targets are:```  
> ```core-image-minimal```  
> ```core-image-full-cmdline```  
> ```core-image-sato```  
> ```core-image-weston```  
> ```meta-toolchain```  
> ```meta-ide-support```
>
> ```You can also run generated QEMU images with a command like 'runqemu qemux86-64'```
>
> ```Other commonly useful commands are:```  
> ```- 'devtool' and 'recipetool' handle common recipe tasks```  
> ```- 'bitbake-layers' handles common layer tasks```  
> ```- 'oe-pkgdata-util' handles common target package tasks```

该脚本运行后，你的工作目录将被设置为build

**检查本地配置文件:** 当构建环境结束后，名为local.conf的本地设置文件就变得可用了。
请在构建目录conf/local.conf中取消一下行的注释
>```BB_HASHSERVE_UPSTREAM = "wss://hashserv.yoctoproject.org/ws"```  
>```SSTATE_MIRRORS ?= "file://.* http://cdn.jsdelivr.net/yocto/>sstate/all/PATH;downloadfilename=PATH"```  
>```BB_HASHSERVE = "auto"```  
>```BB_SIGNATURE_HANDLER = "OEEquivHash"```

**开始构建:** 使用下列命令开始构建，在此实例中操作系统的镜像为core-image-sato
>`$ bitbake core-image-sato`

**使用QEMU模拟图像:** 构建此特定图像后，您可以启动 QEMU，它是 Yocto 项目附带的快速 EMUlator
>`$ runqemu qemux86-64`

## 二、构建其他库步骤（构建Yocto提供的其他的库，比如imx）

### 安装依赖

```command
$sudo apt install gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 python3-subunit zstd liblz4-tool file locales libacl1
$sudo locale-gen en_US.UTF-8
```

### 安装repo工具

1.在home下创建bin目录

```command
$mkdir ~/bin # this step may not be needed if the bin folder already exists
$curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
$chmod a+x ~/bin/repo
```

2.在.bashrc文件中添加下面一行代码，确保~/bin目录被添加到PATH变量中

另外一种安装方式：（Ubuntu包管理系统）
`$ sudo apt-get install repo`

**提示**：
repo自Ubuntu 16.04后开始支持，但是20.04不支持！此外，使用apt方式安装就不需要创建~/bin目录和修改.bashrc文件。

### 配置Git

repo工具依赖Git，因此需要配置Git账户

```command
$git config --list
$git config --global user.name "Your Name"
$git config --global user.email "Your Email"
```

### 准备i.MX Yocto工程

创建一个工程目录

```command
$mkdir imx-yocto-bsp
$cd imx-yocto-bsp
$repo init -u https://source.codeaurora.org/external/imx/imx-manifest -b imx-linux-zeus -m imx-5.4.47-2.2.0.xml
$repo sync
```

### 配置系统映像

imx-setup-relase.sh脚本进行配置的语法如下：
`DISTRO=<distro name> MACHINE=<machine name> source imx-setup-release.sh -b <build dir>`

可以在meta-freescale/conf/machine目录下看到你的i.MX机器的配置文件

```command
$ ls sources/meta-freescale/conf/machine/
imx23evk.conf          imx6sxsabreauto.conf  include
imx25pdk.conf          imx6sxsabresd.conf    ls1012afrwy.conf
imx28evk.conf          imx6ulevk.conf        ls1012ardb.conf
imx51evk.conf          imx6ullevk.conf       ls1021atwr.conf
imx53ard.conf          imx7dsabresd.conf     ls1028ardb.conf
imx53qsb.conf          imx7ulpevk.conf       ls1043ardb.conf
imx6qdlsabreauto.conf  imx8mmevk.conf        ls1046afrwy.conf
imx6qdlsabresd.conf    imx8mqevk.conf        ls1046ardb.conf
imx6slevk.conf         imx8qmmek.conf        ls1088ardb.conf
imx6sllevk.conf        imx8qxpmek.conf       ls1088ardb-pb.conf
```

比如对于i.MX8MQ-EVK平台，可以如下配置：
`DISTRO=fsl-imx-wayland MACHINE=imx8mqevk source imx-setup-release.sh -b build-wayland`
**注意**：
每个构建文件夹必须仅使用一个发行版的方式进行配置。每次DISTRO_FEATURES变量改变，都必须重新指定一个新的build目录。

命令执行完毕就会进入build-wayland目录。

### 构建镜像

Yocto是使用bitbake命令进行构建。具体原理和流程请到[Yocto笔记](./note.md)中寻找。
选择一个你想要的镜像，不同的镜像包含不同的层次内容，不同的image构建的大小和时间都不一样。
比如可以：
`$ bitbake imx-image-multimedia`

### 等待构建完成

构建过程中需要大概10几个小时，才能构建完成。
如果过程中重启电脑或者退出终端后想重新进入配置环境，那么请在创建的文件夹（例子中为imx-yocto-bsp）目录下执行下面的命令。其中，build-wayland为你在DISTRO配置时的build目录。
`source  setup-environment build-wayland`

### 部署镜像

构建完成后，创建的镜像位于 `<build directory>/tmp/deploy/images`

```command
$ cd tmp/deploy/images/imx8mqevk/
$ ls imx-image-multimedia*
imx-image-multimedia.env
imx-image-multimedia-imx8mqevk-20210226114323.rootfs.manifest
imx-image-multimedia-imx8mqevk-20210226114323.rootfs.tar.bz2
imx-image-multimedia-imx8mqevk-20210226114323.rootfs.wic.bmap
imx-image-multimedia-imx8mqevk-20210226114323.rootfs.wic.bz2
imx-image-multimedia-imx8mqevk-20210226114323.testdata.json
imx-image-multimedia-imx8mqevk.manifest
imx-image-multimedia-imx8mqevk.tar.bz2
imx-image-multimedia-imx8mqevk.testdata.json
imx-image-multimedia-imx8mqevk.wic.bmap
imx-image-multimedia-imx8mqevk.wic.bz2
imx-image-multimedia-imx-imx-boot-bootpart.wks
```
