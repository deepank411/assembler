#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int* dec_to_bin(int decimal_num) // convert decimal to binary 1 to 0000001 -> Sakshi
{
    int quo, i, j;
    int* binNumber;
    binNumber = (int*)malloc(7*sizeof(int));
    for(i = 0; i < 7; i++)
    {
        binNumber[i]=0;
    }
    i=6;
    quo = decimal_num;
    while(quo!=0)
    {
         binNumber[i--]= quo % 2;
         quo = quo / 2;
    }
    return binNumber;
}

void main(void) 
{
	char inp[17], op[17], ch, a[10], lbl[17];
	int flag1, flag2, flag3, flag4, i, count1=0, label=0;
	int line_no = 0; //line no in program
	int count = 0; //to count the number of errors

	FILE *file_inp, *file_opcode, *file_reg, *file_hexa, *file_out, *file_lab_out, *file_lab_inp;
	file_inp = fopen("input.txt", "r");		//stores input instructions in standard form
	file_opcode = fopen("opcode.txt", "r"); 	//stores opcodes of all mnemonics
	file_reg = fopen("register.txt", "r");		//stores binary equivalent of all registers
	file_hexa = fopen("hexa_to_bin.txt", "r");		//stores hexadecimal to binary conversion opcodes
	file_out = fopen("output.txt", "w");	//stores binary conversion of full instruction
	file_lab_out = fopen("label.txt", "w");		//stores information about label present in input.txt

	if((!file_inp) || (!file_out) || (!file_opcode) || (!file_reg) || (!file_hexa)) 
		return; //test whether each file opens successfully

	printf("Program for a simple 2 Pass Assembler\n");
	printf("Pros:\n");
	printf("1. Instructions are case insensitive\n");
	printf("2. Error can be dealt with easily\n");
	printf("3. Line No and Area of Error specified\n");
	printf("4. New instructions can be added without any change in the code\n");
	printf("Cons\n");
	printf("1. No support for comments\n");
	printf("---------------------------\n");


    int *p;
	fscanf(file_inp,"%s",a);	//scan input file

	// Code Snippet to identify a label through presence of ":" -> Astha
    while(!feof(file_inp))		//run till end of input file
    {
        if(strchr(a,':') != NULL)	//if colon encountered
        {
            count1++;
            label = strlen(a);

            p = dec_to_bin(count1);

            int counter = 0;
            fprintf(file_lab_out,"%s ",a);
            while(counter < 7)
            {
                fprintf(file_lab_out, "%d", *(p+counter));
                counter++;
            }
            fprintf(file_lab_out,"\n");
        }
        fscanf(file_inp, "%s", a);
    }

    fprintf(file_lab_out,"----"); // append ---- at the end of label.txt
    fclose(file_lab_out);
    fclose(file_inp);
    file_inp = fopen("input.txt", "r");
    file_lab_inp = fopen("label.txt", "r");

	while(!feof(file_inp)) //while no: 1
    {
		rewind(file_opcode); //set filestream to start of file
		rewind(file_reg);
		rewind(file_hexa);
		rewind(file_lab_inp);
		fscanf(file_inp, "%s", inp);

		if(strcasecmp(inp, "HALT")==0)
		{
		    fprintf(file_out,"\n1111101010101110"); //append this to output.txt if HALT present in input.txt
			break;
		}

		//printf("%s\n",inp); //prints all the mnemonics in serai order for testing

        //label -> Astha
        while(1) //while no: 2
        {

			fscanf(file_lab_inp,"%s",lbl);
			//printf("label scanned = %s \n", lbl);
			if(strcasecmp(lbl,"----")==0) // i.e. no label in code
			{
				break; //break out from while no: 2
			}
			//printf("here");
			if(strcasecmp(inp, lbl) == 0) {
                //printf("match");
				fscanf(file_lab_inp,"%s",lbl);
				fprintf(file_out,"\n%s",lbl);
				flag1 = 1;
				break;
			} else {
				fscanf(file_lab_inp,"%s",lbl);
				flag1 = 0;
			}
		}

		while(1) { //while no: 3
			fscanf(file_opcode,"%s",op); //read first opcode from opcode.txt if no match goto next if match then go to second if
			if(strcasecmp(op,"----")==0) //if at the end of opcode.txt
			{
				break; //break from while no: 3
			}
			if(strcasecmp(inp, op) == 0) { //if match between opcode.txt and input file
				fscanf(file_opcode, "%s", op); //this fscanf scans the binary equivalent of the corresponding opcode
				fprintf(file_out, "\n%s", op); //this sends that binary value to output.txt
				flag2 = 1;
				break;
			} else {
				fscanf(file_opcode, "%s", op); //scan corresponding value anyway but is not used
				flag2 = 0;
			}
		}
		if(flag2 == 1)
			line_no++;

		while(1) { //while no: 4 //this loop will repeat until registers are found in the instruction if not goto next loop
			fscanf(file_reg, "%s", op); 
			if(strcasecmp(op, "----") == 0) //if at the end of register.txt
			{
				break; //break from while no: 4
			}
			if(strcasecmp(inp, op) == 0) { //if match between register.txt and input file
				fscanf(file_reg, "%s", op); //scanf the corresponding binary for that register
				fprintf(file_out, " %s", op); //send output to output.txt (space used to separate binary of different parts of instruction)
				flag3 = 1; //set flag
				break; //break from while no: 4
			} else { //else scan corrosponding binary value anyway to maintain flow just dont use it
				fscanf(file_reg,"%s",op);
				flag3 = 0;
			}
		}

		while(1) //deals with hexadecimal values while no: 5
        {
			fscanf(file_hexa,"%s",op);
			if(strcasecmp(op,"----")==0)
			{
				break; //break from while no: 5
			}
			if(strcasecmp(inp,op)==0) {
				fscanf(file_hexa,"%s",op);
				fprintf(file_out," %s",op);
				flag4 = 1; 
				break; //break from while no: 5
			} else {
				fscanf(file_hexa,"%s",op);
				flag4 = 0;
			}
		}

		if(!flag1 && !flag2 && !flag3 && !flag4) {
			line_no++;
			printf("\nError in Line: %d", line_no);
			printf("\n\"%s\" is not a valid Instruction or Register or Value\n",inp);
			fprintf(file_out," '%s'",inp);
			count++;
			printf("\n1. Continue with Execution of Program\t2. Exit from Program\n");
			scanf("%d", &i);
			if(i == 1)
				continue;
			else if(i == 2)
				break;
		}
	}
	fclose(file_inp);
	fclose(file_out);
	fclose(file_opcode);
	fclose(file_lab_inp);
	fclose(file_reg);
	fclose(file_hexa);
	if (count == 0)
		printf("\nProgram Assembled Successfully.\n"); //count variable to print total number of errors
    else
    	printf("%d Error(s) encountered\n", count);
    	return;
}


