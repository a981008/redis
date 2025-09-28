#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

typedef char *sds; // 兼容 c 字符串

struct sdshdr
{
    long len;   // 已用长度
    long free;  // 空闲长度
    char buf[]; // 灵活数组(未分配内存)
};

sds sdsnewlen(const void *init, size_t initlen)
{
    struct sdshdr *sh;

    sh = malloc(sizeof(struct sdshdr) + initlen + 1);
    sh->len = initlen;
    sh->free = 0;
    if (initlen)
    {
        if (init)
            memcpy(sh->buf, init, initlen);
        else
            memset(sh->buf, 0, initlen);
    }
    sh->buf[initlen] = '\0';
    return (char *)sh->buf;
}

sds sdsnew(const char *init)
{
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

struct sdshdr *sds_hdr(sds s)
{
    // s 地址减去头部大小即可得到头部开始的地址
    // s 是 buf 的地址
    return (struct sdshdr *)(s - sizeof(struct sdshdr));
}

int main(int argc, char *argv[])
{
    sds s = sdsnew("hello world");
    struct sdshdr *sh = sds_hdr(s);
    printf("len: %ld, free: %ld, buf: %s\n", sh->len, sh->free, sh->buf);

    return 0;
}
