# Indicator
A simple CLI spinner and progress bar for C++.  
I got inspired by [indicators](https://github.com/p-ranav/indicators) and [progressbar](https://github.com/gipert/progressbar).
## How to use
Here's an example.  
  
![indicator_demo](https://github.com/user-attachments/assets/bff34323-2478-4636-ad67-643d2f52dbb3)
```cpp
#include <indicator/spinner.hpp>
#include <indicator/progressBar.hpp>

using namespace std;

int main()
{
    // hide cursor
    indicator::hideCursor();

    // local variables
    indicator::Spinner s({'>', 'v', '<', '^'}, 125);
    indicator::ProgressBar pb('[', '=', ' ', ']', 36, -10.f, 35.f);

    // spinner
    printf("testing spinner: ");

    s.play();
    indicator::delay_ms(3000);

    s.stop();
    printf("done!\n");

    // progress bar
    printf("testing progress bar: ");

    pb.play();
    for(int i=-10; i<=35; i++)
    {
        pb.setValue(i);
        indicator::delay_ms(100);
    }

    pb.stop();
    printf("done!\n");
    
    return 0;
}
```
