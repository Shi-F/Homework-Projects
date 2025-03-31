#include <cstdint>
#include <iostream>
#include <fstream>
#include <bitset>

#define LENGTH 1
#define MAXLEN 100
#define STUDENT_ID_SECRET 0x7F

int16_t lab1(int16_t n) {
    // initialize
	int16_t c,carry;
    // calculation
	c = n + STUDENT_ID_SECRET;
	carry = (n & STUDENT_ID_SECRET) + (n & STUDENT_ID_SECRET);
    // return value
    return (c - carry);
}

int16_t lab2(int16_t n) {
    // initialize
	int16_t k = 0,a = n, count;
    // calculation
	while(a > 1){
		if((a & 1) == 0){
			//a is even
			count = 0;
			while(a > 0){
				a -= 2;
				count++;
			}
			a = count;	//a = a/2
		}
		else{
			//a is odd
			a = a + a + a + 1;
		}
		k++;
	}
    // return value
    return k;
}

int16_t lab3(char s1[], int n) {
    // initialize
	char *ch1 = s1,*ch2 = s1 + n - 1;
	int16_t result = 1;
    // calculation
	while(ch1 < ch2){
		if(*ch1 != *ch2){
			result = 0;
			break;
		}
		ch1++;
		ch2--;
	}
    // return value
    return result;
}

int16_t lab4(int16_t n) {
    // initialize
    int16_t savings(int16_t n);
	int16_t result;
    // calculation
	result = savings(n);
    // return value
    return result;
}

int16_t earn(int16_t n){
	int16_t e;
	if(n == 0)
		return 6;
	else
	{
		e = earn(n-1);
		return (e+e);
	}
}

int16_t spend(int16_t n){
	int16_t s;
	if(n == 0)
		return 2;
	else if(spend(n-1) >= earn(n-1))
		return 2;
	else{
		s = spend(n-1);
		s = s + s;
		s = s + s;
		return s;
	}
}

int16_t savings(int16_t n){
	if(n == 0)
		return 10;
	else
		return savings(n-1) + earn(n-1) - spend(n-1);
}

int main() {
    std::fstream file;
    file.open("test.txt", std::ios::in);

    // lab1
    int16_t n = 0;
    std::cout << "===== lab1 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab1(n) << std::endl;
    }

    // lab2
    std::cout << "===== lab2 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab2(n) << std::endl;
    }

    // lab3
    std::cout << "===== lab3 =====" << std::endl;
    char s1[MAXLEN];
    for (int i = 0; i < LENGTH; ++i) {
        file >> s1 >> n;
        std::cout << lab3(s1, n) << std::endl;
    }
    
    // lab4
    std::cout << "===== lab4 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n ;
        std::cout << lab4(n) << std::endl;
    }
    
    file.close();
    return 0;
}
