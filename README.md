[Femu - Modern C++ Project Template]

本项目是一个现代 C++20 二进制应用程序模板，采用 CMake 构建系统，推荐搭配 Ninja 和 g++ 编译器。适合快速开始 C++ 项目开发。

## 项目结构
```
Femu/
├── src/            # 主程序源码
│   └── main.cpp    # 示例主程序
├── tests/          # 单元测试（可选，默认启用 Catch2）
│   ├── test_main.cpp
│   └── CMakeLists.txt
├── CMakeLists.txt  # 顶层 CMake 配置
├── README.md       # 项目说明
└── LICENSE         # 许可证
```

## 构建要求
- C++20 编译器（推荐 g++ 10 及以上）
- CMake >= 3.16
- 推荐 Ninja 作为构建后端（可选）

## 构建步骤
1. 安装依赖：
	- g++: `sudo apt install g++`
	- cmake: `sudo apt install cmake`
	- ninja（可选，推荐）：`sudo apt install ninja-build`

2. 生成构建目录并编译：
	```fish
	mkdir build
	cd build
	cmake -G Ninja ..   # 使用 Ninja
	# 或
	cmake ..            # 使用默认生成器
	cmake --build .
	```

3. 运行主程序：
	```fish
	./Femu
	```

## 单元测试（可选）
- 默认启用 Catch2 测试框架，自动下载并集成。
- 构建后运行测试：
	```fish
	cd build
	ctest
	```

## 关键配置说明
- `CMakeLists.txt` 自动设置 C++20 标准，推荐 g++，并支持 Ninja。
- 可通过 `FEMU_ENABLE_TESTS` 选项启用/禁用测试。
- 测试框架可选 Catch2（默认），可扩展为 Google Test。

## 现代 C++最佳实践
- 使用 C++20 标准
- 清晰的项目结构，易于扩展
- 代码风格简洁，注重可读性

---

如需自定义或扩展，请参考 CMake 文档和 Catch2 官方文档。
