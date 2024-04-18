#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>
#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Foundation.h>
#include <iostream>
#include <locale.h>


using namespace std;

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

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "用法: " << argv[0] << " <参数1> <参数2> ..." << std::endl;
        return 1;
    }
    _wsetlocale(LC_ALL, L"zh-CN");


    InitializeWinRT();

    winrt::hstring imagePath = L"C:\\Users\\jiaozhuzhang\\Desktop\\test.png";

    SoftwareBitmap softwareBitmap = LoadImage(imagePath);
    winrt::hstring resultText = PerformOcr(softwareBitmap);

    std::wcout << L"识别结果：" << resultText.c_str() << std::endl;

    return 0;
}