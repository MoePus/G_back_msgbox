# G-back MessageBox

*on windows 8.1+ 32bit only*

*only supports wide string*
##Demo

```
#include <iostream>
#include "gbbox.h"

int main()
{
    GBMSGBOX gb;
	gb.GBBOX(L"Text", L"Caption", 0, { L"OK", L"OK" ,L"OK" ,L"OK" ,L"OK" ,L"OK" ,L"OK" ,L"OK" }, 0, -1, [](HWND hWnd) {std::cout << "hooked handle:"<<hWnd; });
    return 0;
}
```

Just Try It.