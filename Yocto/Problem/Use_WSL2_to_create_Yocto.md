# 使用WSL 2创建 Yocto 项目开发环境

[返回](./Index.md)

使用适用于 Linux 的 Windows 子系统 (WSL 2)，你可以创建一个 Yocto 项目开发环境，以便在 Windows 上进行构建。你可以在 Windows 内部设置 Linux 发行版，并在其中使用 Yocto 项目进行开发。

**1.确保你的 Windows 机器能够运行 WSL 2**：
虽然所有 Windows 11 和 Windows Server 2022 版本都支持 WSL 2，但 Windows 10 和 Windows Server 2019 的首批版本并不支持。请查看Windows 10 和Windows Server 2019的最低版本号。
要检查你正在运行哪个版本，你可以在 Windows 上打开命令提示符并执行命令“ver”：

```command
C:\Users\320248990>  ver
Microsoft Windows [版本 10.0.19045.4291]
```

**2.在 WSL 2 中安装你选择的 Linux 发行版**： 一旦你知道你的 Windows 版本支持 WSL 2，你就可以从 Microsoft Store 安装你选择的发行版。打开 Microsoft Store 并搜索 Linux。虽然有几种 Linux 发行版可用，但假设你选择的将是 Yocto 项目支持的发行版之一，如使用本机 Linux 主机的说明中所述。做出选择后，只需单击“获取”即可下载并安装发行版。

**3.检查 WSL 2 正在使用哪个 Linux 发行版**：使用快捷键`win + x`打开 Windows PowerShell 并运行：

```command
C:\Users\320248990>wsl -l -v
  NAME            STATE           VERSION
* Ubuntu-22.04    Stopped         2
```

如果发现自己的版本可以支持WSL 2但是版本却是WSL 1，那么可以[更改WSL版本](WSL1_to_WSL2.md)
**4.经常优化WSL 2存储**：由于 WSL 2 上处理存储的方式，底层 Linux 发行版使用的存储空间不会立即反映出来，而且由于 BitBake 大量使用存储，经过几次构建后，可能意识不到空间不足。由于 WSL 2 使用 VHDX 文件进行存储，因此可以通过定期手动优化此文件轻松避免此问题：

* **查找 VHDX 文件的位置：以管理员身份打开 Powershell 并运行**：
  
  ```command
  PS C:\Users\320248990> Get-AppxPackage -Name "*Ubuntu*" | Select PackageFamilyName

  PackageFamilyName
  -----------------
  CanonicalGroupLimited.Ubuntu22.04LTS_79rhkp1fndgsc
  ```

  在以下路径上替换 PackageFamilyName 和你的用户来查找 VHDX 文件：

  ```command
  PS C:\Users\320248990> ls C:\Users\320248990\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu22.04LTS_79rhkp1fndgsc\LocalState
    
    目录: C:\Users\320248990\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu22.04LTS_79rhkp1fndgsc\LocalState

  Mode                 LastWriteTime         Length Name
  ----                 -------------         ------ ----
  -a----         2024/5/29     18:15     1549795328 ext4.vhdx
  ```

  那么你的 VHDX 文件的路径为：
  `C:\Users\320248990\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu22.04LTS_79rhkp1fndgsc\LocalState\ext4.vhdx`

* **使用 Windows Powershell 优化 VHDX 文件**：
  要使用optimize-vhd以下 cmdlet，请首先在 Windows 上安装 Hyper-V 选项。然后，以管理员身份打开 Windows Powershell 来优化 VHDX 文件：
  
  ```command
  PS C:\Users\320248990> wsl --shutdown
  PS C:\Users\320248990> optimize-vhd -Path C:\Users\320248990\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu22.04LTS_79rhkp1fndgsc\LocalState\ext4.vhdx -Mode full
  ```

  > 优化 VHDX 文件时应该显示进度条，并且存储现在应该正确反映在 Windows 资源管理器上。

设置好 WSL 2 后，一切就绪，可以像在本机 Linux 机器上运行一样进行开发。
