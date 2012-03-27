#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RTYPE 1
#define ITYPE 2
#define JTYPE 3
#define PSEUDO 4

void print_code(unsigned long num);
void get_para();
void compile(FILE *out);
unsigned long search_reg(char *reg);

char line[1000];
char ins[10];
char rd[10], rs[10], rt[10], sa[10], immediate[10], label[10];
char immediate_rs[10];
char null[1] = "\0";
int line_num = 1;
int type, id;

char *r_table[] = {"add","addu","and","nor","or","slt",
    "sltu","sub","subu","xor","srav","srlv","sllv","sll",
    "srl","sra","div","divu","mult","multu","jalr","jr",
    "mthi","mtlo","mfhi","mflo","syscall","break"};
char *i_table[] = {"addi","addiu","andi","ori","xori",
    "slti","sltiu","beq","bne","bgez","bgtz","blez","bltz",
    "lui","lb","lbu","lh","lhu","lw","lwc1","sb","sh","sw",
    "swc1"};
char *j_table[] = {"j","jal"};
char *p_table[] = {"blt","bgt","ble","bge"};

unsigned long r_func[] = {32,33,  36,   39,  37,    42,
    43,    34,    35,   38,    7,     6,     4,      0,
    2,     3,     26,   27,    24,    25,    9,      8,
    17,    19,    16,     18,     12,      13};
unsigned long i_func[] = {8, 9,     12,   13,   14,
    10,    11,      4,    5,     1,     7,    6,     1,
    15,    32,   36,  33,   37,  35,   49,   40,  41, 43,
    57};

char *reg_table[] = {"$zero","$at","$v0","$v1","$a0","$a1",
    "$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6",
    "$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
    "$t8","$t9","$k0","$k1","gp","sp","fp","ra"};


int main(){
    FILE *fp, *out;
    char check[10];
    unsigned long temp;
    fp = fopen("instructions.txt","r");
    out = fopen("out","wb");
    while(fgets(line,1000,fp)){
        printf("%4d\t",line_num);
        printf("%s",line);
        if(line[0] != '\n'){
            get_para();
            compile(out);
        }
        line_num ++;
    }
    fclose(fp);
    return 0;
}

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


void get_para(){
    int i;

/*-----initialize-----*/
    strcpy(ins, null);
    strcpy(rd, null);
    strcpy(rs, null);
    strcpy(rt, null);
    strcpy(sa, null);
    strcpy(immediate, null);
    strcpy(label, null);
    strcpy(immediate_rs, null);
/*-----end Initialize-----*/
/*-----confirm type-----*/
    sscanf(line, "%s", ins);
    for(i = 0; i < sizeof(r_table)/sizeof(char*); i++){
        if(strcmp(r_table[i], ins)==0){
            type = RTYPE;
            id = i;
        }
    }
    for(i = 0; i < sizeof(i_table)/sizeof(char*); i++){
        if(strcmp(i_table[i], ins)==0){
            type = ITYPE;
            id = i;
        }
    }
    for(i = 0; i < sizeof(j_table)/sizeof(char*); i++){
        if(strcmp(j_table[i], ins)==0){
            type = JTYPE;
            id = i;
        }
    }
    for(i = 0; i < sizeof(p_table)/sizeof(char*); i++){
        if(strcmp(p_table[i], ins)==0){
            type = PSEUDO;
            id = i;
        }
    }

/*-----get details-----*/
    switch(type){
        case RTYPE:
            if(id >= 0 && id <= 9)
                sscanf(line, "%s%s%s%s", ins, rd, rs, rt);
            else if(id >= 10 && id <= 12)
                sscanf(line, "%s%s%s%s", ins, rd, rt, rs);
            else if(id >= 10 && id <= 12)
                sscanf(line, "%s%s%s%s", ins, rd, rt, rs);
            else if(id >= 13 && id <= 15)
                sscanf(line, "%s%s%s%s", ins, rd, rt, sa);
            else if(id >= 16 && id <= 19)
                sscanf(line, "%s%s%s", ins, rs, rt);
            else if(id >= 20 && id <= 20)
                sscanf(line, "%s%s%s", ins, rd, rs);
            else if(id >= 21 && id <= 23)
                sscanf(line, "%s%s", ins, rs);
            else if(id >= 24 && id <= 25)
                sscanf(line, "%s%s", ins, rd);
            else if(id >= 26 && id <= 27)
                sscanf(line, "%s", ins);
            break;
        case ITYPE:
            if(id >= 0 && id <= 6)
                sscanf(line, "%s%s%s%s", ins, rt, rs, immediate);
            else if(id >= 7 && id <= 8)
                sscanf(line, "%s%s%s%s", ins, rs, rt, label);
            else if(id >= 9 && id <= 12)
                sscanf(line, "%s%s%s", ins, rs, label);
            else if(id >= 13 && id <= 13)
                sscanf(line, "%s%s%s", ins, rt, immediate);
            else if(id >= 14 && id <= 23){
                int tmp;
                sscanf(line, "%s%s%s", ins, rt, immediate_rs);
                for(tmp = 0; tmp < strlen(immediate_rs); tmp++){
                    if(immediate_rs[tmp] == '('){
                        strncpy(immediate, immediate_rs,tmp);
                        immediate[tmp] = '\0';
                        strncpy(rs, immediate_rs+tmp+1, strlen(immediate_rs)-tmp-2);
                        rs[strlen(immediate_rs)-tmp-2] = '\0';
                    }
                }
            }
            break;
        case JTYPE:
            sscanf(line,"%s%s", ins, label);
            break;
        case PSEUDO:
            sscanf(line,"%s%s%s%s", ins, rs, rt, label);
            break;
    }
/*-----test-----*/
/*
    if(type == RTYPE)
        printf("\tins = %s rd = %s rs = %s rt = %s sa = %s\n",ins, rd, rs, rt, sa);
    else if(type == ITYPE){
        printf("\tins = %s rs = %s rt = %s immediate = %s\n",ins, rs, rt, immediate);
        printf("\tlabel = %s immediate_rs = %s\n",label, immediate_rs);
    }
    else if(type == JTYPE){
        printf("\tins = %s label = %s\n",ins, label);
    }
    else if(type == PSEUDO){
        printf("\tins = %s rs = %s rt = %s label = %s\n",ins, rs, rt, label);
    }
*/
}

void compile(FILE *out){
    unsigned long buffer = 0x00000000;
    unsigned long ins_bf, rd_bf, rs_bf, rt_bf, sa_bf;
    unsigned long at_bf = 0x00000001;
    unsigned long zero_bf = 0x00000000;
    long im_bf;
    if(type == RTYPE){
        ins_bf = r_func[id];
        rd_bf = search_reg(rd);
        rs_bf = search_reg(rs);
        rt_bf = search_reg(rt);
        sa_bf = (unsigned long)atol(sa);
        //print_code(ins_bf);
        //print_code(rd_bf);
        //print_code(rs_bf);
        //print_code(rt_bf);
        //print_code(sa_bf);
        buffer |= (ins_bf);
        buffer |= (sa_bf << 6);
        buffer |= (rd_bf << 11);
        buffer |= (rt_bf << 16);
        buffer |= (rs_bf << 21);
        print_code(buffer);
        fprintf(out, "%ld", buffer);
    }
    else if(type == ITYPE){
        ins_bf = i_func[id];
        rs_bf = search_reg(rs);
        rt_bf = search_reg(rt);
        if(id == 9)
            rt_bf = 0x00000001;
        im_bf = (long)atol(immediate);
        if(im_bf < 0)
            im_bf &= 0x0000FFFF;
        buffer |= (ins_bf << 26);
        buffer |= (rs_bf << 21);
        buffer |= (rt_bf << 16);
        buffer |= im_bf;
        print_code(buffer);
        fprintf(out, "%ld", buffer);
    }
    else if(type == JTYPE){
        im_bf = (long)atol(label);
        if(im_bf < 0)
            im_bf &= 0x03FFFFFF;
        if(id == 0)
            buffer = 0x08000000;
        else if(id == 1)
            buffer = 0x0C000000;
        buffer |= im_bf;
        print_code(buffer);
        fprintf(out, "%ld", buffer);
    }
    else if(type == PSEUDO){
        ins_bf = 42;
        sa_bf = 0;
        im_bf = (long)atol(label);
        char slt[20];
        char bne[20];
        char beq[20];
        if(id == 0){
            rs_bf = search_reg(rs);
            rt_bf = search_reg(rt);
            buffer |= (ins_bf);
            buffer |= (sa_bf << 6);
            buffer |= (at_bf << 11);
            buffer |= (rt_bf << 16);
            buffer |= (rs_bf << 21);
            print_code(buffer);
            fprintf(out, "%ld", buffer);
            buffer = 0x00000000;
            ins_bf = 5;
            rs_bf = at_bf;
            rt_bf = zero_bf;
            if(im_bf < 0)
                im_bf &= 0x0000FFFF;
            buffer |= (ins_bf << 26);
            buffer |= (rs_bf << 21);
            buffer |= (rt_bf << 16);
            buffer |= im_bf;
            print_code(buffer);
            fprintf(out, "%ld", buffer);
        }
        else if(id == 1){
            rs_bf = search_reg(rt);
            rt_bf = search_reg(rs);
            buffer |= (ins_bf);
            buffer |= (sa_bf << 6);
            buffer |= (at_bf << 11);
            buffer |= (rt_bf << 16);
            buffer |= (rs_bf << 21);
            print_code(buffer);
            fprintf(out, "%ld", buffer);
            buffer = 0x00000000;
            ins_bf = 5;
            rs_bf = at_bf;
            rt_bf = zero_bf;
            if(im_bf < 0)
                im_bf &= 0x0000FFFF;
            buffer |= (ins_bf << 26);
            buffer |= (rs_bf << 21);
            buffer |= (rt_bf << 16);
            buffer |= im_bf;
            print_code(buffer);
            fprintf(out, "%ld", buffer);
        }
        else if(id == 2){
            rs_bf = search_reg(rt);
            rt_bf = search_reg(rs);
            buffer |= (ins_bf);
            buffer |= (sa_bf << 6);
            buffer |= (at_bf << 11);
            buffer |= (rt_bf << 16);
            buffer |= (rs_bf << 21);
            print_code(buffer);
            fprintf(out, "%ld", buffer);
            buffer = 0x00000000;
            ins_bf = 4;
            rs_bf = at_bf;
            rt_bf = zero_bf;
            if(im_bf < 0)
                im_bf &= 0x0000FFFF;
            buffer |= (ins_bf << 26);
            buffer |= (rs_bf << 21);
            buffer |= (rt_bf << 16);
            buffer |= im_bf;
            print_code(buffer);
            fprintf(out, "%ld", buffer);
        }
        else if(id == 3){
            rs_bf = search_reg(rs);
            rt_bf = search_reg(rt);
            buffer |= (ins_bf);
            buffer |= (sa_bf << 6);
            buffer |= (at_bf << 11);
            buffer |= (rt_bf << 16);
            buffer |= (rs_bf << 21);
            print_code(buffer);
            fprintf(out, "%ld", buffer);
            buffer = 0x00000000;
            ins_bf = 4;
            rs_bf = at_bf;
            rt_bf = zero_bf;
            if(im_bf < 0)
                im_bf &= 0x0000FFFF;
            buffer |= (ins_bf << 26);
            buffer |= (rs_bf << 21);
            buffer |= (rt_bf << 16);
            buffer |= im_bf;
            print_code(buffer);
            fprintf(out, "%ld", buffer);
        }
    }
}

unsigned long search_reg(char *reg){
    unsigned long i;
    if(strcmp(reg, null) == 0){
        return 0;
    }
    for(i = 0; i < sizeof(reg_table)/sizeof(char*); i++){
        if(strcmp(reg, reg_table[i])== 0){
            return i;
        }
    }
    return 0;
}
