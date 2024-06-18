# ATF ARM可信固件

## 简介

ARM可信任固件（ARM Trusted Firmware，ATF）是由ARM官方提供的底层固件，该固件统一了ARM底层接口标准，如电源状态控制接口（Power Status Control Interface，PSCI）、安全启动需求（Trusted Board Boot Requirements，TBBR）、安全世界状态（SWS）与正常世界状态（NWS）切换的安全监控模式调用（secure monitor call，smc）操作等。ATF旨在将ARM底层的操作统一使代码能够重用和便于移植。

ATF的源代码共分为bl1、bl2、bl31、bl32、bl33部分，其中bl1、bl2、bl31部分属于固定的固件，bl32和bl33分别用于加载TEEOS和REE侧的镜像。整个加载过程可配置成安全启动的方式，每一个镜像文件在被加载之前都会验证镜像文件的电子签名是否合法。

ATF主要完成的功能如下：

1. 初始化安全世界状态运行环境、异常向量、控制寄存器、中断控制器、配置平台的中断。
2. 初始化ARM通用中断控制器（General Interrupt Controller，GIC）2.0版本和3.0版本的驱动初始化。
3. 执行ARM系统IP的标准初始化操作以及安全扩展组件的基本配置。
4. 安全监控模式调用（Secure Monitor Call，smc）请求的逻辑处理代码（Monitor模式/EL3）。
5. 实现可信板级引导功能，对引导过程中加载的镜像文件进行电子签名检查。
6. 支持自有固件的引导，开发者可根据具体需求将自有固件添加到ATF的引导流程中。

![ATF工作流程](../Photos/ATF_work_flow.png)

## ATF工作流程

ATF将系统启动从







