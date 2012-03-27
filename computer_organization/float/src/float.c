#include <stdio.h>
#include <string.h>
typedef unsigned long dword;

union{
    float f;
    dword k;
}u, v, z1, z2, z3, z4, out;

/*所有以下6个函数中，不能有任何float类型变量*/
dword atof(char*);
void ftoa(char*, dword);
dword fadd(dword, dword);
dword fsub(dword, dword);
dword fmul(dword, dword);
dword fdiv(dword, dword);

int main(int argc, char *argv[])
{
    char s[80], t[80];
    void print_code(unsigned long num);

    /*验证atof()与ftoa()*/
    //scanf("%s", s);
    //u.k = atof(s);
    //ftoa(t, u.k);
    //printf("%s\n%70.40f\n%s\n", s, u.f, t);
    while(scanf("%f", &u.f) && scanf("%f", &v.f)){
        /*验证float + - * / */
        //z1.k = fadd(u.k, v.k);
        //z2.k = fsub(u.k, v.k);
        z3.k = fmul(u.k, v.k);
        z4.k = fdiv(u.k, v.k);
        //printf("add:\n%70.40f\n%70.40f\n\n", z1.f, u.f+v.f);
        //printf("sub:\n%70.40f\n%70.40f\n\n", z2.f, u.f-v.f);
        printf("mul:\n%70.40f\n%70.40f\n\n", z3.f, u.f*v.f);
        printf("div:\n%70.40f\n%70.40f\n\n", z4.f, u.f/v.f);

        out.f = u.f*v.f;
        print_code(z3.k);
        print_code(out.k);
        printf("\n");
        out.f = u.f/v.f;
        print_code(z4.k);
        print_code(out.k);
    }
    return 0;
}

/*
dword atof(char* s){

    void print_code(unsigned long num);
    void binary(unsigned long integer, unsigned long decimal, unsigned long* out, int* length);

    int i, flag = s[0] == '-'? 1: 0;
    int count = 0;
    unsigned long integer, decimal;
    unsigned long no_sign, int_out = 0;
    for(i = 0; i < strlen(s); i++)
        if(s[i] == '.')
            break;
    if(flag == 1)
        integer = atol(s+1);
    else
        integer = atol(s);
    decimal = atol(s+i+1);
    binary(integer, decimal, &int_out, &count);
    print_code(0x00000010);
    print_code(int_out);
    printf("%d\n",count);
    return 0;
}
*/


/*
void integer_binary(unsigned long num, unsigned long* out, int* length){
    if(num / 2 > 0){
        integer_binary(num/2, out, length);
    }
    if(num % 2 == 0)
        *out &= 0xFFFFFFFE;
    else
        *out |= 0x00000001;
    *out <<= 1;
    *length += 1;
}

void binary(unsigned long integer, unsigned long decimal, unsigned long* out, int* length){
    integer_binary(integer, out, length);
    *out >>= 1;
}

*/
/*--------------------------------------*/

/*
 * 用于输出32bits的2进制数
 */
void print_code(unsigned long num){
    int i;
    unsigned long check = 0x80000000;
    for(i = 0; i < 32; i++){
        if(num&check)
            printf("1");
        else
           printf("0");
        check >>= 1;
    }
    printf("\n");
}

/*
 * 加减法器，可以指定从加法作用的位数
 * ctr == 1时 做减法
 * carry 用于记录进位，做减法是，需要置为1
 */
dword adder(dword num1, dword num2,int start, int end, int ctr,int* carry){
    int i, co = ctr;
    int bit1, bit2;
    unsigned long bit_check, adder, out = 0;

    if(ctr == 1){
        num2 = ~num2;
    }
    bit_check = 1;
    bit_check <<= start;
    adder = bit_check;
    for(i = start; i <= end; i++){
        bit1 = (num1&bit_check)?1:0;
        bit2 = (num2&bit_check)?1:0;
        switch(bit1 + 2*bit2 + 4*co){
            case 0: out&=(~adder);co = 0;break;
            case 1:
            case 2:
            case 4: out|=(adder);co = 0;break;
            case 3:
            case 5:
            case 6: out&=(~adder);co = 1;break;
            case 7: out|=(adder);co = 1;break;
        }
        bit_check <<= 1;
        adder <<= 1;
    }
    *carry = co;
    return out;
}

dword fmul(dword num1, dword num2){
    int i, carry = 0;
    dword out;
    dword product = 0;
    dword num1_add1 = (num1|0x00800000);
    dword num2_add1 = (num2|0x00800000);
    num1_add1 &= 0x00FFFFFF;
    num2_add1 &= 0x00FFFFFF;

    /* 阶码相加 */
    out = adder(num1, num2, 23, 30, 0, &carry);
    out ^= 0x40000000;

    /* 尾数做乘法，通过多次的加法实现 */
    carry = 0;
    for(i = 0; i < 24; i++){
        if(num2_add1&1){
            product = adder(product, num1_add1, 0, 31, 0, &carry);
        }
        if(i != 23)
            product >>= 1;
        num2_add1 >>= 1;
    }

    /* 判断尾数乘法当中是否有进位，若有，则得到的积的尾数右移1位，阶码+1 */
    if(product&0x01000000){
        out = adder(out, 0x00800000, 23 ,30, 0, &carry);
        product >>= 1;
    }

    /* 将阶码与尾数合并 */
    out = adder(out, product, 0, 31, 0, &carry);

    /* 赋值符号位 */
    if(num1&0x80000000 && num2&0x80000000){}
    else if(!(num1&0x80000000) && !(num2&0x80000000)){}
    else
        out |= 0x80000000;

    return out;
}

dword fdiv(dword num1, dword num2){
    int i, carry = 1;
    int ctr = 1;
    dword out = 0, quotient = 0;

    dword num1_add1 = (num1|0x00800000);
    dword num2_add1 = (num2|0x00800000);

    num1_add1 &= 0x00FFFFFF;
    num2_add1 &= 0x00FFFFFF;

    /* 阶码相减 */
    out = adder(num1, num2, 23, 30, 1, &carry);
    out = adder(out, 0x00800000, 23, 30, 1, &carry);
    out ^= 0x40000000;
    carry = 1;

    /* 尾数相除，用交叉加减法实现 */
    for(i = 0; i < 24;i++){
        num1_add1 = adder(num1_add1, num2_add1, 0, 24, ctr, &carry);
        quotient <<= 1;
        if(num1_add1&0x01000000){
            ctr = 0;
            quotient &= 0xFFFFFFFE;
        }
        else{
            ctr = 1;
            quotient |= 0x00000001;
        }
        if(i != 23){
            num1_add1 <<= 1;
            num1_add1 &= 0x01FFFFFF;
        }
    }

    /* 判断尾数是否需要移位，同时对阶码进行操作 */
    if(quotient&0x00800000){
        quotient &= 0xFF7FFFFF;
        out = adder(out, quotient, 0, 31, 0, &carry);
    }
    else{
        while(!(quotient&0x00800000)){
            quotient <<= 1;
            out = adder(out, 0x00800000, 0, 31, 1, &carry);
        }
        quotient &= 0xFF7FFFFF;
        out = adder(out, quotient, 0, 31, 0, &carry);
    }

    /* 赋值符号位 */
    if(num1&0x80000000 && num2&0x80000000){}
    else if(!(num1&0x80000000) && !(num2&0x80000000)){}
    else
        out |= 0x80000000;

    return out;
}
