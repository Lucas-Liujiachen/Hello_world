# .gitignore文件

[返回目录](./Index.md)

## 语法

1. 每个配置项单独占一行
2. 空行不匹配任何文件，因此常用作分隔符（方便阅读）
3. 使用 '#' 开头的行被视为注释，可以在文件中添加说明
4. \ 用于转移（如需使用实体 \，则需要加引号 "\"）
5. 每一行描述一个需要忽略的模式，可以是文件、文件夹或者通配符
6. 以 / 开头表示从 .gitignore 文件所在目录开始匹配，否则会在项目中所有项目中搜索
7. 以 / 结尾表示匹配文件夹，而不是同名文件
8. 通过 ! 取消忽略，即使之前有忽略规则，但是如果该文件的父级目录被排除了，那么 ! 将不再起作用
9. [] 通常用于匹配**一个**字符列表，如：a[ mn ]z 可以匹配 amz 和 anz
10. **用于匹配多级目录，如 a/**/b 可匹配 a/b  a/x/b  a/x/y/b
11. 每个目录都可以有自己的.gitignore文件并且会先执行根目录.gitignore再执行子目录的.gitignore
