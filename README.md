# Image Processing CLI (PGM/PPM)

**Author:** Oskar Legner  
**Completed:** January 5, 2019  



## Table of Contents

- [Overview](#overview)  
- [Built With](#built-with)  
- [Features](#features)  
- [Manual](#manual)  
- [Usage Examples](#usage-examples)  
- [Building](#building)  
- [License](#license)  



## Overview
A Command-Line Interface (CLI) utility implemented in C for processing **Portable GrayMap (PGM)** and **Portable PixMap (PPM)** image formats.  
The tool provides functionality for loading, saving, and displaying images, as well as a range of image manipulation operations—controlled through straightforward command-line flags and parameters.

> **Note:** All source code comments and identifiers are written in Polish to comply with the requirements of the Wrocław University of Technology, where this project was completed as a coursework assignment.



## Built With

- **Language:** C99  
- **Build System:** Makefile  
- **Modules:**  
  - `main.h` – program entry and argument parsing  
  - `operacje.*` – image operations (negative, contour, blur, threshold, levels)  
- **External:** Standard C library, `unistd.h` for temp-file cleanup  



## Features

> **Note:** Operations on PPM files automatically convert them to PGM internally before processing.

- **Load** (`-i`) and **Save** (`-o`) PGM/PPM images  
- **Negative** transformation (`-n`)  
- **Contour** / edge detection (`-k`)  
- **Blur**: horizontal (`-rx`) and vertical (`-ry`)  
- **Thresholding** with custom percent (`-p <0–100>`)  
- **Level Adjustment** between two percents (`-z <low> <high>`)  
- **Display** (`-d`) via temporary `tmp.pgm` and system viewer  
- **Robust Error Handling** and built-in `--help` guide  



## Manual

### Syntax

```
./program [--help] | [-i <input> -o <output> -p <percent> -n -k -rx -ry -d -z <level1> <level2>]
```

### Options

| Option                  | Description                                                            |
|-------------------------|------------------------------------------------------------------------|
| `--help`              | Show detailed usage guide                                              |
| `-i <input>`         | Load input image (PGM or PPM)                                          |
| `-o <output>`        | Save processed image                                                   |
| `-p <percent>`       | Apply thresholding (0–100%)                                            |
| `-n`                   | Apply negative transformation                                          |
| `-k`                   | Apply contour (edge detection)                                         |
| `-rx`                  | Apply horizontal blur                                                  |
| `-ry`                  | Apply vertical blur                                                    |
| `-d`                   | Display image (writes and cleans up `tmp.pgm`)                       |
| `-z <level1> <level2>` | Adjust levels between two percent values                              |



## Usage Examples

Load and immediately display a PGM:  
```
./program -i input.pgm -d
```

Create a negative of a PPM:  
```
./program -i input.ppm -n -o negative.pgm
```

Blur horizontally and then threshold at 60%:  
```
./program -i photo.pgm -rx -p 60 -o result.pgm
```

Adjust brightness levels between 15% and 85%:  
```
./program -i image.pgm -z 15 85 -o adjusted.pgm
```



## Building

Compile the project using the Makefile:

```bash
make
```

This produces the executable named `program`.



## License

Distributed under the MIT License.
