**NOTE: This is my personal implementation of assignment2. Please do not copy 
        my code outright. You probably won't learn much that way.**

Building assignment 2 on Mac OS is a little bit more involved than the previous
two assignments. You'll need to install a copy of [FLTK](http://www.fltk.org/).
I suggest using [Homebrew](http://mxcl.github.com/homebrew/) for this. Once
you've got homebrew installed, just run:
```
brew install fltk
```

Another thing: since we've been relying so heavily on vecmath for all of the
projects, I went ahead and made a separate repository for it. To grab vecmath,
just call:
```
git submodule init && git submodule update
```

Now, you should be able to call `make` as usual.

To build on Linux, you should be able to just do `make`, assuming FLTK is installed.

If you need to install FLTK, try the following:
```
wget http://ftp.easysw.com/pub/fltk/1.3.0/fltk-1.3.0-source.tar.gz
tar -xzvf fltk-1.3.0-source.tar.gz
cd fltk-1.3.0
./configure
make
sudo make install
```

I referred to Chapter 7 of Steven Gortler's book to learn more about Quaternions,
although I haven't actually made use of them in my code.

I haven't done any extra credit yet. I'll resubmit if I have enough time.

Artifacts are located in the `artifacts` folder.

Enjoy. [Email me](ethanis@mit.edu) if you run into any issues.
