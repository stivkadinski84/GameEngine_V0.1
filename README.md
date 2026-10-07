# Rendering Pipeline

The engine uses a **software rendering pipeline** where the game draws pixels into a memory buffer, then Windows displays that buffer inside the game window.

## Pipeline

```text
CreateEngine()
    |
    v
BootWindow()
    |
    |-- Creates the Win32 window
    |-- Gets HWND
    |-- Gets HDC
    |
    v
CreatePixelBuffer()
    |
    |-- Allocates memory for every pixel
    |-- uint32_t per pixel
    |
    v
Create_BitMap()
    |
    |-- Describes the pixel buffer to Windows
    |-- Width
    |-- Height
    |-- 32-bit color
    |-- Top-down image
    |
    v
CreateRenderer()
    |
    |-- Renderer.screen points to the pixel buffer
    |
    v
OnUserUpdateGame()
    |
    |-- Game draws into Renderer.screen
    |-- Draw(renderer, x, y, color)
    |
    v
BitMap_Update()
    |
    |-- StretchDIBits()
    |-- Sends the pixel buffer to the window
    |
    v
Win32 Window
    |
    v
Screen
```

## Main Parts

### 1. Window

`BootWindow()` creates the Win32 window and gives the engine:

* `HWND` — identifies the window.
* `HDC` — represents the window's drawing device/context.

### 2. Pixel Buffer

`CreatePixelBuffer()` allocates the actual memory containing the pixels.

```c
uint32_t *pixel_buffer;
```

For an `800x600` screen:

```text
800 × 600 = 480,000 pixels
```

Each pixel uses 4 bytes (`uint32_t`).

### 3. Bitmap Information

`Create_BitMap()` does **not** create another pixel buffer.

It fills a `BITMAPINFO` structure that tells Windows how to interpret our pixel memory:

```text
Width:       800
Height:      600
Bits:        32
Compression: BI_RGB
```

### 4. Renderer

The renderer receives the existing pixel buffer:

```c
renderer.screen = pixel_buffer;
```

This means the renderer and bitmap system use the **same memory**.

```text
Renderer
   |
   v
pixel_buffer
   |
   v
Windows
```

There is no second copy of the screen.

### 5. Drawing

The game calls:

```c
Draw(renderer, x, y, color);
```

The renderer writes directly into the pixel buffer:

```c
screen[y * width + x] = color;
```

### 6. Presenting the Image

`BitMap_Update()` calls:

```c
StretchDIBits();
```

This takes the pixel buffer and displays it inside the Win32 window.

So the basic idea is:

```text
Game Code
   ↓
Renderer
   ↓
Pixel Buffer
   ↓
StretchDIBits
   ↓
Win32 Window
   ↓
Monitor
```

## Important Concept

The engine does not draw directly to the window for every pixel.

Instead:

**Draw everything into memory first, then send the finished pixel buffer to Windows.**
