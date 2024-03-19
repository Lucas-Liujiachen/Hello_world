# \[CMakeLists.txt学习笔记\]

## 一、注意事项

1. CMakeLists.txt文件名严格区分大小写
2. CMake指令可以不区分大小写，每条语句不需要加分号
3. 参数严格区分大小写，并且只能用字母、数字、下划线、破折号
4. 使用${&lt;var&gt;}来引用参数的值，参数之间使用空格进行分隔
5. 使用#来进行注释

## 二、常用命令

### 2-1 cmake_miimum_required命令

语法：
`cmake_minimum_required(VERSION <min>[...<max>] [FATAL_ERROR])`

作用：
指明了对CMake的最低（高）版本的要求

CMake版本号说明：
格式为major.minor[.patch[. tweak]]，例如CMake版本号为3.10.2
major:主要
minor:次要
patch:补充
tweak:调整

参数：

1. VERSION：指明后面的参数为版本号，这个参数是必须的
2. min：指定CMake要求的最低版本号，如果CMake实际版本号低于min指定的版本号，CMake的执行过程会停止
3. max：指定CMake要求的最高版本号，要求该参数不能小于min。
4. FATAL_ERROR：该参数在CMake2.6和以后的版本被忽略，之前是为了使其能够提示失败而不仅仅是一个警告。

### 2-2 project命令

语法：

```CMake
project(<PROJECT-NAME> [<language-name>...])
或
project(<PROJECT-NAME>
[VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
[DESCRIPTION <project-description-string>]
[HOMEPAGE_URL <url-string>]
[LANGUAGES <language-name>...])
```

作用：
project命令用于指定CMake工程的名称，实际上，它还可以指定CMake工程的版本号（VERSION关键字）、简短的描述（DESCRIPTION关键字）、主页URL（HOMEPAGE_URL关键字）编译工程使用的语言（LANGUAGES关键字）。

参数：

1. **PROJECT_NAME**：将当前工程的名称赋值给PROJECT_NAME,同时${PROJECT_NAME}变量赋值为PROJECT_NAME。
2. **VERSION**：指定工程的版本号。
3. **DESCRIPTION**：对工程的文本描述。
4. **HOMEPAGE_URL**：指定工程的主页URL。
5. **LANGUAGES**：选择构建工程需要的编程语言。

赋值：

1. **PROJECT_NAME**：将当前工程的名称赋值给PROJECT_NAME。
2. **PROJECT_SOURCE_DIR**：当前工程的源码路径。
3. **_SOURCE_DIR**：指定工程的源码路径，这个变量和PROJECT_SOURCE_DIR的区别就是，_SOURCE_DIR跟具体的工程名字关联起来，若就是当前工程，则该变量和PROJECT_SOURCE_DIR相等。
4. **PROJECT_BINARY_DIR**：当前工程的二进制路径。
5. **_BINARY_DIR**：指定工程的二进制路径，这个变量和PROJECT_BINARY_DIR的区别就是，_BINARY_DIR跟具体的工程名字关联起来，若就是当前工程，则该变量和PROJECT_BINARY_DIR相等。
6. **CMAKE_PROJECT_NAME**：顶层工程的名称。例如当前调用的CMakeLists.txt位于顶层目录（可以理解为使用cmake命令首次调用的那个CMakeLists.txt），那么工程名还会赋值给CMAKE_PROJECT_NAME。

**注意**：

1. LANGUAGES关键字有两种用法一种是直接跟在工程名后面，可以省略LANGUAGES关键字；另一种是跟在其他关键字（例如VERSION）后面，LANGUAGES关键字不能省略。`project (pro_test “CXX”)`
2. project命令并非必不可少，如果没有调用project命令，cmake仍然会生成一个默认的工程名“Project”，以及工程名对应的变量（例如PROJECT_NAME、CMAKE_PROJECT_NAME、PROJECT_SOURCE_DIR、&lt;PROJECT_NAME&gt;_SOURCE_DIR、PROJECT_BINARY_DIR、&lt;PROJECT_NAME&gt;_BINARY_DIR等）。但是VERSION、DESCRIPTION、HOMEPAGE_URL等选项对应的变量不会被赋值（LANGUAGES例外，即使不指定，默认语言为C和CXX）。
3. project命令需要放置在其他命令调用之前，在cmake_minimum_required命令之后。
4. 如果多次调用project命令，那么CMAKE_PROJECT_NAME、CMAKE_PROJECT_VERSION、CMAKE_PROJECT_DESCRIPTION、CMAKE_PROJECT_HOMEPAGE_URL等变量是以最近一次调用的project命令为准

### 2-3 set命令

语法：

```CMake
set(<variable> <value>... [PARENT_SCOPE])                   #设置普通变量
set(<variable> <value>... CACHE <type> <docstring> [FORCE]) #设置缓存条目
set(ENV{<variable>} [<value>])                              #设置环境变量
```

作用：
set命令可以用来设置普通变量、Cache变量（缓存条目）、环境变量三种变量的值。

参数：
set的值&lt;value&gt;…表示可以给变量设置0个或者多个值，当设置多个值时（大于2个），多个值会通过分号连接符连接成一个真实的值赋值给变量；当设置0个值时，实际上是把变量变为未设置状态，相当于调用unset命令。

**普通变量命令**：

格式：`set(<variable> <value>...[PARENT_SCOPE])`

含义：将变量variable设置为值&lt;value&gt;…，变量variable的作用域为调用set命令的函数或者当前目录。

**PARENT_SCOPE**使用规则：
如果使用了PARENT_SCOPE选项，意味着该变量的作用域会传递到上一层（也就是上一层目录或者当前函数的调用者，如果是函数则传递到函数的调用者，如果是目录则传递到上一层目录），并且在当前作用域该变量不受带PARENT_SCOPE选项的set命令的影响（如果变量之前没有定义，那么在当前作用域仍然是无定义的；如果之前有定义值，那么值和之前定义的值保持一致）。

1.当前函数内使用PARENT_SCOPE定义变量，在函数定义文件中使用
结果：变量无定义。
结论：函数内定义变量，在定义函数的文件中调用，找不到变量的定义。

```CMake
cmake_minimum_required (VERSION 3.10.2)
 project (set_test)
 function (test_fn arg1)
 set (normal_var_in_fn ${arg1} PARENT_SCOPE)
 endfunction (test_fn)
 message (">>> in directory, value = ${normal_var_fn}>")
```

输出为：
`>>> in directory, value =`

2.当前函数内使用PARENT_SCOPE定义变量，在当前函数内使用
结果：变量无定义。
结论：函数内使用选项PARENT_SCOPE定义的变量，在函数内也是无定义的。

```CMake
cmake_minimum_required (VERSION 3.10.2)
project (set_test)

function (test_fn arg1)
    set (normal_var_in_fn ${arg1} PARENT_SCOPE)
    message (">>> in function, value = ${normal_var_fn}")
endfunction (test_fn)

test_fn (hello)
```

输出为：
`>>> in function, value =`

3.当前函数内使用PARENT_SCOPE定义变量，在函数内使用，但是存在set不带PARENT_SCOPE的命令定义
结果：函数内使用的是不带PARENT_SCOPE的set定义的那个变量
结论：PARENT_SCOPE定义的变量作用域在上一层函数，在本函数内我们只能使用不带PARENT_SCOPE定义的变量

```CMake
cmake_minimum_required (VERSION 3.10.2)
project (set_test)

function (test_fn arg1)
    set (normal_var_in_fn nohello)
    set (normal_var_in_fn ${arg1} PARENT_SCOPE)
    message (">>> in function, value = ${normal_var_in_fn}")
endfunction (test_fn)

test_fn (hello)
```

输出为：
`>>> in function, value = nohello`

4..当前函数内使用PARENT_SCOPE定义变量，在当前函数定义文件中的另一个函数中使用
结果：调用者可以调用该变量
结论：PARENT_SCOPE将该变量的作用域传送到了上一层调用函数

```CMake
cmake_minimum_required (VERSION 3.10.2)
project (set_test)

function (test_fn arg1)
    set (normal_var_in_fn nohello)
    set (normal_var_in_fn ${arg1} PARENT_SCOPE)
    message (">>> in function, value = ${normal_var_in_fn}")
endfunction (test_fn)

function (test_fn_parent arg1)
    test_fn (${arg1})
    message (">>> in parent function, value = ${normal_var_in_fn}")
endfunction (test_fn_parent)

test_fn_parent (hello)
```

输出为：

```CMake
>>> in function, value = nohello
>>> in parent function, value = hello
```

5.当前目录使用PARENT_SCOPE定义变量，在其他目录中使用
结果：其他目录可以使用该变量
结论：PARENT_SCOPE将该变量的作用域传到了上一层调用

```CMake
cmake_minimum_required (VERSION 3.10.2)
project (set_sub_test)

set (normal_var_in_sub_dir sub_hello)
set (normal_var_in_sub_dir hello PARENT_SCOPE)

message (">>>>>> in sub directory, value = ${normal_var_in_sub_dir}")
```

```CMake
cmake_minimum_required (VERSION 3.10.2)
project (set_test)

add_subdirectory (sub)

message (">>> in top level, value = ${normal_var_in_sub_dir}")
```

输出为：

```CMake
>>>>>> in sub directory, value = sub_hello
>>> in top level, value = hello
```

**Cache变量命令**：

格式：`set(<variable> <value>... CACHE <type> <docstring> [FORCE])`

含义：

1. 将缓存条目variable设置为值&lt;value&gt; ...
2. 除非用户进行设置或使用了选项FORCE，默认情况下缓存条目的值不会被覆盖。
3. 缓存条目可以通过CMAKE的GUI界面的add entry按钮来增加。
4. 缓存条目的实质为可以跨层级进行传递的变量，类似于全局变量。

&lt;type&gt;分类：

- BOOL：布尔值ON/OFF,CMAKE的GUI界面对此类缓存条目会提供一个复选框。
- FILEPATH：文件路径，CMAKE的GUI界面对此类缓存条目会提供一个文件选择框。
- PATH：目录路径，CMAKE的GUI界面对此类缓存条目会提供一个目录选择框。
- STRING / STRINGS：文本行，CMAKE的GUI界面对此类缓存条目会提供一个文本框（对应STRING）或下拉选择框（对应STRINGS）。
- INTERNAL：文本行，但是只用于内部，不对外呈现。主要用于运行过程中存储变量，因此使用该type意味着使用FORCE。

注意事项：

- 如果变量先前未定义或者使用了FORCE选项，则缓存条目会直接被赋值。
- 可以在使用cmake构建的使用通过-D选项来给缓存条目赋值，这样CMakeLists.txt内的set命令只会为缓存条目添加类型。
- 如果变量类型是目录或者文件路径，通过-D选项传入的若只是相对路径，那么set会给这个相对路径前添加当前的工作目录以变成绝对路径（如果已经是绝对路径则不会处理）。

**环境变量命令**：

格式：`set(ENV{<variable>} [<value>])`

含义：将环境变量设置为value，**注意**value可以没有。之后可以使用`$ENV{variable}`来获得环境变量的值。

注意事项：

- 该命令设置的环境变量只在当前的cmake进程生效，既不会影响调用者的环境变量，也不会影响系统环境变量。
- 如果&lt;value&gt;值为空或者ENV{&lt;variable&gt;}后没有参数，则该命令会清除掉当前环境变量的值。
- &lt;value&gt;后的参数会被忽略。

### 2-4 add_subdirectory命令

语法：
`add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])`

作用：
添加一个子目录并构建该子目录，告诉CMake本目录还有其他子目录的CMakeLists.txt需要编译。

参数：

1. source_dir:**必选参数**，该参数指定了一个子目录，该子目录应该包含CMakeLists.txt和代码文件。子目录可以是相对本目录的相对路径，也可以是绝对路径。
2. binary_dir:**可选参数**，该参数指定了一个用于存放输出文件的目录，若没有指定，则默认存放输出文件的目录为source_dir。
3. EXCLUDE_FROM_ALL:**可选参数**，若指定了该参数，则子目录下的目标不会被父目录下的目标文件包含进去，即父目录的CMakeLists.txt不会构建子目录的目标文件，必须在子目录下显式去构建。例外情况：当父目录的目标依赖于子目录的目标，则子目录的目标仍然会被构建出来以满足依赖关系（例如使用了target_link_libraries）。

### 2-5 add_executable命令

语法：

```CMake
add_executable (<name> [WIN32] [MACOSX_BUNDLE]
      [EXCLUDE_FROM_ALL]
      [source1] [source2 ...])
add_executable (<name> IMPORTED [GLOBAL])
add_executable (<name> ALIAS <target>)
```

作用：
使用指定的源文件来生成目标可执行文件。可执行文件分为三类：普通可执行目标文件、导入可执行目标文件、别名可执行目标文件

- **普通可执行目标文件**
  - name:可执行目标文件的名字，在一个cmake文件中，**这个名字必须全局唯一**
  - WIN32:用于windows系统下创建一个以WinMain为入口的可执行目标文件（通常入口函数为main），它不是一个控制台应用程序，而是一个GUI应用程序。当WIN32选项使用的时候，可执行目标的 WIN32_EXECUTABLE会被置位ON。
  - MACOSX_BUNDLE:用于mac系统或者IOS系统下创建一个GUI可执行应用程序，当MACOSX_BUNDLE选项使用的时候，可执行目标的MACOSX_BUNDLE会被置位ON。
  - EXCLUDE_FROM_ALL:用于指定可执行目标是否会被构建，当该选项使用的时候，可执行目标不会被构建。
  - \[source1] \[source2 ...]:构建可执行目标文件所需要的源文件。也可以通过target_sources()继续为可执行目标文件添加源文件，要求是在调用target_sources之前，可执行目标文件必须已经通过add_executable或add_library定义了。

- **导入可执行目标文件**

语法：
`add_executable(<name> IMPORTED [GLOBAL])`

作用：
将工程外部的可执行目标文件导入进来，不会有任何构建可执行目标文件的行为发生。

参数：

&#10052;**name**:导入可执行文件的名字
&#10052;**IMPORTED**:选项指定后，属性IMPORTED会被置为TRUE，在工程内构建的可执行目标文件的属性IMPORTED会被置为FALSE
&#10052;**GLABAL**:可执行目标文件的范围为文件创建的目录及子目录，指定GLOBAL则将范围扩大到整个工程

- **别名可执行目标文件**

语法：
`add_executable(<name> ALIAS <target>)`

作用：
为可执行目标文件创建一个别名。创建别名后，可以只用别名进行可执行目标的读、测试操作，但是不能利用别名对可执行目标的修改属性的操作。

## 三、其他
