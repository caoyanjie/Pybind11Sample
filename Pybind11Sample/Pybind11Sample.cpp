// PyBindTest1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

#include "pybind11.h"
#include "embed.h"
#include <algorithm>

#include <fstream>
#include <streambuf>
#include <sstream>

// 测试枚举
enum TestEnum
{
	NODE,
	TASK_INPUT,
	TRIGGER_OUTPUT,
	TRIGGER_INPUT
};

enum PythonGuiAlign : int
{
	ALIGN_CENTER = 1 << 0,			// center align
	ALIGN_LEFT = 1 << 1,			// left align
	ALIGN_RIGHT = 1 << 2,			// right align
	ALIGN_TOP = 1 << 3,				// top align
	ALIGN_BOTTOM = 1 << 4,			// bottom align
	ALIGN_EXPAND = 1 << 5,			// expanded widget
	ALIGN_OVERLAP = 1 << 6,			// overlapped widget
	ALIGN_BACKGROUND = 1 << 7,		// background widget
	ALIGN_FIXED = 1 << 8,			// fixed depth
};

// 测试函数
void testFunc()
{
	std::cout << "this is C++ api testFunc()"  << std::endl;
}

// 默认参数函数
void testDefaultArgFunc(int a = 8)
{
	std::cout << "default arg: " << a << std::endl;
}

// 测试普通类
class TestClass
{
public:
	TestClass() : m_name("cao"), m_age(18), m_defaultV(0) { }

	TestClass(const std::string& name, int age, int defaultV=8) : m_name(name), m_age(age), m_defaultV(defaultV) { }

	void printInfo()
	{
		std::cout << "this is C++ api class: " << "我叫" << m_name << ", " << m_age << "岁了 default: " << m_defaultV << std::endl;
	}

	static int staticMethod(int v)
	{
		std::cout << "this is static method" << std::endl;
		return v;
	}

private:
	std::string 	m_name;
	int				m_age;
	int				m_defaultV;
};

// 无构造函数类
class ExportFile
{
public:
	const char* read(char *fileName)
	{
		return "helloworld";
	}
private:
	std::string m_fileContents;
};

// 测试单例类
class TestSingleton
{
public:
	static TestSingleton* getInstance()
	{
		static TestSingleton s_instance;
		return &s_instance;
	}

	void setInfo(const std::string& name, int age)
	{
		m_name = name;
		m_age = age;
	}

	void printInfo()
	{
		std::cout << "name: " << m_name << " age: " << m_age << std::endl;
	}

private:
	TestSingleton()
	{
		m_name = "c";
		m_age = 18;
	}

	std::string m_name;
	int			m_age;
};

// 测试继承类
class BaseClass
{
public:
	BaseClass(std::string name) : name(name), value1(0), m_value2(0) { }

	BaseClass(int value1, int value2) : name("")
	{
		this->value1 = value1;
		m_value2 = value2;
	}

	std::string getName()
	{
		return name;
	}

	int getValue2()
	{
		return m_value2;
	}

	std::string		name;
	int				value1;

private:
	int				m_value2;
};

class ChildClass : public BaseClass
{
public:
	ChildClass(std::string name) : BaseClass(name) { }
};

void process(BaseClass* baseClass)
{
}

// 导出枚举
void exportEnum(pybind11::module& m)
{
	pybind11::enum_<TestEnum>(m, "TestEnum")
		.value("NODE", TestEnum::NODE)
		.value("TASK_INPUT", TestEnum::TASK_INPUT)
		.value("TRIGGER_OUTPUT", TestEnum::TRIGGER_OUTPUT)
		.value("TRIGGER_INPUT", TestEnum::TRIGGER_INPUT);

	pybind11::enum_<PythonGuiAlign>(m, "ALIGN", pybind11::arithmetic())
		.value("ALIGN_CENTER", PythonGuiAlign::ALIGN_CENTER)
		.value("ALIGN_LEFT", PythonGuiAlign::ALIGN_LEFT)
		.value("ALIGN_RIGHT", PythonGuiAlign::ALIGN_RIGHT)
		.value("ALIGN_TOP", PythonGuiAlign::ALIGN_TOP)
		.value("ALIGN_BOTTOM", PythonGuiAlign::ALIGN_BOTTOM)
		.value("ALIGN_EXPAND", PythonGuiAlign::ALIGN_EXPAND)
		.value("ALIGN_OVERLAP", PythonGuiAlign::ALIGN_OVERLAP)
		.value("ALIGN_BACKGROUND", PythonGuiAlign::ALIGN_BACKGROUND)
		.value("ALIGN_FIXED", PythonGuiAlign::ALIGN_FIXED)
		.export_values();
}

// 导出函数
void exportFunction(pybind11::module& m)
{
	///m.def("test_func", &testFunc, "test");
	m.def("test_func", &testFunc, pybind11::return_value_policy::reference);

	m.def("test_default_arg_func", &testDefaultArgFunc, pybind11::arg("arg")=(int)8);
}

// 导出普通类
void exportNormalClass(pybind11::module& m)
{
	pybind11::class_<TestClass>(m, "TestClass")
		.def(pybind11::init<>())
		//.def(pybind11::init<const std::string&, int>())
		//.def(pybind11::init<const std::string&, int, int>())
		.def(pybind11::init<const std::string&, int, int>(), pybind11::arg("name"), pybind11::arg("age"), pybind11::arg("defaultV") = 8)
		//.def(pybind11::init([](const std::string& a, int b, int c = 20) { return new TestClass(a, b, c); }))
		.def("print_info", &TestClass::printInfo)
		//.def("static_method", &TestClass::staticMethod)
		.def_static("static_method", (int(*)(int))&TestClass::staticMethod)
		;

	// 导出无构造函数类
	pybind11::class_<ExportFile>(m, "File")
		.def(pybind11::init<>())
		.def("read", &ExportFile::read);
}

// 导出单例类
void exportSingletonClass(pybind11::module& m)
{
	//pybind11::class_<TestSingleton, pybind11::nodelete>(m, "TestSingle")
	//	.def("print_info", &TestSingleton::printInfo);

	//pybind11::class_<TestSingleton, std::unique_ptr<TestSingleton, pybind11::nodelete>>(m, "TestSingleton")
	//	.def(pybind11::init([]() { return std::unique_ptr<TestSingleton, pybind11::nodelete >> (&TestSingleton::getInstance()); });

	pybind11::class_<TestSingleton, std::unique_ptr<TestSingleton, pybind11::nodelete>>(m, "TestSingleton")
		//.def(pybind11::init<>());
		.def("print_info", &TestSingleton::printInfo);
}

// 导出继承类
void exportCClass(pybind11::module& m)
{
#pragma region WidgetButton
	// 导出继承类
	pybind11::class_<BaseClass>(m, "BaseClass")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<int, int>())
		.def("get_name", &BaseClass::getName)
		.def_readonly("value1", &BaseClass::value1);
		//.add_property("value2", &BaseClass::getValue2);
	///pybind11::class_<ChildClass, boost::python::bases<BaseClass>>("ChildClass", boost::python::init<std::string>());
	pybind11::class_<ChildClass, BaseClass>(m, "ChildClass")
		.def(pybind11::init<std::string>());
	m.def("process", &process);
#pragma endregion
}

// 导出模板
PYBIND11_EMBEDDED_MODULE(MyTestModule, m) 
//PYBIND11_MODULE(MyTestModule, m) 
{
	exportEnum(m);
	exportFunction(m);
	exportNormalClass(m);
	exportSingletonClass(m);
	exportCClass(m);
}

void getExceptionInfo()
{
	PyObject *_exc, *_val, *_tb;
	PyErr_Fetch(&_exc, &_val, &_tb);
	PyErr_NormalizeException(&_exc, &_val, &_tb);

	pybind11::handle _hexc(_exc), _hval(_val), _htb(_tb);
	std::string _errorInfo;
	if (!_hval)
	{
		///_errorInfo = boost::python::extract<const char*>(boost::python::str(_hexc));
		//_errorInfo = boost::python::extract<const char*>(boost::python::str(_hexc));
		int a = 8;
	}
	else
	{
		///boost::python::object _formatException(boost::python::import("traceback").attr("format_exception"));
		///boost::python::object _formattedList(_formatException(_hexc, _hval, _htb));
		///boost::python::object _formatted(boost::python::str("").join(_formattedList));
		pybind11::object _formatException(pybind11::module::import("traceback").attr("format_exception"));
		pybind11::object _formattedList(_formatException(_hexc, _hval, _htb));
		//pybind11::object _formatted(pybind11::str("").join(_formattedList));
		//_errorInfo = boost::python::extract<const char*>(_formatted);
	}
	std::string _replace("<string>");
	std::string _baseName = "base_name.py";

	if (_errorInfo.find(_replace) != _errorInfo.npos)
	{
		_errorInfo.replace(_errorInfo.find(_replace), _replace.length(), _baseName);
	}

	std::cout << "Error: " << _errorInfo.c_str() << std::endl;
}

// 执行 python 脚本
void runPythonFile(const std::string& fileFullPath)
{
	std::string _fileFullPath = fileFullPath;
	std::replace(_fileFullPath.begin(), _fileFullPath.end(), '\\', '/');

	// 初始化解释器
	pybind11::scoped_interpreter python{};

	// 添加模块搜索目录
	std::string _cmd = "import sys \nimport os \nsys.path.append(os.path.split('" + _fileFullPath + "')[0])";
	//PyRun_SimpleString(_cmd.c_str());
	pybind11::exec(_cmd);

	// 从文件路径中提取模块名
	auto _fileStart = _fileFullPath.rfind("/");
	std::string _filename = _fileFullPath.substr(_fileStart+1, _fileFullPath.size()-3);
	std::string _moduleName = _filename.substr(0, _filename.size() - 3);

	// 读取源码
	std::ifstream t(_fileFullPath);
	std::string _codeContent((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	// 执行 python 文件
	//pybind11::module _m = pybind11::module::import(_moduleName.c_str());
	pybind11::object _module = pybind11::module::import("__main__").attr("__dict__");
	try
	{
		pybind11::exec(pybind11::str(_codeContent), _module, _module);
	}
	catch (const pybind11::error_already_set& e)
	{
		std::cout << e.what() << std::endl;
		PyErr_SetString(PyExc_RuntimeError, e.what());
		std::cout << e.what() << std::endl;
		getExceptionInfo();
	}
	///pybind11::object _obj = _module["ca"]();
	///_obj.attr("testa")();

	// 指针转换
	//TestClass* _tc = new TestClass("c", 2);
	//pybind11::object _obj1 = pybind11::cast(_tc); 
	//_obj1.attr("print_info")();

	// 空值
	//try 
	//{
	//	//pybind11::object _obj = _module.attr("__dict__");
	//	pybind11::object _obj = pybind11::dict();
	//	_obj["aaa"] = pybind11::none();
	//}
	//catch (const pybind11::error_already_set& e)
	//{
	//	std::cout << e.what() << std::endl;
	//}

}

void runTest(const std::string& fileFullPath)
{
	pybind11::scoped_interpreter python{};

	// 读取源码
	std::ifstream t(fileFullPath);
	std::string _codeContent((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	// 执行 python 文件
	pybind11::object _module = pybind11::module::import("__main__").attr("__dict__");
	try
	{
		pybind11::exec(pybind11::str(_codeContent), _module, _module);
	}
	catch (const pybind11::error_already_set& e)
	{
		std::cout << e.what() << std::endl;
	}
}

int main()
{
	///pybind11::call_method
	///pybind11::import
	///pybind11::ptr
	///pybind11::extra

	///pybind11::object 
	//pybind11::list _args{};
	//_args.append("he");
	///pybind11::str
	///pybind11::len()
	///pybind11::handle

	// 导出模板时，如果用PYBIND11_EMBEDDED_MODULE则不需要此行，如果用PYBIND11_MODULE则需要此行
	// PyImport_AppendInittab("MyTestModule", &PyInit_MyTestModule);


	//while (true)
	//{
		runPythonFile("C:\\Users\\caoyanjie\\Desktop\\test\\python\\test_file.py");
		//runTest("C:\\Users\\caoyanjie\\Desktop\\test\\python\\test_file.py");
		//Sleep(5000);
	//}

}


