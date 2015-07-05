# Photon Mapping

## Preparing

- [openmp](http://openmp.org/wp/openmp-compilers/): we use this for parallel computation, if you do not use `clang-omp++`, please modify the `Makefile` to meet your needs.

- [ppmtojpeg](http://netpbm.sourceforge.net/doc/index.html): a tool on Unix.

## Compiling

Simply type 

    $ make

and everything is done.

## Usage

    $ ./rt <scene file> [-s samples]

Syntax for scene file is rather straightforward, see `scenes/` for some examples.

Here, `samples` means how many samples do you want for each pixel, should be a squared positive integer.

## Documentation

See `doc/report.pdf` for details. (However, this document is written in Chinese.)