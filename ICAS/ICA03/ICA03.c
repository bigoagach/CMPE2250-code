// Q1 
void Q1(void){
    unsigned int i = 5632 & 0x321 >> 3;
    /* what is i in binary
        step 1: convert values to binary
        step 2: shift 0x321 3 bits to the right
        step 3: AND each bit
            i = 1011000000000 & 001100100001 >> 3
            i = 1011000000000 & 000001100100
            i = 00000000 
        i in binary is 00000000 ***********ANSWER***********

    what is i in hex
        i in hexidecimal is 0x00 ***********ANSWER***********

    what is i in decimal
        i in decimal is 0 ***********ANSWER************/
}
// Q2 
void Q2(void){
    unsigned int i = (unsigned char)5632 | 0x321 >> 3;
    /* what is i in binary
    - from my understanding, unsigned char has 8 bits, 
      cutting off the other bit from the number
        step 1: convert values to binary
        step 2: shift 0x321 3 bits to the right
        step 3: OR each bit
            i = 00000000 | 001100100001 >> 3
            i = 00000000 | 01100100
            i = 01100100
        i in binary is 01100100 ***********ANSWER***********

    what is i in hex
        step 1: seperate the i(binary) into 4s
        step 2: convert the sections of four into hex
        step 3: combine
            i = 0110 0100
            i = 6 4
            i = 0x64
        i in hexidecimal is 0x64 ***********ANSWER***********

    what is i in decimal
        step 1: convert i into decimal
            i = 01100100 = 2^2 + 2^5 + 2^6
            i = 100
        i in decimal is 100 ***********ANSWER*************/
}
// Q3 
void Q3(void){
    unsigned char i = (unsigned char)(5632 & 0xF371 >> 3);
    /* what is i in binary
        step 1: convert values to binary
        step 2: shift 0x321 3 bits to the right
        step 3: AND each bit
        step 4: cast the unsigned char
            i = 1011000000000 & 001100100001 >> 3
            i = 1011000000000 & 000001100100
            i = 0000000000000
            i = 00000000
        i in binary is 00000000 ***********ANSWER***********

    what is i in hex
        i in hexidecimal is 0x00 ***********ANSWER***********

    what is i in decimal
        i in decimal is 0 ***********ANSWER************/
}

// Q4 
void Q4(void){
    unsigned int i = (unsigned char)(0x12345678ul / 4);
    /* what is i in binary
        step 1: convert 0x12345678 (unsigned long) into a binary number then a decimal
        step 2: divide the number by 4
        step 3: convert back to binary
        step 4: cast to unsigned char
            0x12345678 --> 1 2 3 4 5 6 7 8
            0x12345678 = 0001 0010 0011 0100 0101 0110 0111 1000
            00010010001101000101011001111000 = 305,419,896
            305419896 / 4 = 76,354,974
            76354974 = 0100 1000 1101 0001 0101 1001 1110
            i = 1001 1110
        i in binary is 10011110 ***********ANSWER***********

    what is i in hex
        step 1: convert from binary to hex
            i = 1001 1110 = 9 E
            i = 0x9E
        i in hexidecimal is 0x9E ***********ANSWER***********   

    what is i in decimal
        step 1: convert from binary to decimal
            i = 1001 1110 = 2^1 + 2^2 + 2^3 + 2^4 + 2^7
            i = 158
        i in decimal is 158 ***********ANSWER***********
    */
}
// Q5 
void Q5(void){
    unsigned int i = 0b10101010001001000101001100101010 >> 2 & 0xF5A56832u;
    /* what is i in binary
        step 1: shift binary number 2 to the right
        step 2: convert hex to binary
        step 3: AND the two binary numbers
            10101010001001000101001100101010 >> 2 = 00101010100010010001010011001010
            0xF5A56832 = 1111 0101 1010 0101 0110 1000 0011 0010
            i = 0010 0000 1000 0001 0000 0000 0000 0010
        i in binary is 0010 0000 1000 0001 0000 0000 0000 0010 ***********ANSWER***********
   
    what is i in hex
        step 1: convert from binary to hex
            i = 0010 0000 1000 0001 0000 0000 0000 0010 = 2 0 8 1 0 0 0 2
            i = 0x20810002
        i in hexidecimal is 0x20810002 ***********ANSWER*********** 

    what is i in decimal
        step 1: convert from binary to decimal
            i = (wont show the 2^n cuz why D:)
            i = 545,325,058
        i in decimal is 545325058 ***********ANSWER************/
}
// Q6 
void Q6(void){
    unsigned int i = 8 * 0x34 + 0b1101010011 << 4 & 0b110011001100 >> 2;
    /* what is i in binary
        step 1: perform shifting first
        step 2: AND
        step 3: Multiply
        step 4: Add
            1101010011 << 4 = 0001 0011 0000
            110011001100 >> 2 = 0011 0011 0011
            000100110000 & 001100110011 = 000100110000
            8 * 0x34 = 8 * 52 = 416
            416 = 0001 1010 0000
            000110100000 + 000100110000 = 0010 1101 0000
        i in binary is 0010 1101 0000 ***********ANSWER***********

    what is i in hex
        step 1: convert from binary to hex
            i = 2 D 0
        i in hexidecimal is 0x2D0 ***********ANSWER*********** 

    what is i in decimal
        step 1: convert from binary to decimal
            i = 720
        i in decimal is 720 ***********ANSWER************/
} 
