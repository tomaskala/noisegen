# noisegen

Generators for various colors of noise.


## Usage

The generator emits the specified noise as a wav file, by default to the
standard output. You can use the following parameters for configuration.
```
noisegen [OPTIONS] NOISE

Noise: One of "brown" or "white".

Options:
-d INT    Duration of the noise in seconds (>0).
-v INT    Noise volume (0-100).
-k INT    Moving average kernel size, used to smoothen the noise. Set to 1 to
          disable smoothing.
-c FLOAT  Filter coefficient for the brown noise generator (0-1).
-o FILE   File to store the resulting noise in or "-" for the standard output.
```
The defaults are set for a pleasing brown noise experience.

A typical usage would be to pipe the output to a music player, e.g.,
```
$ ./noisegen brown | mpv -
```

Another possibility is to store the noise in an audio file, although for
compression, it would be best to convert it to an mp3 file.
```
$ ./noisegen brown | ffmpeg -i - -vn -b:a 320k noise.mp3
```
