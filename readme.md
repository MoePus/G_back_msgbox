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
	gb.GBBOX(L"Text", L"Caption", 0, { L"OK()", L"CANCEL"}, GBMSGBOX::GB_NO_MIRROR , -1, [](HWND hWnd) {std::cout << "hooked handle:"<<hWnd; });
    return 0;
}
```
##GBBOX mirror options

* GB_NO_MIRROR
* GB_CAPTION_MIRROR
* GB_TEXT_MIRROR
* GB_TEXTANDCAPTION_MIRROR

Just Try It.

