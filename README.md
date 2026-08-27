# linR

**linR** is a small and lightweight simple linear regression library written in C99.

It calculates a linear model in the form:

```text
y = mx + b
```

where:

* `m` is the slope
* `b` is the intercept

The library can be used to build a regression model from `(x, y)` data points, make predictions, calculate residual errors, calculate the residual sum of squares, and determine the coefficient of determination (`R²`).

The implementation uses an online algorithm, which means the original data points do not have to be stored in memory. New observations can be added one by one while the regression statistics are updated incrementally.

## Features

* Written in C99
* Simple API
* No dynamic memory allocation
* No need to store the complete dataset
* Incremental / online data processing
* Linear prediction
* Residual error calculation
* Residual Sum of Squares (RSS)
* Total Sum of Squares (TSS)
* R² calculation
* Basic input validation
* More numerically stable than the direct `sum(x)` / `sum(x²)` approach

## Files

* **`linr.h`** — public API and the `Result` structure
* **`linr.c`** — linear regression implementation
* **`main.c`** — example program

## Building

### Linux

```bash
gcc -std=c99 -Wall -Wextra -pedantic -c linr.c -o linr.o
gcc -std=c99 -Wall -Wextra -pedantic -c main.c -o main.o
gcc main.o linr.o -o linr -lm
```

Run:

```bash
./linr
```

### Windows / MinGW

```bash
gcc -std=c99 -Wall -Wextra -pedantic -c linr.c -o linr.o
gcc -std=c99 -Wall -Wextra -pedantic -c main.c -o main.o
gcc main.o linr.o -o linr.exe
```

Run:

```bash
linr.exe
```

Depending on the compiler/toolchain, you may also need to link the math library explicitly.

## Usage

A minimal example:

```c
#include <stdio.h>

#include "linr.h"

int main(void)
{
    Result result;
    double prediction;

    linRInit(&result);

    linRPush(&result, 1.0, 2.1);
    linRPush(&result, 2.0, 4.0);
    linRPush(&result, 3.0, 6.2);
    linRPush(&result, 4.0, 8.1);

    if (!linRCalc(&result)) {
        fprintf(stderr, "Linear regression calculation failed.\n");
        return 1;
    }

    linRShow(&result);

    prediction = linRPredict(&result, 5.0);

    printf("Prediction for x = 5: %lf\n", prediction);
    printf("RSS: %lf\n", linRErrorSquare(&result));
    printf("R^2: %lf\n", linRR2(&result));

    return 0;
}
```

## Working with an array

Data can also be stored in an ordinary C array and pushed into the model:

```c
#include <stdio.h>
#include <stddef.h>

#include "linr.h"

int main(void)
{
    Result result;

    double XY[][2] = {
        {1714, 2.40},
        {1664, 2.52},
        {1760, 2.54},
        {1685, 2.74},
        {1693, 2.83},
        {1670, 2.91},
        {1764, 3.00},
        {1792, 3.01},
        {1850, 3.01},
        {1775, 3.07}
    };

    size_t i;
    size_t n;
    double x;
    double predictedY;

    n = sizeof(XY) / sizeof(XY[0]);

    linRInit(&result);

    for (i = 0; i < n; i++) {
        if (!linRPush(&result, XY[i][0], XY[i][1])) {
            fprintf(stderr, "Invalid data point at index %zu.\n", i);
            return 1;
        }
    }

    if (!linRCalc(&result)) {
        fprintf(stderr, "Linear regression calculation failed.\n");
        return 1;
    }

    linRShow(&result);

    x = 2050.123;
    predictedY = linRPredict(&result, x);

    printf("Predicted Y = f(%lf) = %lf\n", x, predictedY);
    printf("RSS = %lf\n", linRErrorSquare(&result));
    printf("R^2 = %lf\n", linRR2(&result));

    return 0;
}
```

## API

### `linRInit`

```c
void linRInit(Result *r);
```

Initializes a `Result` structure.

Example:

```c
Result result;

linRInit(&result);
```

This must be called before adding data points.

---

### `linRPush`

```c
bool linRPush(Result *r, double x, double y);
```

Adds a new `(x, y)` observation to the regression model.

Example:

```c
linRPush(&result, 10.0, 25.0);
```

Returns `false` if the input is invalid.

Adding a new point invalidates a previously calculated model, so `linRCalc()` must be called again before making predictions.

---

### `linRCalc`

```c
bool linRCalc(Result *r);
```

Calculates the regression equation:

```text
y = mx + b
```

Returns `false` if the model cannot be calculated, for example when:

* fewer than two data points have been added
* all X values are identical
* the resulting coefficients are not finite

Example:

```c
if (!linRCalc(&result)) {
    fprintf(stderr, "Regression failed.\n");
    return 1;
}
```

---

### `linRShow`

```c
void linRShow(const Result *r);
```

Prints the calculated regression equation.

Example output:

```text
y = 0.002315x + -0.912541
```

---

### `linRPredict`

```c
double linRPredict(const Result *r, double x);
```

Predicts the Y value belonging to a given X value.

Example:

```c
double y;

y = linRPredict(&result, 2050.0);
```

The function returns `NAN` if the model has not been calculated or the input is invalid.

---

### `linRError`

```c
double linRError(const Result *r, double x, double y);
```

Returns the residual error for a given observation:

```text
error = actual_y - predicted_y
```

Example:

```c
double error;

error = linRError(&result, 2050.0, 3.81);
```

---

### `linRErrorSquare`

```c
double linRErrorSquare(const Result *r);
```

Returns the Residual Sum of Squares:

```text
RSS = Σ(yi - ŷi)²
```

A smaller RSS generally means that the regression line fits the observations more closely.

---

### `linRTotalSquare`

```c
double linRTotalSquare(const Result *r);
```

Returns the Total Sum of Squares:

```text
TSS = Σ(yi - mean(y))²
```

---

### `linRR2`

```c
double linRR2(const Result *r);
```

Returns the coefficient of determination:

```text
R² = 1 - RSS / TSS
```

For a normal linear regression model, values closer to `1.0` indicate that the model explains more of the variation in Y.

The function returns `NAN` when R² is undefined.

## How it works

Instead of storing every data point, **linR** maintains running statistical values:

```c
typedef struct {
    size_t n;
    double meanX;
    double meanY;
    double Sxx;
    double Sxy;
    double Syy;
    double m;
    double b;
    bool calculated;
} Result;
```

The regression coefficients are calculated from these accumulated values.

The slope is:

```text
m = Sxy / Sxx
```

and the intercept is:

```text
b = meanY - m * meanX
```

This approach has two useful properties:

1. Memory usage stays constant regardless of the number of observations.
2. It avoids some of the numerical problems caused by directly evaluating expressions such as:

```text
N * Σx² - (Σx)²
```

with large values.

## Streaming data

Because observations are processed incrementally, the library can also be used with streaming data:

```c
Result result;

linRInit(&result);

while (get_next_sample(&x, &y))
    linRPush(&result, x, y);

if (linRCalc(&result))
    linRShow(&result);
```

The memory used by `Result` does not increase as more observations are added.

## Important notes

`linRPush()` does not automatically recalculate the regression coefficients.

For example:

```c
linRPush(&result, 1.0, 2.0);
linRPush(&result, 2.0, 4.0);

linRCalc(&result);

linRPush(&result, 3.0, 6.0);
```

After the last `linRPush()`, the model becomes invalid and must be recalculated:

```c
linRCalc(&result);
```

before calling `linRPredict()` again.

The library also rejects `NAN` and infinite input values.

## Limitations

This project implements **simple linear regression with one independent variable**.

It does not currently provide:

* multiple linear regression
* polynomial regression
* weighted regression
* automatic outlier detection
* confidence intervals
* hypothesis testing
* storage of individual observations

Because individual observations are not stored, the library cannot later enumerate the original dataset. Applications that need this functionality should store the data separately.

## License

See the repository for license information.

## Contact

**Varga Laszlo**

Website: https://vargalaszlo.com
Email: [mail@vargalaszlo.com](mailto:mail@vargalaszlo.com)
Project: https://github.com/vargalaszlo87/linear-regression

[![portfolio](https://img.shields.io/badge/my_portfolio-000?style=for-the-badge\&logo=ko-fi\&logoColor=white)](http://vargalaszlo.com)
