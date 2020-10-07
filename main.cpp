#include <curl/curl.h>
#include "histogram.h"
#include "svg.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <stdio.h>

DWORD WINAPI GetVersion(void);
using namespace std;

struct Input
{
    vector<double> numbers;
    size_t bin_count;
}
;

vector<double> input_numbers(istream&in, size_t count)
{
    vector<double> result(count);
    for (size_t i = 0; i < count; i++)
    {
        in >> result[i];
    }
    return result;
}

vector<size_t> make_histogram(struct Input Input)
{
    double min=0;
    double max=0;
    find_minmax(Input, min, max);
    vector<size_t> bins(Input.bin_count);
    for (double number : Input.numbers)
    {
        size_t bin = (size_t)(number - min);
        if (bin == Input.bin_count)
        {
            bin--;
        }
        bins[bin]++;
    }
    return bins;
}
/*����� �� �����������
void show_histogram_text(const vector<size_t>& bins)
{
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;
    size_t max_count = 0;
    for (size_t count : bins)
    {
        if (count > max_count)
        {
            max_count = count;
        }
    }
    const bool scaling_needed = max_count > MAX_ASTERISK;
    for (size_t bin : bins)
    {
        if (bin < 100)
        {
            cout << ' ';
        }
        if (bin < 10)
        {
            cout << ' ';
        }
        cout << bin << "|";
        size_t height = bin;
        if (scaling_needed)
        {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }
        for (size_t i = 0; i < height; i++)
        {
            cout << '*';
        }
        cout << '\n';
    }
}
*/

Input
read_input(istream&in, bool prompt)
{
    Input data;
    size_t number_count;
    if (prompt==true)
    {
        cerr<<"Enter number count: ";
    }
    in >> number_count;
    if (prompt==true)
    {
        cerr<<"Enter numbers: ";
    }
    data.numbers=input_numbers(cin, number_count);
    size_t bin_count;
    if (prompt==true)
    {
    cerr << "Enter column count: ";
    }
    in >> data.bin_count;
    return data;
}
size_t write_data(void* items, size_t item_size, size_t item_count, void* ctx)
{
    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    const char* char_items = reinterpret_cast<const char*>(items);
    size_t data_size = item_size * item_count;
    buffer->write(char_items, data_size);
    return data_size;
}

Input
download(const string& address)
{
    stringstream buffer;
    char *ip;
    CURL* curl =curl_easy_init();
   if(curl)
   {
       CURLcode res;
       curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        res = curl_easy_perform(curl);
        if(res!=CURLE_OK)
            {
                cout << curl_easy_strerror(curl_easy_perform(curl));
                exit(1);
            }
        curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip);
        cerr << "IP:" << ip;
        curl_easy_cleanup(curl);
   }
    return read_input(buffer, false);
}

int main(int argc, char* argv[])
{
    DWORD dwVersion = 0;
    dwVersion = GetVersion();
    DWORD info = GetVersion();
    DWORD mask = 0x0000ffff;
    DWORD platform = info >> 16;
    DWORD version = info & mask;
    DWORD mask_major = 0b00000000'00000000'00000000'11111111;
    DWORD version_major = version & mask_major;
    DWORD version_minor = version >> 8;
    printf("Win 16-x version is %x\n", version);
    printf("Win decimal-version is %u\n", version);
    printf("Win major version is %x\n", version_major);
    printf("Win minor version is %u\n", version_minor);
    Input input;
    if(argc>1)
    {
        input= download(argv[1]);
    }
    else
    {
        input= read_input(cin, true);
    }
    const auto bins=make_histogram(input);
    show_histogram_svg(bins);
    return(0);
}
