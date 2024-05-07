# Yocto下载指北

## 下载前设置

1. 请确保虚拟机被分配了至少600G的存储空间。[VMware更改虚拟机存储空间](./Problem/More_disk_space.md)
2. 由于bitbake涉及下载内容，请确保虚拟机已经打开代理，从而加快bitbake。
   (加了代理都要下载将近一天，不加下到下辈子吧！)[如何为虚拟机的bash设置代理](./Problem/Bash_Proxy.md)
3. df

## 步骤

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
