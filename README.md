# Summary

This project utilises the `SetWindowsHookEx` function in order to insert a hook in the keyboard's hook chain. The software essentially acts as a proxy and replaces input with the user provided input, when the user provided input runs out the hook is uninstalled from the hook chain, and the system continues as usual.

# Installation
Create a new c++ project and download the source code from this repository with
```
git clone https://github.com/guysudai1/force_write/
```

# Usage
1. Replace the `desired_string` variable with your own string.
2. Run the software.
