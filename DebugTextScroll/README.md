# DebugTextScroll

The DebugTextScroll subsystem prints log messages to the screen with color, making it more obvious when an error occurs.

## Usage

Simply include the header and register the subsystem. It will automatically start printing log messages to the screen.

```cpp
#include <Urho3D/DebugTextScroll/DebugTextScroll.h>

void MyApplication::Start() {
#ifdef DEBUG
    RegisterSubsystem<DebugTextScroll>();
#endif
}
```


