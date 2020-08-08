## lsb_steg

A simple least significant bit steganography implementation in C.  Uses an image's lower two bits to store data at a rate of one byte per pixel.

### Demo

Input file:

![input.png](https://github.com/Cubified/lsb_steg/blob/master/demo/input.png)

Encoding:

     $ ./find_x encode input.png
     Input data to be encoded:
     hello, world

Output file:

![output.png](https://github.com/Cubified/lsb_steg/blob/master/demo/output.png)

Decoding:

     $ ./find_x decode output.png
     hello, world

Diff (cropped and amplified, still somewhat faint due to most bits being 0):

![diff.png](https://github.com/Cubified/lsb_steg/blob/master/demo/diff.png)

### Compiling and Running

`lsb_steg` has no dependencies other than the included [stb_image libraries](https://github.com/nothings/stb), meaning it can be compiled and run with:

     $ make
     $ ./find_x encode input.png [optional input filename]
     $ ./find_x decode output.png [optional output filename]

Additionally, `find_x` contains a `diff` mode, capable of amplifying and writing the difference between original and modified images:

     $ ./find_x diff [original.png] [output.png]

### Drawbacks

This implementation currently only supports 4-channel, 8-bit PNG files, as two bits of each data byte are stored within each channel, and the modifications made are somewhat glaring when the original image is known.  This being said, it is a functional implementation and proof-of-concept.

### To-Do

- Consolidate logic (reading/writing code use many similar yet slightly different one-liners)
- Improve error messages

### Further Reading (/Watching)

- [Dr. Mike Pound's excellent explanation of steganography and its history](https://www.youtube.com/watch?v=TWEXCYQKyDc)
