char *change(int val, int base, char *retbuf)
{
    static char *str = "0123456789ABCDEF ";
    char *p;
    char buf[15];

    p = buf+14;
    *p = 0;

    do{
        *--p = str[val % base];
    }while(val /= base);

    strcpy(retbuf,p);
    return retbuf;
}

void main()
{
    char buff[20];

    change(100,2,buff);
    printf( "%s\n ",buff);
    change(100,8,buff);
    printf( "%s\n ",buff);
    change(100,10,buff);
    printf( "%s\n ",buff);
    change(100,16,buff);
    printf( "%s\n ",buff);
}
