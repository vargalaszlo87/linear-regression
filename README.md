# linR

The **linR** is an implementation of linear-regression in C (C99). You can use it for simple linear predection. 

# Getting started

#### Files
- **linr.h** header file of this project, the place of structures too.
- **linr.c** core of the project. 
- **main.c** example file, you can see the using.

#### Self compiling
If you use **`Windows`**

```
gcc -std=c99 -c main.c -o main.o -I.
gcc -std=c99 -c linr.c -o linr.o -I.
gcc main.o linr.o -o linr.exe
```

If you use **`Linux`**

```
gcc -std=c99 -c main.c -o main.o -I. -lm
gcc -std=c99 -c linr.c -o linr.o -I. -lm
gcc main.o linr.o -o linr
```

#### Methods (just the importants)
- **linRInit(Data, Result)** allocate the memories
- **linRPushData(Data, Result, X Value, Y Value)** push the X and Y datas for memory
- **linRCalc(Result)** calculation the y = mx + b equation
- **linRShow(Result)** writes to result to the screen
- **linRPredictY** make a Y(X) predirection
- **linRErrorSquare** gives the square error

# Usage
```C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "linr.h"

int main(int argc, char *argv[]) {

    Data data;
    Result result;

    // TEST DATAS
    double XY[84][2] = {
        {1714, 2.40},
        {1664, 2.52},
        {1760, 2.54},
        {1685, 2.74},
        {1693, 2.83},
        {1670, 2.91},
        {1764, 3.00},
        {1764, 3.00},
        {1792, 3.01},
        {1850, 3.01},
        {1735, 3.02},
        {1775, 3.07},
        {1735, 3.08},
        {1712, 3.08},
        {1773, 3.12},
        {1872, 3.17},
        {1755, 3.17},
        {1674, 3.17},
        {1842, 3.17},
        {1786, 3.19},
        {1761, 3.19},
        {1722, 3.19},
        {1663, 3.20},
        {1687, 3.21},
        {1974, 3.24},
        {1826, 3.28},
        {1787, 3.28},
        {1821, 3.28},
        {2020, 3.28},
        {1794, 3.28},
        {1769, 3.28},
        {1934, 3.28},
        {1775, 3.29},
        {1855, 3.29},
        {1880, 3.29},
        {1849, 3.31},
        {1808, 3.32},
        {1954, 3.34},
        {1777, 3.37},
        {1831, 3.37},
        {1865, 3.37},
        {1850, 3.38},
        {1966, 3.38},
        {1702, 3.39},
        {1990, 3.39},
        {1925, 3.40},
        {1824, 3.40},
        {1956, 3.40},
        {1857, 3.41},
        {1979, 3.41},
        {1802, 3.41},
        {1855, 3.42},
        {1907, 3.42},
        {1634, 3.42},
        {1879, 3.44},
        {1887, 3.47},
        {1730, 3.47},
        {1953, 3.47},
        {1781, 3.47},
        {1891, 3.48},
        {1964, 3.49},
        {1808, 3.49},
        {1893, 3.50},
        {2041, 3.51},
        {1893, 3.51},
        {1832, 3.52},
        {1850, 3.52},
        {1934, 3.54},
        {1861, 3.58},
        {1931, 3.58},
        {1933, 3.59},
        {1778, 3.59},
        {1975, 3.60},
        {1934, 3.60},
        {2021, 3.61},
        {2015, 3.62},
        {1997, 3.64},
        {2020, 3.65},
        {1843, 3.71},
        {1936, 3.71},
        {1810, 3.71},
        {1987, 3.73},
        {1962, 3.76},
        {2050, 3.81}
    };
    data.N = 84;
    // /TEST DATAS

    // Init
    linRInit(&data, &result);

    // Push datas
    for (int i = 0; i < data.N; i++)
        linRPushData(&data, &result, XY[i][0], XY[i][1]);

    // Calc equation y = mx + b
    linRCalc(&result);
    // Show the eq.
    linRShow(&result);

    // Predict: which Y belongs to this X
    double X = 2050.123;
    double predictedY = linRPredict(&result, X);
    double err2 = linRErrorSquare(&data, &result);

    printf ("Predicted Y = f(%lf) = %lf with %lf error^2\n", X, predictedY, err2);


    return 0;
}
```

# Contact

Varga Laszlo - https://vargalaszlo.com - mail@vargalaszlo.com.com

Project Link: https://github.com/vargalaszlo87/linear-regression

[![portfolio](https://img.shields.io/badge/my_portfolio-000?style=for-the-badge&logo=ko-fi&logoColor=white)](http://vargalaszlo.com)
