#include<stdio.h>

// or for better readability/"plug-ability"
int main() {
	int t;
    for (t = 0; ; t++)
        putchar(
            t*((15&t>>11)%12)&55-(t>>5|t>>12)|t*(t>>10)*32
        );
}