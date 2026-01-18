# Super Simple Workspace Indicator For Windows Taskbar

### Preview

![VDMIndicator.exe](./screenshot.png)

### Features 

- Shows The workspace count.
- Highlightes the current active workspace.

### Usage
- Download VDMIndicator.exe from ![Release]("https://github.com/Raj-Kashyap001/VDMIndicator/releases") 
and Run the VDMIndicator.exe

- You should see the indicator on taskbar now.

- You can configure it by right clicking the taskbar icon (not indicator) and choosing 
settings.

### Building from Source

```pwsh
    g++ -o VDMIndicator.exe main.c settings-dialog.c -lgdiplus -lcomctl32 -luser32 -lshell32 -lgdi32 -ladvapi32 -mwindows

```

### Licence

This project is licensed under the MIT License.
