/*
 * input two 8-bit true form number
 * calculate the result of add and minus
 * output the result in true form
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

int main(){

    void itot(int num, char true_form[]);
    void add(char num1[], char num2[], char sum[], int *of);
    void minus(char num1[], char num2[], char difference[], int *of);
    void ttoi(char true_form[], int *num);
    void shift_right(char num[], int bits);

    int num1, num2;
    char true_form1[9], true_form2[9];
    char sum_t[9], difference_t[9], product_t[17], quotient_t[17];
    int sum, difference, product, quotient, of;

    while(scanf("%d%d", &num1, &num2)){
        // initialize the arrays
        memset(true_form1, '0', 8);
        memset(true_form2, '0', 8);
        memset(sum_t, '0', 8);
        memset(difference_t, '0', 8);
        memset(product_t, '0', 16);
        memset(quotient_t, '1', 16);
        true_form1[8] = true_form2[8] = '\0';
        sum_t[8] = difference_t[8] = product_t[16] = quotient_t[16] = '\0';

        // get the two decimal number
        /* 
         * 由于采用8bits原码来运算
         * 输入的数范围在 -127~127 之间
         * 若不在该范围内，在assert函数中将无法通过，放弃程序运行
         */
        assert(num1>= -127 && num1 <= 127 && num2 >= -127 && num2 <= 127);
        // change to binary (true form)
        itot(num1, true_form1);
        itot(num2, true_form2);
        // output and test the itot function
        printf("num1 = %s\nnum2 = %s\n", true_form1, true_form2);

        // add and minus
        add(true_form1, true_form2, sum_t, &of);
        printf("sum = %s overflow = %d\n",  sum_t, of);
        minus(true_form1, true_form2, difference_t, &of);
        printf("difference = %s overflow = %d\n",  difference_t, of);

        // change the binary numbers to decimal ones
        ttoi(sum_t, &sum);
        printf("sum = %d\n", sum);
        ttoi(difference_t, &difference);
        printf("difference = %d\n", difference);
    }
}

/*
 * transform integer into true form code
 *
 * first transform it to binary code
 * then assign the sign bit
 */
void itot(int num, char true_form[]){
    int i = 0, j, k;
    int tmp;

    if(num >= 0)
        true_form[7] = '0';
    else{
        true_form[7] = '1';
        num = -num;
    }
    while(num != 0){
        true_form[i] = '0' + num % 2;
        num /= 2;
        i++;
    }
    for(j = 0, k = 7; j < k; j++, k--){
        tmp = true_form[j];
        true_form[j] = true_form[k];
        true_form[k] = tmp;
    }
}
/*
 * 七位无符号数的加法
 * switch语句中的cases为加数1 加数2 和进位的加权和
 * switch其实是一个1bit加法器
 * 若有overflow， 则of为1
 * 若没有，则of为0
 */
void adder_7bits(char num1[], char num2[], char sum[], int carry0, int *of){
    int carry = carry0;
    int i;
    for(i = 7; i > 0 ; i--){
        switch((num1[i] - '0') + 2*(num2[i] - '0') + 4*carry){
            case 0: sum[i] = '0'; break;
            case 1:
            case 2: sum[i] = '1'; break;
            case 3: sum[i] = '0'; carry = 1; break;
            case 4: sum[i] = '1'; carry = 0; break;
            case 5:
            case 6: sum[i] = '0'; carry = 1; break;
            case 7: sum[i] = '1'; carry = 1; break;
        }
    }
    *of = carry0 ^ carry;
}
/*
 * 加法运算：
 * 若两个数符号位相同，则通过七位无符号数加法将绝对值相加，再赋值给得数的符号位
 * 若两数符号位不同，则用将绝对值较小者取补码，调用七位无符号数加法得到结果，将绝对值大者的符号赋值给得数
 */
void add(char num1[], char num2[], char sum[], int *of){
    int i;
    int carry = 0;
    char bigger[9], smaller[9];
    int flag1 = num1[0];
    int flag2 = num2[0];
    if(flag1 == flag2){
        adder_7bits(num1, num2, sum, 0, of);
        sum[0] = flag1;
    }else{
        if(strcmp(num1 + 1,num2 + 1) == 1){
            strcpy(bigger, num1);
            strcpy(smaller, num2);
        }else{
            strcpy(bigger, num2);
            strcpy(smaller, num1);
        }
        for(i = 7; i > 0; i--){
            smaller[i] = smaller[i] == '1'? '0': '1';
        }
        adder_7bits(bigger, smaller, sum, 1, of);
        sum[0] = bigger[0];
    }
}
/*
 * 减法运算：
 * 将减数的符号位改为相反的值
 * 再调用加法运算得到结果
 */

void minus(char num1[], char num2[], char difference[], int *of){
    char _num2[9];
    strcpy(_num2, num2);
    _num2[0] = _num2[0] == '1'? '0': '1';
    add(num1, _num2, difference, of);
}

/*
 * transform true form code into integer
 *
 * first transform binary code to integer
 * then if the sign bit is 1, let the integer be negative
 *      else do nothing
 */
void ttoi(char true_form[], int *num){
    int i, base = 1;
    *num = 0;
    for(i = 7; i > 0; i--){
        *num += true_form[i] == '1'? base: 0;
        base *= 2;
    }
    if(true_form[0] == '1')
        *num = -*num;
}
