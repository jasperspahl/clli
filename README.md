# Ideas

File parsing [png-parsing]


[png-parsing]: [https://www.youtube.com/watch?v=M9ZwuIv3xz8]

## File parsing

a file consists of chunks of data

```c
struct chunk {
    size_t title_length;
    char title[title_length];
    size_t data_length;
    char data[data_length];
    uint32_t starts;
    size_t url_length;
    char url[url_length];
}
```

## Project Flow

base screen with selection options
