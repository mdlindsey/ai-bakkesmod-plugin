# BakkesMod Template Contribution Guide

Each subheader outlines a task that needs to be done.

## imgui <> vcpkg

While all other dependencies and instances of `imgui` have been replaced with those coming from `vckpg`, there is still one notable dependency in `./BakkesPlugin/pch/pch.h`:
```c++
#include "imgui/imgui.h"
```

The BakkesMod Plugin Manager (F2) settings UI for each plugin relies on `imgui` for rendering settings. In many cases a successful build is not a guarantee of success as the settings view can still crash the game.
