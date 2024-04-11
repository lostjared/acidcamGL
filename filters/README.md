
# Shader Management README

## Overview

Just simply add shaders to index.txt and use the arrow keys when the  program is loaded to move between them in the order they are listed in the file.
Place the shaders in the same directory as index.txt and do not use the full path name just the filename

This document provides instructions on how to manage shader files in your project. Shaders are essential for rendering graphics in our application. They define the visual aspects of pixels and vertices on the screen. To efficiently manage and access these shaders, we use an `index.txt` file. This file acts as a central list of all shader files used by the application.

## Prerequisites

- Basic understanding of how shaders work in graphics programming.

1. **Locate the `index.txt` File**: This file is located in the root of the filters directory.

2. **Add Shader Filenames**: Open `index.txt` in a text editor. Add the full filename of each shader you wish to use in the program, one filename per line. For example:
 
    ```
    shader1.glsl
    shadedr2.glsl
    ```
    Ensure that each filename matches exactly with the name of the shader file in your project filters directory.

3. **Save Changes**: After adding all necessary shader filenames, save and close `index.txt`.

## Accessing Shaders in the Program

The program is configured to read the `index.txt` file at startup. It automatically loads the shaders listed in this file. Here’s a brief overview of how this process works:

- On startup, the program reads `index.txt`.
- For each line in the file, the program attempts to load the shader file with the corresponding filename.
- If a shader is successfully loaded, it's made available for the application to use in rendering operations. By pressing up an down arrow keys

## Best Practices

- **Consistency**: Keep the naming convention of shader files consistent to avoid confusion. That is .glsl
- **Update Regularly**: Whenever you add or remove a shader from your project, ensure to update `index.txt` accordingly.
- **Error Handling**: Implement error checking in your program to handle cases where a shader file listed in `index.txt` cannot be found or loaded.

## Conclusion

Managing shaders through an `index.txt` file simplifies the process of adding, updating, and removing shaders from your project. It provides a flexible and efficient way to handle shader files, making your project more organized and maintainable.
For more detailed instructions on shader programming or troubleshooting specific issues, please refer to the project's technical documentation or contact the development team.
