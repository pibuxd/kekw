# Kekw

nice lang in development right now

## what have I done?
+ inteeger variables, bool operators
+ functions
+ builtin print() which also can pass string

```txt
var sum = (n){
  if n > 1 {
    return n + sum(n-1);
  };

  return n;
};

print("SUM = ", sum(6));
```

output:
```txt
SUM = 21
```

## Installing
```sh
make
./kekw.out test.kekw
```