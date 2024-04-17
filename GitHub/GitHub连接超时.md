# GitHub连接超时

GitHub连接超时系DNS污染造成,导致很多时候push等操作无法正常运行。

## 现象

登录`github.com`，发现可以访问，但是使用cmd ping一下github发现失败了
![Time out](../photos/Time%20out.png)

## 解决

* 打开网站[https://www.ipaddress.com/ip-lookup](https://www.ipaddress.com/ip-lookup),并在框内输入域名，来查询对应的IP地址：
![IP Lookup](../photos/IP%20Lookup.png)
* 查询对应地区的github.com主机名
![Host name](../photos/Host%20name.png)
* 查询如下两个主机名的IP

  ``` git
  140.82.113.4 github.com
  199.232.69.194 github.global.ssl.fastly.net
  ```

* 补充到`C:\Windows\System32\drivers\etc\hosts`文件的末尾（需要管理员权限）
* 测试一下
![Ping success](../photos/Ping%20success.png)
