# 将 WSL 1 升级为 WSL 2

[返回](./Use_WSL2_to_create_Yocto.md)

## 确保你的Windows 10版本足够新

WSL 2 需要 Windows 10 版本 1903 或更高版本，内部版本 18362 或更高版本。要检查您正在运行哪个版本，您可以在 Windows 上打开命令提示符并执行命令“ver”：

```command
C:\Users\320248990>  ver
Microsoft Windows [版本 10.0.19045.4291]
```

## 启动虚拟机平台

* 使用快捷键`win + x`以管理员身份打开Powershell
* 运行命令：`dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart`
* 重启计算机

## 安装 Linux 内核更新包

下载并安装最新的 Linux 内核更新包。可以从以下链接获取：[WSL2更新包](https://aka.ms/wsl2kernel)

## 设置 WSL 2 为默认版本（可选）

使用快捷键`win + x`以管理员身份打开Powershell并运行如下命令：
`wsl --set-default-version 2`

## 将现有的 WSL 1 发行版转换为 WSL 2

运行以下命令将特定的发行版（例如 Ubuntu-22.04）转换为 WSL 2
`wsl --set-version Ubuntu-22.04 2`

## 验证

执行结束后，再次运行`wsl -l -v`验证是否成功

```command
C:\Users\320248990>wsl -l -v
  NAME            STATE           VERSION
* Ubuntu-22.04    Stopped         2
```
