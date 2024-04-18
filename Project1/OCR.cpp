#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Foundation.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <locale.h>
#include <codecvt>
#include <filesystem>


using namespace std;
namespace fs = std::filesystem;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Ocr;
using namespace Windows::Globalization;
using namespace winrt::Windows::Foundation;

void InitializeWinRT()
{
    init_apartment();
}

SoftwareBitmap LoadImage(const winrt::hstring& imagePath)
{
    StorageFile imageFile = StorageFile::GetFileFromPathAsync(imagePath).get();
    IRandomAccessStream fileStream = imageFile.OpenAsync(FileAccessMode::Read).get();
    BitmapDecoder decoder = BitmapDecoder::CreateAsync(fileStream).get();
    SoftwareBitmap softwareBitmap = decoder.GetSoftwareBitmapAsync().get();

    return softwareBitmap;
}

winrt::hstring PerformOcr(const SoftwareBitmap& softwareBitmap)
{
    winrt::hstring languageTag = L"zh-CN";
    Windows::Globalization::Language lang(languageTag);
    OcrEngine ocrEngine = OcrEngine::TryCreateFromLanguage(lang);
    OcrResult ocrResult = ocrEngine.RecognizeAsync(softwareBitmap).get();
       
    // 遍历 OCR 结果中的每一行


    return ocrResult.Text();
}

std::wstring toWstring(const char* utf8str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &utf8str[0], -1, NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &utf8str[0], -1, &wstrTo[0], size_needed);
    return wstrTo;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "用法: " << argv[0] << " <image path> <language> ..." << std::endl;
        return 1;
    }

    std:string imgPath = string(argv[1]);
    if (!fs::exists(imgPath)) {
        std::cout << "文件不存在" << std::endl;
        return 1; // 或者返回其他您认为合适的值
    }


    winrt::hstring imagePath = winrt::to_hstring(imgPath);
    // winrt:: language = winrt::to_hstring(argv[2]);
    // std::cout << L"imagePath：" << to_string(imagePath) << std::endl;

    // 设置目标宽字符字符串的缓冲区大小
    const int bufferSize = 50;
    wchar_t language[bufferSize];

    // 调用MultiByteToWideChar进行转换
    int result = MultiByteToWideChar(CP_UTF8, 0, argv[2], -1, language, bufferSize);

    _wsetlocale(LC_ALL, language); // L"zh-CN"
    

    InitializeWinRT();

    // winrt::hstring imagePath = L"C:\\Users\\jiaozhuzhang\\Desktop\\test.png";

    SoftwareBitmap softwareBitmap = LoadImage(imagePath);
    winrt::hstring resultText = PerformOcr(softwareBitmap);

    std::wcout << L"result:" << resultText.c_str() << std::endl;

    return 0;
}