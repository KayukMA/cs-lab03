#include "histogram.h"
#include <vector>
#include <iostream>
using namespace std;

void find_minmax(const vector<double>& numbers, double& min, double& max, size_t number_count)
{
    int p=numbers.size();
    if(p!=0)
        {
            min = numbers[0];
            max = numbers[0];
            for (size_t i=0; i<number_count; i++)
                {
                    if (numbers[i] < min)
                    {
                        min = numbers[i];
                    }
                    if (numbers[i] > max)
                        {
                            max = numbers[i];
                        }
                }
        }
}
