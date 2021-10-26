# Read Line

A simple experiment in reading individual lines from a file.  
Should work for any open file descriptor; in this example `STDIN`.

## Limitations

- Only `read`, `malloc` and `free` were permitted library functions
- The function prototype for `read_line()` was prescribed


## Usage examples

```sh
$> make
$> cat file > ./test
$> ./test <[file]
$> ./test <<EOF
```

## TODO

- Analyse and improve safety
- Clean up, remove any cruft.
- Behind-the-scenes line buffer should be free'd without burdening user.  
  At present it is the sole memory leak.
