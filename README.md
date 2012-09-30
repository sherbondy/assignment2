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

Enjoy. [Email me](ethanis@mit.edu) if you run into any issues.
