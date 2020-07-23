#include<iostream>
#include<string>
#include<string.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wconversion-null"

using namespace std;
 
#define MAX_ENTRIES 1024
#define INST_SIZE 4
//defining the data structures needed

class label;
class command{
	char** args;
	int Cmd_Address;
	int Cmd_Code;
	public:		
		void createcommand(char** args_given,int address){
			args=args_given;
			Cmd_Address=address;
			Cmd_Code=0;
			
		}
		
		void pushtoCmd(command (&CommandList)[100]){
			int i=0;
			for(i=0;i<100;i++){
				if(CommandList[i].Cmd_Address==-1)
				break;
			}
			CommandList[i].args=args;
			CommandList[i].Cmd_Address=Cmd_Address;
			CommandList[i].Cmd_Code=Cmd_Code;
			
		}
		
	friend void InitCmd(command (&CmdList)[100]);	
	friend int getNextCmdAddress(command *);
	friend class symboltable;
	int friend resolveRegister(label** labList,char* regString);
	void friend Render_Parse_Table();
};

class label{
	char Lab_Name[256];
	int Lab_Address;
	int Lab_Size;
	public:
		//fills label with information
		void createlabel(char* temp,int address,int size){
			strcpy(Lab_Name,temp);
			Lab_Address=address;
			Lab_Size=size;
		}
		// Initialises the list with null
		
		//Inserts label into label list
		void pushtoList(label (&labelList)[100]){
			int i=0;
			for(i=0;i<100;i++){
				if(labelList[i].Lab_Address==-1)
				break;
			}
			strcpy(labelList[i].Lab_Name,Lab_Name);
			labelList[i].Lab_Address=Lab_Address;
			labelList[i].Lab_Size=Lab_Size;
			
		}
		
	
	friend void InitLab(label (&LabList)[100]);
	friend int getNextLabAddress(label* labList);
	friend class symboltable;
	int friend resolveRegister(label** labList,char* regString);
};

class symboltable{
	command *CommandList;
	label *LabelList;
	
	public:
		void Construct_Table(command* CmdList,label* LabList){
			CommandList=CmdList;
			LabelList=LabList;
		}
		
		void printLabelList()
	{
    	int i = 0;
    	while(LabelList[i].Lab_Address!=-1 && i < 100)
    	{
        	cout<<LabelList[i].Lab_Name<<" "<<LabelList[i].Lab_Address;
        	i++;
    	}
	}
	
		void printCmdList()
	{
    	int i = 0;
    	while(CommandList[i].Cmd_Address!=-1 && i < 100)
    	{
        	cout<<CommandList[i].args[0]<<" "<<CommandList[i].Cmd_Address;
        	i++;
    	}
	}
		int getOpcode(int i)
{	char* cmd=CommandList[i].args[0];
    if(strcmp(cmd, "sll\0")==0)
        return 0;
    else if(strcmp(cmd, "j\0")==0)
        return 2;
    else if(strcmp(cmd, "beq\0")==0)
        return 4;
    else if(strcmp(cmd, "bne\0")==0)
        return 5;
    else if(strcmp(cmd, "addi\0")==0)
        return 8;
    else if(strcmp(cmd, "ori\0")==0)
        return 13;
    else if(strcmp(cmd, "lui\0")==0)
        return 15;
    else if(strcmp(cmd, "add\0")==0)
        return 32;
    else if(strcmp(cmd, "sub\0")==0)
        return 34;
    else if(strcmp(cmd, "lw\0")==0)
        return 35;
    else if(strcmp(cmd, "nor\0")==0)
        return 39;
     else if(strcmp(cmd, "sw\0")==0)
        return 43;
    else if(strcmp(cmd, "_lui\0")==0)
        return -15;
    else if(strcmp(cmd, "_ori\0")==0)
        return -13;
    else
    {
        printf("[ERROR] Unable to get opcode for `%s`. Invalid command.\n", cmd);
        return 0;
    }
    
}

	char getType(int i)
{	char* cmd=CommandList[i].args[0];
    if( 
        (strcmp(cmd, "add\0")==0) || 
        (strcmp(cmd, "nor\0")==0) ||
        (strcmp(cmd, "sll\0")==0) ||
        (strcmp(cmd, "sub\0")==0) 
        )
        return 'r';
    else if(
        (strcmp(cmd, "addi\0")==0) ||
        (strcmp(cmd, "ori\0")==0) ||
        (strcmp(cmd, "lui\0")==0) ||
        (strcmp(cmd, "_ori\0")==0) ||
        (strcmp(cmd, "_lui\0")==0) ||
        (strcmp(cmd, "sw\0")==0) ||
        (strcmp(cmd, "lw\0")==0) ||
        (strcmp(cmd, "bne\0")==0) ||
        (strcmp(cmd, "beq\0")==0)
        )
        return 'i';
    else if( (strcmp(cmd, "j\0")==0) )
        return 'j';
    else
    {
        printf("[ERROR]: `%s` is not a valid command.\n", cmd);
        return '\0';
    }
}

	label* queryLabel(char* query)
    {
    int i = 0;
    while(LabelList[i].Lab_Address!=-1)
    {
        if(strcmp(query, LabelList[i].Lab_Name)==0)
        {
            return &LabelList[i];
        }
        i++;
    }
    return NULL;
    }
    void Print_Code(){
    	int i = 0;
    while(CommandList[i].Cmd_Address!=-1 && i < 100)
    {
        
        printf("0x%08X: 0x%08X\n", CommandList[i].Cmd_Address, CommandList[i].Cmd_Code);
        i++;
    }
	}
	
	
	void friend Render_Parse_Table();
	int friend resolveRegister(label** labList,char* regString);
	
};

//Declaring global variables

symboltable SymbolTable;

//Defining functions




void InitCmd(command (&CmdList)[100]){
		
		for(int i=0;i<100;i++){
		CmdList[i].Cmd_Address=-1;
}
}

void InitLab(label (&LabList)[100]){
	for(int i=0;i<100;i++){
		LabList[i].Lab_Address=-1;
}
}


int getNextLabAddress(label* labList)
{
    int address = 0;
    int i=0;
    for(i=0;i<100;i++){
    	if(labList[i].Lab_Address==-1)
    	break;
    }
    if(i==0){
	return 0;}
	
    address=labList[i-1].Lab_Address+labList[i-1].Lab_Size;
    return address;
}

int getNextCmdAddress(command *CmdList){
	int i=0;
	for(i=0;i<100;i++){
		if(CmdList[i].Cmd_Address==-1)
		break;
	}
	if(i==0){
	
	return 0;}
	int new_address=CmdList[i-1].Cmd_Address+4;
	return (CmdList[i-1].Cmd_Address+4);
	
}

int stringContains(char* str, char target)
{
    int j;
    for(j = 0; j < strlen(str); j++)
    {
        if(str[j] == target)
        {
            return j;
        }
    }
    return 0;
}

char* removeRangeFromString(char* str, int indexLow, int indexHigh)
{
    char* temp = (char*)calloc( strlen(str), sizeof(char));
    strcpy(temp, "\0");

    int i;
    int j = 0;
    for(i = 0; i < strlen(str); i ++)
    {
        if(i < indexLow || i > indexHigh)
        {
            temp[j] = str[i];
            j++;
        }
    }

    strcpy(str, temp);
    free(temp);

    return str;
}

char** initStringsArray(int size)
{
    char** newArray = new char*[size+1];
    newArray[0] = NULL;
    return newArray;
}

void pushArgsArray(char** args, char* newArg)
{
    int i = 0;
    while(args[i]!=NULL)
    {
        i++;
    }

    if(i < 4)
    {
        args[i] = new char[strlen(newArg+1)];
        strcpy(args[i], newArg);
        args[i+1] = NULL;
    }
    else
    {
        printf("[ERROR]: Expected a maximum of %d args. Unable to add argument `%s` to `%s` command.\n", 4, newArg, args[0]);
    }
}

void destroyStringArray(char** arr)
{
    int i = 0;
    while(arr[i]!=NULL)
    {
        free(arr[i]);
        i++;
    }

    free(arr);
}

int registerToDecimal(char* regString)
{
    // Temp. Registers ($t)
    if(strcmp(regString, "$t0\0") == 0)
        return 8;
    else if(strcmp(regString, "$t1\0")==0)
        return 9;
    else if(strcmp(regString, "$t2\0")==0)
        return 10;
    else if(strcmp(regString, "$t3\0")==0)
        return 11;
    else if(strcmp(regString, "$t4\0")==0)
        return 12;
    else if(strcmp(regString, "$t5\0")==0)
        return 13;
    else if(strcmp(regString, "$t6\0")==0)
        return 14;
    else if(strcmp(regString, "$t7\0")==0)
        return 15;
    // Save Registers ($s)
    else if(strcmp(regString, "$s0\0")==0)
        return 16;
    else if(strcmp(regString, "$s1\0")==0)
        return 17;
    else if(strcmp(regString, "$s2\0")==0)
        return 18;
    else if(strcmp(regString, "$s3\0")==0)
        return 19;
    else if(strcmp(regString, "$s4\0")==0)
        return 20;
    else if(strcmp(regString, "$s5\0")==0)
        return 21;
    else if(strcmp(regString, "$s6\0")==0)
        return 22;
    else if(strcmp(regString, "$s7\0")==0)
        return 23;
    // Null Register ($0)
    else if(strcmp(regString, "$0\0")==0)
        return 0;
    // Pseudo-Instruction Register
    else if(strcmp(regString, "$1\0")==0)
        return 1;
    // Others (Immediate)
    else
        return atoi(regString);
}

int resolveRegister(label** labList,char* regString)
{
    if(SymbolTable.queryLabel(regString))
    {
        if(1)
        {
            printf("Resolving label `%s` to address `%d`.\n", regString, SymbolTable.queryLabel(regString)->Lab_Address);
        }
        return SymbolTable.queryLabel(regString)->Lab_Address;
    }
    return registerToDecimal(regString);
}

void Render_Parse_Table(){
	int i = 0;
	symboltable *pt=&SymbolTable;
    while(pt->CommandList[i].Cmd_Address!=-1)
    {
        // Assign Registers from Parsed Data
        int8_t rs = 0;
        int8_t rt = 0;
        int8_t rd = 0;
        int8_t shamt = 0;
        int8_t func = 0;
        int32_t imm = 0;
        int32_t address = 0;
        int8_t opCode = 0;

        opCode = SymbolTable.getOpcode(i);

        switch(SymbolTable.getType(i))
        {
            case 'r':
            {
                func = opCode;
                if(opCode == 32 || opCode == 39 || opCode == 34) // ADD/NOR/SUB
                {
                    rd = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    rs = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                }
                else if(opCode == 0) // SLL
                {
                    rd = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    shamt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                }
            }
            case 'i':
            {
                if(opCode == 8 || opCode == 13) // ADDI/ORI
                {
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    rs = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                }
                else if(opCode == 15) // LUI
                {
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                }
                else if(opCode == 43 || opCode == 35) // SW/LW
                {
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    rs = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                }
                else if(opCode == 5) // BNE
                {
                    rs = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                }
                else if(opCode == 4) // BEQ
                {
                    rs = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                    imm = (pt->CommandList[i].Cmd_Address - imm)/INST_SIZE;
                    imm = ~imm;
                }
                else if(opCode == -15) // Handling for LUI generated from LA
                {
                    opCode = 15;
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    // Trim Lower-Half of Bits
                    imm = imm & 0xFFFF0000;
                    imm = imm >> 16;
                }
                else if(opCode == -13) // Handling for ORI generated from LA
                {
                    opCode = 13;
                    rt = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1]);
                    rs = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[2]);
                    imm = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[3]);
                }
                
                // Enforce Bounds
                imm = imm & 0x0000FFFF;
            }
            case 'j':
            {
                address = resolveRegister(&(pt->LabelList), pt->CommandList[i].args[1])/INST_SIZE;
            }
        }


        // Further Resolution
        switch(SymbolTable.getType(i))
        {
            case 'r':
            {
                // R-Type Instructions
                // Begin Instruction With 0
                pt->CommandList[i].Cmd_Code = 0;

                // RS
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 5;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | rs;

                // RT
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 5;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | rt;

                // RD
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 5;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | rd;

                // Shamt
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 5;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | shamt;

                // Function
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 6;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | func;
                break;
            }
            case 'i':
            {
                // I-Type Instructions
                // Begin Instruction With Opcode
                pt->CommandList[i].Cmd_Code = opCode;

                // RS
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 5;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | rs;

                // RT
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 5;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | rt;

                // Immediate
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 16;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | imm;
                break;
            }
            case 'j':
            {
                // J-Type Instructions
                // Begin Instruction With Opcode
                pt->CommandList[i].Cmd_Code = opCode;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code << 26;
                pt->CommandList[i].Cmd_Code = pt->CommandList[i].Cmd_Code | address;
                break;
            }
            default:
            {
                // Unknown Types
            }
        }

        i++;
    }
}
void Parse_Input(){
	
	command CmdList[100];
	label LabList[100];
		
	InitCmd(CmdList);
	
	InitLab(LabList);
	
	char user_input[256];
	while(fgets(user_input,100, stdin)){
		unsigned dataAllocations = 0;
		char directive;
		char temp[256];
        char tempDataDirective[256];
        char tempSize[256];
		
		if(sscanf(user_input, "\t.%s", temp))
        {
            if(strcmp(temp,"text")==0)
            {
                directive='t';
            }
            else if(strcmp(temp,"data")==0)
            {
                directive='d';
            }

        }
        
        else
        {
            // Text Directive Parsing
            if(directive == 't')
            {
                // Label Handling
                if(stringContains(user_input,':'))
                {
                    // Save Label & Remove From Command
                    int colonIndex = stringContains(user_input,':');
                    strncpy(temp, user_input, colonIndex);
                    temp[colonIndex] = '\0';
                    removeRangeFromString(user_input,0,colonIndex);
                    label lab;
					lab.createlabel(temp,getNextLabAddress(LabList),INST_SIZE);
                    lab.pushtoList(LabList);
                }

                // Command Handling
                char cmd[20];
                char params[50];
                if(sscanf(user_input,"\t%s\t%s", cmd, params) == 2)
                { 
                    if(strcmp(cmd, "la\0")==0)
                    {
                        // Load Address Expand to LUI/ORI Instructions
                        char argsBuffer[5];
                        char** laArgs = initStringsArray(4);

                        int i;
                        int prevI = 0;
                        for(i = 0; i < strlen(params)+1 ; i++)
                        {
                            if(params[i]==',' || params[i]=='(' || params[i]==')' || params[i]=='\0' || params[i]=='\n')
                            {
                                int j = 0;
                                while(prevI != i)
                                {
                                    argsBuffer[j] = params[prevI];
                                    prevI++;
                                    j++;
                                }
                                argsBuffer[j] = '\0';
                                prevI++;
                                pushArgsArray(laArgs, argsBuffer);
                            }
                        }

                        char** luiArgs = initStringsArray(4);
                        char** oriArgs = initStringsArray(4);

                        pushArgsArray(luiArgs, "_lui\0");
                        pushArgsArray(luiArgs, "$1\0");
                        pushArgsArray(luiArgs, laArgs[1]);

                        pushArgsArray(oriArgs, "_ori\0");
                        pushArgsArray(oriArgs, laArgs[0]);
                        pushArgsArray(oriArgs, "$1\0");
                        pushArgsArray(oriArgs, laArgs[1]);
                        
                        command cmd1;
						cmd1.createcommand(luiArgs,getNextCmdAddress(CmdList));
						cmd1.pushtoCmd(CmdList);
						command cmd2;
						cmd2.createcommand(oriArgs,getNextCmdAddress(CmdList));
                        cmd2.pushtoCmd(CmdList);
                        destroyStringArray(laArgs);
                    }
                    else
                    {
                        char argsBuffer[5];
                        char** args = initStringsArray(4);
                        pushArgsArray(args, cmd);

                        int i;
                        int prevI = 0;
                        for(i = 0; i < strlen(params)+1 ; i++)
                        {
                            if(params[i]==',' || params[i]=='(' || params[i]==')' || params[i]=='\0' || params[i]=='\n')
                            {
                                int j = 0;
                                while(prevI != i)
                                {
                                    argsBuffer[j] = params[prevI];
                                    prevI++;
                                    j++;
                                }
                                argsBuffer[j] = '\0';
                                prevI++;
                                if((strlen(argsBuffer)>0))
                                {
                                    pushArgsArray(args, argsBuffer);
                                }
                            }
                        }
                        command cmd;
                        cmd.createcommand(args,getNextCmdAddress(CmdList));
						cmd.pushtoCmd(CmdList);
                    }
                }
                else
                {
                    printf("[ERROR]: Invalid command `%s` encountered.", cmd);
                }
            }
            else if(directive == 'd')
            {
                // Data Directive Parsing

                if(stringContains(user_input,':'))
                {
                    // Prepare for Memory Allocation
                    unsigned bytesDeclared = INST_SIZE;

                    // Save Label & Remove From Command
                    int colonIndex = stringContains(user_input,':');
                    strncpy(temp, user_input, colonIndex);
                    temp[colonIndex] = '\0';
                    removeRangeFromString(user_input,0,colonIndex);
						
						
                    // Evaluate Data Sub-Directive
                    if(sscanf(user_input, "\t.%s %s", tempDataDirective, tempSize))
                    {	
                        if(strcmp(tempDataDirective,"space")==0)
                        {
                            bytesDeclared = atoi(tempSize);
                        }
                        else if(strcmp(tempDataDirective,"word")==0)
                        {
                            bytesDeclared = INST_SIZE;
                        }
                        else
                        {
                            printf("[ERROR]: Invalid Sub-Directive `%s` in `.data` Directive.\n", tempDataDirective);
                        }
                    }

                    if(dataAllocations == 0)
                    {
                    label labnew;
					labnew.createlabel(temp,getNextCmdAddress(CmdList),bytesDeclared);
                    labnew.pushtoList(LabList);
                    dataAllocations++;
                    }
                    else
                    {
                    	label labnew;
						labnew.createlabel(temp,getNextLabAddress(LabList),bytesDeclared);
                    	labnew.pushtoList(LabList);
                    }
                }
            }
        }
    }
	SymbolTable.Construct_Table(CmdList,LabList);	
		
}
	

void Introductory_Message(){
	
	// Prints introductory message
	cout<<"******MIPS ASSEMBLER**********"<<endl;
	cout<<"Enter the instructions. (Max. = 100)"<<endl;
	cout<<"When done, enter 'exit' to get the assembly language."<<endl;
	
}

int main(){
	Introductory_Message();
	//Pass 1
	// Generate symbol table
	Parse_Input();
    //Pass 2	
    // Generate the machine code and print it
    Render_Parse_Table();
    //The parse table now has the machine code
    //We print the commands now
    cout<<endl<<"The machine code equivalent is: "<<endl;
    SymbolTable.Print_Code();

    cout<<endl<<"Done!";
    getchar();
    
	
}
