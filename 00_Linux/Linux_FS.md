# Linux文件系统

[返回目录](./index.md)

## Linux文件系统的目录结构及内容

Ubuntu中的目录遵循 FHS 标准(Filesystem Hierarchy Standard，文件系统层次标准)。它定义了文件系统中目录、文件分类存放的原则、定义了系统运行所需的最小文件、目录的集合，并列举了不遵循这些原则的例外情况及其原因。FHS 并不是一个特强制的标准，但是大多是Linux、Unix发行版本遵循 FHS.

```plaintext
/
├─bin        所有用户都可以使用的、基本的命令  
├─boot       启动文件，比如内核  
├─dev        设备文件，Linux特有的  
├─etc        配置文件  
├─home       家目录  
│   ├─lucas  用户lucas的家目录  
├─lib        库  
├─media      插上U盘等外设时，会挂载到该目录下  
├─mnt        用来挂载其他文件系统  
├─opt        Optional，可选程序  
├─proc       用来挂在虚拟的proc文件系统，可以查看个进程(process)的信息  
├─root       root用户的家目录  
├─sbin       基本的系统命令，系统管理员才能使用  
├─sys        用来挂载虚拟的sys文件系统，可以查看系统信息：比如设备信息  
├─tmp        临时目录，存放临时文件  
├─usr        Unix Software Resource，存放可分享的与不可变动的数据  
├─var        主要针对常态性变动的文件，包括缓存(cache)、log 文件等
```  
