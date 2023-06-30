#include <QCoreApplication>
#include <stdio.h>
#define add 0
#define sub 1
#define slt 2
#define Or 3
#define nand 4
#define addi 5
#define slti 6
#define ori 7
#define lui 8
#define lw 9
#define sw 10
#define beq 11
#define jalr 12
#define J 13
#define halt 14
#define spc  15
struct MemoryBlock
{
    bool flag=false;
    int data;
};
int registers[16];
MemoryBlock memory[65536];


struct instruction
{
    char mnemonic[7];
    int instType;
    int rd;
    int rt;
    int rs;
    int imm;
    int fill;
    int space;
    char line[72];
    int lineint;
};
 int charArray2int(char*);
int hex2int( char* );
void int2hex32Bits(char *,int );
int findCountOfInstructions(FILE*);
void fillInstructions(FILE*,int size,instruction*);
short calculateIMM(char* );
void findDirectives(int size,instruction*);
void doInstructions(int size,instruction*);
void int2hex16Bits(char *,int);
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    for(int i=0;i<65536;i++)
        memory[i].flag=false;
    for(int i=0;i<16;i++)
        registers[i]=0;

    FILE* machp;
    int size=findCountOfInstructions(machp);
    instruction* instructionsArr=(instruction*)malloc(sizeof(instruction)*size);
    fillInstructions(machp,size,instructionsArr);

    findDirectives(size,instructionsArr);
    doInstructions(size,instructionsArr);
    printf("registers :\n");
    for(int i=0;i<16;i++)
    {
        printf("register[%i]= %i\n",i,registers[i]);
    }
    printf("------------------------------------\n\n\n");

    printf("data memory: \n");
    for(int i=0;i<65536;i++)
    {
        if(memory[i].flag)
            printf("block %i = %i\n",i,memory[i].data );
    }
    return a.exec();
}

 int charArray2int(char* arr)
{
    int s=1;
    int i=-1;
    int res=0;
    if(arr[0]=='-')
    {
        s=-1;
        i=0;
    }
    while(arr[++i]!='\0')
    {
        res=(res*10)+(arr[i]-'0');
    }
    return res*s;
}
int hex2int( char* hex)
{
    int result=0;
    while ((*hex)!='\0')
    {
        if (('0'<=(*hex))&&((*hex)<='9'))
            result = result*16 + (*hex) -'0';
        else if (('a'<=(*hex))&&((*hex)<='f'))
            result = result*16 + (*hex) -'a'+10;
        else if (('A'<=(*hex))&&((*hex)<='F'))
            result = result*16 + (*hex) -'A'+10;
        hex++;
    }
    return(result);
}
void int2hex16Bits(char *lower,int a){
    sprintf(lower,"%X",a);
    if(a <0x10){
    lower[4]='\0';
    lower[3]=lower[0];
    lower[2]='0';
    lower[1]='0';
    lower[0]='0';
            }
    else if(a <0x100){
    lower[4]='\0';
    lower[3]=lower[1];
    lower[2]=lower[0];
    lower[1]='0';
    lower[0]='0';
            }
    else if(a <0x1000){
    lower[4]='\0';
    lower[3]=lower[2];
    lower[2]=lower[1];
    lower[1]=lower[0];
    lower[0]='0';
             }

  }
void int2hex32Bits(char *lower,int a)
{
    sprintf(lower,"%X",a);
    if(a <0x10){
    lower[8]='\0';
    lower[7]=lower[0];
    lower[6]='0';
    lower[5]='0';
    lower[4]='0';
    lower[3]='0';
    lower[2]='0';
    lower[1]='0';
    lower[0]='0';
            }
    else if(a <0x100){
    lower[8]='\0';
    lower[7]=lower[1];
    lower[6]=lower[0];
    lower[5]='0';
    lower[4]='0';
    lower[3]='0';
    lower[2]='0';
    lower[1]='0';
    lower[0]='0';
            }
    else if(a <0x1000){
    lower[8]='\0';
    lower[7]=lower[2];
    lower[6]=lower[1];
    lower[5]=lower[0];
    lower[4]='0';
    lower[3]='0';
    lower[2]='0';
    lower[1]='0';
    lower[0]='0';
             }
    else if(a <0x10000 )
    {
        lower[8]='\0';
        lower[7]=lower[3];
        lower[6]=lower[2];
        lower[5]=lower[1];
        lower[4]=lower[0];
        lower[3]='0';
        lower[2]='0';
        lower[1]='0';
        lower[0]='0';
    }
    else if(a <0x100000 )
    {
        lower[8]='\0';
        lower[7]=lower[4];
        lower[6]=lower[3];
        lower[5]=lower[2];
        lower[4]=lower[1];
        lower[3]=lower[0];
        lower[2]='0';
        lower[1]='0';
        lower[0]='0';
    }
    else if(a <0x1000000 )
    {
        lower[8]='\0';
        lower[7]=lower[5];
        lower[6]=lower[4];
        lower[5]=lower[3];
        lower[4]=lower[2];
        lower[3]=lower[1];
        lower[2]=lower[0];
        lower[1]='0';
        lower[0]='0';
    }
    else if(a <0x10000000 )
    {
        lower[8]='\0';
        lower[7]=lower[6];
        lower[6]=lower[5];
        lower[5]=lower[4];
        lower[4]=lower[3];
        lower[3]=lower[2];
        lower[2]=lower[1];
        lower[1]=lower[0];
        lower[0]='0';
    }
}

int findCountOfInstructions(FILE* inputFile)
{
    int lineNo=0;
    size_t lineSize=72;
    char* line=(char*)malloc(72);
    fopen_s(&inputFile,"outPut.txt","r");
    while(fgets(line,lineSize,inputFile)!=NULL)
    {
        if(line[0]=='\n')
            break;
        lineNo++;
    }
    free(line);
    rewind(inputFile);
    fclose(inputFile);
    return lineNo;

}
void fillInstructions(FILE* inputFile,int size,instruction* instructionArr)
{
    int i=-1;
    size_t lineSize=16;
    char* line=(char*)malloc(16);
    fopen_s(&inputFile,"outPut.txt","r");
     while(fgets(line,lineSize,inputFile)!=NULL)
     {
         if(line[0]=='\n')
             break;
         for(int k=0;k<strlen(line);k++)
            if(line[k]=='\n')
                line[k]=NULL;
         i++;
         strcpy(instructionArr[i].line,line);
         instructionArr[i].lineint=charArray2int(line);
     }
     free(line);
     rewind(inputFile);
     fclose(inputFile);
}

void findDirectives(int size,instruction* instructionsArr)
{
     char hexcode[9];
     char temp[5];
     int opCode;
     int number;
     bool flag=false;
    for(int i=0;i<size;i++)
    {
        int2hex32Bits(hexcode,instructionsArr[i].lineint);
        if(flag==false)
        {
        temp[0]=hexcode[1];
        temp[1]='\0';
        opCode=hex2int(temp);
        if(opCode==14)
            {
            flag=true;
            continue;
              }
        }
      //  temp[0]=hexcode[5];
      //  temp[1]=hexcode[6];
       // temp[2]=hexcode[7];
       // temp[3]='\0';
      //  if(temp[0]=='8' || temp[0]=='9' ||(temp[0]>='A' && temp[0]<='F')||(temp[0]>='a' && temp[0]<='f'))
       // {
       //     number=calculateIMM(temp);
      //  }
      //  else
        //   number=hex2int(temp);
        if(flag)
        {
            temp[0]=hexcode[1];
            temp[1]='\0';
            opCode=hex2int(temp);
            int number;
            temp[0]=hexcode[4];
            temp[1]=hexcode[5];
            temp[2]=hexcode[6];
            temp[3]=hexcode[7];
            if(temp[0]=='8' || temp[0]=='9' ||(temp[0]>='A' && temp[0]<='F')||(temp[0]>='a' && temp[0]<='f'))
             {
                number=calculateIMM(temp);
             }
              else
                number=hex2int(temp);

        if(opCode==15)
         {

            for(int j=1;j<number;j++)
            {
                 memory[i+j].flag=true;
                memory[i+j].data=0;

            }
        }
        else{
            memory[i].data=charArray2int(instructionsArr[i].line);
            memory[i].flag=true;
             }
        }
    }
}
void doInstructions(int size,instruction* instructionArr)
{
    int rd,rs,rt;
    int first12bits;
    short imm;
    char hexcode[9];
    char temp[5];
    int opCode;
   int pc=0;
   while(pc>=0 && pc<=(size-1))
   {
       int2hex32Bits(hexcode,instructionArr[pc].lineint);
       temp[0]=hexcode[1];
       temp[1]='\0';
       opCode=hex2int(temp);
       if(opCode<=4)
       {
         temp[0]=hexcode[4];
         temp[1]='\0';
         rd=hex2int(temp);

         temp[0]=hexcode[3];
         temp[1]='\0';
         rt=hex2int(temp);

         temp[0]=hexcode[2];
         temp[1]='\0';

         rs=hex2int(temp);
         if(opCode==add)
         {
             registers[rd]=registers[rs]+registers[rt];
         }
         else if(opCode==sub)
             registers[rd]=registers[rs]-registers[rt];
         else if(opCode==slt)
         {
             if(registers[rs]<registers[rt])
                 registers[rd]=1;
             else
                 registers[rd]=0;
         }
         else if(opCode==Or)
             registers[rd]=registers[rs] | registers[rt];
         else if(opCode==nand)
            registers[rd]=(~registers[rs])|(~registers[rt]);
         pc++;
         continue;
       }
       else if(opCode<=12)
       {

           temp[0]=hexcode[4];
           temp[1]=hexcode[5];
           temp[2]=hexcode[6];
           temp[3]=hexcode[7];
           temp[4]='\0';
           if(temp[0]=='8' || temp[0]=='9' ||(temp[0]>='A' && temp[0]<='F')||(temp[0]>='a' && temp[0]<='f'))
           {
               imm=calculateIMM(temp);
           }
           else
               imm=hex2int(temp);

           temp[0]=hexcode[2];
           temp[1]='\0';
            rs=hex2int(temp);

           temp[0]=hexcode[3];
           temp[1]='\0';
           rt=hex2int(temp);

           if(opCode==addi)
           {
               signed int immediate=imm;
               registers[rt]=registers[rs]+immediate;

           }
           else if(opCode==slti)
           {
               signed int immediate=imm;
               if(registers[rs]<immediate)
                   registers[rt]=1;
               else
                   registers[rt]=0;

           }
           else if(opCode==ori)
           {
               unsigned int immediate=imm;
               registers[rt]=registers[rs]|immediate;

           }
           else if(opCode==lui)
           {
               signed int immediate=imm;
               immediate=immediate<<16;

           }
           else if(opCode==lw)
           {

               signed int immediate=imm;

               if(memory[immediate+registers[rs]].flag==true)
                    registers[rt]=memory[immediate+registers[rs]].data;

           }
           else if(opCode==sw)
           {
               signed int immediate=imm;
               memory[registers[rs]+immediate].flag=true;
               memory[registers[rs]+immediate].data=registers[rt];

           }
           else if(opCode==beq)
           {

               if(registers[rs]==registers[rt])
               {

                   signed int immediate=imm;
                   pc=pc+1+immediate;
                   continue;
               }

           }
           else if(opCode==jalr)
           {
              registers[rt]=pc+1;
              pc=registers[rs];
              continue;
           }
           pc++;
           continue;
       }
       else if(opCode<=14)
       {

              temp[0]=hexcode[4];
              temp[1]=hexcode[5];
              temp[2]=hexcode[6];
              temp[3]=hexcode[7];
              temp[4]='\0';
              imm=hex2int(temp);
              if(opCode==J)
                {
                   unsigned int immediate=imm;
                   pc=immediate;
                   continue;
                }
             else if(opCode==halt)
              {
                   pc++;
                   return;
              }
       }
   }
   if(pc<0 || pc>(size))
   {
       printf("error\n");
       exit(1);
   }

}
short calculateIMM(char* hex)
{
   short binary[16];
   short  temp[5];
   for(int i=0;i<4;i++)
   {
       if(hex[i]=='F' || hex[i]=='f')
       {
           temp[0]=1;temp[1]=1;temp[2]=1;temp[3]=1;
       }
       else if(hex[i]=='E' || hex[i]=='e')
       {
           temp[0]=1;temp[1]=1;temp[2]=1;temp[3]=0;
       }
      else  if(hex[i]=='D' || hex[i]=='d')
       {
           temp[0]=1;temp[1]=1;temp[2]=0;temp[3]=1;
       }
      else if(hex[i]=='C' || hex[i]=='c')
       {
           temp[0]=1;temp[1]=1;temp[2]=0;temp[3]=0;
       }
      else  if(hex[i]=='B' || hex[i]=='b')
       {
           temp[0]=1;temp[1]=0;temp[2]=1;temp[3]=1;
       }
       else  if(hex[i]=='A' || hex[i]=='a')
        {
            temp[0]=1;temp[1]=0;temp[2]=1;temp[3]=0;
        }
       else  if(hex[i]=='9')
        {
            temp[0]=1;temp[1]=0;temp[2]=0;temp[3]=1;
        }
       else  if(hex[i]=='8')
        {
            temp[0]=1;temp[1]=0;temp[2]=0;temp[3]=0;
        }
       else  if(hex[i]=='7')
       {
           temp[0]=0;temp[1]=1;temp[2]=1;temp[3]=1;
       }
       else  if(hex[i]=='6')
       {
           temp[0]=0;temp[1]=1;temp[2]=1;temp[3]=0;
       }
       else  if(hex[i]=='5')
        {
            temp[0]=0;temp[1]=1;temp[2]=0;temp[3]=1;
        }
       else  if(hex[i]=='4')
        {
            temp[0]=0;temp[1]=1;temp[2]=0;temp[3]=0;
        }
       else  if(hex[i]=='3')
        {
            temp[0]=0;temp[1]=0;temp[2]=1;temp[3]=1;
        }
       else  if(hex[i]=='2')
        {
            temp[0]=0;temp[1]=0;temp[2]=1;temp[3]=0;
        }
       else  if(hex[i]=='1')
        {
            temp[0]=0;temp[1]=0;temp[2]=0;temp[3]=1;
        }
       else  if(hex[i]=='0')
        {
            temp[0]=0;temp[1]=0;temp[2]=0;temp[3]=0;
        }

       for(int j=0;j<4;j++)
           binary[(4*i)+j]=temp[j];
   }
   bool flag=false;
   for(int i=15;i>=0;i--)
   {
       if(flag==false)
       {
           if(binary[i]==1)
           {
               flag=true;
               continue;
           }
       }
       if(flag)
       {
           if(binary[i]==1)
               binary[i]=0;
           else
              binary[i]=1;
       }
   }
   short result=0;
   for(int i=0;i<16;i++)
       result=(result*2)+binary[i];
   result *=-1;
   return result;
}
