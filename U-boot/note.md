# U-boot学习笔记

[返回目录](../Index.md)  
[扩展知识1-ATF](./ATF/Index.md)

## U-boot启动流程

XIP是eXecution In Place的缩写，意思是就地执行（CPU直接访问而不用加载到RAM中）。
CPU可以直接读取Flash得到指令，而不需要将指令加载到RAM（内存）中，并且CPU直接执行，就好像程序直接在Flash中执行了。

### ① U-boot烧写在XIP设备上

1. 硬件初始化
2. 从Flash将kernel拷贝到RAM（内存）中
3. 启动kernel

### ② U-boot烧写在非XIP设备上

1. 通过BROM（BootROM）将U-boot拷贝到RAM（内存）中
2. 执行U-boot
3. 硬件初始化（不初始化RAM）
4. 从Flash将kernel拷贝到RAM中
5. 启动kernel

## U-boot源码结构

```plaintext
├─.
  ├─arch                        // 架构相关
  │  ├─arm
  │  │  ├─cpu
  │  │  │  ├─armv7
  │  │  │  │  ├─mx6
  │  │  ├─dts
  │  │  │  └─include
  │  │  ├─include
  │  │  │  ├─asm
  │  │  │  │  ├─arch-armv7
  │  │  │  │  ├─arch-imx
  │  │  │  │  ├─arch-imx8
  │  │  │  │  ├─arch-imx8m
  │  │  │  └─debug
  │  │  ├─lib
  │  │  ├─mach-rockchip
  │  │  |  ├─rk3036
  ├─board                       // 单板相关
  │  ├─freescale
  │  │  ├─common
  │  │  │  └─p_corenet
  │  │  ├─imx8mm_evk
  │  │  ├─imx8mn_evk
  │  │  ├─imx8mp_evk
  │  │  ├─imx8mq_evk
  │  │  ├─mx6ullevk
  ├─cmd                         // 通用命令
  │  ├─arm
  │  ├─mvebu
  │  ├─riscv
  │  ├─sandbox
  │  └─x86
  ├─common                      // 通用文件
  │  ├─eeprom
  │  ├─init
  │  └─spl
  ├─configs
  ├─disk
  ├─drivers                     // 驱动
  ├─dts
  ├─fs                          // 文件系统
  │  ├─btrfs
  │  │  ├─common
  │  │  ├─crypto
  │  │  └─kernel-shared
  │  ├─cbfs
  │  ├─cramfs
  │  ├─erofs
  │  ├─ext4
  │  ├─fat
  │  ├─jffs2
  │  ├─sandbox
  │  ├─squashfs
  │  ├─ubifs
  │  ├─yaffs2
  │  └─zfs
  ├─include
  ├─lib                         // 库
  ├─net                         // 网络协议
```
