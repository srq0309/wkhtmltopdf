// Tester.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <cstdlib>
#include <string>

#define BUILDING_WKHTMLTOX
#include "../lib/pdf.h"

#pragma comment(lib, "wkhtmltox.lib")


/* Print out loading progress information */
void progress_changed(wkhtmltopdf_converter * c, int p)
{
    printf("%3d%%\r", p);
    fflush(stdout);
}

/* Print loading phase information */
void phase_changed(wkhtmltopdf_converter * c)
{
    const auto phase = wkhtmltopdf_current_phase(c);
    printf("%s\n", wkhtmltopdf_phase_description(c, phase));
}

/* Print a message to stderr when an error occurs */
void error(wkhtmltopdf_converter * c, const char * msg)
{
    fprintf(stderr, "Error: %s\n", msg);
}

/* Print a message to stderr when a warning is issued */
void warning(wkhtmltopdf_converter * c, const char * msg)
{
    fprintf(stderr, "Warning: %s\n", msg);
}

#include <Windows.h>
std::string GbkToUtf8(const std::string& strGbk)//传入的strGbk是GBK编码
{
    //gbk转unicode
    int len = MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, NULL, 0);
    wchar_t *strUnicode = new wchar_t[len];
    wmemset(strUnicode, 0, len);
    MultiByteToWideChar(CP_ACP, 0, strGbk.c_str(), -1, strUnicode, len);

    //unicode转UTF-8
    len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
    char * strUtf8 = new char[len];
    WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

    std::string strTemp(strUtf8);//此时的strTemp是UTF-8编码
    delete[]strUnicode;
    delete[]strUtf8;
    strUnicode = NULL;
    strUtf8 = NULL;
    return strTemp;
}

int main()
{
    // 使用图像设备初始化
    wkhtmltopdf_init(true);

    // [ps] global_settings实际用来初始化PdfConvertPrivate这个负责转换渲染后的网页为PDF的类
    const auto wk_global_settings = wkhtmltopdf_create_global_settings();
    // 输出结果
    wkhtmltopdf_set_global_setting(wk_global_settings, "out", "test.pdf");
    // 加载和存储cookie信息的文件
    wkhtmltopdf_set_global_setting(wk_global_settings, "load.cookieJar", "myjar.jar");

    // 添加水印
    // 启用水印A（共支持AB共2个）
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.use", "true");
    // 坐标系旋转
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.rotate", "-45");
    // 文字区域
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.left", "-0.9");
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.top", "0.0");
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.width", "1.5");
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.height", "1.0");
    // 水印颜色
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.color", "(0,160,230,80)");
    // 字体
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.font_family", "Microsoft YaHei");
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.font_size", "36");
    // 文字内容
    wkhtmltopdf_set_global_setting(wk_global_settings, "WaterMarkA.text", 
        GbkToUtf8("测试水印\n深圳市丽海弘金科技有限公司\nsangruiqiang@hongkingsystem.cn").c_str());

    const auto wk_object_settings = wkhtmltopdf_create_object_settings();
    /* We want to convert to convert the qstring documentation page */
    wkhtmltopdf_set_object_setting(wk_object_settings, "page", R"(../../../docs/libwkhtmltox/pdf_8h.html)");

    /* Create the actual converter object used to convert the pages */
    const auto wk_conv = wkhtmltopdf_create_converter(wk_global_settings);

    // [ps] 这四个回掉函数实际以槽函数连接到PdfConvertPrivate对应的信号

    // 当前进度百分百信息回掉函数
    wkhtmltopdf_set_progress_changed_callback(wk_conv, progress_changed);

    // 进度更变回掉函数
    wkhtmltopdf_set_phase_changed_callback(wk_conv, phase_changed);

    // 错误信息回掉函数
    wkhtmltopdf_set_error_callback(wk_conv, error);

    // 警告信息回掉函数
    wkhtmltopdf_set_warning_callback(wk_conv, warning);

    /*
    * Add the the settings object describing the qstring documentation page
    * to the list of pages to convert. Objects are converted in the order in which
    * they are added
    */
    // [ps] 增加PdfConvertPrivate的objects:PageObject(QWebPage)对象
    wkhtmltopdf_add_object(wk_conv, wk_object_settings, nullptr);

    /* Perform the actual conversion */
    if (!wkhtmltopdf_convert(wk_conv)) {

        fprintf(stderr, "Conversion failed!");

        /* Output possible http error code encountered */
        printf("httpErrorCode: %d\n", wkhtmltopdf_http_error_code(wk_conv));
    }

    // 清理资源
    wkhtmltopdf_destroy_converter(wk_conv);
    wkhtmltopdf_destroy_object_settings(wk_object_settings);
    wkhtmltopdf_destroy_global_settings(wk_global_settings);

    /* We will no longer be needing wkhtmltopdf funcionality */
    wkhtmltopdf_deinit();

    system("pause");
    return 0;
}

