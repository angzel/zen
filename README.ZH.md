# 关于Zen

Zen包含了多种功能，这些功能适用于不同的应用场景。建议将Zen当作一系列可复用的源代码，在使用时，将相关源文引入目标工程。
如果需要定义静态库或动态库，可自行选择相关功能的源文件来创建库工程。

## 源文件结构说明

源代码位于source目录下，依据不同功能，分为不同模块。在source目录的子目录是模块分组（如basic为基础模块组），组内包含多个相关模块，每个模块目录命名格式为zen-xxx。
如数学模块位于source/basic/zen-math目录下。模块之间可能存在依赖关系，阅读模块目录下的README可获得相关信息。

## 添加模块的不同情形

1）相关模块只包含头文件，且不依赖其它模块或库时，直接将模块目录添加到c++工程的头文件包含路径中即可。

2）模块中包含源文件（cpp、mm、m文件）时，还需要添加相关源文件到目标工程编译列表中。

3）若依赖其它模块，添加依赖的模块。

4）若依赖第三方库，添加对第三方库的引用。

## 示例
以[sample/image_convert](sample/image_convert) 为例：这个工程主要用到zen-image，zen-jpg，zen-png模块；这三个模块又依赖基础库（basic/*）。
此外，zen-jpg和zen-png分别依赖外部的jpg和png的sdk开发库。

## 使用第三方库
仓库zen-external.git中包含了一些已编译好的第三方库（include和prebuilt），建议将其下external目录软链接到本仓库的根目录下（.gitignore中设为忽略）。
使用时可添加如下的搜索路径（其中“mac”字段应与相应系统一致）到目标工程中。

``` makefile
HEADER_SEARCH_PATHS = "$(SRCROOT)/../../external/mac/include"
LIBRARY_SEARCH_PATHS = "$(SRCROOT)/../../external/mac/prebuilt"
```

另，添加prebuilt下的静态库到工程静态库列表中。
