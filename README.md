### GRAPREP
---
**Grad**ient **Rep**resentation

Represents every byte of a specified file using an RGB gradient.
---
To be used it must be launched frm the command line.
Using `GRADREP  /?` will print the help message:
```
Syntax: GRADREP [PathToFile]    [Flags]

        /F      Opens the window in fullscreen mode, not compatible with /FW.
        /FW     Opens the window in fullscreen-windowed mode, not compatible with /F.
        /S      Skips space characters.
        /N      Simulates newlines.
        /NC     Hides console until the window is closed.
        /new    Runs the program from another cmd instance.
        /newc   Runs the program from another cmd instance and closes the old one.

        /R      Draws the pixels in a red gradient.
        /G      Draws the pixels in a green gradient.
        /B      Draws the pixels in a blue gradient.

        Flags can be typed in any order, color flags can be combined.
```
This is only avaiable for Windows for now, compiling it for any Linux distro will give errors.

Gradrep with a 0.85MB executable file:
![alt text](https://i.imgur.com/BeyY2xC.png)
